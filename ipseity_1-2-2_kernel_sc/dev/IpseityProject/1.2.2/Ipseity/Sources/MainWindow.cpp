/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2014, 2013 Ipseity Core Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifdef USE_PYTHON
#include <Python.h> // Must appear first
#endif


#include "MainWindow.h"

#include "Defines.h"
#include "AL/AbstractMultiAgentEnvironmentInstance.h"
#include "AL/AbstractBaseEnvironment.h"
#include "IO/FileSearch.h"
#include "IO/ParameterRepository.h"
#include "IO/ParameterSlider.h"

#include "IO/XMLScanner.h"
#include "AL/XMLTags_Environments.h"

#include <QDebug>


const char* g_PathSupervisionModules = PLUGIN_PATH "SupervisionModules/";
const char* g_PathWorkspaces = DATA_PATH "Workspaces/";
const char* g_PathCognitiveSystems = DATA_PATH "CognitiveSystems/";
const char* g_PathScenarios = DATA_PATH "Scenarios/";

const char* g_CognitiveSystemExt = ".cs.xml";


const uint32 MAX_RECENT_SESSION_COUNT = 20;


enum
{
    ICON_LOAD			= 0,
    ICON_LOADED,
    ICON_USE,
    ICON_DELETE,
    ICON_RESET,
    ICON_SHARE_CS,
    ICON_SIMULATE,
    ICON_PAUSE,
    ICON_STOP,
    ICON_PLUG_CS,
    ICON_UNPLUG_CS,
    ICON_PLUG_UNPLUG_CS,
    ICON_NO_CS,

    ICON_COUNT
};


const char* g_IconNames[] =
{
    ":/images/IconLoad.png",
    ":/images/IconLoaded.png",
    ":/images/IconUse.png",
    ":/images/IconDelete.png",
    ":/images/IconReset.png",
    ":/images/IconShareCS.png",
    ":/images/IconSimulate.png",
    ":/images/IconPause.png",
    ":/images/IconStop.png",
    ":/images/IconPlugCS.png",
    ":/images/IconUnplugCS.png",
    ":/images/IconPlugUnplugCS.png",
    ":/images/IconNoCS.png"
};


#define TABID_ENVIRONMENT m_WidgetMainTab->indexOf(m_WidgetTabEnvironment)
#define TABID_SYSTEM_SCHEDULING m_WidgetMainTab->indexOf(m_WidgetTabSystemScheduling)
#define TABID_TAXONS m_WidgetMainTab->indexOf(m_WidgetTabTaxons)
#define TABID_AGENTS m_WidgetMainTab->indexOf(m_WidgetTabAgents)
#define TABID_COGNITIVESYSTEMS m_WidgetMainTab->indexOf(m_WidgetTabCognitiveSystemType)
#define TABID_BENCHMARK m_WidgetMainTab->indexOf(m_WidgetTabBenchmark)


const uint32 cstMaxEpisodes = 10000;


int
getTimeInterval( int speed )
{
    if (speed == 0)
    {
        return 0;
    }
    else
    {
        return 1000/speed;
    }
}


FILE* g_DebugFile = NULL;


void
messageOutput( QtMsgType type, const char* msg )
{
    if (g_DebugFile)
    {
        switch (type)
        {
            case QtDebugMsg:
                fprintf( g_DebugFile, "-- %s\n", msg );
                break;
            case QtWarningMsg:
                fprintf( g_DebugFile, "?? %s\n", msg );
                break;
            case QtCriticalMsg:
                fprintf( g_DebugFile, "!? %s\n", msg );
                break;
            case QtFatalMsg:
                fprintf( g_DebugFile, "!! %s\n", msg );
                abort();
        }

        fflush( g_DebugFile );
    }
}


MainWindow* g_MainWindow = NULL;

ParameterRepository g_ParameterRepository;


MainWindow::MainWindow( uint32 instance_id ) :
    QMainWindow( NULL ),
    m_SessionGroup( NULL ),
    m_Environment( NULL )
{
#ifdef USE_PYTHON
    // Initialize the Python interpreter
    Py_Initialize();

    qDebug() << "Python initialized!";
#endif

    QDir current_path( QDir::currentPath() );

    m_CurrentPath = current_path.path().toStdString().c_str();

    cout << "Current path: " << m_CurrentPath.c_str() << endl;

    cout << "Instance id: " << instance_id << endl;

    m_PluginPath = m_CurrentPath;
    m_PluginPath += "/";
    m_PluginPath += PLUGIN_PATH;

//    cout << "PluginPath: " << m_PluginPath.c_str() << endl;

    m_SimulationInProgress = false;
    m_SupervisionModule = NULL;

    g_MainWindow = this;

    setMinimumWidth( cstWindowMinimumWidth );
    setMinimumHeight( cstWindowMinimumHeight );

    setWindowTitle( WINDOW_TITLE );

    if (g_Settings.contains("MainWindowGeometry"))
    {
        this->setGeometry( g_Settings.value("MainWindowGeometry").toRect() );
    }

    m_Icons = new QPixmap[ICON_COUNT];

    for( uint32 i = 0; i < ICON_COUNT; ++i )
    {
        m_Icons[i].load( g_IconNames[i] );
    }

    m_SimulationTimer = new QTimer(this);
    m_SimulationTimer->setInterval( getTimeInterval( cstDefaultSpeed ) );
    m_SimulationTimer->setSingleShot(false);
    connect( m_SimulationTimer, SIGNAL(timeout()), this, SLOT(simulationStepForward()) );


    m_RenderTimer = new QTimer(this);
    m_RenderTimer->setSingleShot( false );
    connect( m_RenderTimer, SIGNAL(timeout()), this, SLOT(render()) );

    m_About = new DialogAbout();

    createMenus();
    createCentralWidget();
}


MainWindow::~MainWindow()
{
    g_Settings.setValue( "MainWindowGeometry", this->geometry() );
    g_Settings.setValue( "ScenarioSupervisionModule", m_ComboBoxSupervisionModules->currentText().toStdString().c_str() );
    g_Settings.setValue( "RecentSessions", m_RecentSessions );

    closeEnvironment();

    clearParameters();

    m_PcSystemScheduling.free();
    m_SystemScheduling = NULL;

    m_PcSupervisionModule.free();
    m_SupervisionModule = NULL;

#ifdef USE_PYTHON
    // Destroy the Python interpreter
    Py_Finalize();
#endif
}


void
MainWindow::about()
{
    m_About->show();
}


//*****************************************************************************
//
//  Creation of the interface widgets
//
//*****************************************************************************


bool
fetchWorkspace( const char* xml_file_name )
{
    g_MainWindow->m_EditWorkspaceName->addItem( xml_file_name );
    g_MainWindow->m_Workspaces.push_back( xml_file_name );

    return true;
}


bool
addAuthorIntoComboBox( const char* name )
{
    g_MainWindow->m_ComboBoxAuthors->addItem( QString( name ) );

    return true;
}


bool
addScenariiSetIntoComboBox( const char* name )
{
    g_MainWindow->m_ComboBoxScenariiSets->addItem( QString( name ) );

    return true;
}


bool
addCognitiveSystem( const char* name )
{
    uint32 taxon_id;

    if (g_MainWindow->m_Environment->fetchCognitiveSystemTaxonId( name, taxon_id ))
    {
        g_MainWindow->m_CognitiveSystemNames[taxon_id].push_back( QString( name ) );

        return true;
    }
    else
    {
        return false;
    }
}


bool
addSupervisionModule( const char* dll_name )
{
    PluginWrapper<AbstractSupervisionModule> p;

    p.setPluginSearchRootPath( g_MainWindow->m_PluginPath.c_str() );

    if (p.check( "ScenarioSupervision", dll_name ))
    {
        QString name;

        name = dll_name;
        name = name.left(name.lastIndexOf(PLUGIN_EXTENSION));
        g_MainWindow->m_SupervisionModuleNames.push_back( name.toStdString().c_str() );
        g_MainWindow->m_ComboBoxSupervisionModules->addItem( name );

        return true;
    }
    else
    {
        return false;
    }
}


//*****************************************************************************
//
//  Creation of the menus
//
//*****************************************************************************


void
MainWindow::createMenus()
{
    if (g_Settings.contains( "RecentSessions" ))
    {
        m_RecentSessions = g_Settings.value( "RecentSessions" ).toStringList();
    }

    m_MaxRecentSessionCount = MAX_RECENT_SESSION_COUNT;
    if (g_Settings.contains( "MaxRecentSessionCount" ))
    {
        m_MaxRecentSessionCount = g_Settings.value( "MaxRecentSessionCount" ).toInt();
    }

    // Menu File
    m_MenuFile = new QMenu( tr("&File"), this );

    m_MenuRecentSessions = new QMenu( "Recent sessions" );
    m_MenuFile->addMenu( m_MenuRecentSessions );

    updateRecentSessions();

    m_MenuFile->addSeparator();

    // Menu File->Quit
    m_ActionQuit = new QAction( tr("Quit"), this );
    m_ActionQuit->setShortcut(tr("Ctrl+Q"));
    connect( m_ActionQuit, SIGNAL(triggered()), this, SLOT(quit()) );
    m_MenuFile->addAction( m_ActionQuit );


    // Menu Help
    m_MenuHelp = new QMenu( tr( "&Help" ), this );

    m_ActionDisplayParameters = new QAction( tr("Display parameters"), this );
    m_ActionDisplayParameters->setShortcut(tr("Ctrl+1"));
    connect( m_ActionDisplayParameters, SIGNAL(triggered()), this, SLOT(displayParameters()) );
    m_MenuHelp->addAction( m_ActionDisplayParameters );

    m_MenuHelp->addSeparator();

    m_ActionAbout = new QAction( tr("&About %1...").arg( APP_NAME ), this );
    connect( m_ActionAbout, SIGNAL(triggered()), this, SLOT(about()) );
    m_MenuHelp->addAction( m_ActionAbout );

    menuBar()->addMenu( m_MenuFile );
    menuBar()->addMenu( m_MenuHelp );
}


void
MainWindow::displayParameters()
{
    g_ParameterRepository.displayControls();
    g_ParameterRepository.displayParameters();
}


void
MainWindow::createWidgetTabEnvironment()
{
    m_WidgetTabEnvironments = new QTabWidget();

    QWidget* env_selection = new QWidget();
    m_WidgetTabEnvironments->addTab( env_selection, "&Selection" );

    QGridLayout* layout_env_selection = new QGridLayout( env_selection );
    layout_env_selection->setAlignment( Qt::AlignTop );

    m_GroupboxEnvironments = new QGroupBox(tr("Environment"));

    m_WidgetEnvironments = new WidgetEnvironments( m_PluginPath.c_str() );
    m_WidgetEnvironments->attachView( 0, m_GroupboxEnvironments );


    // Workspaces
    m_GroupboxWorkspaces = new QGroupBox(tr("Workspace"));
    QGridLayout* layout_workspace = new QGridLayout( m_GroupboxWorkspaces );
    layout_workspace->setAlignment( Qt::AlignLeft );

    m_LabelWorkspaceName = new QLabel( "Name:" );

    m_EditWorkspaceName = new QComboBox();
    m_EditWorkspaceName->setEditable( true );
    m_EditWorkspaceName->setMinimumWidth( 160 );
    m_EditWorkspaceName->setFixedHeight( 30 );

    m_ButtonLoadWorkspace = new QPushButton();
    m_ButtonLoadWorkspace->setShortcut( QKeySequence(Qt::Key_Return) );
    m_ButtonLoadWorkspace->setIconSize( QSize( 40,40 ) );
    m_ButtonLoadWorkspace->setIcon( m_Icons[ICON_USE] );
    m_ButtonLoadWorkspace->setEnabled( false );

    m_ButtonResetWorkspaceData = new QPushButton();
    m_ButtonResetWorkspaceData->setIconSize( QSize( 40,40 ) );
    m_ButtonResetWorkspaceData->setIcon( m_Icons[ICON_RESET] );
    m_ButtonResetWorkspaceData->setToolTip( tr("Reset workspace data") );
    m_ButtonResetWorkspaceData->setEnabled( false );

    m_ButtonDeleteWorkspace = new QPushButton();
    m_ButtonDeleteWorkspace->setIconSize( QSize( 40,40 ) );
    m_ButtonDeleteWorkspace->setIcon( m_Icons[ICON_DELETE] );
    m_ButtonDeleteWorkspace->setToolTip( tr("Delete workspace") );
    m_ButtonDeleteWorkspace->setEnabled( false );

    layout_workspace->addWidget( m_LabelWorkspaceName, 0,0,2,1 );
    layout_workspace->addWidget( m_EditWorkspaceName, 0,1,2,1 );
    layout_workspace->addWidget( m_ButtonLoadWorkspace, 0,2,2,1 );
    layout_workspace->addWidget( m_ButtonResetWorkspaceData, 0,3,2,1 );
    layout_workspace->addWidget( m_ButtonDeleteWorkspace, 0,4,2,1 );


    layout_env_selection->addWidget( m_GroupboxEnvironments, 0,0,1,1 );
    layout_env_selection->addWidget( m_GroupboxWorkspaces, 1,0,1,1 );

    // Controls
    m_WidgetEnvironmentParameters = new QWidget();
    m_WidgetTabEnvironments->addTab( m_WidgetEnvironmentParameters, "&Parameters" );
    m_WidgetTabEnvironments->setTabEnabled( 1, false );


    connect( m_WidgetEnvironments, SIGNAL(updateWorkspaces()), this, SLOT(updateWorkspaces()) );
    connect( m_EditWorkspaceName, SIGNAL(editTextChanged(QString)), this, SLOT(onEditWorkspaceName(QString)) );
    connect( m_ButtonLoadWorkspace, SIGNAL(clicked()), this, SLOT(loadWorkspace()) );
    connect( m_ButtonResetWorkspaceData, SIGNAL(clicked()), this, SLOT(resetWorkspaceData()) );
    connect( m_ButtonDeleteWorkspace, SIGNAL(clicked()), this, SLOT(deleteWorkspace()) );
}


void
MainWindow::createWidgetTabSystemScheduling()
{
    m_WidgetTabSystemScheduling = new QTabWidget();

    QGridLayout* layout_system_scheduling = new QGridLayout( m_WidgetTabSystemScheduling );

    m_GroupboxSystemScheduling = new QGroupBox( tr( "Parameters" ) );
    layout_system_scheduling->addWidget( m_GroupboxSystemScheduling );
    layout_system_scheduling->setAlignment( Qt::AlignTop );

    m_PcSystemScheduling.setParameterFactory( &g_ParameterRepository );
    m_PcSystemScheduling.setPluginSearchRootPath( m_PluginPath.c_str() );
    m_PcSystemScheduling.setWorkspace( m_WorkspacePath.c_str() );
    if (m_PcSystemScheduling.load( "SystemScheduling", "SSM_Parametric" ))
    {
        m_SystemScheduling = reinterpret_cast<AbstractSystemScheduling*>(m_PcSystemScheduling.get());
        m_SystemScheduling->attachView( 0, m_GroupboxSystemScheduling );
    }
}


void
MainWindow::onEditWorkspaceName( QString text )
{
    initWorkspaceButtonStatus( text.toStdString().c_str() );
}


void
MainWindow::initWorkspaceButtonStatus( const char* workspace_name )
{
    bool state = (strlen( workspace_name ) == 0);

    m_ButtonLoadWorkspace->setDisabled( state );

    m_ButtonLoadWorkspace->setIcon( m_Icons[ICON_USE] );
    m_ButtonLoadWorkspace->setToolTip( tr("Create") );

    for( uint32 i = 0; i < m_Workspaces.size(); ++i )
    {
        if (!strcmp( workspace_name, m_Workspaces[i].toStdString().c_str() ))
        {
            m_ButtonLoadWorkspace->setIcon( m_Icons[ICON_LOAD] );
            m_ButtonLoadWorkspace->setToolTip( tr("Load") );
            break;
        }
    }

    m_WorkspacePath = g_PathWorkspaces;
    m_WorkspacePath += m_EnvironmentName;
    m_WorkspacePath += "/";
    m_WorkspacePath += workspace_name;
    m_WorkspacePath += "/";

    QDir dir( m_WorkspacePath.c_str() );
    QFileInfo file( dir, "environment.data" );

    m_ButtonResetWorkspaceData->setEnabled( file.exists() );
    m_ButtonDeleteWorkspace->setDisabled( state );
}


void
MainWindow::updateWorkspaces()
{
    m_EnvironmentName = m_WidgetEnvironments->getCurrentEnvironmentName();

    m_EditWorkspaceName->clear();
    if (strlen( m_EnvironmentName.c_str() ))
    {
        Ipseity::String path;

        path = g_PathWorkspaces;
        path += m_EnvironmentName;
        path += "/";

        FileSearch::lookForFiles( path.c_str(), FS_CALL_FUNCTION_FOR_DIRECTORY, fetchWorkspace );
    }

    initWorkspaceButtonStatus( m_EditWorkspaceName->currentText().toStdString().c_str() );
}


void
MainWindow::createWidgetTabBenchmark()
{
    m_WidgetTabBenchmark = new QWidget;
    QGridLayout* layout_simulation = new QGridLayout( m_WidgetTabBenchmark );


    layout_simulation->setColumnStretch( 0, 80 );
    layout_simulation->setColumnStretch( 1, 20 );
    layout_simulation->setHorizontalSpacing( 5 );


    // Scenarii
    m_GroupboxScenariiSets = new QGroupBox(tr("Set of scenarios"));
    QGridLayout* layout_scenarii = new QGridLayout(m_GroupboxScenariiSets);
    layout_scenarii->setAlignment( Qt::AlignLeft );

    m_ComboBoxAuthors = new ParameterComboBox( "Author", "Author:" );
    m_ComboBoxAuthors->setFixedWidth( 200 );
    m_ComboBoxAuthors->setEnabled( false );

    m_ComboBoxScenariiSets = new ParameterComboBox( "ProductionName", "Production Name:" );
    m_ComboBoxScenariiSets->setFixedWidth( 200 );
    m_ComboBoxScenariiSets->setEnabled( false );

    m_ButtonLoadScenariiSet = new ControlButton( "LoadScenarii", "Load", "L" );
    m_ButtonLoadScenariiSet->setIcon( m_Icons[ICON_USE] );
    m_ButtonLoadScenariiSet->setIconSize( QSize( 40,40 ) );
    m_ButtonLoadScenariiSet->setEnabled( false );

    m_ComboBoxAuthors->addToLayout( layout_scenarii, 0,0 );
    m_ComboBoxScenariiSets->addToLayout( layout_scenarii, 1,0 );
    layout_scenarii->addWidget( m_ButtonLoadScenariiSet, 0,2,2,1 );

    // Scenarios Management
    m_GroupboxScenariosManagement = new QGroupBox(tr("Scenarios Management"));
    layout_simulation->addWidget( m_GroupboxScenariosManagement, 1,0,4,1 );

    layout_simulation->addWidget( m_GroupboxScenariiSets, 0,0,1,1 );


    // Parameter Setting
//    m_GroupboxParameterSetting = new QGroupBox(tr("Parameter Setting"));
//    layout_simulation->addWidget( m_GroupboxParameterSetting, 1,0,1,1 );


    QGridLayout* gridlayout;

    // Supervision
    m_GroupBoxSupervision = new QGroupBox( "Supervision" );
    m_GroupBoxSupervision->setMaximumHeight( 160 );
    gridlayout = new QGridLayout( m_GroupBoxSupervision );
    gridlayout->setAlignment( Qt::AlignTop );

    QLabel* label_sm = new QLabel( "Modules:" );
    gridlayout->addWidget( label_sm, 0,0,1,1, Qt::AlignCenter );

    m_ComboBoxSupervisionModules = new QComboBox();
    m_ComboBoxSupervisionModules->setMinimumHeight( 30 );
    FileSearch::lookForFiles( g_PathSupervisionModules, FS_ITERATE_FILES | FS_RECURSIVE_PATHS, addSupervisionModule, PLUGIN_EXTENSION );
    gridlayout->addWidget( m_ComboBoxSupervisionModules, 1,0,1,1, Qt::AlignCenter );

    m_ButtonUseSupervisionModule = new QPushButton();
    m_ButtonUseSupervisionModule->setIcon( m_Icons[ICON_LOAD] );
    m_ButtonUseSupervisionModule->setIconSize( QSize( 40,40 ) );
    m_ButtonUseSupervisionModule->setMinimumSize( 64,64 );
    m_ButtonUseSupervisionModule->setToolTip( tr("Load") );
    m_ButtonUseSupervisionModule->setCheckable( true );
    m_ButtonUseSupervisionModule->setChecked( false );
    connect( m_ButtonUseSupervisionModule, SIGNAL(toggled(bool)), this, SLOT(useSupervisionModule(bool)) );
    gridlayout->addWidget( m_ButtonUseSupervisionModule, 2,0,1,1, Qt::AlignCenter );

    layout_simulation->addWidget( m_GroupBoxSupervision,0,1,1,1 );


    // Controls of the simulation
    m_GroupBoxSimulation = new QGroupBox( tr("Simulation") );
    m_GroupBoxSimulation->setMaximumWidth( 200 );
    gridlayout = new QGridLayout( m_GroupBoxSimulation );
    gridlayout->setAlignment( Qt::AlignTop );
    gridlayout->setHorizontalSpacing( 5 );

    m_ButtonSimulateScenarii = new ControlButton( "Simulate", "Play", "Ret" );
    m_ButtonSimulateScenarii->setIcon( m_Icons[ICON_SIMULATE] );
    m_ButtonSimulateScenarii->setIconSize( QSize( 40,40 ) );
    m_ButtonSimulateScenarii->setCheckable(true);
    connect(m_ButtonSimulateScenarii,SIGNAL(toggled(bool)),this,SLOT(simulateScenarii(bool)));
    gridlayout->addWidget(m_ButtonSimulateScenarii,0,0,1,1, Qt::AlignCenter);

    m_ButtonStop = new ControlButton( "Stop", "Stop", "S" );
    m_ButtonStop->setIcon( m_Icons[ICON_STOP] );
    m_ButtonStop->setIconSize( QSize( 40,40 ) );
    m_ButtonStop->setCheckable(false);
    m_ButtonStop->setEnabled( false );
    connect(m_ButtonStop,SIGNAL(clicked()),this,SLOT(stopSimulation()));
    gridlayout->addWidget(m_ButtonStop,0,2,1,1, Qt::AlignCenter);


    m_CheckboxResetCSEveryNewTrial = new ParameterCheckBox( "ResetCSOnNewTrial", "Reset CS every new trial", "R" );
    m_CheckboxResetCSEveryNewTrial->setChecked( true );
    gridlayout->addWidget(m_CheckboxResetCSEveryNewTrial,1,0,1,5, Qt::AlignLeft);

    m_CheckboxActionEveryNEpisodes = new QCheckBox( "Every" );
    m_CheckboxActionEveryNEpisodes->setFixedWidth( 60 );
    connect( m_CheckboxActionEveryNEpisodes, SIGNAL(toggled(bool)), this, SLOT(onActionEpisodeFrequencyChanged(bool)) );
    gridlayout->addWidget(m_CheckboxActionEveryNEpisodes,2,0,1,1, Qt::AlignLeft);
    m_SpinBoxActionOnEpisodes = new QSpinBox();
    m_SpinBoxActionOnEpisodes->setMinimumSize( 40, 30 );
    m_SpinBoxActionOnEpisodes->setMinimum( 1 );
    m_SpinBoxActionOnEpisodes->setMaximum( cstMaxEpisodes );
    m_SpinBoxActionOnEpisodes->setValue( 1 );
    m_SpinBoxActionOnEpisodes->setEnabled( false );
    gridlayout->addWidget(m_SpinBoxActionOnEpisodes,2,2,1,1, Qt::AlignLeft);
    m_LabelActionEveryNEpisodes = new QLabel( "Episodes" );
    m_LabelActionEveryNEpisodes->setEnabled( false );
    gridlayout->addWidget(m_LabelActionEveryNEpisodes,2,3,1,2, Qt::AlignLeft);

    m_ComboboxActionEveryNEpisodes = new QComboBox();
    m_ComboboxActionEveryNEpisodes->setMinimumSize( 120, 30 );
    m_ComboboxActionEveryNEpisodes->addItem( "Pause" );
    m_ComboboxActionEveryNEpisodes->setEnabled( false );
    gridlayout->addWidget(m_ComboboxActionEveryNEpisodes,3,0,1,4, Qt::AlignCenter);

    m_SpinboxRealTimeSpan = new ParameterSpinBox( "RealTimeSpan", "ms" );
    m_SpinboxRealTimeSpan->setFixedWidth( 60 );
    m_SpinboxRealTimeSpan->setRange(1,10000);
    m_SpinboxRealTimeSpan->setValue(cstDefaultSpeed);
    connect( m_SpinboxRealTimeSpan,SIGNAL(valueChanged(int)),this,SLOT(realtimeSpanChanged(int)) );

    m_CheckboxRealTimeSpan = new ParameterCheckBox( "UseRealTimeSpan", "Real Time\nSpan:" );
    m_CheckboxRealTimeSpan->setFixedWidth( 160 );
    m_CheckboxRealTimeSpan->setChecked( true );
    connect( m_CheckboxRealTimeSpan, SIGNAL(toggled(bool)), this, SLOT(realtimeSpanEnabled(bool)) );
    gridlayout->addWidget( m_CheckboxRealTimeSpan,4,0,1,2, Qt::AlignLeft );
    m_SpinboxRealTimeSpan->addToLayout( gridlayout,4,2,false );

    m_SpinboxSimulatedTimeSpan = new ParameterSpinBox( "SimulatedTimeSpan", "ms" );
    m_SpinboxSimulatedTimeSpan->setRange(1,100000);
    m_SpinboxSimulatedTimeSpan->setValue(cstDefaultSpeed);
    m_SpinboxSimulatedTimeSpan->setFixedWidth( 60 );
    m_CheckboxSimulatedTimeSpan = new ParameterCheckBox( "UseSimulatedTimeSpan", "Simulated\nTime Span:" );
    m_CheckboxSimulatedTimeSpan->setFixedWidth( 160 );
    m_CheckboxSimulatedTimeSpan->setChecked( true );
    connect( m_CheckboxSimulatedTimeSpan, SIGNAL(toggled(bool)), this, SLOT(simulatedTimeSpanEnabled(bool)) );
    gridlayout->addWidget( m_CheckboxSimulatedTimeSpan,5,0,1,2, Qt::AlignLeft );
    m_SpinboxSimulatedTimeSpan->addToLayout( gridlayout,5,2, false );

    m_LabelElapsedTime = new QLabel( "Elapsed Time:" );
    gridlayout->addWidget( m_LabelElapsedTime,6,0,1,4, Qt::AlignLeft );

    layout_simulation->addWidget( m_GroupBoxSimulation, 1,1,2,1 );


    // Rendering
    m_GroupBoxRenderingWindows = new QGroupBox( "Rendering Windows" );
    m_LayoutRenderingWindows = new QGridLayout( m_GroupBoxRenderingWindows );
    m_LayoutRenderingWindows->setAlignment( Qt::AlignTop );


    uint32 frame_rate = 60;

    m_RenderTimer->setInterval( getTimeInterval( frame_rate ) );

    m_SliderFrameRate = new ParameterSlider( "FrameRate", "FPS:\n%1 Hz", Qt::Horizontal, 50 );
    m_SliderFrameRate->setRange(1,100);
    m_SliderFrameRate->setValue( frame_rate);
    m_SliderFrameRate->setTickPosition(QSlider::TicksBelow);
    m_SliderFrameRate->setTickInterval(10);
    connect(m_SliderFrameRate,SIGNAL(valueChanged(int)),this,SLOT(frameRateChanged(int)));
    m_SliderFrameRate->addToLayout( m_LayoutRenderingWindows, 0,0 );

    m_CheckboxMainRenderingWindows = new ParameterCheckBox( "FlagMainWindows", "Main Windows", "Ctrl+W" );
    connect(m_CheckboxMainRenderingWindows,SIGNAL(toggled(bool)),this,SLOT(renderAllWindows(bool)));
    m_LayoutRenderingWindows->addWidget( m_CheckboxMainRenderingWindows,1,0,1,2 );

    m_GroupCheckboxRenderingWindows = new QButtonGroup();
    m_GroupCheckboxRenderingWindows->setExclusive( false );
    connect( m_GroupCheckboxRenderingWindows, SIGNAL(buttonClicked(int)), this, SLOT(renderWindow(int)) );

    layout_simulation->addWidget( m_GroupBoxRenderingWindows, 3,1,2,1 );

    connect( m_ComboBoxAuthors, SIGNAL(currentIndexChanged(int)), this, SLOT(changeComboBoxAuthorIndex(int)) );
    connect( m_ComboBoxScenariiSets, SIGNAL(currentIndexChanged(int)), this, SLOT(changeComboBoxScenariiSetIndex(int)));
    connect( m_ButtonLoadScenariiSet, SIGNAL(clicked()), this, SLOT(loadScenarii()) );
}


void
MainWindow::createWidgetTabTaxons()
{
    m_WidgetTabTaxons = new QWidget;
    QGridLayout* layout_environment_taxons = new QGridLayout( m_WidgetTabTaxons );

    QGroupBox* groupbox_cs_per_taxon = new QGroupBox(tr("List of Cognitive Systems per Taxon"));
    QGridLayout* taxon_layout = new QGridLayout(groupbox_cs_per_taxon);

    m_WidgetTaxonsList = new WidgetTaxonsList();
    connect( m_WidgetTaxonsList, SIGNAL(itemSelectionChanged()), this, SLOT(updateCognitiveSystemButtons()) );
    connect( m_WidgetTaxonsList, SIGNAL(currentNameIndexChanged(int,int)), this, SLOT(updateCognitiveSystem(int,int)) );
    connect( m_WidgetTaxonsList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(plugCurrentCognitiveSystem(QTreeWidgetItem*,int)) );

    QWidget* widget_taxon_controls = new QWidget;
    QVBoxLayout* tc_layout = new QVBoxLayout(widget_taxon_controls);
    tc_layout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    m_ButtonPlugCognitiveSystems = new ControlButton( "PlugCS", "Plug the selected cognitive system(s)", "P" );
    m_ButtonPlugCognitiveSystems->setIconSize( QSize( 40,40 ) );
    m_ButtonPlugCognitiveSystems->setIcon( m_Icons[ICON_PLUG_CS] );
    connect( m_ButtonPlugCognitiveSystems, SIGNAL(clicked()), this, SLOT(plugSelectedCognitiveSystems()) );
    tc_layout->addWidget( m_ButtonPlugCognitiveSystems );

    m_ButtonResetCognitiveSystems = new ControlButton( "ResetCS", "Reset the selected cognitive system(s)", "R" );
    m_ButtonResetCognitiveSystems->setIconSize( QSize( 40,40 ) );
    m_ButtonResetCognitiveSystems->setIcon( m_Icons[ICON_RESET] );
    connect( m_ButtonResetCognitiveSystems, SIGNAL(clicked()), this, SLOT(resetSelectedCognitiveSystems()) );
    tc_layout->addWidget( m_ButtonResetCognitiveSystems );

    m_ButtonShareCognitiveSystems = new ControlButton( "ShareCS", "Toggle the share property of the\nselected cognitive system(s)", "S" );
    m_ButtonShareCognitiveSystems->setIconSize( QSize( 40,40 ) );
    m_ButtonShareCognitiveSystems->setIcon( m_Icons[ICON_SHARE_CS] );
    connect( m_ButtonShareCognitiveSystems, SIGNAL(clicked()), this, SLOT(toggleShareSelectedCognitiveSystems()) );
    tc_layout->addWidget( m_ButtonShareCognitiveSystems );

    taxon_layout->addWidget( m_WidgetTaxonsList );

    layout_environment_taxons->addWidget( widget_taxon_controls,0,0 );
    layout_environment_taxons->addWidget( groupbox_cs_per_taxon,0,1 );
}

void
MainWindow::createWidgetTabCognitiveSystems()
{
    m_WidgetTabCognitiveSystemSetPerTaxon = new QTabWidget();
    m_WidgetTabCognitiveSystemSetPerTaxon->setUsesScrollButtons( true );

    m_WidgetTabCognitiveSystemSetPerAgent = new QTabWidget();
    m_WidgetTabCognitiveSystemSetPerAgent->setUsesScrollButtons( true );

    m_WidgetTabCognitiveSystemType = new QTabWidget();
    m_WidgetTabCognitiveSystemType->setTabPosition( QTabWidget::West );
    m_WidgetTabCognitiveSystemType->setUsesScrollButtons( true );
    m_WidgetTabCognitiveSystemType->addTab( m_WidgetTabCognitiveSystemSetPerTaxon, tr("Per Taxon") );
    m_WidgetTabCognitiveSystemType->addTab( m_WidgetTabCognitiveSystemSetPerAgent, tr("Per Agent") );
}

void
MainWindow::createWidgetTabAgents()
{
    m_WidgetTabAgents = new QWidget();

    QGridLayout* layout_agents = new QGridLayout( m_WidgetTabAgents );

    m_WidgetAgentsList = new WidgetAgentsList();

    layout_agents->addWidget( m_WidgetAgentsList );
}

void
MainWindow::createCentralWidget()
{
    setParameterRepository( &g_ParameterRepository );

    m_WidgetMainTab = new QTabWidget(this);
    m_WidgetMainTab->setMovable( true );

    createWidgetTabEnvironment();
    createWidgetTabSystemScheduling();
    createWidgetTabTaxons();
    createWidgetTabAgents();
    createWidgetTabCognitiveSystems();
    createWidgetTabBenchmark();

    m_WidgetMainTab->addTab( m_WidgetTabEnvironments, tr("&Environments") );
    m_WidgetMainTab->addTab( m_WidgetTabTaxons, tr("&Taxons") );
    m_WidgetMainTab->addTab( m_WidgetTabAgents, tr("&Agents") );
    m_WidgetMainTab->addTab( m_WidgetTabCognitiveSystemType, tr("&Cognitive Systems") );
    m_WidgetMainTab->addTab( m_WidgetTabSystemScheduling, tr("S&ystem Scheduling") );
    m_WidgetMainTab->addTab( m_WidgetTabBenchmark, tr("&Benchmark") );

    setCentralWidget( m_WidgetMainTab );

    onEnvironmentClosed();
}


//*****************************************************************************
//
//  Quit
//
//*****************************************************************************


void
MainWindow::quit()
{
    close();
}


//*****************************************************************************
//
//  Session
//
//*****************************************************************************


void
MainWindow::updateRecentSessions()
{
    while (m_RecentSessions.size() > m_MaxRecentSessionCount)
    {
        m_RecentSessions.removeFirst();
    }

    m_MenuRecentSessions->clear();

    if (m_RecentSessions.size())
    {
        m_MenuRecentSessions->setEnabled( true );

        SafeDelete( m_SessionGroup );
        m_SessionGroup = new QActionGroup( this );

        QAction* action;

        foreach( QString line, m_RecentSessions )
        {
            action = new QAction( line.toStdString().c_str(), m_MenuRecentSessions );
            m_SessionGroup->addAction( action );
            m_MenuRecentSessions->addAction( action );
        }
        connect( m_SessionGroup, SIGNAL(triggered(QAction*)), this, SLOT(selectSession(QAction*)));

        m_MenuRecentSessions->addSeparator();

        QAction* action_clear_recent_sessions = new QAction( "Clear sessions", m_MenuRecentSessions );
        m_MenuRecentSessions->addAction( action_clear_recent_sessions );
        connect( action_clear_recent_sessions, SIGNAL(triggered()), this, SLOT(clearRecentSessions()) );
    }
    else
    {
        m_MenuRecentSessions->setEnabled( false );
    }
}


void
MainWindow::clearRecentSessions()
{
    m_RecentSessions.clear();
    updateRecentSessions();
}


void
MainWindow::addToRecentSessions( const char* env_name, const char* workspace_name )
{
    QString name;

    name = env_name;
    name += "/";
    name += workspace_name;

    if (!m_RecentSessions.contains( name ))
    {
        m_RecentSessions << name;
    }

    updateRecentSessions();
}


void
MainWindow::loadSession( const char* env_name, const char* workspace_name )
{
    int32 env_id;

    if (m_WidgetEnvironments->existsEnvironment( env_name, env_id ))
    {
        m_WidgetEnvironments->setSelectedEnvironment( env_id );
        m_EditWorkspaceName->setEditText( workspace_name );
        loadEnvironment( env_id );
        chooseDefaultTaxons();
        loadScenarii();
    }
}


void
MainWindow::selectSession( QAction* action )
{
    QString env_name = action->text().section( "/", 0, 0 );
    QString workspace_name = action->text().section( "/", 1 );

    loadSession( env_name.toStdString().c_str(), workspace_name.toStdString().c_str() );
}


//*****************************************************************************
//
//  Workspaces
//
//*****************************************************************************


void
MainWindow::loadWorkspace()
{
    m_ButtonResetWorkspaceData->setDisabled( true );
    m_ButtonDeleteWorkspace->setDisabled( true );
    loadEnvironment();
}


void
MainWindow::resetWorkspaceData()
{
    FileSearch::removeFilesRecursively( m_WorkspacePath.c_str(), ".ini", true );
    m_ButtonResetWorkspaceData->setDisabled( true );
}


void
MainWindow::deleteWorkspace()
{
    FileSearch::removeFilesRecursively( m_WorkspacePath.c_str(), "", true );
    m_ButtonDeleteWorkspace->setDisabled( true );

    QDir dir( m_WorkspacePath.c_str() );

    dir.rmdir( "." );

    updateWorkspaces();
}


//*****************************************************************************
//
//  Environment
//
//*****************************************************************************


void
MainWindow::onEnvironmentClosed()
{
    m_WidgetMainTab->setTabEnabled( TABID_TAXONS, false );
    m_WidgetMainTab->setTabEnabled( TABID_AGENTS, false );
    m_WidgetMainTab->setTabEnabled( TABID_COGNITIVESYSTEMS, false );
    m_WidgetMainTab->setTabEnabled( TABID_SYSTEM_SCHEDULING, false );
    m_WidgetMainTab->setTabEnabled( TABID_BENCHMARK, false );
    m_WidgetTabEnvironments->setTabEnabled( 1, false );
    m_WidgetTabEnvironments->setTabEnabled( 2, false );

    m_ComboBoxAuthors->setEnabled( false );
    m_ComboBoxAuthors->clear();
    m_ComboBoxScenariiSets->setEnabled( false );
    m_ComboBoxScenariiSets->clear();
    m_ButtonLoadScenariiSet->setEnabled( false );

    m_ButtonUseSupervisionModule->setChecked( false );

    updateScenariiList();
    cleanCognitiveSystems();
    updateTaxonsList();
}


void
MainWindow::onEnvironmentLoaded()
{
    m_WidgetMainTab->setTabEnabled( TABID_TAXONS, true );
    m_WidgetMainTab->setTabEnabled( TABID_BENCHMARK, true );

    bool display;

    // Rendering
    display = (m_Environment->getNbrMainRenderingWindows() > 0);
    m_CheckboxMainRenderingWindows->setEnabled( display );

    loadLastSupervisionModule();

    if (m_SupervisionModule)
    {
        if (!m_Environment->requireScenarioFile())
        {
            m_SupervisionModule->addScenario( "DefaultScenario" );
        }
    }

    updateScenariiList();
    updateTaxonsList();

    openSettings( m_WorkspacePath.c_str() );
}


void
MainWindow::loadEnvironment()
{
    if (m_Environment)
    {
        m_ButtonLoadWorkspace->setIcon( m_Icons[ICON_LOAD] );
        m_WidgetEnvironments->setEnabled( true );

        closeEnvironment();
    }
    else
    {
        loadEnvironment( m_WidgetEnvironments->getCurrentEnvironmentId() );
    }
}


void
MainWindow::loadEnvironment( int32 env_id )
{
    closeEnvironment();

    QString dll_name = m_WidgetEnvironments->getEnvironmentFileName( env_id );

    m_EnvironmentBasePath = m_CurrentPath;
    m_EnvironmentBasePath += "/";
    m_EnvironmentBasePath += m_WidgetEnvironments->getEnvironmentPathName( env_id );

    if (!dll_name.isEmpty())
    {
        m_PcEnvironment.setParameterFactory( &g_ParameterRepository );
        m_PcEnvironment.setPluginSearchRootPath( m_PluginPath.c_str() );
        m_PcEnvironment.setWorkspace( m_WorkspacePath.c_str() );
        if (m_PcEnvironment.load( "Environment", "MultiAgentEnvironment" ))
        {
            m_Environment = reinterpret_cast<AbstractEnvironment*>(m_PcEnvironment.get());
        }

        if (m_Environment)
        {
            m_SimulationInProgress = false;

            QDir().mkpath( m_WorkspacePath.c_str() );
//			deleteRenderingWidgets();

            installDebugHandler();

            m_Environment->setAgentManager( this );
            m_Environment->setSystemScheduling( m_SystemScheduling );
            m_Environment->initialize( m_EnvironmentBasePath.c_str(), dll_name.toStdString().c_str(), m_WidgetEnvironments->getEnvironmentConfigFileName( env_id ) );
            m_Name = m_Environment->getName();

//            cout << m_WidgetEnvironments->getEnvironmentName( env_id ) << endl;
//            cout << m_Environment->getBaseName() << endl;

            addToRecentSessions( m_WidgetEnvironments->getEnvironmentName( env_id ), m_EditWorkspaceName->currentText().toStdString().c_str() );

            m_WidgetTabEnvironments->setTabEnabled( 1, m_Environment->attachView( 0, m_WidgetEnvironmentParameters ) );

            m_CognitiveSystemsPath = g_PathCognitiveSystems;
            m_CognitiveSystemsPath += m_WidgetEnvironments->getEnvironmentName( env_id );
            m_CognitiveSystemsPath += "/";

            if (m_Environment->requireScenarioFile())
            {
                m_ComboBoxAuthors->setEnabled( true );
                m_ComboBoxAuthors->clear();
                m_ComboBoxScenariiSets->setEnabled( false );

                m_ScenariiPath = g_PathScenarios;
                m_ScenariiPath += m_WidgetEnvironments->getEnvironmentName( env_id );
                m_ScenariiPath += "/";

//                cout << "Scenarii path: " << m_ScenariiPath.c_str() << endl;

                FileSearch::lookForFiles( m_ScenariiPath.c_str(), FS_CALL_FUNCTION_FOR_DIRECTORY, addAuthorIntoComboBox );
            }

            onEnvironmentLoaded();

            activateWindow();

            m_ButtonLoadWorkspace->setIcon( m_Icons[ICON_LOADED] );
            m_ButtonLoadWorkspace->setToolTip( tr("Close") );
            m_ButtonResetWorkspaceData->setEnabled( false );
            m_ButtonDeleteWorkspace->setEnabled( false );
            m_WidgetEnvironments->setEnabled( false );
        }
        else
        {
            m_ComboBoxAuthors->setEnabled( false );
            m_ComboBoxAuthors->clear();
            m_ComboBoxScenariiSets->setEnabled( false );
            m_ComboBoxScenariiSets->clear();
            m_ButtonLoadScenariiSet->setEnabled( false );
            m_WidgetMainTab->setCurrentIndex( TABID_TAXONS );
        }
    }
}


void
MainWindow::closeEnvironment()
{
    if (m_Environment)
    {
        stopTimers();
        for( uint32 window_id = 0; window_id < m_RenderingWidgets.size(); ++window_id )
        {
            m_RenderingWidgets[window_id]->setVisible( false );
        }

        closeSettings();

        cout << "Name: " << m_Environment->getName() << endl;

        uninstallDebugHandler();
    }

    deleteRenderingWidgets();
    m_PcEnvironment.free();
    m_Environment = NULL;

    onEnvironmentClosed();

    initWorkspaceButtonStatus( m_EditWorkspaceName->currentText().toStdString().c_str() );
}


void
MainWindow::changeComboBoxAuthorIndex( int index )
{
    m_LastAuthorId = index;

    m_ComboBoxScenariiSets->clear();
    m_ComboBoxScenariiSets->setEnabled( true );

    if (m_Environment)
    {
        Ipseity::String path;

        path = m_ScenariiPath;
        path += m_ComboBoxAuthors->currentText().toStdString().c_str();
        path += "/";

        FileSearch::lookForFiles( path.c_str(), FS_CALL_FUNCTION_FOR_DIRECTORY, addScenariiSetIntoComboBox );
        m_ComboBoxScenariiSets->setCurrentIndex( 0 );

        m_ButtonLoadScenariiSet->setEnabled( (m_ComboBoxScenariiSets->count() > 0) );
    }
}


void
MainWindow::changeComboBoxScenariiSetIndex( int index )
{
    m_LastScenariiSetId = index;
}


//*****************************************************************************
//
//  Supervision Module
//
//*****************************************************************************


void
MainWindow::useSupervisionModule( bool toggled )
{
    if (toggled)
    {
        m_PcSupervisionModule.setParameterFactory( &g_ParameterRepository );
        m_PcSupervisionModule.setPluginSearchRootPath( m_PluginPath.c_str() );
        m_PcSupervisionModule.setWorkspace( m_WorkspacePath.c_str() );
        bool ret = m_PcSupervisionModule.load( "ScenarioSupervision", m_SupervisionModuleNames[m_ComboBoxSupervisionModules->currentIndex()].toStdString().c_str() );

        if (ret)
        {
            m_SupervisionModule = reinterpret_cast<AbstractSupervisionModule*>(m_PcSupervisionModule.get());

//            m_SupervisionModule->attachView( 0, m_GroupboxParameterSetting );
            m_SupervisionModule->attachView( 0, m_GroupboxScenariosManagement );
            updateScenariiList();
        }

        m_ButtonUseSupervisionModule->setIcon( m_Icons[ICON_LOADED] );
        m_ButtonUseSupervisionModule->setToolTip( "Close" );

        m_ButtonSimulateScenarii->setEnabled( true );
    }
    else
    {
        m_PcSupervisionModule.free();
        m_SupervisionModule = NULL;

        m_ButtonUseSupervisionModule->setIcon( m_Icons[ICON_LOAD] );
        m_ButtonUseSupervisionModule->setToolTip( "Use" );

        m_ButtonSimulateScenarii->setEnabled( false );
    }

    m_ComboBoxSupervisionModules->setDisabled( toggled );
}


void
MainWindow::loadLastSupervisionModule()
{
    if (g_Settings.contains("ScenarioSupervisionModule"))
    {
        Ipseity::String name;
        uint32 index;

        name = g_Settings.value("ScenarioSupervisionModule").toString().toStdString().c_str();

        for( index = 0; index < m_ComboBoxSupervisionModules->count(); ++index )
        {
            if (!strcmp( m_ComboBoxSupervisionModules->itemText( index ).toStdString().c_str(), name.c_str() ))
            {
                m_ComboBoxSupervisionModules->setCurrentIndex( index );
                m_ButtonUseSupervisionModule->setChecked( true );
                break;
            }
        }
    }
}


//*****************************************************************************
//
//  Scenarii
//
//*****************************************************************************


void
MainWindow::loadScenarii()
{
    loadScenarii( m_ComboBoxAuthors->currentText().toStdString().c_str(), m_ComboBoxScenariiSets->currentText().toStdString().c_str() );
}


void
MainWindow::loadScenarii( const char* author_name, const char* production_name )
{
    if (m_SupervisionModule)
    {
        QString name;

        name = m_ScenariiPath.c_str();
        name += author_name;
        name += "/";
        name += production_name;
        name += "/";
        name += "Scenarii.xml";

        QDir currentPath1( QDir::currentPath() );
        QDir currentPath2( m_Environment->getBasePath() );
        m_SupervisionModule->readScenarii( m_Environment->getBasePath(), qPrintable(currentPath2.relativeFilePath(currentPath1.absoluteFilePath(name))) );

        updateScenariiList();
    }
}


void
MainWindow::updateScenariiList()
{
    if (m_Environment)
    {
        bool display;

        display = (m_SupervisionModule && m_SupervisionModule->canSimulate());
//        display = m_SupervisionModule->canSimulate();
        m_WidgetMainTab->setTabEnabled( TABID_AGENTS, display );
        m_WidgetMainTab->setTabEnabled( TABID_SYSTEM_SCHEDULING, display );
//        m_GroupBoxSupervision->setEnabled( display );

//        display = (m_SupervisionModule && m_SupervisionModule->canSimulate());
        m_ButtonSimulateScenarii->setEnabled( display );

//        m_LabelScenarioDescriptionContent->clear();
    }
}


void
MainWindow::retryScenario()
{
    if (!m_SupervisionModule->isEnded())
    {
        m_SupervisionModule->restartCurrentScenario();
        m_Environment->startSimulation();
    }
}

void
MainWindow::previousScenario()
{
    if (!m_SupervisionModule->isEnded())
    {
        m_SupervisionModule->gotoPreviousScenario();
        m_Environment->startSimulation();
    }
}

void
MainWindow::nextScenario()
{
    if (!m_SupervisionModule->isEnded())
    {
        m_SupervisionModule->gotoNextScenario();
        m_Environment->startSimulation();
    }
}


//*****************************************************************************
//
//  Taxons
//
//*****************************************************************************


void
MainWindow::chooseDefaultTaxons()
{
    for( uint32 taxon_id = 0; taxon_id < m_Environment->getNbrTaxons(); ++taxon_id )
    {
        plugCognitiveSystem( taxon_id );
    }

    updateWidgetTabCognitiveSystems();
    updateCognitiveSystemButtons();
}


void
MainWindow::updateTaxonsList()
{
    m_WidgetTaxonsList->clear();
    m_WidgetTabCognitiveSystemSetPerTaxon->clear();
//    m_WidgetTabCognitiveSystemSet->setFixedSize( 600, 500 );

    if (m_Environment)
    {
        FileSearch::lookForFiles( m_CognitiveSystemsPath.c_str(), FS_USE_LONGNAMES | FS_ITERATE_FILES, addCognitiveSystem, g_CognitiveSystemExt, 0 );

        QFileInfo fi;
        QWidget* widget_taxon;
        Taxon taxon;

        for( uint32 i = 0; i < m_Environment->getNbrTaxons(); i++ )
        {
            m_Environment->fetchTaxon( i, taxon );

            QString name( taxon.Name.c_str() );

            m_WidgetTaxonsList->addListItem( name, taxon.Status );

            widget_taxon = new QWidget();
            m_WidgetTabCognitiveSystemSetPerTaxon->addTab( widget_taxon, taxon.Name.c_str() );
            m_WidgetTabCognitiveSystemSetPerTaxon->setTabEnabled( i, false );
            m_WidgetTabCognitiveSystemSetPerTaxon->adjustSize();

            if (m_CognitiveSystemNames.find(i) != m_CognitiveSystemNames.end())
            {
                for( uint32 k = 0; k < m_CognitiveSystemNames[i].size(); ++k )
                {
                    fi = m_CognitiveSystemNames[i][k];

                    m_WidgetTaxonsList->addCognitiveSystem( fi.baseName().toStdString().c_str() );
                }
            }
        }
    }
}


//*****************************************************************************
//
//  Agents
//
//*****************************************************************************


void
MainWindow::onAgentAdded( uint32 agent_id, uint32 taxon_id )
{
    cout << "Agent added: " << agent_id << endl;

    Taxon taxon;

    if (m_Environment->fetchTaxon( taxon_id, taxon ))
    {
        m_WidgetAgentsList->addListItem( agent_id, m_Environment->getAgent( agent_id )->getName(), taxon.Name.c_str() );

        if (m_PluggedCognitiveSystems[taxon_id] && !(m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem))
        {
            if (loadCognitiveSystem( false, agent_id ))
            {
                updateWidgetTabCognitiveSystems();
            }
        }
    }
}


void
MainWindow::onAgentRemoved( uint32 agent_id )
{
    cout << "Agent removed: " << agent_id << endl;
}


//*****************************************************************************
//
//  Cognitive systems
//
//*****************************************************************************


void
MainWindow::plugCurrentCognitiveSystem( QTreeWidgetItem* item, int )
{
    uint32 taxon_id = m_WidgetTaxonsList->indexOfTopLevelItem( item );
    bool ret;

    if (m_CognitiveSystemNames.find( taxon_id ) != m_CognitiveSystemNames.end())
    {
        if (!m_PluggedCognitiveSystems[taxon_id])
        {
            if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem)
            {
                deleteCognitiveSystem( true, taxon_id );
                ret = loadCognitiveSystem( true, taxon_id );

                m_WidgetTabCognitiveSystemSetPerTaxon->setTabEnabled( taxon_id, ret );
                if (ret)
                {
                    m_WidgetTabCognitiveSystemSetPerTaxon->setCurrentIndex( taxon_id );
                }

                cout << "CS linked to taxon #" << taxon_id << endl;
            }
            else
            {
                ret = true;
            }

            m_PluggedCognitiveSystems[taxon_id] = ret;
            m_WidgetTaxonsList->setCognitiveSystemEnabled( taxon_id, !ret );
        }
        else
        {
            if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem)
            {
                deleteCognitiveSystem( true, taxon_id );
            }

            m_PluggedCognitiveSystems[taxon_id] = false;
            m_WidgetTaxonsList->setCognitiveSystemEnabled( taxon_id, true );
        }
    }

    updateWidgetTabCognitiveSystems();
    updateCognitiveSystemButtons();
}


void
MainWindow::plugSelectedCognitiveSystems()
{
    QList<QTreeWidgetItem*> list = m_WidgetTaxonsList->selectedItems();

    if (list.size())
    {
        uint32 taxon_id;

        for( uint32 i = 0; i < list.size(); ++i )
        {
            taxon_id = m_WidgetTaxonsList->indexOfTopLevelItem( list.at( i ) );

            plugCognitiveSystem( taxon_id );
        }

        updateWidgetTabCognitiveSystems();
        updateCognitiveSystemButtons();
    }
}


void
MainWindow::plugCognitiveSystem( uint32 taxon_id )
{
    if (m_CognitiveSystemNames.find( taxon_id ) != m_CognitiveSystemNames.end())
    {
        if (!m_PluggedCognitiveSystems[taxon_id])
        {
            bool ret;

            if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem)
            {
                deleteCognitiveSystem( true, taxon_id );
                ret = loadCognitiveSystem( true, taxon_id );

                m_WidgetTabCognitiveSystemSetPerTaxon->setTabEnabled( taxon_id, ret );
                if (ret)
                {
                    m_WidgetTabCognitiveSystemSetPerTaxon->setCurrentIndex( taxon_id );
                }

                cout << "CS linked to taxon #" << taxon_id << endl;
            }
            else
            {
                ret = true;
            }

            m_PluggedCognitiveSystems[taxon_id] = ret;
            m_WidgetTaxonsList->setCognitiveSystemEnabled( taxon_id, !ret );
        }
        else
        {
            if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem)
            {
                deleteCognitiveSystem( true, taxon_id );
            }

            m_PluggedCognitiveSystems[taxon_id] = false;
            m_WidgetTaxonsList->setCognitiveSystemEnabled( taxon_id, true );
        }
    }
}

void
MainWindow::resetSelectedCognitiveSystems()
{
    QList<QTreeWidgetItem*> list = m_WidgetTaxonsList->selectedItems();

    if (list.size())
    {
        uint32 taxon_id;

        for( uint32 i = 0; i < list.size(); ++i )
        {
            taxon_id = m_WidgetTaxonsList->indexOfTopLevelItem( list.at( i ) );

            if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem &&
                m_CognitiveSystemNames.find( taxon_id ) != m_CognitiveSystemNames.end())
            {
                m_CognitiveSystemsPerTaxon[taxon_id]->reset();
            }
        }

        m_ButtonResetCognitiveSystems->setEnabled( false );
    }
}


void
MainWindow::cleanCognitiveSystems()
{
    cleanCognitiveSystemsPerTaxon();
    cleanCognitiveSystemsPerAgent();
}


void
MainWindow::cleanCognitiveSystemsPerTaxon()
{
    m_CognitiveSystemNames.clear();
    m_CognitiveSystemIndex.clear();
    m_PluggedCognitiveSystems.clear();

    map<uint32,PluginWrapper<AbstractCognitiveSystem>* >::iterator itr;
    uint32 taxon_id;

    for( itr = m_PiCognitiveSystemsPerTaxon.begin(); itr != m_PiCognitiveSystemsPerTaxon.end(); ++itr )
    {
        taxon_id = (*itr).first;

        qDebug() << "Delete CS of taxon #" << taxon_id;

        (*itr).second->free();
        SafeDelete( (*itr).second );
    }
    m_PiCognitiveSystemsPerTaxon.clear();
    m_CognitiveSystemsPerTaxon.clear();
}


void
MainWindow::cleanCognitiveSystemsPerAgent()
{
    m_WidgetAgentsList->clear();
    m_WidgetTabCognitiveSystemSetPerAgent->clear();

    map<uint32,PluginWrapper<AbstractCognitiveSystem>* >::iterator itr;
    uint32 agent_id;

    cout << ">> Clear Agent CS" << endl;
    for( itr = m_PiCognitiveSystemsPerAgent.begin(); itr != m_PiCognitiveSystemsPerAgent.end(); ++itr )
    {
        agent_id = (*itr).first;

        cout << "Delete CS of agent #" << agent_id << endl;

        (*itr).second->free();
        SafeDelete( (*itr).second );
    }
    m_PiCognitiveSystemsPerAgent.clear();
    m_CognitiveSystemsPerAgent.clear();
}


void
MainWindow::deleteCognitiveSystem( bool shared, uint32 id )
{
    if (shared)
    {
        if (m_PiCognitiveSystemsPerTaxon.find( id ) != m_PiCognitiveSystemsPerTaxon.end())
        {
            qDebug() << "Delete CS of taxon #" << id;
            m_PiCognitiveSystemsPerTaxon[id]->free();
            SafeDelete( m_PiCognitiveSystemsPerTaxon[id] );
            m_PiCognitiveSystemsPerTaxon.erase( id );
            m_CognitiveSystemsPerTaxon.erase( id );
            m_WidgetTabCognitiveSystemSetPerTaxon->setTabEnabled( id, false );
        }
    }
}


bool
MainWindow::loadCognitiveSystem( bool shared, uint32 id )
{
    uint32 taxon_id;

    if (shared)
    {
        taxon_id = id;
    }
    else
    {
        taxon_id = m_Environment->getAgent( id )->getTaxonId();
    }

    QFileInfo fi;

    fi = m_CognitiveSystemNames[taxon_id][m_CognitiveSystemIndex[taxon_id]];

    Taxon taxon;
    Ipseity::String instance_name;

    if (m_Environment->fetchTaxon( taxon_id, taxon ))
    {
        if (shared)
        {
            instance_name = taxon.Name;
        }
        else
        {
            instance_name = m_Environment->getAgent( id )->getName();
        }
    }
    instance_name += ".";
    instance_name += fi.baseName().toStdString().c_str();

    cout << "Create " << instance_name.c_str() << endl;

    PluginWrapper<AbstractCognitiveSystem>* cs_pi;

    cs_pi = new PluginWrapper<AbstractCognitiveSystem>;

    cs_pi->setParameterFactory( &g_ParameterRepository );
    cs_pi->setPluginSearchRootPath( m_PluginPath.c_str() );
    cs_pi->setWorkspace( m_WorkspacePath.c_str() );
    if (cs_pi->load( "CognitiveSystem", "CognitiveSystem", instance_name.c_str() ))
    {
        AbstractCognitiveSystem* cs;

        cs = reinterpret_cast<AbstractCognitiveSystem*>(cs_pi->get());

        cout << ">> CS " << cs->getName() << " loaded" << endl;

        taxon.Status = m_WidgetTaxonsList->getTaxonStatus( taxon_id );
        cs->setEnvironment( m_Environment );
        if (cs->init( m_CognitiveSystemsPath.c_str(), fi.fileName().toStdString().c_str(), taxon ))
        {
            // Attach GUI
            if (shared)
            {
                cs->attachView( 0, m_WidgetTabCognitiveSystemSetPerTaxon->widget( taxon_id ) );
            }
            else
            {
                QWidget* widget_agent = new QWidget();
                int32 index = m_WidgetTabCognitiveSystemSetPerAgent->addTab( widget_agent, m_Environment->getAgent( id )->getName() );

                cs->attachView( 0, m_WidgetTabCognitiveSystemSetPerAgent->widget( index ) );
            }

            // Store the loaded CS
            if (shared)
            {
                qDebug() << "CS added for taxon #" << id;
                m_PiCognitiveSystemsPerTaxon[id] = cs_pi;
                m_CognitiveSystemsPerTaxon[id] = cs;
            }
            else
            {
                qDebug() << "CS added for agent #" << id;
                m_PiCognitiveSystemsPerAgent[id] = cs_pi;
                m_CognitiveSystemsPerAgent[id] = cs;
            }

            qDebug()  << "Add CS #" << id << "with instance name '" << instance_name.c_str() << "'";

            return true;
        }
    }
    else
    {
        delete cs_pi;
    }

    return false;
}


void
MainWindow::updateCognitiveSystem( int taxon_id, int index )
{
    m_CognitiveSystemIndex[taxon_id] = index;

//    cout << m_CognitiveSystemNames[taxon_id][m_CognitiveSystemIndex[taxon_id]].toStdString().c_str() << endl;
}


void
MainWindow::transmitScenarioUidToCognitiveSystems( uint32 scenario_id )
{
    uint32 nbr_episodes;
    uint64 scenario_uid;

    nbr_episodes =  m_Environment->getNbrEpisodes();
    scenario_uid = m_SupervisionModule->getCurrentScenarioUid();

    m_CognitiveSystems.clear();

    for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerTaxon.begin(); itr != m_CognitiveSystemsPerTaxon.end(); ++itr )
    {
        if ((*itr).second)
        {
            (*itr).second->startNewEpisode( nbr_episodes, scenario_uid );
            m_CognitiveSystems.push_back( (*itr).second );
        }
        else
        {
            qDebug() << "No start for CS #" << (*itr).first;
        }
    }

    for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerAgent.begin(); itr != m_CognitiveSystemsPerAgent.end(); ++itr )
    {
        if ((*itr).second)
        {
            (*itr).second->startNewEpisode( nbr_episodes, scenario_uid );
            m_CognitiveSystems.push_back( (*itr).second );
        }
        else
        {
            qDebug() << "No start for CS #" << (*itr).first;
        }
    }

    qDebug() << "# CS: " << m_CognitiveSystems.size();
}


void
MainWindow::updateWidgetTabCognitiveSystems()
{
    int32 nbr_cs;

    nbr_cs = m_CognitiveSystemsPerTaxon.size()+m_CognitiveSystemsPerAgent.size();

    m_WidgetMainTab->setTabEnabled( TABID_COGNITIVESYSTEMS, (nbr_cs > 0) );
    m_WidgetTabCognitiveSystemType->setTabEnabled( 0, (m_CognitiveSystemsPerTaxon.size() > 0) );
    m_WidgetTabCognitiveSystemType->setTabEnabled( 1, (m_CognitiveSystemsPerAgent.size() > 0) );
}


void
MainWindow::updateCognitiveSystemButtons()
{
    if (!m_SimulationInProgress)
    {
        QList<QTreeWidgetItem*> list = m_WidgetTaxonsList->selectedItems();

        if (list.size())
        {
            uint32 taxon_id;
            int32 nbr_plugged_cs = 0;
            int32 nbr_valid_cs = 0;
            int32 nbr_cs_reset = 0;

            for( int32 i = 0; i < list.size(); ++i )
            {
                taxon_id = m_WidgetTaxonsList->indexOfTopLevelItem( list.at( i ) );

                if (m_CognitiveSystemNames.find( taxon_id ) != m_CognitiveSystemNames.end())
                {
                    ++nbr_valid_cs;

                    if (m_PluggedCognitiveSystems[taxon_id])
                    {
                        ++nbr_plugged_cs;
                    }

                    if (m_CognitiveSystemsPerTaxon.find( taxon_id ) != m_CognitiveSystemsPerTaxon.end() && m_CognitiveSystemsPerTaxon[taxon_id]->canReset())
                    {
                        ++nbr_cs_reset;
                    }
                }
            }

            if (list.size() == nbr_plugged_cs)
            {
                m_ButtonPlugCognitiveSystems->setIcon( m_Icons[ICON_UNPLUG_CS] );
                m_ButtonPlugCognitiveSystems->setToolTip( "Unplug the Cognitive System(s)" );
            }
            else if (nbr_plugged_cs == 0)
            {
                m_ButtonPlugCognitiveSystems->setIcon( m_Icons[ICON_PLUG_CS] );
                m_ButtonPlugCognitiveSystems->setToolTip( "Plug the Cognitive System(s)" );
            }
            else
            {
                m_ButtonPlugCognitiveSystems->setIcon( m_Icons[ICON_PLUG_UNPLUG_CS] );
                m_ButtonPlugCognitiveSystems->setToolTip( "Plug/Unplug the Cognitive System(s)" );
            }

            if (nbr_valid_cs == 0)
            {
                m_ButtonPlugCognitiveSystems->setIcon( m_Icons[ICON_NO_CS] );
                m_ButtonPlugCognitiveSystems->setToolTip( "No associated Cognitive System" );
            }

            bool flag_nbr_valid = (nbr_valid_cs > 0);

            m_ButtonPlugCognitiveSystems->setEnabled( flag_nbr_valid );
            m_ButtonResetCognitiveSystems->setEnabled( (nbr_plugged_cs > 0) && (nbr_cs_reset > 0) );
            m_ButtonShareCognitiveSystems->setEnabled( flag_nbr_valid );
        }
        else
        {
            m_ButtonPlugCognitiveSystems->setEnabled( false );
            m_ButtonResetCognitiveSystems->setEnabled( false );
            m_ButtonShareCognitiveSystems->setEnabled( false );
        }
    }
    else
    {
        m_ButtonPlugCognitiveSystems->setEnabled( false );
        m_ButtonResetCognitiveSystems->setEnabled( false );
        m_ButtonShareCognitiveSystems->setEnabled( false );
    }
}


//*****************************************************************************
//
//  Simulation
//
//*****************************************************************************


void
MainWindow::transmitStartSimulationToCognitiveSystems()
{
    cout << "Start new simulation" << endl;
    for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerTaxon.begin(); itr != m_CognitiveSystemsPerTaxon.end(); ++itr )
    {
        if ((*itr).second)
        {
            (*itr).second->startNewSimulation();
        }
    }

    for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerAgent.begin(); itr != m_CognitiveSystemsPerAgent.end(); ++itr )
    {
        if ((*itr).second)
        {
            (*itr).second->startNewSimulation();
        }
    }
}


void
MainWindow::transmitEndSimulationToCognitiveSystems()
{
//    cout << "End simulation" << endl;
    for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerTaxon.begin(); itr != m_CognitiveSystemsPerTaxon.end(); ++itr )
    {
        if ((*itr).second)
        {
            (*itr).second->endSimulation();
        }
    }

    for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerAgent.begin(); itr != m_CognitiveSystemsPerAgent.end(); ++itr )
    {
        if ((*itr).second)
        {
            (*itr).second->endSimulation();
        }
    }
}


void
MainWindow::setCognitiveSystemsLearningMode( bool learning_mode )
{
    for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerTaxon.begin(); itr != m_CognitiveSystemsPerTaxon.end(); ++itr )
    {
        if ((*itr).second)
        {
            (*itr).second->setMode( learning_mode );
        }
    }

    for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerAgent.begin(); itr != m_CognitiveSystemsPerAgent.end(); ++itr )
    {
        if ((*itr).second)
        {
            (*itr).second->setMode( learning_mode );
        }
    }
}


void
MainWindow::prepareAgentCognitiveSystemResetMap()
{
    m_AgentCognitiveSystemResetMap.clear();
}


void
MainWindow::resetTaxonCognitiveSystems()
{
    if (m_CheckboxResetCSEveryNewTrial->isChecked())
    {
        AbstractCognitiveSystem* cs;

        for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerTaxon.begin(); itr != m_CognitiveSystemsPerTaxon.end(); ++itr )
        {
            cs = (*itr).second;
            if (cs)
            {
                cout << "Reset taxon cs: " << cs->getName() << endl;
                cs->reset();
            }
        }
    }
}

void
MainWindow::resetAgentCognitiveSystems()
{
    if (m_CheckboxResetCSEveryNewTrial->isChecked())
    {
        AbstractCognitiveSystem* cs;
        uint32 cs_id;

        for( map<uint32,AbstractCognitiveSystem*>::iterator itr = m_CognitiveSystemsPerAgent.begin(); itr != m_CognitiveSystemsPerAgent.end(); ++itr )
        {
            cs_id = (*itr).first;
            cs = (*itr).second;
            if (m_AgentCognitiveSystemResetMap.find( cs_id ) == m_AgentCognitiveSystemResetMap.end())
            {
                cout << "Reset agent cs: " << cs->getName() << " (" << cs_id << ")" << endl;
                m_AgentCognitiveSystemResetMap[cs_id] = true;
                cs->reset();
            }
        }
    }
}


void
MainWindow::toggleShareSelectedCognitiveSystems()
{
    m_WidgetTaxonsList->toggleShareSelectedCognitiveSystems();
}


void
MainWindow::simulationStepForward()
{
    AbstractAgent* agent;
    uint32 taxon_id;
    uint32 agent_id;
    uint32 scenario_id;
    bool ended = false;

    float64 elapsed_milliseconds;

    if (m_CheckboxSimulatedTimeSpan->isChecked())
    {
        elapsed_milliseconds = (float64) m_SpinboxSimulatedTimeSpan->value();
    }
    else
    {
        elapsed_milliseconds = (float64) m_SpinboxRealTimeSpan->value();
    }

    m_LabelElapsedTime->setText( tr("Elapsed Time: %1").arg( elapsed_milliseconds ) );

    m_Environment->enableStepByStep( true );
    do
    {
        switch( m_Environment->simulate( elapsed_milliseconds ) )
        {
            case SS_FACE_CURRENT_SCENARIO:
                cleanCognitiveSystemsPerAgent();
                updateWidgetTabCognitiveSystems();
                m_Environment->setExperiment( m_SupervisionModule->getTrialId(), m_SupervisionModule->getEpisodeId(), m_SupervisionModule->getCurrentScenarioId(), m_SupervisionModule->getCurrentScenarioFileName(), m_SupervisionModule->isLearningMode() );

                if (m_SupervisionModule->isNewTrial())
                {
                    cout << "Trial: " << m_SupervisionModule->getTrialId() << endl;
                    prepareAgentCognitiveSystemResetMap();
                }

                if (m_SupervisionModule->isLearningMode())
                {
                    cout << "Learn ";
                }
                else
                {
                    cout << "Test ";
                }
                cout << m_SupervisionModule->getEpisodeId();
                cout << endl;

                break;

            case SS_PREPARE_NEW_EPISODE:
                renderAllWindows( m_CheckboxMainRenderingWindows->isChecked() );

                // Link agents to cognitive systems
                for( agent_id = 0; agent_id < m_Environment->getNbrAgents(); ++agent_id )
                {
                    agent = m_Environment->getAgent( agent_id );
                    if (agent)
                    {
                        taxon_id = agent->getTaxonId();
                        if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem)
                        {
                            if (m_CognitiveSystemsPerTaxon.find( taxon_id ) != m_CognitiveSystemsPerTaxon.end())
                            {
                                m_CognitiveSystemsPerTaxon[taxon_id]->linkAgent( agent_id, agent->getName() );
                            }
                        }
                        else
                        {
                            if (m_CognitiveSystemsPerAgent.find( agent_id ) != m_CognitiveSystemsPerAgent.end())
                            {
                                m_CognitiveSystemsPerAgent[agent_id]->linkAgent( agent_id, agent->getName() );
                            }
                        }
                    }
                }

                scenario_id = m_SupervisionModule->getCurrentScenarioId();
                transmitScenarioUidToCognitiveSystems( scenario_id );
//                m_LabelScenarioDescriptionContent->setText( tr("%1").arg(m_Environment->getScenarioDescription(scenario_id)) );
                if (m_SupervisionModule->isNewTrial())
                {
//                    cout << ">>> New trial!" << endl;
                    resetTaxonCognitiveSystems();
                }
                resetAgentCognitiveSystems();
                setCognitiveSystemsLearningMode( m_SupervisionModule->isLearningMode() );

                m_ElapsedTime.start();
                break;

            case SS_AGENT_PERCEIVE:
                agent_id = m_Environment->getSimulationAgentId();
                agent = m_Environment->getAgent( agent_id );
                if (agent)
                {
                    taxon_id = agent->getTaxonId();
//                    cout << "Agent #" << agent_id << " perceives" << endl;
                    if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem)
                    {
                        if (m_CognitiveSystemsPerTaxon.find( taxon_id ) != m_CognitiveSystemsPerTaxon.end())
                        {
                            m_CognitiveSystemsPerTaxon[taxon_id]->perceive( agent_id, m_Environment->getCurrentStimulus( agent_id ) );
                        }
                    }
                    else
                    {
                        if (m_CognitiveSystemsPerAgent.find( agent_id ) != m_CognitiveSystemsPerAgent.end())
                        {
                            m_CognitiveSystemsPerAgent[agent_id]->perceive( agent_id, m_Environment->getCurrentStimulus( agent_id ) );
                        }
                    }
                }
                break;

            case SS_AGENT_ADD_INFLUENCE:
                agent_id = m_Environment->getSimulationAgentId();
                agent = m_Environment->getAgent( agent_id );
                if (agent)
                {
                    taxon_id = agent->getTaxonId();
//                    cout << "Agent #" << agent_id << " deliberates" << endl;
                    if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem)
                    {
                        if (m_CognitiveSystemsPerTaxon.find( taxon_id ) != m_CognitiveSystemsPerTaxon.end())
                        {
                            m_Environment->addInfluence( agent_id, m_CognitiveSystemsPerTaxon[taxon_id]->selectResponse( agent_id, false ) );
                        }
                    }
                    else
                    {
                        if (m_CognitiveSystemsPerAgent.find( agent_id ) != m_CognitiveSystemsPerAgent.end())
                        {
                            m_Environment->addInfluence( agent_id, m_CognitiveSystemsPerAgent[agent_id]->selectResponse( agent_id, false ) );
                        }
                    }
                }
                break;

            case SS_AGENT_END_EPISODE:
                agent_id = m_Environment->getSimulationAgentId();
                agent = m_Environment->getAgent( agent_id );
                if (agent)
                {
                    taxon_id = agent->getTaxonId();
                    if (m_WidgetTaxonsList->getTaxonStatus( taxon_id ) & TS_SharedCognitiveSystem)
                    {
                        if (m_CognitiveSystemsPerTaxon.find( taxon_id ) != m_CognitiveSystemsPerTaxon.end())
                        {
                            m_CognitiveSystemsPerTaxon[taxon_id]->perceive( agent_id, m_Environment->getCurrentStimulus( agent_id ) );
                            m_CognitiveSystemsPerTaxon[taxon_id]->selectResponse( agent_id, true );
                            m_CognitiveSystemsPerTaxon[taxon_id]->endEpisode( m_Environment->isScenarioSucceededByAgent( agent_id ) );
                        }
                    }
                    else
                    {
                        if (m_CognitiveSystemsPerAgent.find( agent_id ) != m_CognitiveSystemsPerAgent.end())
                        {
                            m_CognitiveSystemsPerAgent[agent_id]->perceive( agent_id, m_Environment->getCurrentStimulus( agent_id ) );
                            m_CognitiveSystemsPerAgent[agent_id]->selectResponse( agent_id, true );
                            m_CognitiveSystemsPerAgent[agent_id]->endEpisode( m_Environment->isScenarioSucceededByAgent( agent_id ) );
                        }
                    }
                }
                break;

            case SS_END_EPISODE:
                qDebug() << "Episode ended after " << m_ElapsedTime.elapsed() << " ms";

                ended = m_SupervisionModule->selectNextScenario();
                break;

            case SS_SIMULATION_HALTED:
                qDebug() << "Simulation halted after " << m_ElapsedTime.elapsed() << " ms";

                changeSimulationStatus( false );
                stopTimers();

                if (m_ButtonSimulateScenarii->isChecked())
                {
                    m_ButtonSimulateScenarii->toggle();
                }
                break;
        }
    }
    while( m_Environment->continueSimulation() && !ended);

    if (m_SupervisionModule)
    {
        if (m_SupervisionModule->isEnded())
        {
            m_Environment->stopSimulation( false );
            transmitEndSimulationToCognitiveSystems();

            changeSimulationStatus( false );
            stopTimers();

            if (m_ButtonSimulateScenarii->isChecked())
            {
                m_ButtonSimulateScenarii->toggle();
            }
        }
        else
        {
            scenario_id = m_SupervisionModule->getCurrentScenarioId();
            m_SupervisionModule->setIterations( scenario_id, m_Environment->getNbrIterations(), m_Environment->getMaxIterations() );

            if (m_Environment->isEpisodeEnded() && m_CheckboxActionEveryNEpisodes->isChecked() && m_ComboboxActionEveryNEpisodes->currentIndex() == 0)
            {
                if ((m_Environment->getNbrEpisodes() % m_SpinBoxActionOnEpisodes->value()) == 0)
                {
                    simulateScenarii( false );
                    m_ButtonSimulateScenarii->setChecked( false );
                }
            }
        }
    }
}


void
MainWindow::simulateScenarii( bool toggled )
{
    m_ButtonSimulateScenarii->setIcon( toggled ? m_Icons[ICON_PAUSE] : m_Icons[ICON_SIMULATE] );
    if (toggled)
    {
        m_ButtonSimulateScenarii->setToolTip( "Pause" );
    }
    else
    {
        m_ButtonSimulateScenarii->setToolTip( "Play" );
    }

    if (toggled)
    {
        if (!m_SimulationInProgress)
        {
            changeSimulationStatus( true );

            m_SupervisionModule->start();
            m_Environment->startSimulation();
            transmitStartSimulationToCognitiveSystems();
        }

        if (m_RenderingWidgets.size())
        {
            m_RenderingWidgets[0]->activateWindow();
        }

        startTimers();
    }
    else
    {
        stopTimers();
    }
}


void
MainWindow::changeSimulationStatus( bool in_progress )
{
    m_SimulationInProgress = in_progress;

    updateCognitiveSystemButtons();

    m_ButtonLoadWorkspace->setDisabled( in_progress );
    m_GroupboxScenariiSets->setDisabled( in_progress );
    m_GroupBoxSupervision->setDisabled( in_progress );
    m_SupervisionModule->setSimulationStatus( in_progress );
    m_ButtonStop->setEnabled( in_progress );
}


void
MainWindow::stopSimulation()
{
    startTimers();
    m_Environment->stopSimulation( true );
    transmitEndSimulationToCognitiveSystems();
}


void
MainWindow::realtimeSpanChanged( int value )
{
    m_SimulationTimer->setInterval( value );
}


void
MainWindow::realtimeSpanEnabled( bool toggle )
{
    m_SpinboxRealTimeSpan->setEnabled( toggle );

    if (toggle)
    {
        realtimeSpanChanged( m_SpinboxRealTimeSpan->value() );
        m_CheckboxRealTimeSpan->setText( "Real Time\nSpan:" );
    }
    else
    {
        m_CheckboxSimulatedTimeSpan->setChecked( true );
        realtimeSpanChanged( 0 );
        m_CheckboxRealTimeSpan->setText( "Fastest\nSpeed" );
    }
}

void
MainWindow::simulatedTimeSpanEnabled( bool toggle )
{
    if (m_CheckboxRealTimeSpan->isChecked() || toggle)
    {
        m_SpinboxSimulatedTimeSpan->setEnabled( toggle );
    }
    else
    {
        m_CheckboxSimulatedTimeSpan->setChecked( true );
    }
}


void
MainWindow::frameRateChanged( int value )
{
    m_RenderTimer->setInterval( getTimeInterval( value ) );
}


void
MainWindow::onActionEpisodeFrequencyChanged( bool toggle )
{
    m_SpinBoxActionOnEpisodes->setEnabled( toggle );
    m_LabelActionEveryNEpisodes->setEnabled( toggle );
    m_ComboboxActionEveryNEpisodes->setEnabled( toggle );
}


//*****************************************************************************
// Timer
//*****************************************************************************


void
MainWindow::startTimers()
{
    m_SimulationTimer->start();
    if (isRendering())
    {
        m_RenderTimer->start();
    }
}


void
MainWindow::stopTimers()
{
    m_SimulationTimer->stop();
    m_RenderTimer->stop();
}


//*****************************************************************************
//
//  Rendering
//
//*****************************************************************************


void
MainWindow::deleteRenderingWidgets()
{
    m_CheckboxMainRenderingWindows->setEnabled( false );
    m_CheckboxMainRenderingWindows->setChecked( false );

    for( uint32 i = 0; i < m_RenderingWidgets.size(); ++i )
    {
        SafeDelete( m_RenderingWidgets[i] );

        m_LayoutRenderingWindows->removeWidget( m_CheckboxRenderingWindows[i] );
        m_GroupCheckboxRenderingWindows->removeButton( m_CheckboxRenderingWindows[i] );
        SafeDelete( m_CheckboxRenderingWindows[i] );
    }

    m_MainRenderingWidgetsInitialized = false;
    m_RenderingWidgets.clear();
    m_CheckboxRenderingWindows.clear();
}


void
MainWindow::deleteScenarioRenderingWidgets()
{
    for( uint32 i = m_Environment->getNbrMainRenderingWindows(); i < m_RenderingWidgets.size(); ++i )
    {
        SafeDelete( m_RenderingWidgets[i] );

        m_LayoutRenderingWindows->removeWidget( m_CheckboxRenderingWindows[i] );
        m_GroupCheckboxRenderingWindows->removeButton( m_CheckboxRenderingWindows[i] );
        SafeDelete( m_CheckboxRenderingWindows[i] );
    }

    if (m_Environment->getNbrMainRenderingWindows() > 0)
    {
        m_RenderingWidgets.erase( m_RenderingWidgets.begin()+m_Environment->getNbrMainRenderingWindows(), m_RenderingWidgets.end()  );
        m_CheckboxRenderingWindows.erase( m_CheckboxRenderingWindows.begin()+m_Environment->getNbrMainRenderingWindows(), m_CheckboxRenderingWindows.end()  );
    }
}


void
MainWindow::createRenderingWidget( uint32 window_id )
{
    WidgetRenderer* rendering_window;
    QGLWidget* shared_widget = NULL;

    if (window_id > 0)
    {
        shared_widget = m_RenderingWidgets[0];
    }

    rendering_window = new WidgetRenderer( window_id, m_Environment->getName(), m_Environment->getWindowName( window_id ), this, shared_widget );
    rendering_window->init( m_Environment, &m_CognitiveSystems );
    connect( rendering_window, SIGNAL(closed(uint32)), this, SLOT(closeRenderingWindow(uint32)) );
    m_RenderingWidgets.push_back( rendering_window );

    if (window_id == 0)
    {
        connect( rendering_window, SIGNAL(previousScenario()), this, SLOT(previousScenario()) );
        connect( rendering_window, SIGNAL(nextScenario()), this, SLOT(nextScenario()) );
        connect( rendering_window, SIGNAL(retryScenario()), this, SLOT(retryScenario()) );
    }

    QCheckBox* checkbox = new QCheckBox( QString( m_Environment->getWindowName( window_id ) ) );
    m_CheckboxRenderingWindows.push_back( checkbox );
    m_LayoutRenderingWindows->addWidget( m_CheckboxRenderingWindows[window_id],2+window_id,0,1,2 );
    m_GroupCheckboxRenderingWindows->addButton( m_CheckboxRenderingWindows[window_id] );
    m_GroupCheckboxRenderingWindows->setId( m_CheckboxRenderingWindows[window_id], window_id );
}


void
MainWindow::createRenderingWidgets( bool todo )
{
    if (todo)
    {
        if (!m_MainRenderingWidgetsInitialized)
        {
            uint32 nbr_rendering_windows = m_Environment->getNbrMainRenderingWindows();

            for( uint32 window_id = 0; window_id < nbr_rendering_windows; ++window_id )
            {
                createRenderingWidget( window_id );
            }

            m_MainRenderingWidgetsInitialized = true;
        }

        deleteScenarioRenderingWidgets();

        uint32 nbr_rendering_windows = m_Environment->getNbrScenarioRenderingWindows();

        for( uint32 window_id = 0; window_id < nbr_rendering_windows; ++window_id )
        {
            createRenderingWidget( m_Environment->getNbrMainRenderingWindows()+window_id );
        }
    }
}


bool
MainWindow::isRendering()
{
    if (m_SimulationTimer->isActive())
    {
        for( uint32 window_id = 0; window_id < m_RenderingWidgets.size(); ++window_id )
        {
            if (m_RenderingWidgets[window_id]->isVisible())
            {
                return true;
            }
        }
    }

    return false;
}


void
MainWindow::renderAllWindows( bool render )
{
    createRenderingWidgets( render );

    for( uint32 window_id = 0; window_id < m_RenderingWidgets.size(); ++window_id )
    {
        m_CheckboxRenderingWindows[window_id]->setChecked( render );
        m_RenderingWidgets[window_id]->setVisible( render );
        m_RenderingWidgets[window_id]->setBaseSize( 320, 240 );
    }

    if (isRendering())
    {
        m_RenderTimer->start();
    }
    else
    {
        m_RenderTimer->stop();
    }
}


void
MainWindow::closeRenderingWindow( uint32 window_id )
{
    m_CheckboxRenderingWindows[window_id]->setChecked( false );
}


void
MainWindow::renderWindow( int window_id )
{
    if (window_id < m_RenderingWidgets.size())
    {
        m_RenderingWidgets[window_id]->setVisible( m_CheckboxRenderingWindows[window_id]->isChecked() );
    }
}


void
MainWindow::render()
{
    for( uint32 window_id = 0; window_id < m_RenderingWidgets.size(); ++window_id )
    {
        if (m_RenderingWidgets[window_id]->isVisible())
        {
            m_RenderingWidgets[window_id]->render();
        }
    }
}


//*****************************************************************************
// Debugger
//*****************************************************************************


void
MainWindow::installDebugHandler()
{
    if (g_Settings.contains( "LogFile" ))
    {
        // Install debug handler
        Ipseity::String file_name;

        file_name = m_WorkspacePath;
        file_name += g_Settings.value( "LogFile" ).toString().toStdString().c_str();

        if (g_Settings.contains( "LogFileMode" ))
        {
            g_DebugFile = fopen( file_name.c_str(), g_Settings.value( "LogFileMode" ).toString().toStdString().c_str() );
        }
        else
        {
            g_DebugFile = fopen( file_name.c_str(), "a" );
        }

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();

        fprintf( g_DebugFile, "\n************************************\n" );
        fprintf( g_DebugFile, "   Date: %d/%02d/%02d - Time: %02d:%02d\n", date.year(), date.month(), date.day(), time.hour(), time.minute() );
        fprintf( g_DebugFile, "************************************\n\n" );

        qInstallMsgHandler( messageOutput );
    }
}


void
MainWindow::uninstallDebugHandler()
{
    if (g_DebugFile)
    {
        fclose( g_DebugFile );
        g_DebugFile = NULL;
    }

    qInstallMsgHandler( 0 );
}
