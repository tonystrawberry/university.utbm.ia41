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


#include "CognitiveSystem.h"

#include "AL/XMLTags_Agents.h"
#include "IO/XMLScanner.h"


#include <fstream>


const char* g_ModuleExt = ".module.xml";
const char* g_BindingsExt = ".bindings.xml";


const uint32 cstMaxEpisodes = 10000;
const uint32 cstDefaultNbrEpisodes = 10;


//*****************************************************************************


FILE*
updateHeader( const char* file_name )
{
    FILE* file;
    uint32 nbr_iterations = 0;

    if (file = fopen( file_name, "r" ))
    {
        fclose( file );

        file = fopen( file_name, "r+" );
        fscanf( file, "%15d", &nbr_iterations );
        fseek( file, 0, SEEK_SET );
        ++nbr_iterations;
    }
    else
    {
        file = fopen( file_name, "w" );
    }
    fprintf( file, "%15d ", nbr_iterations );

    return file;
}


void
updateHeader( const char* file_name, const Stimulus & stimulus )
{
    FILE* file;

    file = updateHeader( file_name );
    fprintf( file, "%8d ", stimulus.getNbrSensoryChannels() );
    for( uint32 i = 0; i < stimulus.getNbrSensoryChannels(); ++i )
    {
        fprintf( file, "%15d ", stimulus.getSensoryData( i ).getDimension() );
    }
    fprintf( file, "\n" );
    fclose( file );
}


void
updateHeader( const char* file_name, uint32 vector_size )
{
    FILE* file;

    file = updateHeader( file_name );
    fprintf( file, "%8d %15d\n", 1, vector_size );
    fclose( file );
}


//*****************************************************************************


CognitiveSystem::CognitiveSystem() :
    m_Environment( NULL ),
    m_InputManager( NULL ),
    m_StimulusIntegrationFunction( NULL )
{
}


CognitiveSystem::~CognitiveSystem()
{
    save();

    for( uint32 i = 0; i < m_PcCognitiveModules.size(); ++i )
    {
        m_PcCognitiveModules[i]->free();
        delete m_PcCognitiveModules[i];
    }

    if (m_Layout)
    {
        clearParameters();

        delete m_WidgetControlAgent;
        delete m_WidgetAgents;
        delete m_LabelSelectedCS;
        delete m_WidgetSelectedCS;
        delete m_Controls;
        delete m_Layout;
    }

    SafeDelete( m_InputManager );

    SafeDelete( m_StimulusIntegrationFunction );
}


const char*
CognitiveSystem::getBaseName() const
{
    return "CognitiveSystem";
}


const char*
CognitiveSystem::getDescription() const
{
    return "";
}


const char*
CognitiveSystem::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
CognitiveSystem::defineParameters()
{
    m_Layout = new QGridLayout();
    m_Layout->setAlignment( Qt::AlignTop | Qt::AlignLeft );

    m_WidgetControlAgent = new QCheckBox( "Control Agent" );
    m_Layout->addWidget( m_WidgetControlAgent, 0,0,1,1 );

    m_WidgetAgents = new QComboBox();
    m_WidgetAgents->setFixedSize( 80, 30 );
    m_Layout->addWidget( m_WidgetAgents, 0,1,1,1 );

    m_LabelSelectedCS = new QLabel( "Select Active Cognitive Module" );
    m_Layout->addWidget( m_LabelSelectedCS, 1,0,1,1 );

    m_WidgetSelectedCS = new QComboBox();
    m_WidgetSelectedCS->setFixedHeight( 30 );
    m_Layout->addWidget( m_WidgetSelectedCS, 1,1,1,1 );


    m_CheckboxLearningMode = new ParameterCheckBox( "FlagLearningMode", "Learn" );
    m_CheckboxLearningMode->addToLayout( m_Layout, 2,0 );


    // Auto save memory
    m_CheckboxSaveEvery = new ParameterCheckBox( "FlagSaveMemory", "Save memory every:" );
    m_CheckboxSaveEvery->setChecked( false );

    m_SpinboxNbrEpisodes = new ParameterSpinBox( "MemorySavingFrequency", "episodes" );
    m_SpinboxNbrEpisodes->setMinimum( 1 );
    m_SpinboxNbrEpisodes->setMaximum( cstMaxEpisodes );
    m_SpinboxNbrEpisodes->setValue( cstDefaultNbrEpisodes );
    m_SpinboxNbrEpisodes->setEnabled( false );

    m_CheckboxUseSIF = new ParameterCheckBox( "FlagUseSIF", "Use SIF", "Ctrl+U" );
    m_CheckboxUseSIF->setEnabled( false );
    m_CheckboxUseSIF->setChecked( false );


    m_CheckboxSavePerformance = new ParameterCheckBox( "FlagSavePerformance", "Save performance", "Ctrl+P" );
    m_CheckboxSavePerformance->setChecked( false );

    m_LineEditInteractionFilePrefix = new ParameterLineEdit( "InteractionFilePrefix", "File Prefix:", "expert" );
//    m_LineEditInteractionFilePrefix->setFixedWidth( 160 );

    m_CheckboxResetInteractions = new ParameterCheckBox( "FlagEmptyInteractionFile", "Empty file at every new simulation", "Ctrl+E" );
    m_CheckboxResetInteractions->setChecked( false );


    m_CheckboxSaveInteractions = new ParameterCheckBox( "FlagSaveInteractions", "Save interactions", "Ctrl+I" );
    m_CheckboxSaveInteractions->setChecked( false );

    m_CheckboxMergeEpisodes = new ParameterCheckBox( "FlagAppendInteractions", "Append interactions", "Ctrl+A" );
    m_CheckboxMergeEpisodes->setChecked( true );
    m_CheckboxMergeEpisodes->setEnabled( false );

    m_CheckboxIgnoreUnproductiveTransitions = new ParameterCheckBox( "FlagIgnoreUnproductiveTransitions", "Ignore unproductive transitions" );
    m_CheckboxIgnoreUnproductiveTransitions->setChecked( false );
    m_CheckboxIgnoreUnproductiveTransitions->setEnabled( false );

    m_CheckboxIncrementActions = new ParameterCheckBox( "FlagIncrementActions", "Increment actions", "Ctrl++" );
    m_CheckboxIncrementActions->setChecked( true );
    m_CheckboxIncrementActions->setEnabled( false );


    m_CheckboxSaveEvery->addToLayout( m_Layout, 3,0 );
    m_SpinboxNbrEpisodes->addToLayout( m_Layout, 3,1, false );
    m_CheckboxUseSIF->addToLayout( m_Layout, 4,0 );
    m_LineEditInteractionFilePrefix->addToLayout( m_Layout, 5,0, true );
    m_CheckboxResetInteractions->addToLayout( m_Layout, 6,0 );
    m_CheckboxSavePerformance->addToLayout( m_Layout, 7,0 );
    m_CheckboxSaveInteractions->addToLayout( m_Layout, 8,0 );
    m_CheckboxMergeEpisodes->addToLayout( m_Layout, 8,1 );
    m_CheckboxIgnoreUnproductiveTransitions->addToLayout( m_Layout, 9,0 );
    m_CheckboxIncrementActions->addToLayout( m_Layout, 9,1 );

    // Controls
    m_Controls = new QGroupBox( "Controls" );
    m_Layout->addWidget( m_Controls, 10,0,1,4 );

    connect( m_WidgetAgents, SIGNAL(currentIndexChanged(int)), this, SLOT(controlAgent(int)) );
    connect( m_WidgetSelectedCS, SIGNAL(currentIndexChanged(int)), this, SLOT(selectCognitiveModule(int)) );
    connect( m_CheckboxLearningMode, SIGNAL(toggled(bool)), this, SLOT(onChangeLearningMode(bool)) );
    connect( m_CheckboxSaveEvery, SIGNAL(toggled(bool)), this, SLOT(onChangeSaveEvery(bool)) );
    connect( m_CheckboxSaveInteractions, SIGNAL(toggled(bool)), this, SLOT(onChangeSaveInteractions(bool)) );
    connect( m_CheckboxMergeEpisodes, SIGNAL(toggled(bool)), this, SLOT(onChangeMergeEpisodes(bool)) );
}

bool
CognitiveSystem::attachView( uint32 view_id, QWidget* widget )
{
    widget->setLayout( m_Layout );

    return true;
}


void
CognitiveSystem::setEnvironment( AbstractEnvironment* environment )
{
    m_Environment = environment;
}


bool
CognitiveSystem::init( const char* base_path, const char* file_name, const Taxon & taxon )
{
    if (file_name)
    {
        m_BasePath = base_path;

        qDebug() << "Loading parameters of CS from" << file_name << "in" << base_path;
        qDebug() << "Taxon Name:" << taxon.Name.c_str() << "- TaxonId:" << taxon.Id;

        XMLScanner scanner_cs( file_name, base_path );
        XMLScanner::Token t;

        if (!scanner_cs.isLoaded())
        {
            return false;
        }

        Ipseity::String module_filename;
        Ipseity::String bindings_filename;
        PluginWrapper<AbstractStimulusIntegrationFunction>* pi_sif;

        while( scanner_cs.nextToken( &t ) )
        {
            switch( t.kind )
            {
                case XMLScanner::Token::Tag:
                    t.queryTagId( g_Tags );
                    break;

                case XMLScanner::Token::Attribute:
                    switch( t.tagId )
                    {
                        case TAG_MODULE:
                            if( t.keyIs( "Name" ) )
                            {
                                module_filename = t.stringValue();
                                if (!strstr( t.stringValue(), g_ModuleExt ))
                                {
                                    module_filename += g_ModuleExt;
                                }
                                m_ModuleNames.push_back( module_filename.c_str() );
                            }
                            break;

                        case TAG_STIMULUS_INTEGRATION_FUNCTION:
							if( t.keyIs( "Name" ) )
							{
                                pi_sif = new PluginWrapper<AbstractStimulusIntegrationFunction>();

                                if (pi_sif->load( "StimulusIntegrationFunction", t.stringValue() ))
								{
                                    m_PcStimulusIntegrationFunction = pi_sif;
                                    m_StimulusIntegrationFunction = reinterpret_cast<AbstractStimulusIntegrationFunction*>(pi_sif->get());
                                    m_CheckboxUseSIF->setEnabled( true );
                                    m_CheckboxUseSIF->setText( tr("Use SIF: %1").arg(m_StimulusIntegrationFunction->getName()) );
                                }
								else
								{
                                    delete pi_sif;
								}
							}
							break;

                        case TAG_CONTROL_BINDINGS:
                            if( t.keyIs( "Name" ) )
                            {
                                bindings_filename = t.stringValue();
                                if (!strstr(t.stringValue(), g_BindingsExt ))
                                {
                                    bindings_filename += g_BindingsExt;
                                }
                            }
                            break;
                    }
                    break;

                case XMLScanner::Token::Close:
                    t.queryTagId( g_Tags );
                    break;
            }
        }

        m_Taxon.Status = taxon.Status;

        m_WidgetControlAgent->setEnabled( false );
        m_WidgetAgents->setEnabled( false );

        if (m_Taxon.Status & TS_Controllable)
        {
            XMLScanner scanner_cb( bindings_filename.c_str(), base_path );
            EventParameters event_parameters;

            if (scanner_cb.isLoaded())
            {
                m_InputManager = new IpseityInputManager();
                m_InputManager->reset();

                event_parameters.Vector.setDimension( 0 );
                event_parameters.CommandId = EP_COMMAND_CHANGE_CS_STATUS;
                m_InputManager->addEvent( "ChangeControlMode", "F1", true, event_parameters );

                m_WidgetControlAgent->setChecked( true );

                Ipseity::String event_name;
                Ipseity::String keys;
                bool single_event;
                bool controllable_cs = false;

                while( scanner_cb.nextToken( &t ) )
                {
                    switch( t.kind )
                    {
                        case XMLScanner::Token::Tag:
                            t.queryTagId( g_Tags );
                            break;

                        case XMLScanner::Token::Attribute:
                            switch( t.tagId )
                            {
                                case TAG_EVENT:
                                    if( t.keyIs( "Name" ) )
                                    {
                                        event_name = t.stringValue();
                                        event_parameters.CommandId = 0;
                                        single_event = false;
                                    }
                                    else if( t.keyIs( "Keys" ) )
                                    {
                                        keys = t.stringValue();
                                        controllable_cs = true;
                                    }
                                    else if( t.keyIs( "Type" ) )
                                    {
                                        if (!strcmp( t.stringValue(), "Single" ))
                                        {
                                            single_event = true;
                                        }
                                    }
                                    else if (t.keyIs( "Command" ))
                                    {
                                        if (!strcmp( t.stringValue(), "SuggestResponse" ))
                                        {
                                            event_parameters.CommandId = EP_COMMAND_SUGGEST_RESPONSE;
                                        }
                                        else if (!strcmp( t.stringValue(), "GenerateReward" ))
                                        {
                                            event_parameters.CommandId = EP_COMMAND_GENERATE_REWARD;
                                        }
                                        else if (!strcmp( t.stringValue(), "SendToEnvironment" ))
                                        {
                                            event_parameters.CommandId = EP_COMMAND_SENDTO_ENVIRONMENT;
                                        }
                                    }
                                    else if (t.keyIs( "Parameters" ))
                                    {
                                        uint32 nbr = t.nbrKeyParameters();

                                        event_parameters.Vector.setDimension( nbr );

                                        for( uint32 i = 0; i < nbr; ++i )
                                        {
                                            event_parameters.Vector[i] = t.floatValue( i );
                                        }
                                    }
                                    break;
                            }
                            break;

                        case XMLScanner::Token::Close:
                            t.queryTagId( g_Tags );
                            switch( t.tagId )
                            {
                                case TAG_EVENT:
                                    if (controllable_cs)
                                    {
                                        m_InputManager->addEvent( event_name.c_str(), keys.c_str(), single_event, event_parameters );
                                    }
                                    break;
                            }
                            break;
                    }
                }

                qDebug() << "Control commands";
//				qDebug() << (*m_InputManager);
            }
        }

        m_PerformancePath = m_VolatileVariables.m_Workspace;
        m_PerformancePath += "CognitiveSystems/";
        m_PerformancePath += "Taxons/";
        m_PerformancePath += taxon.Name;
        m_PerformancePath += "/";
        QDir().mkpath( m_PerformancePath.c_str() );

		qDebug() << "Attach " << m_ModuleNames.size() << "cognitive module(s)...";

        m_SelectedCognitiveModuleId = 0;

        m_WidgetSelectedCS->setEnabled( (m_ModuleNames.size() > 0) );

        PluginWrapper<AbstractCognitiveModule>* pi_cs;
        AbstractCognitiveModule* cs;
        Ipseity::String module_name;
        Ipseity::String performance_path;

        for( uint32 i = 0; i < m_ModuleNames.size(); ++i )
        {
            XMLScanner scanner_module( m_ModuleNames[i].c_str(), base_path );

            while( scanner_module.nextToken( &t ) )
            {
                switch( t.kind )
                {
                    case XMLScanner::Token::Tag:
                        t.queryTagId( g_Tags );
                        break;

                    case XMLScanner::Token::Attribute:
                        switch( t.tagId )
                        {
                            case TAG_MODULE:
                                if( t.keyIs( "Name" ) )
                                {
                                    pi_cs = new PluginWrapper<AbstractCognitiveModule>();

                                    pi_cs->setParent( this );
                                    if (pi_cs->load( "CognitiveModule", t.stringValue() ))
                                    {
                                        cs = reinterpret_cast<AbstractCognitiveModule*>(pi_cs->get());
                                        cs->setSearchRootPath( pi_cs->getPluginPath() );

                                        qDebug() << "Found in " << pi_cs->getPluginPath();

                                        module_name = m_ModuleNames[i];
                                        module_name = module_name.getSubString( 0, module_name.findFirstChar( '.' )-1 );

                                        if (taxon.Status & TS_SharedCognitiveSystem)
                                        {
                                            performance_path = m_PerformancePath;
                                            performance_path += module_name;
                                            performance_path += "/";
                                            QDir().mkpath( performance_path.c_str() );

                                            cs->setWorkspace( performance_path.c_str() );

                                            qDebug() << "***  Init module" << m_ModuleNames[i].c_str();
                                            if (cs->init( base_path, m_ModuleNames[i].c_str(), taxon, m_InputManager ))
                                            {
                                                m_PcCognitiveModules.push_back( pi_cs );
                                                m_CognitiveModules.push_back( cs );

                                                m_WidgetSelectedCS->addItem( module_name.c_str() );
                                            }
                                            else
                                            {
                                                delete pi_cs;
                                            }
                                        }
                                        else
                                        {
                                            m_PcCognitiveModules.push_back( pi_cs );
                                            m_CognitiveModules.push_back( cs );
                                        }
                                    }
                                    else
                                    {
                                        delete pi_cs;
                                    }
                                }
                                break;
                        }
                        break;

                    case XMLScanner::Token::Close:
                        t.queryTagId( g_Tags );
                        break;
                }
            }
        }

        qDebug() << "CS initialized!";

        return ((m_ModuleNames.size() == 0) || (m_CognitiveModules.size() > 0));
    }

    return false;
}


void
CognitiveSystem::setMode( bool learning_mode )
{
    m_LearningMode = learning_mode;
    for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
    {
        m_CognitiveModules[i]->setMode( learning_mode );
    }
}


bool
CognitiveSystem::canReset() const
{
	for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
	{
		if (m_CognitiveModules[i]->canReset())
		{
			return true;
		}
	}

    return false;
}


void
CognitiveSystem::reset()
{
	for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
	{
		m_CognitiveModules[i]->reset();
	}
}


uint32
CognitiveSystem::getNbrAgents() const
{
    return m_AgentIds.size();
}


void
CognitiveSystem::startNewSimulation()
{
    Ipseity::String filename;
    std::ofstream file;

    m_Prefix = m_PerformancePath;
    m_Prefix += m_LineEditInteractionFilePrefix->toString();
    if (m_LearningMode)
    {
        m_Prefix += "_learn";
    }
    else
    {
        m_Prefix += "_eval";
    }

    if (m_CheckboxResetInteractions->isChecked())
    {
        if (m_CheckboxSaveInteractions->isChecked() && m_CheckboxMergeEpisodes->isChecked())
        {
            filename = m_Prefix;
            filename += "_episodes.txt";
            file.open( filename.c_str(), std::ofstream::out );
            file.close();
            filename = m_Prefix;
            filename += "_states.txt";
            file.open( filename.c_str(), std::ofstream::out );
            file.close();
            filename = m_Prefix;
            filename += "_next_states.txt";
            file.open( filename.c_str(), std::ofstream::out );
            file.close();
            filename = m_Prefix;
            filename += "_actions.txt";
            file.open( filename.c_str(), std::ofstream::out );
            file.close();
        }

        if (m_CheckboxSavePerformance->isChecked())
        {
            filename = m_Prefix;
            filename += "_perf.txt";
            file.open( filename.c_str(), std::ofstream::out );
            file.close();
        }
    }

    for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
    {
        m_CognitiveModules[i]->startNewSimulation();
    }
}


void
CognitiveSystem::endSimulation()
{
    for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
    {
        m_CognitiveModules[i]->endSimulation();
    }
}


void
CognitiveSystem::startNewEpisode( uint32 nbr_total_episodes, uint64 scenario_uid )
{
    m_NbrEpisodes = nbr_total_episodes;

	if (m_CognitiveModules.size())
	{
        m_AgentSensation.clear();
		m_AgentNbrDecisions.clear();

		for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
		{
			m_CognitiveModules[i]->startNewEpisode( scenario_uid );
		}
	}
}

void
CognitiveSystem::endEpisode( bool is_success )
{
    if (m_Taxon.Status & TS_Controllable)
    {
        uint32 nbr_decisions = 0;
        uint32 nbr_agents = 0;

        for( std::map<uint32,uint32>::const_iterator itr = m_AgentNbrDecisions.begin(); itr != m_AgentNbrDecisions.end(); ++itr )
        {
            nbr_decisions += (*itr).second;
            ++nbr_agents;
        }

        if (nbr_agents == 0)
        {
            qDebug() << "***********************!!!!!";
            qDebug() << "# agents = 0!!!!!";
            qDebug() << "***********************!!!!!";
        }
        else
        {
            nbr_decisions /= nbr_agents;
        }

        for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
        {
            m_CognitiveModules[i]->endEpisode( is_success, nbr_decisions );
        }

        if (m_CheckboxSavePerformance->isChecked())
        {
            Ipseity::String filename;
            std::ofstream file;

            filename = m_Prefix;
            filename += "_perf.txt";

            file.open( filename.c_str(), std::ofstream::app );
            file << nbr_decisions << " " << m_Environment->getAgentPerformance( m_AgentIds[0] ) << endl;
            file.close();
        }
    }

    if (m_CheckboxSaveEvery->isChecked() && (m_NbrEpisodes % m_SpinboxNbrEpisodes->value()) == 0)
    {
        save();
    }

    m_AgentIds.clear();
    m_WidgetAgents->clear();
    m_WidgetAgents->setEnabled( false );
}


void
CognitiveSystem::linkAgent( uint32 agent_id, const char* agent_name )
{
    m_AgentIds.push_back( agent_id );
	m_WidgetAgents->addItem( agent_name );

    m_AgentNbrDecisions[agent_id] = 0;

    if (m_AgentIds.size() == 1)
    {
        m_WidgetControlAgent->setEnabled( (m_InputManager != NULL) );
        m_WidgetAgents->setEnabled( (m_InputManager != NULL) );
        controlAgent( 0 );

        if (!(m_Taxon.Status & TS_SharedCognitiveSystem))
        {
            Ipseity::String module_name;
            Ipseity::String performance_path;

            m_PerformancePath = m_VolatileVariables.m_Workspace;
            m_PerformancePath += "CognitiveSystems/";
            m_PerformancePath += "Agents/";
            m_PerformancePath += agent_name;
            m_PerformancePath += "/";
            QDir().mkpath( m_PerformancePath.c_str() );

            for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
            {
                module_name = m_ModuleNames[i];
                module_name = module_name.getSubString( 0, module_name.findFirstChar( '.' )-1 );

                performance_path = m_PerformancePath;
                performance_path += module_name;
                performance_path += "/";
                QDir().mkpath( performance_path.c_str() );

                m_CognitiveModules[i]->setWorkspace( performance_path.c_str() );

                qDebug() << "***  Init module" << m_ModuleNames[i].c_str();
                if (m_CognitiveModules[i]->init( m_BasePath.c_str(), m_ModuleNames[i].c_str(), m_Taxon, m_InputManager ))
                {
                    m_WidgetSelectedCS->addItem( module_name.c_str() );
                }
            }
        }
    }
}


void
CognitiveSystem::perceive( uint32 agent_id, const Stimulus & x )
{
    if (m_AgentSensation.find( agent_id ) != m_AgentSensation.end())
    {
        m_AgentOldSensation[agent_id] = m_AgentSensation[agent_id];
    }

    m_AgentSensation[agent_id] = x;
}


const Response &
CognitiveSystem::selectResponse( uint32 agent_id, bool last_decision )
{
    if (m_AgentSensation.find( agent_id ) != m_AgentSensation.end())
    {
        if (m_StimulusIntegrationFunction)
        {
            m_AgentModifiedSensation[agent_id] = m_StimulusIntegrationFunction->integrate( m_AgentSensation[agent_id] );
            m_AgentModifiedSensation[agent_id].setTerminal( m_AgentSensation[agent_id].isTerminal() );
            m_AgentModifiedSensation[agent_id].setGoal( m_AgentSensation[agent_id].isAGoal() );
        }

        if (m_StimulusIntegrationFunction && m_CheckboxUseSIF->isChecked())
        {
            for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
            {
                m_CognitiveModules[i]->observe( m_AgentModifiedSensation[agent_id] );
            }
        }
        else
        {
            for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
            {
                m_CognitiveModules[i]->observe( m_AgentSensation[agent_id] );
            }
        }
    }

    bool controlled = false;
    bool controlled_response = false;
    bool response_found = false;
    bool save_interactions;

    if (m_InputManager)
    {
        if (m_Taxon.Status & TS_Controllable && m_InputManager->isEventOccurred( 0, EP_COMMAND_CHANGE_CS_STATUS ))
        {
            m_WidgetControlAgent->toggle();
        }

        if (m_WidgetControlAgent->isChecked() && m_ControlledAgentId == agent_id)
        {
            for( uint32 i = 1; i < m_InputManager->getNbrEvents( EP_COMMAND_SUGGEST_RESPONSE ); ++i )
            {
                if (m_InputManager->isEventOccurred( i, EP_COMMAND_SUGGEST_RESPONSE ))
                {
                    m_Response = m_InputManager->getEventParameters( i, EP_COMMAND_SUGGEST_RESPONSE ).Vector;
                    controlled = true;
                    break;
                }
            }

            if (!controlled)
            {
//                cout << "oops" << endl;
                m_Response = m_InputManager->getEventParameters( 0, EP_COMMAND_SUGGEST_RESPONSE ).Vector;
            }

            for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
            {
                m_CognitiveModules[i]->suggestResponse( m_Response );
            }

            response_found = true;
            controlled_response = true;
        }
    }

    if (!controlled_response && m_CognitiveModules.size())
    {
        m_Response = m_CognitiveModules[m_SelectedCognitiveModuleId]->selectResponse();

        for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
        {
            if (i != m_SelectedCognitiveModuleId)
            {
                m_CognitiveModules[i]->suggestResponse( m_Response );
            }
        }

        response_found = true;
    }

    save_interactions = (m_InputManager && controlled) || (!m_InputManager && response_found);

    if (save_interactions)
    {
        ++m_AgentNbrDecisions[agent_id];
    }

    if (m_CheckboxIgnoreUnproductiveTransitions->isChecked() &&
        m_AgentSensation.find( agent_id ) != m_AgentSensation.end() &&
        m_AgentOldSensation[agent_id] == m_AgentSensation[agent_id])
    {
        save_interactions = false;
    }

    //*********************************************
    // Save interactions
    if (save_interactions)
    {
        if (m_AgentSensation.find( agent_id ) != m_AgentSensation.end())
        {
            if (m_ControlledAgentId == agent_id && m_CheckboxSaveInteractions->isChecked())
            {
                Ipseity::String filename;

                if (!last_decision)
                {
                    std::ofstream file;

                    // Episode
                    filename = m_PerformancePath;
                    if (m_CheckboxMergeEpisodes->isChecked())
                    {
                        filename += m_LineEditInteractionFilePrefix->toString();
                    }
                    else
                    {
                        filename += "episode";
                        filename += m_NbrEpisodes;
                    }
                    filename += "_episodes.txt";

                    updateHeader( filename.c_str(), 1 );

                    file.open( filename.c_str(), std::ofstream::app );
                    if (m_AgentNbrDecisions[agent_id] == 1)
                    {
                        file << 1 << endl;
                    }
                    else
                    {
                        file << 0 << endl;
                    }
                    file.close();


                    // Current state
                    filename = m_PerformancePath;
                    if (m_CheckboxMergeEpisodes->isChecked())
                    {
                        filename += m_LineEditInteractionFilePrefix->toString();
                    }
                    else
                    {
                        filename += "episode";
                        filename += m_NbrEpisodes;
                    }
                    filename += "_states.txt";

                    if (m_StimulusIntegrationFunction)
                    {
                        updateHeader( filename.c_str(), m_AgentModifiedSensation[agent_id] );
                    }
                    else
                    {
                        updateHeader( filename.c_str(), m_AgentSensation[agent_id] );
                    }

                    file.open( filename.c_str(), std::ofstream::app );
                    if (m_StimulusIntegrationFunction)
                    {
                        file << m_AgentModifiedSensation[agent_id] << endl;
                    }
                    else
                    {
                        file << m_AgentSensation[agent_id] << endl;
                    }
                    file.close();
                }

                if (m_AgentNbrDecisions[agent_id] > 1)
                {
                    filename = m_PerformancePath;
                    if (m_CheckboxMergeEpisodes->isChecked())
                    {
                        filename += m_LineEditInteractionFilePrefix->toString();
                    }
                    else
                    {
                        filename += "episode";
                        filename += m_NbrEpisodes;
                    }
                    filename += "_next_states.txt";

                    if (m_StimulusIntegrationFunction)
                    {
                        updateHeader( filename.c_str(), m_AgentModifiedSensation[agent_id] );
                    }
                    else
                    {
                        updateHeader( filename.c_str(), m_AgentSensation[agent_id] );
                    }

                    std::ofstream file;

                    file.open( filename.c_str(), std::ofstream::app );
                    if (m_StimulusIntegrationFunction)
                    {
                        file << m_AgentModifiedSensation[agent_id] << endl;
                    }
                    else
                    {
                        file << m_AgentSensation[agent_id] << endl;
                    }
                    file.close();
                }
            }
        }

        if (m_ControlledAgentId == agent_id &&
            m_AgentSensation.find( agent_id ) != m_AgentSensation.end() &&
            m_CheckboxSaveInteractions->isChecked() &&
            !last_decision)
        {
            Ipseity::String filename;

            filename = m_PerformancePath;
            if (m_CheckboxMergeEpisodes->isChecked())
            {
                filename += m_LineEditInteractionFilePrefix->toString();
            }
            else
            {
                filename += "episode";
                filename += m_NbrEpisodes;
            }
            filename += "_actions.txt";

            updateHeader( filename.c_str(), m_Response.getDimension() );

            std::ofstream file;
            Response response;

            response = m_Response;
            if (m_CheckboxIncrementActions->isChecked())
            {
                ++response[0];
            }

            file.open( filename.c_str(), std::ofstream::app );
            file << response << endl;
            file.close();
        }
    }
    //*********************************************

    if (response_found)
    {
        return m_Response;
    }
    else
    {
        return Response::VoidResponse;
    }
}


void
CognitiveSystem::pressKey( int32 key )
{
    if (m_InputManager)
    {
        m_InputManager->pressKey( key );
    }
}


void
CognitiveSystem::releaseKey( int32 key )
{
    if (m_InputManager)
    {
        m_InputManager->releaseKey( key );
    }
}


//*****************************************************************************


void
CognitiveSystem::save()
{
    if (m_Taxon.Status & TS_Controllable)
    {
        for( uint32 i = 0; i < m_CognitiveModules.size(); ++i )
        {
            qDebug() << "Saving memory of" << m_CognitiveModules[i]->getName() << "...";
            m_CognitiveModules[i]->saveMemory();

            qDebug() << "Saving statistics of" << m_CognitiveModules[i]->getName() << "...";
            m_CognitiveModules[i]->saveStats();
        }
    }
}


//*****************************************************************************


void
CognitiveSystem::controlAgent( int agent_id )
{
    if (m_Taxon.Status & TS_Controllable && agent_id < m_AgentIds.size())
    {
        m_ControlledAgentId = m_AgentIds[agent_id];
    }
}


void
CognitiveSystem::selectCognitiveModule( int module_id )
{
    m_SelectedCognitiveModuleId = module_id;
    m_CognitiveModules[module_id]->attachView( 0, m_Controls );
}


void
CognitiveSystem::onChangeLearningMode( bool toggle )
{
    setMode( toggle );
}


void
CognitiveSystem::onChangeSaveEvery( bool toggle )
{
    m_SpinboxNbrEpisodes->setEnabled( toggle );
}


void
CognitiveSystem::onChangeSaveInteractions( bool toggle )
{
    m_CheckboxMergeEpisodes->setEnabled( toggle );
    m_CheckboxIgnoreUnproductiveTransitions->setEnabled( toggle );
    m_CheckboxIncrementActions->setEnabled( toggle );

//    bool flag = (toggle && m_CheckboxMergeEpisodes->isChecked());
//    m_LineEditInteractionFilePrefix->setEnabled( flag );
//    m_CheckboxResetInteractions->setEnabled( flag );
}


void
CognitiveSystem::onChangeMergeEpisodes( bool toggle )
{
//    if (m_CheckboxSaveInteractions->isChecked())
//    {
//        m_LineEditInteractionFilePrefix->setEnabled( toggle );
//        m_CheckboxResetInteractions->setEnabled( toggle );
//    }
}
