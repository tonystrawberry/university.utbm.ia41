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


#include "MultiAgentEnvironment.h"

#include "StringFunctions.h"
#include "IO/XMLScanner.h"
#include "AL/XMLTags_Environments.h"

#include <QIcon>
#include <QKeySequence>
#include <QTime>
#include <QDebug>


/*
    1- initialize
        - update scenarii
        - update role information (autonomous, learning)
    2- simulate
        1- faceCurrentScenario
        2- addInfluences
        3- applyLaws
        4- agentsEndEpisode
*/


MultiAgentEnvironment::MultiAgentEnvironment() :
    AbstractMultiAgentEnvironment(),
    m_MultiAgentEnvironment( NULL ),
    m_NbrTotalEpisodes( 0 ),
    m_Layout( NULL )
{
}


MultiAgentEnvironment::~MultiAgentEnvironment()
{
    saveParameters();

    if (m_Layout)
    {
        clearParameters();

        delete m_LayoutStatistics;
        delete m_GroupBoxGeneralParameters;

        m_PcMAE.free();
        delete m_GroupBoxSpecificParameters;
        delete m_GroupBoxDisturbanceParameters;

        delete m_Layout;
    }
}


//*****************************************************************************


const QIcon &
MultiAgentEnvironment::getIcon() const
{
    return m_MultiAgentEnvironment->getIcon();
}


const char*
MultiAgentEnvironment::getBaseName() const
{
    return "MultiAgentEnvironment";
}


const char*
MultiAgentEnvironment::getDescription() const
{
    return "Generic multi-agent environment";
}


const char*
MultiAgentEnvironment::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
MultiAgentEnvironment::defineParameters()
{
    m_Layout = new QGridLayout();
    m_Layout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    m_GroupBoxGeneralParameters = new QGroupBox( "General" );
    m_Layout->addWidget( m_GroupBoxGeneralParameters, 0,0 );

    m_LayoutStatistics = new QGridLayout( m_GroupBoxGeneralParameters );
    m_LayoutStatistics->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    m_CheckboxSaveStats = new ParameterCheckBox( "FlagSaveStats", "Save Stats", "Ctrl+S" );
    m_CheckboxSaveStats->setChecked( true );
    m_LayoutStatistics->addWidget( m_CheckboxSaveStats, 1,1,1,1 );

    m_CheckboxSaveEpisodeStats = new ParameterCheckBox( "FlagSaveEpisodeStats", "Save Episode Stats", "Ctrl+E" );
    m_CheckboxSaveEpisodeStats->setChecked( false );
    m_LayoutStatistics->addWidget( m_CheckboxSaveEpisodeStats, 1,2,1,1 );

    m_CheckboxMaxIterations = new ParameterCheckBox( "FlagMaxIterations", "Set Max Iterations", "Ctrl+I" );
    m_CheckboxMaxIterations->setChecked( false );
    m_LayoutStatistics->addWidget( m_CheckboxMaxIterations, 2,1,1,1 );

    m_SpinboxMaxIterations = new ParameterSpinBox( "MaxIterations", "Maximum Number of iterations" );
    m_SpinboxMaxIterations->setMinimum( 0 );
    m_SpinboxMaxIterations->setMaximum( 100000 );
    m_LayoutStatistics->addWidget( m_SpinboxMaxIterations, 2,2,1,1 );

    m_GroupBoxSpecificParameters = new QGroupBox( "Specific" );
    m_Layout->addWidget( m_GroupBoxSpecificParameters, 1,0 );

    m_GroupBoxDisturbanceParameters = new QGroupBox( "Disturbance" );
    m_Layout->addWidget( m_GroupBoxDisturbanceParameters, 2,0 );
}


bool
MultiAgentEnvironment::attachView( uint32 view_id, QWidget* widget )
{
    m_MultiAgentEnvironment->attachView( 0, m_GroupBoxSpecificParameters );
    m_MultiAgentEnvironment->attachView( 1, m_GroupBoxDisturbanceParameters );
    widget->setLayout( m_Layout );

    return true;
}


//*****************************************************************************


uint32
MultiAgentEnvironment::getNbrEpisodes() const
{
    return m_NbrTotalEpisodes;
}


bool
MultiAgentEnvironment::sendCommand( const char *command )
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->sendCommand( command );
    }
    else
    {
        return false;
    }
}


bool
MultiAgentEnvironment::pressKey( QKeyEvent* ev )
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->pressKey( ev );
    }
    else
    {
        return false;
    }
}


void
MultiAgentEnvironment::initialize( const char* base_path, const char* environment_instance_name, const char* config_filename )
{
#ifdef TRACE
    cout << "> Loading Environment '" << m_EnvironmentInstanceName.c_str() << "' ..." << endl;
#endif
    m_BasePath = base_path;

    qDebug() << "Base path:" << getBasePath();

    m_PcMAE.setParent( this );
    if (m_PcMAE.load( "EnvironmentInstance", environment_instance_name ))
    {
        m_MultiAgentEnvironment = reinterpret_cast<AbstractMultiAgentEnvironmentInstance*>(m_PcMAE.get());
        m_MultiAgentEnvironment->setParentEnvironment( this );

        m_Name = m_MultiAgentEnvironment->getName();
    }

    if (m_MultiAgentEnvironment)
    {
//        cout << "Init env with config file: " << config_filename << endl;
        m_MultiAgentEnvironment->initialize( config_filename );
        onInitialize();

        loadParameters();
    }
}


bool
MultiAgentEnvironment::requireScenarioFile() const
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->requireScenarioFile();
    }
    else
    {
        return false;
    }
}


//*****************************************************************************


uint32
MultiAgentEnvironment::getNbrTaxons() const
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->getNbrTaxons();
    }
    else
    {
        return 0;
    }
}


bool
MultiAgentEnvironment::fetchTaxon( uint32 taxon_id, Taxon & taxon ) const
{
    if (m_MultiAgentEnvironment && taxon_id < getNbrTaxons())
    {
        taxon = m_MultiAgentEnvironment->getTaxon( taxon_id );

        return true;
    }
    else
    {
        return false;
    }
}


//*****************************************************************************


uint32
MultiAgentEnvironment::getNbrSensoryInterfaces() const
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->getNbrSensoryInterfaces();
    }
    else
    {
        return 0;
    }
}


bool
MultiAgentEnvironment::fetchSensoryInterface( uint32 sensory_interface_id, SensoryInterface & sensory_interface ) const
{
    if (m_MultiAgentEnvironment && sensory_interface_id < getNbrSensoryInterfaces())
    {
        sensory_interface = m_MultiAgentEnvironment->getSensoryInterface( sensory_interface_id );

        return true;
    }
    else
    {
        return false;
    }
}


bool
MultiAgentEnvironment::checkSensoryInterfaceCompatibility( uint32 taxon_id, const SensoryInterface & sensory_interface ) const
{
    if (m_MultiAgentEnvironment)
    {
        Taxon taxon;

        if (fetchTaxon( taxon_id, taxon ))
        {
            return m_MultiAgentEnvironment->getSensoryInterface( taxon.SensoryInterfaceId ).checkCompatibility( sensory_interface );
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


//*****************************************************************************


bool
MultiAgentEnvironment::isIdle( uint32 agent_id ) const
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->isIdle( agent_id );
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_IDLE] << endl;
#endif
        return true;
    }
}


const Stimulus &
MultiAgentEnvironment::getCurrentStimulus( uint32 agent_id )
{
    if (m_MultiAgentEnvironment)
    {
        m_Stimulus = m_MultiAgentEnvironment->getCurrentStimulus( agent_id );
        m_Agents[agent_id]->setInteractionState( m_Stimulus.getStatus() );

        return m_Stimulus;
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_SENSATION] << endl;
#endif
        return Stimulus::VoidStimulus;
    }
}


const PerformanceMeasure &
MultiAgentEnvironment::getAgentPerformance( uint32 agent_id )
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->getAgentPerformance( agent_id );
    }
    else
    {
        return m_VoidPerformance;
    }
}


//*****************************************************************************


uint32
MultiAgentEnvironment::getNbrMainRenderingWindows() const
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->getNbrMainRenderingWindows();
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_CANRENDER] << endl;
#endif
        return 0;
    }
}


uint32
MultiAgentEnvironment::getNbrScenarioRenderingWindows() const
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->getNbrScenarioRenderingWindows();
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_CANRENDER] << endl;
#endif
        return 0;
    }
}


const char*
MultiAgentEnvironment::getWindowName( uint32 window_id )
{
    if (m_MultiAgentEnvironment)
    {
        return m_MultiAgentEnvironment->getWindowName( window_id );
    }
    else
    {
        return "";
    }
}


void
MultiAgentEnvironment::initRenderer()
{
    if (m_MultiAgentEnvironment)
    {
        m_MultiAgentEnvironment->initRenderer();
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_INIT_RENDER] << endl;
#endif
    }
}


void
MultiAgentEnvironment::quitRenderer()
{
    if (m_MultiAgentEnvironment)
    {
        m_MultiAgentEnvironment->quitRenderer();
    }
}


void
MultiAgentEnvironment::resizeWindow( uint32 window_id, int32 w, int32 h )
{
    if (m_MultiAgentEnvironment)
    {
        m_MultiAgentEnvironment->resizeWindow( window_id, w, h );
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_RESIZE] << endl;
#endif
    }
}


void
MultiAgentEnvironment::renderWindow( uint32 window_id, QGLWidget* widget )
{
    if (m_MultiAgentEnvironment && m_CurrentScenarioLoaded)
    {
        m_MultiAgentEnvironment->renderWindow( window_id, widget );
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_RENDER] << endl;
#endif
    }
}


//*****************************************************************************


void
MultiAgentEnvironment::integrateInfluences( float64 elapsed_milliseconds )
{
//    cout << elapsed_milliseconds << endl;

    if (m_MultiAgentEnvironment)
    {
        m_MultiAgentEnvironment->applyLaws( elapsed_milliseconds );

        if (m_CheckboxSaveEpisodeStats->isChecked())
        {
            fprintf( m_EpisodeStatFile, "%10.2f ", m_CurrentTime );
            fprintf( m_EpisodeStatFile, "%10d ", m_NbrTicks );
            m_MultiAgentEnvironment->saveEpisodeStats( m_EpisodeStatFile );
            fprintf( m_EpisodeStatFile, "\n" );
        }

        m_EpisodeEnded = (m_MultiAgentEnvironment->isEpisodeEnded() || ((m_MaxTicks > 0) && (m_NbrTicks >= m_MaxTicks)) || ((m_MaxTime > 0.0) && (m_CurrentTime >= m_MaxTime)));

        if (!m_EpisodeEnded)
        {
            for( uint32 agent_id = 0; agent_id < getNbrAgents(); ++agent_id )
            {
                if (m_Agents[agent_id] && m_Agents[agent_id]->isTerminal())
                {
                    m_EpisodeEnded = true;
                    break;
                }
            }
        }

//		cout << "Integration done!" << endl;
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_APPLY_LAWS] << endl;
#endif
    }
}


void
MultiAgentEnvironment::addInfluence( uint32 agent_id, const Response & influence )
{
    if (m_MultiAgentEnvironment && influence.getDimension() > 0)
    {
        m_MultiAgentEnvironment->addInfluence( agent_id, influence );
    }
//    else
//    {
//        cout << g_ClassName << g_Errors[ERR_INFLUENCE] << endl;
//    }
}


void
MultiAgentEnvironment::onEndEpisode()
{
    if (m_CheckboxSaveStats->isChecked())
    {
//        fprintf( m_StatFile, "%10d ", m_NbrTotalEpisodes );
        fprintf( m_StatFile, "%10d ", m_TrialId );
        fprintf( m_StatFile, "%10d ", m_EpisodeId );
        fprintf( m_StatFile, "%02d ", m_EpisodeEnded );
        fprintf( m_StatFile, "%10d ", m_NbrTicks );
        fprintf( m_StatFile, "%10d ", m_Time.elapsed() );
        if (m_MultiAgentEnvironment)
        {
            m_MultiAgentEnvironment->saveStats( m_StatFile );
        }
        fprintf( m_StatFile, " %d", m_ScenarioUid ); //m_ScenarioFileName.c_str() );
        fprintf( m_StatFile, "\n" );
        fclose( m_StatFile );
    }

    if (m_CheckboxSaveEpisodeStats->isChecked())
    {
        fclose( m_EpisodeStatFile );
		fclose( m_EpisodeInteractionsFile );
	}

    if (m_MultiAgentEnvironment)
    {
        m_MultiAgentEnvironment->onEndEpisode();
    }

    m_CheckboxSaveStats->setEnabled( true );
    m_CheckboxSaveEpisodeStats->setEnabled( true );
}


void
MultiAgentEnvironment::load()
{
    if (m_MultiAgentEnvironment)
    {
        ++m_NbrTotalEpisodes;

        m_CheckboxSaveStats->setEnabled( false );
        m_CheckboxSaveEpisodeStats->setEnabled( false );

        m_Time.start();

        cout << "Load scenario: '" << m_ScenarioFileName.c_str() << "'" << endl;

        m_MultiAgentEnvironment->load( m_ScenarioFileName.c_str() );
        m_MultiAgentEnvironment->disturb();

        if (m_CheckboxMaxIterations->isChecked())
        {
            setMaxIterations( m_SpinboxMaxIterations->value() );
        }
        else
        {
            setMaxIterations( m_MultiAgentEnvironment->getMaxIterations() );
        }

        Ipseity::String file_name;
        char* mode;

        if (m_LearningMode)
        {
            mode = "l";
        }
        else
        {
            mode = "e";
        }

        file_name = m_VolatileVariables.m_Workspace;
        file_name += "stats_";
        file_name += mode;
        file_name += ".txt";

        if (m_CheckboxSaveStats->isChecked())
        {
            m_StatFile = fopen( file_name.c_str(), "a" );
        }
        else
        {
            m_StatFile = NULL;
        }

        if (m_CheckboxSaveEpisodeStats->isChecked())
        {
			char ep_filename[256];

            sprintf( ep_filename, "%sstats_%s_t%06d_ep%08d.txt", m_VolatileVariables.m_Workspace.c_str(), mode, m_TrialId, m_EpisodeId );
			m_EpisodeStatFile = fopen( ep_filename, "a" );

            sprintf( ep_filename, "%sinteractions_%s_t%06d_ep%08d.txt", m_VolatileVariables.m_Workspace.c_str(), mode, m_TrialId, m_EpisodeId );
			m_EpisodeInteractionsFile = fopen( ep_filename, "a" );
        }
        else
        {
            m_EpisodeStatFile = NULL;
			m_EpisodeInteractionsFile = NULL;
        }
    }
    else
    {
#ifdef TRACE
        cout << g_ClassName << g_Errors[ERR_LOAD] << endl;
#endif
    }
}


void
MultiAgentEnvironment::loadParameters()
{
    Ipseity::String file_name;

    file_name = m_VolatileVariables.m_Workspace;
    file_name += "environment.data";

    FILE* fp = fopen( file_name.c_str(), "rb" );
    if (fp)
    {
        fread( &m_NbrTotalEpisodes, sizeof( uint32 ), 1, fp );
        fclose( fp );
    }
}


void
MultiAgentEnvironment::saveParameters()
{
    Ipseity::String file_name;

    file_name = m_VolatileVariables.m_Workspace;
    file_name += "environment.data";

    FILE* fp = fopen( file_name.c_str(), "wb" );
    if (fp)
    {
        fwrite( &m_NbrTotalEpisodes, sizeof( uint32 ), 1, fp );
        fclose( fp );
    }
}
