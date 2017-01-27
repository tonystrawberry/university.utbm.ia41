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


#ifndef MainWindow_h
#define MainWindow_h


#include <QtGui>
#include <QGroupBox>
#include <QGridLayout>
#include <QActionGroup>
#include <QToolBar>
#include <QSpinBox>
#include <QStringList>

#include "AL/AbstractEnvironment.h"
#include "AL/AbstractSystemScheduling.h"
#include "AL/AbstractCognitiveSystem.h"
#include "AL/AbstractSupervisionModule.h"
#include "AL/AbstractAgentManager.h"
#include "IO/PluginWrapper.h"
#include "IO/InputManager.h"
#include "IO/ParameterHost.h"
#include "IO/ParameterCheckBox.h"
#include "IO/ParameterComboBox.h"
#include "IO/ParameterSlider.h"
#include "IO/ParameterSpinBox.h"

#include "IO/ControlButton.h"

#include "DialogAbout.h"
#include "WidgetEnvironments.h"
#include "WidgetTaxonsList.h"
#include "WidgetAgentsList.h"
#include "WidgetRenderer.h"

#include <set>


class MainWindow : public QMainWindow, public ParameterHost, public AbstractAgentManager
{
    Q_OBJECT

    public:
        MainWindow( uint32 instance_id );
        ~MainWindow();

        void onAgentAdded( uint32 agent_id, uint32 taxon_id );
        void onAgentRemoved( uint32 agent_id );

    private:
        void createMenus();
        void createCentralWidget();
        void createWidgetTabEnvironment();
        void createWidgetTabSystemScheduling();
        void createWidgetTabTaxons();
        void createWidgetTabCognitiveSystems();
        void createWidgetTabAgents();
        void createWidgetTabBenchmark();

        void changeSimulationStatus( bool );

        void onEnvironmentClosed();
        void onEnvironmentLoaded();

        // Sessions
        void updateRecentSessions();
        void loadSession( const char* env_name, const char* workspace_name );
        void addToRecentSessions( const char* env_name, const char* workspace_name );

        // Taxons and cognitive systems
        void chooseDefaultTaxons();
        void updateTaxonsList();
        void updateWidgetTabCognitiveSystems();

        // Scenarii
        void updateScenariiList();

        void loadEnvironment( int32 env_id );
        void closeEnvironment();

        void loadScenarii( const char* author_name, const char* production_name );

        void initWorkspaceButtonStatus( const char* session_name );


        void loadLastSupervisionModule();

        void cleanCognitiveSystems();
        void cleanCognitiveSystemsPerTaxon();
        void cleanCognitiveSystemsPerAgent();
        void deleteCognitiveSystem( bool shared, uint32 id );
        bool loadCognitiveSystem( bool shared, uint32 id );
        void plugCognitiveSystem( uint32 taxon_id );

        void transmitStartSimulationToCognitiveSystems();
        void transmitEndSimulationToCognitiveSystems();
        void transmitScenarioUidToCognitiveSystems( uint32 scenario_uid );
        void setCognitiveSystemsLearningMode( bool learning_mode );
        void prepareAgentCognitiveSystemResetMap();
        void resetTaxonCognitiveSystems();
        void resetAgentCognitiveSystems();

        void deleteRenderingWidgets();
        void deleteScenarioRenderingWidgets();
        void createRenderingWidget( uint32 window_id );
        void createRenderingWidgets( bool );

        bool isRendering();

        void stopTimers();
        void startTimers();

        void installDebugHandler();
        void uninstallDebugHandler();


    private slots:
        void loadWorkspace();
        void deleteWorkspace();
        void resetWorkspaceData();
        void updateWorkspaces();

        // Environment
        void loadEnvironment();

        void changeComboBoxAuthorIndex( int );
        void changeComboBoxScenariiSetIndex( int );

        // Supervision Module
        void useSupervisionModule( bool );

        void onEditWorkspaceName( QString );

        // Scenarii
        void loadScenarii();

        // Quit
        void quit();

        // Sessions
        void selectSession( QAction* action );
        void clearRecentSessions();

        // Simulation
        void realtimeSpanEnabled( bool toggle );
        void simulatedTimeSpanEnabled( bool toggle );

        void simulationStepForward();

        void render();
        void renderWindow( int );

        void simulateScenarii( bool );
        void stopSimulation();

        void renderAllWindows( bool );
        void closeRenderingWindow( uint32 );
        void retryScenario();
        void previousScenario();
        void nextScenario();

        void realtimeSpanChanged(int);
        void frameRateChanged(int);

        void onActionEpisodeFrequencyChanged(bool);

        // Cognitive Systems
        void plugCurrentCognitiveSystem(QTreeWidgetItem*,int);
        void plugSelectedCognitiveSystems();
        void resetSelectedCognitiveSystems();
        void updateCognitiveSystem( int, int );
        void updateCognitiveSystemButtons();
        void toggleShareSelectedCognitiveSystems();

        void displayParameters();
        void about();


    public:
        Ipseity::String m_CurrentPath;
        Ipseity::String m_EnvironmentBasePath;
        Ipseity::String m_PluginPath;
        Ipseity::String m_ScenariiPath;
        Ipseity::String m_WorkspacePath;
        Ipseity::String m_CognitiveSystemsPath;

        Ipseity::String m_EnvironmentName;

        vector<QString> m_Workspaces;
        uint32 m_MaxRecentSessionCount;
        QStringList m_RecentSessions;

        WidgetEnvironments* m_WidgetEnvironments;
        ParameterComboBox* m_ComboBoxAuthors;
        ParameterComboBox* m_ComboBoxScenariiSets;
        QComboBox* m_ComboBoxSupervisionModules;

        vector<QString> m_SupervisionModuleNames;

        AbstractEnvironment* m_Environment;

        // Session
        QComboBox* m_EditWorkspaceName;

        // Cognitive systems
        map<uint32,vector<QString> > m_CognitiveSystemNames;
        map<uint32,uint32> m_CognitiveSystemIndex;
        map<uint32,bool> m_PluggedCognitiveSystems;

        vector<AbstractCognitiveSystem*> m_CognitiveSystems;
        map<uint32,PluginWrapper<AbstractCognitiveSystem>* > m_PiCognitiveSystemsPerTaxon;
        map<uint32,AbstractCognitiveSystem*> m_CognitiveSystemsPerTaxon;
        map<uint32,PluginWrapper<AbstractCognitiveSystem>* > m_PiCognitiveSystemsPerAgent;
        map<uint32,AbstractCognitiveSystem*> m_CognitiveSystemsPerAgent;
        map<uint32,bool> m_AgentCognitiveSystemResetMap;

    private:
        // Timers
        QTime m_ElapsedTime;
        QTimer* m_RenderTimer;
        bool m_SimulationInProgress;

        // Scenario and author
        uint32 m_LastAuthorId;
        uint32 m_LastScenariiSetId;

        // Environment
        PluginWrapper<AbstractEnvironment> m_PcEnvironment;

        // System Scheduling
        PluginWrapper<AbstractSystemScheduling> m_PcSystemScheduling;
        AbstractSystemScheduling* m_SystemScheduling;

        // Supervision Module
        PluginWrapper<AbstractSupervisionModule> m_PcSupervisionModule;
        AbstractSupervisionModule* m_SupervisionModule;

        //GUI
        QTabWidget* m_WidgetMainTab;
        QWidget* m_WidgetTabTaxons;
        QWidget* m_WidgetTabAgents;
        QTabWidget* m_WidgetTabCognitiveSystemType;
        QTabWidget* m_WidgetTabCognitiveSystemSetPerTaxon;
        QTabWidget* m_WidgetTabCognitiveSystemSetPerAgent;
        QWidget* m_WidgetTabBenchmark;

        QTabWidget* m_WidgetTabEnvironments;
        QTabWidget* m_WidgetTabSystemScheduling;

        QGroupBox* m_GroupboxWorkspaces;
        QLabel* m_LabelWorkspaceName;
        QPushButton* m_ButtonLoadWorkspace;
        QPushButton* m_ButtonResetWorkspaceData;
        QPushButton* m_ButtonDeleteWorkspace;

        QGroupBox* m_GroupboxEnvironments;
        QGroupBox* m_GroupboxSystemScheduling;
        QGroupBox* m_GroupboxScenariiSets;
        ControlButton* m_ButtonLoadScenariiSet;
        QWidget* m_WidgetEnvironmentParameters;

        QGroupBox* m_GroupboxParameterSetting;
        QGroupBox* m_GroupboxScenariosManagement;
        QGroupBox* m_GroupboxSimulation;

        //Environment GUI
        ControlButton* m_ButtonSimulateScenarii;
        ControlButton* m_ButtonStop;
        ParameterCheckBox* m_CheckboxResetCSEveryNewTrial;
        QCheckBox* m_CheckboxActionEveryNEpisodes;
        QSpinBox* m_SpinBoxActionOnEpisodes;
        QLabel* m_LabelActionEveryNEpisodes;
        QComboBox* m_ComboboxActionEveryNEpisodes;


        QGroupBox* m_GroupBoxSimulation;
        ParameterCheckBox* m_CheckboxRealTimeSpan;
        ParameterSpinBox* m_SpinboxRealTimeSpan;
        ParameterCheckBox* m_CheckboxSimulatedTimeSpan;
        ParameterSpinBox* m_SpinboxSimulatedTimeSpan;
        QLabel* m_LabelElapsedTime;
        QTimer* m_SimulationTimer;

        // Supervision
        QGroupBox* m_GroupBoxSupervision;
        QPushButton* m_ButtonUseSupervisionModule;

        // Rendering windows
        QGroupBox* m_GroupBoxRenderingWindows;
        QGridLayout* m_LayoutRenderingWindows;
        QButtonGroup* m_GroupCheckboxRenderingWindows;
        std::vector<QCheckBox*> m_CheckboxRenderingWindows;
        ParameterCheckBox* m_CheckboxMainRenderingWindows;
        QCheckBox* m_CheckboxScenarioRenderingWindows;
        bool m_MainRenderingWidgetsInitialized;
        std::vector<WidgetRenderer*> m_RenderingWidgets;

        // Frame rate
        ParameterSlider* m_SliderFrameRate;

        // Scenarios
        QLabel* m_LabelScenarioDescriptionContent;

        // Taxons
        WidgetTaxonsList* m_WidgetTaxonsList;

        // Cognitive systems
        bool m_ResetAllCognitiveSystems;
        ControlButton* m_ButtonPlugCognitiveSystems;
        ControlButton* m_ButtonResetCognitiveSystems;
        ControlButton* m_ButtonShareCognitiveSystems;

        // Agents
        WidgetAgentsList* m_WidgetAgentsList;

        // Menus
        QMenu* m_MenuFile;
        QMenu* m_MenuRecentSessions;
        QActionGroup* m_SessionGroup;
        QMenu* m_MenuHelp;

        // Icons
        QPixmap* m_Icons;

        // Actions
        QAction* m_ActionDisplayParameters;
        QAction* m_ActionQuit;

        QAction* m_ActionAbout;

        DialogAbout* m_About;
};


#endif
