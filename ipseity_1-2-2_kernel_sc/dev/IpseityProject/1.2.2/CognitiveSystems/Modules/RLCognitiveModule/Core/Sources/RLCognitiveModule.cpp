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


#include "RLCognitiveModule.h"


#include "StringFunctions.h"
#include "Math/RandomNumberGenerator.h"
#include "IO/XMLScanner.h"
#include "AL/RLCognitiveSystem/CognitiveSystem/XMLTags_CognitiveSystem.h"


#include <ostream>
#include <QDebug>


RLCognitiveModule::RLCognitiveModule() :
    m_BehaviorModule( NULL ),
    m_LearningModule( NULL ),
    m_QFunction( NULL ),
    m_VFunction( NULL ),
    m_FiniteGreedyPolicy( NULL ),
    m_DataChannelId( 0 ),
    m_RewardChannelId( 1 ),
    m_KeepPerformance( true ),
    m_GenerateFigures( true ),
    m_Layout( NULL )
{
    RandomNumberGenerator::initSeed( 0 );

    m_SensoryInterface.add( "Percepts" );
    m_SensoryInterface.add( "Reward" );
}

RLCognitiveModule::~RLCognitiveModule()
{
    qDebug() << "Saving stats of " << getName() << "...";
    saveStats();

    deleteWidgets();
}


const char*
RLCognitiveModule::getBaseName() const
{
    return "ReinforcementLearning";
}


const char*
RLCognitiveModule::getDescription() const
{
    return "";
}


const char*
RLCognitiveModule::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
RLCognitiveModule::defineParameters()
{
    m_Layout = new QGridLayout();

    m_CheckboxLearning = new ParameterCheckBox( "FlagLearning", "&Learning", "Ctrl+L" );
    m_CheckboxLearning->setChecked( true );
    m_CheckboxLearning->addToLayout( m_Layout, 0,0 );

    m_CheckboxUseGreedyPolicy = new ParameterCheckBox( "FlagGreedyPolicy", "&UseGreedyPolicy", "Ctrl+U" );
    m_CheckboxUseGreedyPolicy->setChecked( false );
    m_CheckboxUseGreedyPolicy->addToLayout( m_Layout, 0,1 );

    m_CheckboxSaveEpisodeData = new ParameterCheckBox( "FlagSaveEpisodeData", "&Save episode data", "Ctrl+S" );
    m_CheckboxSaveEpisodeData->setChecked( false );
    m_CheckboxSaveEpisodeData->addToLayout( m_Layout, 1,0 );

    m_DoubleSpinboxGamma = new ParameterDoubleSpinBox( "Gamma", "Gamma" );
    m_DoubleSpinboxGamma->addToLayout( m_Layout, 2,0 );
    m_DoubleSpinboxGamma->setRange( 0.0, 1.0 );
    m_DoubleSpinboxGamma->setValue( 0.90 );
    m_DoubleSpinboxGamma->setSingleStep( 0.1 );

    m_GroupboxBehaviorModule = new QGroupBox( "Behavior Module" );
    m_GroupboxMemory = new QGroupBox( "Q-Function" );
    m_GroupboxLearningModule = new QGroupBox( "Learning Module" );

    m_Layout->addWidget( m_GroupboxBehaviorModule,      3,0,1,4,Qt::AlignLeft );
    m_Layout->addWidget( m_GroupboxMemory,              4,0,1,4,Qt::AlignLeft );
    m_Layout->addWidget( m_GroupboxLearningModule,      5,0,1,4,Qt::AlignLeft );
}


bool
RLCognitiveModule::attachView( uint32 view_id, QWidget* widget )
{
    if (m_BehaviorModule)
    {
        qDebug() << "*** Attach BM " << m_BehaviorModule->getName();
        m_GroupboxBehaviorModule->setTitle( tr("Behavior Module: %1").arg( m_BehaviorModule->getBaseName() ) );
        m_BehaviorModule->attachView( 0, m_GroupboxBehaviorModule );
    }
    if (m_QFunction)
    {
        qDebug() << "*** Attach QF " << m_QFunction->getName();
        m_GroupboxMemory->setTitle( tr("Q-Function: %1").arg( m_QFunction->getBaseName() ) );
        m_QFunction->attachView( 0, m_GroupboxMemory );
    }
    if (m_LearningModule)
    {
        qDebug() << "*** Attach LM " << m_LearningModule->getName();
        m_GroupboxLearningModule->setTitle( tr("Learning Module: %1").arg( m_LearningModule->getBaseName() ) );
        m_LearningModule->attachView( 0, m_GroupboxLearningModule );
    }

    widget->setLayout( m_Layout );

    return true;
}


void
RLCognitiveModule::deleteWidgets()
{
    SafeDelete( m_QFunction );
    SafeDelete( m_VFunction );
    SafeDelete( m_FiniteGreedyPolicy );

    m_PcBehaviorModule.free();
    m_PcLearningModule.free();

    if (m_Layout)
    {
        delete m_CheckboxLearning;
        delete m_CheckboxUseGreedyPolicy;
        delete m_CheckboxSaveEpisodeData;
        delete m_DoubleSpinboxGamma;

        delete m_GroupboxMemory;
        delete m_GroupboxBehaviorModule;
        delete m_GroupboxLearningModule;

        delete m_Layout;
    }

}


bool
RLCognitiveModule::canLearn() const
{
    return (m_LearningModule != NULL && m_QFunction != NULL);
}


//*****************************************************************************


bool
RLCognitiveModule::init( const char* base_path, const char* file_name, const Taxon & taxon, AbstractInputManager*  )
{
    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        qCritical() << "CognitiveModule not initialized!";
        qCritical() << "File '" << file_name << "' from" << base_path << "not loaded!";

        return false;
    }


    m_FileName = file_name;
    m_BasePath = scanner.path;


//    qDebug() << "Save in:" << m_Workspace.c_str();


    XMLScanner::Token t;
    Ipseity::String mod_name;
    bool def_var = false;
    std::vector<Ipseity::ScalarDomain> list_cd;
    Ipseity::Scalar min, max, interval;
    Ipseity::ScalarDomain cd;
    uint32 channel_id;

    while( scanner.nextToken( &t ) )
    {
        switch( t.kind )
        {
            case XMLScanner::Token::Tag:
                t.queryTagId( g_Tags );
                switch( t.tagId )
                {
                    case TAG_STIMULUS_VARIABLES:
                    case TAG_RESPONSE_VARIABLES:
                        def_var = true;
                        list_cd.clear();
                        break;
                }
                break;

            case XMLScanner::Token::Attribute:
                switch( t.tagId )
                {
                    case TAG_MODULE:
                        break;

                    case TAG_STIMULUS_VARIABLES:
                        if (t.keyIs( "ChannelId" ))
                        {
                            m_DataChannelId = channel_id = t.intValue();
                        }
                        break;

                    case TAG_REWARD_VARIABLES:
                        if (t.keyIs( "ChannelId" ))
                        {
                            m_RewardChannelId = channel_id = t.intValue();
                        }
                        break;

                    case TAG_VARIABLE:
                        if (def_var)
                        {
                            if (t.keyIs( "Name" ))
                            {
                                cd.Name = t.stringValue();
                                cd.setPeriodic( false );
                            }
                            else if (t.keyIs( "Range" ))
                            {
                                min = t.floatValue( 0 );
                                max = t.floatValue( 1 );
                                interval = 0.0;
                            }
                            else if (t.keyIs( "Interval" ))
                            {
                                interval = t.floatValue();
                            }
                            else if (t.keyIs( "Periodic" ))
                            {
                                cd.setPeriodic( (strcmp( t.stringValue(), "yes" ) ? false : true) );
                            }
                        }
                        break;

                case TAG_BEHAVIOR_MODULE:
                        if (t.keyIs( "Name" ))
                        {
                            mod_name = t.stringValue();
                            qDebug() << "Searching Behavior Module '" << mod_name.c_str() << "'...";

                            m_PcBehaviorModule.setParent( this );
                            if (m_PcBehaviorModule.load( "RLBehaviorModule", mod_name.c_str() ))
                            {
                                m_BehaviorModule = reinterpret_cast<AbstractRLBehaviorModule*>(m_PcBehaviorModule.get());
                                m_BehaviorModule->loadParameters( file_name, base_path );
                                qDebug() << "Module '" << m_BehaviorModule->getName() << "' loaded!";
                            }
                            else
                            {
                                qCritical() << "Module '" << t.stringValue() << "' NOT loaded!";
                            }
                        }
                        break;

                    case TAG_QFUNCTION:
                        if (t.keyIs( "Name" ))
                        {
                            qDebug() << "Init Memory";
                            m_QFunction = new QFunction( t.stringValue(), base_path, file_name );
                            m_QFunction->setParent( this );
                            qDebug() << "Done!";
                        }
                        break;

                    case TAG_LEARNING_MODULE:
                        if (t.keyIs( "Name" ))
                        {
                            mod_name = t.stringValue();
                            qDebug() << "Searching Learning Module '" << mod_name.c_str() << "'...";

                            m_PcLearningModule.setParent( this );
                            if (m_PcLearningModule.load( "LearningModule", mod_name.c_str() ))
                            {
                                m_LearningModule = reinterpret_cast<AbstractLearningModule*>(m_PcLearningModule.get());
                                m_LearningModule->loadParameters( file_name, base_path );
                                qDebug() << "Module '" << t.stringValue() << "' loaded!";
                            }
                        }
                        break;
                }
                break;

            case XMLScanner::Token::Close:
                t.queryTagId( g_Tags );
                switch( t.tagId )
                {
                    case TAG_STIMULUS_VARIABLES:
                    {
                        qDebug() << ">>  " << list_cd.size() << " stimulus variables";
                        SensorySignalProperties* sensory_properties = new SensorySignalProperties();

                        sensory_properties->Name = "Stimulus";
                        sensory_properties->setDimension( list_cd.size() );
                        for( uint32 i = 0; i < list_cd.size(); ++i )
                        {
                            sensory_properties->setDomain( i, list_cd[i] );
                        }
                        qDebug() << (*sensory_properties);
                        m_StimulusDescription[channel_id] = sensory_properties;
                        qDebug() << ">>>  Channel" << channel_id << "Stimulus Dimension:" << m_StimulusDescription[channel_id]->getDimension();
                        def_var = false;
                        break;
                    }

                    case TAG_RESPONSE_VARIABLES:
                    {
                        qDebug() << ">> " << list_cd.size() << "response variables";
                        m_ResponseDescription.Name = "Response";
                        m_ResponseDescription.setDimension( list_cd.size() );
                        for( uint32 i = 0; i < list_cd.size(); ++i )
                        {
                            m_ResponseDescription.setDomain( i, list_cd[i] );
                        }
                        qDebug() << m_ResponseDescription;
                        qDebug() << ">>>  Response Cardinality:" << m_ResponseDescription.getCardinality().Count;
                        def_var = false;
                        break;
                    }

                    case TAG_VARIABLE:
                        if (def_var)
                        {
                            cd.init( min, max, interval );
                            list_cd.push_back( cd );
                        }
                        break;
                }
                break;
        }
    }

    qDebug() << "++  Init Description of ResponseModule";
    m_ResponseModule.setResponseSignalProperties( &m_ResponseDescription );

    qDebug() << "++  Init the V-Function";
    m_VFunction = new VFunction( m_QFunction, &m_ResponseModule );

    qDebug() << "++  Init the Q-Function";
    m_QFunction->init( m_StimulusDescription[m_DataChannelId], &m_ResponseDescription );

    m_FiniteGreedyPolicy = new FiniteGreedyPolicy( m_QFunction, &m_ResponseModule );

    if (m_BehaviorModule)
    {
        qDebug() << "++  Init BehaviorModule";

        qDebug() << "  ++  Use a FiniteGreedyPolicy";
        m_BehaviorModule->init( m_DataChannelId, m_FiniteGreedyPolicy, m_QFunction );
    }

    if (m_LearningModule)
    {
        qDebug() << "++  Init LearningModule";
        m_LearningModule->init( m_DataChannelId, m_RewardChannelId, m_DoubleSpinboxGamma->value(), m_VFunction, m_QFunction );
        m_OnPolicy = m_LearningModule->isOnlinePolicy();
    }

    qDebug() << "Loading stats of " << getName() << "...";
    loadStats();

    return true;
}


void
RLCognitiveModule::setMode( bool learning_mode )
{
    m_CheckboxLearning->setChecked( learning_mode );
    m_CheckboxUseGreedyPolicy->setChecked( !learning_mode );
}


bool
RLCognitiveModule::canReset() const
{
    return (canLearn() && m_QFunction->canReset());
}


void
RLCognitiveModule::reset()
{
    if (m_KeepPerformance)
    {
        m_Performance.reset();
    }

    if (m_QFunction)
    {
        m_QFunction->reset();
    }

    cout << "Reset CS" << endl;
    saveMemory();
}


void
RLCognitiveModule::startNewSimulation()
{
}

void
RLCognitiveModule::endSimulation()
{
}


void
RLCognitiveModule::startNewEpisode( uint64 scenario_uid )
{
    if (m_KeepPerformance)
    {
        m_Performance.startEpisode( scenario_uid );
    }

    if (m_QFunction)
    {
        m_QFunction->clearEligibilityTraces();
    }

    m_NbrStimuli = 0;
    m_DiscountedReturn = m_Return = 0.0;

    if (m_CheckboxSaveEpisodeData->isChecked())
    {
        Ipseity::String file_name;

        file_name = m_VolatileVariables.m_Workspace;
        file_name += "episode_";
        file_name += m_Performance.getNbrTotalEpisodes();
        file_name += "_sr.txt";

//        cout << "Save in " << file_name.c_str() << endl;

        m_FileEpisodeDataStimuliResponses.open( file_name.c_str(), ios_base::out );

        m_FileEpisodeDataStimuliResponses << "# Step <Stimulus> Reward <Response>" << endl << endl;
    }

    qDebug() << "Loading memory of " << getName() << "...";
    loadMemory();
}

void
RLCognitiveModule::endEpisode( bool is_success, uint32 nbr_decisions )
{
    qDebug() << "In this episode:";
    qDebug() << "  Success:" << is_success;
    qDebug() << "  Nbr responses:" << nbr_decisions;
    qDebug() << "  Discounted Return:" << m_DiscountedReturn << "- Return:" << m_Return;

    if (m_KeepPerformance)
    {
        m_Performance.endEpisode( is_success, nbr_decisions, m_DiscountedReturn );
    }

    if (m_CheckboxSaveEpisodeData->isChecked())
    {
        m_FileEpisodeDataStimuliResponses << endl << "# Discounted Return: " << m_DiscountedReturn << endl;
        m_FileEpisodeDataStimuliResponses << "# Return: " << m_Return << endl;
        m_FileEpisodeDataStimuliResponses.close();
    }

    qDebug() << "Saving memory of " << getName() << "...";
    saveMemory();
}


void
RLCognitiveModule::observe( const Sensation & x )
{
    if (m_CheckboxSaveEpisodeData->isChecked() && m_FileEpisodeDataStimuliResponses.is_open())
    {
        m_FileEpisodeDataStimuliResponses.width( 10 );
        m_FileEpisodeDataStimuliResponses << m_NbrStimuli << " " << x.getSensoryData( m_DataChannelId ) << "   " << x.getSensoryData( m_RewardChannelId ) << "   ";
    }

    if (m_NbrStimuli == 0)
    {
        m_NextStimulus = x;
    }
    else
    {
        m_CurrentStimulus = m_NextStimulus;
        m_NextStimulus = x;

//        cout << "CS: " << m_CurrentStimulus << endl;
//        cout << "NS: " << m_NextStimulus << endl;

        if (m_LearningModule)
        {
            if (!m_OnPolicy)
            {
                m_LearningModule->learn( m_CurrentStimulus, m_NextResponse, m_BestResponse, m_NextStimulus );
            }

            m_DiscountedReturn = m_NextStimulus.getSensoryData( m_RewardChannelId )[0]+m_DoubleSpinboxGamma->value()*m_DiscountedReturn;
            m_Return += m_NextStimulus.getSensoryData( m_RewardChannelId )[0];
        }
    }
    ++m_NbrStimuli;
}


const Response &
RLCognitiveModule::selectResponse()
{
    onBeforeSelect();

    if (m_BehaviorModule && m_LearningModule)
    {
        if (m_CheckboxUseGreedyPolicy->isChecked())
        {
//			cout << "BehaviorModule Response" << endl;

            if (m_FiniteGreedyPolicy)
            {
                m_NextResponse = m_FiniteGreedyPolicy->selectBestResponse( m_NextStimulus.getSensoryData( m_DataChannelId ) );
                m_BestResponse = true;
            }

//			cout << m_NbrStimuli << ": " << m_NextResponse << endl;
        }
        else
        {
//            cout << "Best Q-Valued Response" << endl;
            m_NextResponse = m_BehaviorModule->selectResponse( m_NextStimulus );
            m_BestResponse = m_BehaviorModule->isBestResponseSelected();
        }
    }
    else
    {
        m_NextResponse = Response::VoidResponse;  // null response
        m_BestResponse = false;
    }

    learn();

    if (m_CheckboxSaveEpisodeData->isChecked() && m_FileEpisodeDataStimuliResponses.is_open())
    {
        m_FileEpisodeDataStimuliResponses << m_NextResponse << endl;
    }

    return m_NextResponse;
}


void
RLCognitiveModule::suggestResponse( const Response & response )
{
    onBeforeSelect();

    m_NextResponse = response;
    m_BestResponse = false;

    learn();
}


void
RLCognitiveModule::onBeforeSelect()
{
    if (m_KeepPerformance)
    {
        m_Performance.makeDecision();
    }

    m_CurrentResponse = m_NextResponse;
}


void
RLCognitiveModule::learn()
{
    if (m_OnPolicy && m_NbrStimuli > 1 && m_CheckboxLearning->isChecked())
    {
        m_LearningModule->learn( m_CurrentStimulus, m_CurrentResponse, m_NextStimulus, m_NextResponse );
    }
}


void
RLCognitiveModule::loadMemory()
{
    if (m_QFunction)
    {
        Ipseity::String ln;

        ln = m_VolatileVariables.m_Workspace;
        ln += m_FileName.getSubString( 0, m_FileName.findFirstChar( '.' )-1 );
        ln += ".qvalues";
        if (m_QFunction->load( ln.c_str() ))
        {
            qDebug() << "Q-value memory of cognitive system " << ln.c_str() << " loaded!";
        }
    }
}

void
RLCognitiveModule::saveMemory()
{
    if (m_QFunction)
    {
        Ipseity::String ln;

        ln = m_VolatileVariables.m_Workspace;
        ln += m_FileName.getSubString( 0, m_FileName.findFirstChar( '.' )-1 );
        ln += ".qvalues";
        if (m_QFunction->save( ln.c_str() ))
        {
            qDebug() << "Q-value memory of cognitive system " << ln.c_str() << " saved!";
        }
    }
}


void
RLCognitiveModule::loadStats()
{
    if (m_KeepPerformance)
    {
        if (m_Performance.load( "perf", m_VolatileVariables.m_Workspace.c_str() ))
        {
            qDebug() << "Stats of " << m_FileName.c_str() << " loaded!";
        }
    }
}

void
RLCognitiveModule::saveStats()
{
    if (m_KeepPerformance)
    {
        if (m_Performance.save( "perf", m_VolatileVariables.m_Workspace.c_str() ))
        {
            qDebug() << "Stats of " << m_FileName.c_str() << " saved!";

            if (m_GenerateFigures)
            {
                m_Performance.generateFigures( "perf", m_VolatileVariables.m_Workspace.c_str() );
            }
        }
    }
}


//*****************************************************************************
