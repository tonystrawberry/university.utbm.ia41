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


#include "SMControlledByUser.h"

#include "StringFunctions.h"
#include "IO/XMLScanner.h"


#include <fstream>


enum {
    TAG_SCENARIO        = 0,

    TAG_END
};


const char* g_SMTags[] =
{
    "Scenario",

    ""
};


const uint32 cstMaximumEpisodeCount = 100000;


SMControlledByUser::SMControlledByUser() :
    m_LayoutScenarios( NULL )
{
}


SMControlledByUser::~SMControlledByUser()
{
    if (m_LayoutScenarios)
    {
        m_WidgetScenariiList->clear();

//        delete m_LabelNbrParameterSettings;

        delete m_ProgressBarGlobal;


        clearParameters();

        delete m_ButtonSetEpisodeCount;
        delete m_ButtonSetLearningEpisodeCount;
        delete m_ButtonSetTestEpisodeCount;

        delete m_WidgetScenariiList;

        delete m_LayoutScenarioEpisodes;
        delete m_GroupBoxScenarioEpisodes;
        delete m_AncillaryLayout;
        delete m_AncillaryWidget;
        delete m_LayoutScenarios;
//        delete m_LayoutParameterSetting;
    }
}


const char*
SMControlledByUser::getBaseName() const
{
    return "SupervisionByUser";
}


const char*
SMControlledByUser::getDescription() const
{
    return "Parameter Settings in files. Scenarios are ordered by the user.";
}


const char*
SMControlledByUser::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
SMControlledByUser::defineParameters()
{
//    m_LayoutParameterSetting = new QGridLayout();

//    m_CheckboxUseParameterSettingFile = new ParameterCheckBox( "FlagUseParameterSettingFile", "Parameter Setting File" );
//    m_CheckboxUseParameterSettingFile->addToLayout( m_LayoutParameterSetting, 0,0, false );

//    m_LineEditParameterSettingFile = new ParameterLineEdit( "ParameterSettingFile", "", "experiment.txt" );
//    m_LineEditParameterSettingFile->addToLayout( m_LayoutParameterSetting, 0,1 );

//    m_SpinBoxParameterSettingId = new ParameterSpinBox( "ParameterSettingId", "Setting Id:" );
//    m_SpinBoxParameterSettingId->setFixedWidth( 120 );
//    m_SpinBoxParameterSettingId->setRange( 0, 10000000 );
//    m_SpinBoxParameterSettingId->setValue( 0 );
//    m_SpinBoxParameterSettingId->addToLayout( m_LayoutParameterSetting, 1,0, true );

//    m_LabelNbrParameterSettings = new QLabel( "" );
//    m_LayoutParameterSetting->addWidget( m_LabelNbrParameterSettings, 1,2 );


    m_LayoutScenarios = new QGridLayout();

    m_SpinboxNbrTrials = new ParameterSpinBox( "NbrTrials", "# Trials:" );
    m_SpinboxNbrTrials->setFixedWidth( 80 );
    m_SpinboxNbrTrials->setRange( 1, 10000 );
    m_SpinboxNbrTrials->addToLayout( m_LayoutScenarios, 0,0 );

    m_SpinboxNbrLoops = new ParameterSpinBox( "NbrLoops", "Loop over:" );
    m_SpinboxNbrLoops->setFixedWidth( 80 );
    m_SpinboxNbrLoops->setRange( 1, 10000 );
    m_SpinboxNbrLoops->addToLayout( m_LayoutScenarios, 1,0 );


    m_AncillaryWidget = new QWidget;
    m_AncillaryLayout = new QHBoxLayout( m_AncillaryWidget );

    m_GroupBoxScenarioEpisodes = new QGroupBox( tr("Scenario Episodes") );
    m_LayoutScenarioEpisodes = new QHBoxLayout( m_GroupBoxScenarioEpisodes );
    m_LayoutScenarioEpisodes->setAlignment( Qt::AlignLeft );

    m_ButtonSetEpisodeCount = new QPushButton(tr("Episode\ncount:"));
    m_ButtonSetLearningEpisodeCount = new QPushButton(tr("Learn\nevery:"));
    m_ButtonSetTestEpisodeCount = new QPushButton(tr("Test\nevery:"));

    m_SpinboxEpisodeCount = new ParameterSpinBox( "NbrEpisodes", "" );
    m_SpinboxEpisodeCount->setMinimumSize( 80, 30 );
    m_SpinboxEpisodeCount->setMaximum( cstMaximumEpisodeCount );
    m_SpinboxEpisodeCount->setSingleStep( 10 );

    m_LayoutScenarioEpisodes->addWidget(m_ButtonSetEpisodeCount);
    m_LayoutScenarioEpisodes->addWidget(m_ButtonSetLearningEpisodeCount);
    m_LayoutScenarioEpisodes->addWidget(m_ButtonSetTestEpisodeCount);
    m_LayoutScenarioEpisodes->addWidget(m_SpinboxEpisodeCount);

    m_AncillaryLayout->addWidget( m_GroupBoxScenarioEpisodes,0,0 );

    m_WidgetScenariiList = new WidgetScenariiList();

    m_LayoutScenarios->addWidget( m_AncillaryWidget, 1,2,1,4 );
    m_LayoutScenarios->addWidget( m_WidgetScenariiList, 2,0,1,6 );

    m_ProgressBarGlobal = new QProgressBar();
    m_ProgressBarGlobal->setMinimum(0);
    m_ProgressBarGlobal->setValue(0);

    m_LayoutScenarios->addWidget( m_ProgressBarGlobal, 3,0,1,6 );

    setSimulationStatus( false );

//    connect( m_CheckboxUseParameterSettingFile, SIGNAL(toggled(bool)), this, SLOT(onUseExperimentFile(bool)) );
    connect( m_ButtonSetEpisodeCount, SIGNAL(clicked()), this, SLOT(setEpisodeCount()) );
    connect( m_ButtonSetLearningEpisodeCount, SIGNAL(clicked()), this, SLOT(setLearningEpisodes()) );
    connect( m_ButtonSetTestEpisodeCount, SIGNAL(clicked()), this, SLOT(setTestEpisodes()) );
    connect( m_WidgetScenariiList, SIGNAL(itemSelectionChanged()), this, SLOT(onScenariiListSelectionChanged()) );
}


bool
SMControlledByUser::attachView( uint32 view_id, QWidget* widget )
{
    if (view_id == 0)
    {
        widget->setLayout( m_LayoutScenarios );
    }

    return true;
}


bool
SMControlledByUser::canSimulate()
{
    return (m_WidgetScenariiList->selectedItems().size() > 0);
}


void
SMControlledByUser::start()
{
    if (canSimulate())
    {

        uint32 count = m_WidgetScenariiList->topLevelItemCount();

        // reset the iterations of all the scenarios
        for( uint32 i = 0; i < count; ++i )
        {
           WidgetScenarioItem* item = dynamic_cast<WidgetScenarioItem*> (m_WidgetScenariiList->topLevelItem(i));
           if(item)
           {
               item->resetProgressBar();
               item->resetCurrentEpisode();
               item->updateIterations( 0, 0 );
           }
        }

        // init the progression bar
        m_ProgressBarGlobal->setValue( 0 );
        m_ProgressBarGlobal->setMaximum( getNbrTotalEpisodes() );

        cout << "Total episodes " << getNbrTotalEpisodes() << endl;

        m_TrialId = 1;
        m_EpisodeId[0] = m_EpisodeId[1] = 1;
        m_NewTrial = true;
        m_NbrRemainingLoops = m_SpinboxNbrLoops->value();
        m_ItemId = 0;
        initScenarioId();

        m_Mode = MODE_LEARN;

        m_Ended = false;
    }
    else
    {
        m_Ended = true;
    }
}


bool
SMControlledByUser::isLearning() const
{
    WidgetScenarioItem* currentScenarioItem = dynamic_cast<WidgetScenarioItem*>(m_WidgetScenariiList->selectedItems().at( m_ItemId ));
    uint32 frq = currentScenarioItem->getLearnEveryEpisode();
    if (frq == 0)
    {
        return false;
    }
    else
    {
        return ((currentScenarioItem->getCurrentEpisode() % frq) == 0);
    }
}


bool
SMControlledByUser::isTesting() const
{
    WidgetScenarioItem* currentScenarioItem = dynamic_cast<WidgetScenarioItem*>(m_WidgetScenariiList->selectedItems().at( m_ItemId ));
    uint32 frq = currentScenarioItem->getTestEveryEpisode();
    if (frq == 0)
    {
        return false;
    }
    else
    {
        return ((currentScenarioItem->getCurrentEpisode() % frq) == 0);
    }
}


bool
SMControlledByUser::isEnded() const
{
    return m_Ended;
}


void
SMControlledByUser::setSimulationStatus( bool in_progress )
{
    bool f_disabled = (!canSimulate() || in_progress);

//    m_CheckboxUseParameterSettingFile->setDisabled( f_disabled );
//    m_LineEditParameterSettingFile->setDisabled( f_disabled );
//    m_SpinBoxParameterSettingId->setDisabled( f_disabled );
    m_SpinboxNbrTrials->setDisabled( f_disabled );
    m_SpinboxNbrLoops->setDisabled( f_disabled );
    m_GroupBoxScenarioEpisodes->setDisabled( f_disabled );
    m_WidgetScenariiList->setDisabled( in_progress );
}


bool
SMControlledByUser::isNewTrial() const
{
    return m_NewTrial;
}


uint32
SMControlledByUser::getTrialId() const
{
    return m_TrialId;
}


uint32
SMControlledByUser::getEpisodeId() const
{
    return m_EpisodeId[m_Mode];
}


uint32
SMControlledByUser::getCurrentScenarioId() const
{
    return m_ScenarioId;
}

uint64
SMControlledByUser::getCurrentScenarioUid() const
{
    return m_Scenarii.getUid( m_ScenarioId );
}

const char*
SMControlledByUser::getCurrentScenarioFileName() const
{
    return m_Scenarii.getFileName( m_ScenarioId );
}


bool
SMControlledByUser::isLearningMode() const
{
    return (m_Mode == MODE_LEARN);
}


void
SMControlledByUser::clearScenarii()
{
    m_Scenarii.clear();
    m_WidgetScenariiList->clear();
}


bool
SMControlledByUser::readScenarii( const char* base_path, const char* file_name )
{
    bool ret = false;

    clearScenarii();

    Ipseity::String long_name;

    long_name = base_path;
    long_name += file_name;

    Ipseity::String scenarii_base_path;
    scenarii_base_path = long_name.getSubString( 0, long_name.findLastChar( '/' ) );

#ifdef TRACE
    cout << "Scenarii base path: " << scenarii_base_path << endl;
#endif

    XMLScanner scannerml( file_name, base_path );

    if (!scannerml.isLoaded())
    {
#ifdef TRACE
        cout << "Scenarii NOT loaded!" << endl;
#endif
        return false;
    }

    XMLScanner::Token t;

    while( scannerml.nextToken( &t ) )
    {
        switch( t.kind )
        {
            case XMLScanner::Token::Tag:
                t.queryTagId( g_SMTags );
                break;

            case XMLScanner::Token::Attribute:
                switch( t.tagId )
                {
                    case TAG_SCENARIO:
                        if( t.keyIs( "Name" ) )
                        {
                            long_name = scenarii_base_path;
                            long_name += t.stringValue();

                            addScenario( long_name.c_str() );
                        }
                        break;
                }
                break;

            case XMLScanner::Token::Close:
                t.queryTagId( g_SMTags );
                break;
        }
    }

    ret = true;

    return ret;
}


void
SMControlledByUser::addScenario( const char* filename )
{
    uint32 id = m_Scenarii.add( filename );
    m_WidgetScenariiList->addListItem( m_Scenarii.getName( id ), 1 );
    m_WidgetScenariiList->selectAll();
    initParametersSettings();
}


bool
SMControlledByUser::selectNextScenario()
{
    ++m_EpisodeId[m_Mode];
    m_NewTrial = false;

    bool learn = isLearning();
    bool test = isTesting();

    if (isLearningMode())
    {
        if (test)
        {
            m_Mode = MODE_TEST;
        }
    }
    else if (learn)
    {
        m_Mode = MODE_LEARN;
    }

    if (m_EpisodeRuns > 0)
    {
        --m_EpisodeRuns;
    }
    if (m_EpisodeRuns == 0)
    {
        countCurrentScenario();
    }


    WidgetScenarioItem* currentScenarioItem = dynamic_cast<WidgetScenarioItem*>(m_WidgetScenariiList->selectedItems().at( m_ItemId ));
    if (currentScenarioItem->getCurrentEpisode() > currentScenarioItem->getEpisodeCount())
    {
        ++m_ItemId;

        if (m_ItemId == m_WidgetScenariiList->selectedItems().size())
        {
            --m_NbrRemainingLoops;

            if (m_NbrRemainingLoops == 0)
            {
//                if (m_SettingId && m_SettingId->nextNumber())
//                {
//                    m_NbrRemainingLoops = m_SpinboxNbrLoops->value();
//                    m_SpinBoxParameterSettingId->setValue( m_SpinBoxParameterSettingId->value()+1 );
//                }
//                else
                {
                    ++m_TrialId;

                    if (m_TrialId > m_SpinboxNbrTrials->value())
                    {
                        m_Ended = true;
                    }
                    else
                    {
                        m_EpisodeId[0] = m_EpisodeId[1] = 1;
                        m_NewTrial = true;
                        m_NbrRemainingLoops = m_SpinboxNbrLoops->value();
                    }
//                    m_SpinBoxParameterSettingId->setValue( 0 );
                }
            }

            if (!m_Ended)
            {
                uint32 count = m_WidgetScenariiList->topLevelItemCount();

                // reset the iterations of all the scenarios
                for( uint32 i = 0; i < count; ++i )
                {
                   WidgetScenarioItem* item = dynamic_cast<WidgetScenarioItem*> (m_WidgetScenariiList->topLevelItem(i));
                   if (item)
                   {
                       item->resetProgressBar();
                       item->resetCurrentEpisode();
                       item->updateIterations( 0, 0 );
                   }
                }

                m_ItemId = 0;
                initScenarioId();
            }
        }
        else
        {
            initScenarioId();
        }
    }

    return m_Ended;
}


void
SMControlledByUser::setIterations( uint32 scenario_id, uint32 nbr_iterations, uint32 max_iterations )
{
    WidgetScenarioItem* currentScenarioItem = dynamic_cast<WidgetScenarioItem*>(m_WidgetScenariiList->topLevelItem( scenario_id ));
    currentScenarioItem->updateIterations( nbr_iterations, max_iterations );
}


void
SMControlledByUser::restartCurrentScenario()
{
    m_Mode = MODE_TEST;
    m_EpisodeRuns = 0;
    initScenarioId();
    m_ProgressBarGlobal->setMaximum( m_ProgressBarGlobal->maximum()+1 );
}


void
SMControlledByUser::gotoPreviousScenario()
{
    m_Mode = MODE_TEST;
    m_EpisodeRuns = 0;
    if (m_ItemId > 0)
    {
        --m_ItemId;
    }
    initScenarioId();
    m_ProgressBarGlobal->setMaximum( m_ProgressBarGlobal->maximum()+1 );
}


void
SMControlledByUser::gotoNextScenario()
{
    m_Mode = MODE_TEST;
    m_EpisodeRuns = 0;
    if (m_ItemId < m_WidgetScenariiList->selectedItems().size()-1)
    {
        ++m_ItemId;
    }
    initScenarioId();
    m_ProgressBarGlobal->setMaximum( m_ProgressBarGlobal->maximum()+1 );
}


//*****************************************************************************


void
SMControlledByUser::initScenarioId()
{
    WidgetScenarioItem* currentScenarioItem = dynamic_cast<WidgetScenarioItem*>(m_WidgetScenariiList->selectedItems().at( m_ItemId ));
    m_ScenarioId = currentScenarioItem->getScenarioId();
    m_EpisodeRuns = isLearning() ? 1 : 0;
    m_EpisodeRuns += isTesting() ? 1 : 0;
}


void
SMControlledByUser::countCurrentScenario()
{
    WidgetScenarioItem* currentScenarioItem = dynamic_cast<WidgetScenarioItem*>(m_WidgetScenariiList->selectedItems().at( m_ItemId ));
    if (currentScenarioItem)
    {
        currentScenarioItem->nextEpisode();
        m_ProgressBarGlobal->setValue( m_ProgressBarGlobal->value()+1 );
        m_EpisodeRuns = isLearning() ? 1 : 0;
        m_EpisodeRuns += isTesting() ? 1 : 0;
    }
}


uint32
SMControlledByUser::getNbrTotalEpisodes() const
{
    uint32 nbr_episodes = 0;
    WidgetScenarioItem* i;

    foreach(QTreeWidgetItem* item, m_WidgetScenariiList->selectedItems())
    {
        i = dynamic_cast<WidgetScenarioItem*>( item );

        if (i->getLearnEveryEpisode() > 0 || i->getTestEveryEpisode() > 0)
        {
            nbr_episodes += i->getEpisodeCount();
        }
    }

    return m_SpinboxNbrTrials->value()*m_SpinboxNbrLoops->value()*nbr_episodes;
}


//******************************************************************************************************************


void
SMControlledByUser::setEpisodeCount()
{
    int v = m_SpinboxEpisodeCount->value();

    foreach(QTreeWidgetItem* item, m_WidgetScenariiList->selectedItems())
    {
        WidgetScenarioItem* i = dynamic_cast<WidgetScenarioItem*>(item);

        if (i != NULL)
        {
            i->setEpisodeCount( v );
        }
    }
}


void
SMControlledByUser::setLearningEpisodes()
{
    int v = m_SpinboxEpisodeCount->value();

    foreach(QTreeWidgetItem* item, m_WidgetScenariiList->selectedItems())
    {
        WidgetScenarioItem* i = dynamic_cast<WidgetScenarioItem*>(item);

        if (i != NULL)
        {
            i->setLearningEpisodeCount( v );
        }
    }
}


void
SMControlledByUser::setTestEpisodes()
{
    int v = m_SpinboxEpisodeCount->value();

    foreach(QTreeWidgetItem* item, m_WidgetScenariiList->selectedItems())
    {
        WidgetScenarioItem* i = dynamic_cast<WidgetScenarioItem*>(item);

        if (i != NULL)
        {
            i->setTestEpisodeCount( v );
        }
    }
}


void
SMControlledByUser::onScenariiListSelectionChanged()
{
    bool f_enabled = canSimulate();

//    m_CheckboxUseParameterSettingFile->setEnabled( f_enabled );
//    m_LineEditParameterSettingFile->setEnabled( f_enabled && m_CheckboxUseParameterSettingFile->isChecked() );
//    m_SpinBoxParameterSettingId->setEnabled( f_enabled && m_CheckboxUseParameterSettingFile->isChecked() );
    m_SpinboxNbrTrials->setEnabled( f_enabled );
    m_SpinboxNbrLoops->setEnabled( f_enabled );
    m_GroupBoxScenarioEpisodes->setEnabled( f_enabled );
}


void
SMControlledByUser::onUseExperimentFile( bool toggle )
{
//    m_LineEditParameterSettingFile->setEnabled( toggle );
//    m_SpinBoxParameterSettingId->setEnabled( toggle );
}
