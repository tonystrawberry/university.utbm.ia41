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


#ifndef AbstractEnvironment_h
#define AbstractEnvironment_h


#include "AL/IpseityTypes.h"
#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "AL/Taxon.h"
#include "AL/SensoryInterface.h"
#include "AL/AbstractAgent.h"
#include "AL/AbstractAgentManager.h"

#include "IO/AbstractPlugin.h"


class AbstractSystemScheduling;

class QGLWidget;
class QIcon;


//! Abstract environment.
/*!
    \brief Abstract methods of a multiagent environment.
    \author Fabrice LAURI
*/

class AbstractEnvironment : public AbstractPlugin
{
    public:
        //! Get the path where is located the environment.
        /*!
         * \return String.
        */
        virtual const QIcon & getIcon() const = 0;

        //! Get the path where is located the environment.
        /*!
         * \return String.
        */
        virtual const char* getBasePath() const = 0;

        //! Set the name of the file containing the parameters of the environment.
        /*!
         * \param file_name File name containing the parameters.
        */
        virtual void setParameterFileName( const char* file_name ) = 0;

        //! Get the name of the file containing the parameters of the environment.
        /*!
         * \return String.
        */
        virtual const char* getParameterFileName() const = 0;

        //! Initialize the environment.
        /*!
         * This method is called only once after the environment has been instanciated.
         * \param base_path Path where is located the environment.
         * \param environment_instance_name Name of the environment.
         * \param config_filename Name of the configuration file used to init the environment.
        */
        virtual void initialize( const char* base_path, const char* environment_instance_name, const char* config_filename ) = 0;

        //! Get the maximum number of iterations per episode.
        /*!
         * \return Integer.
        */
        virtual uint32 getMaxIterations() const = 0;

        //! Set the maximum number of iterations for all episodes.
        /*!
         * \param max_ticks Maximum number of iterations.
        */
        virtual void setMaxIterations( uint32 max_ticks ) = 0;

        //! Get the maximum simulation time (in milliseconds) for the current episode.
        /*!
         * \return 64-bit float.
        */
        virtual float64 getMaxSimulationTime() const = 0;

        //! Set the maximum simulation time (in milliseconds) for the current episode.
        /*!
         * \param max_time Maximum time.
        */
        virtual void setMaxSimulationTime( float64 max_time ) = 0;

        //! Get the total number of iterations performed.
        /*!
         * \return Integer.
        */
        virtual uint32 getNbrIterations() const = 0;

        //! Get the number of episodes.
        /*!
         * An episode lasts a certain period of time.
         * During this time, the agents take some decisions.
         * \return Integer.
        */
        virtual uint32 getNbrEpisodes() const = 0;


        //-------------------------------------------------------------------------------
        //  Scenarii management
        //-------------------------------------------------------------------------------

        //! Does the environment require a file for loading scenarii?
        /*!
         * \return Boolean.
        */
        virtual bool requireScenarioFile() const = 0;


        //-------------------------------------------------------------------------------
        //  Command
        //-------------------------------------------------------------------------------

        //! Send a command to the environment.
        /*!
         * \param command Command.
         * \return Boolean: True if the command succeeded, False otherwise.
        */
        virtual bool sendCommand( const char* command ) = 0;


        //-------------------------------------------------------------------------------
        //  Simulation management
        //-------------------------------------------------------------------------------

        //! Set the information of the current experiment.
        /*!
         * \param trial_id ID of the trial.
         * \param episode_id ID of the episode under progress.
         * \param scenario_uid UID of the current scenario.
         * \param scenario_filename Filename of the current scenario to load.
         * \param learning_mode Learning mode. False means testing.
        */
        virtual void setExperiment( uint32 trial_id, uint32 episode_id, uint64 scenario_uid, const char* scenario_filename, bool learning_mode ) = 0;

        //! Start a simulation.
        /*!
        */
        virtual void startSimulation() = 0;

        //! Stop the simulation.
        /*!
         * \param halted Halted by the user (True) or not (False).
        */
        virtual void stopSimulation( bool halted ) = 0;

        //! Enable/Disable step-by-step simulation.
        /*!
         * \param flag If True, every iteration of the simulation stops
        */
        virtual void enableStepByStep( bool flag ) = 0;

        //! Simulate one time step of the environment.
        /*!
         * \param elapsed_milliseconds Elapsed milliseconds.
         * \return Environmental state.
        */
        virtual uint32 simulate( float64 elapsed_milliseconds ) = 0;

        //! Does the simulation continue?
        /*!
         * \return Boolean: False if the simulation is finished or paused, True otherwise.
        */
        virtual bool continueSimulation() = 0;

        //! Select the identifier of the current agent perceiving or acting in the simulation.
        /*!
         * \return Integer.
        */
        virtual uint32 getSimulationAgentId() const = 0;

        //! Is the simulation terminated?
        /*!
         * \return Boolean.
        */
        virtual bool isSimulationTerminated() const = 0;

        //! Is the episode ended?
        /*!
         * \return Boolean.
        */
        virtual bool isEpisodeEnded() const = 0;


        //-------------------------------------------------------------------------------
        //  Taxons
        //-------------------------------------------------------------------------------

        //! Get the number of taxons defined in the environment.
        /*!
         * \return Integer.
        */
        virtual uint32 getNbrTaxons() const = 0;

        //! Get a taxon.
        /*!
            \param taxon_id Identifier of the taxon.
            \return Description of the taxon.
        */
        virtual bool fetchTaxon( uint32 taxon_id, Taxon & taxon ) const = 0;


        //-------------------------------------------------------------------------------
        //  Sensory Interfaces
        //-------------------------------------------------------------------------------

        //! Get the number of sensory interfaces.
        /*!
        */
        virtual uint32 getNbrSensoryInterfaces() const = 0;

        //! Fetch a sensory interface.
        /*!
            \param sensory_interface_id Identifier of a sensory interface.
            \param sensory_interface Sensory interface returned if the identifier exists.
            \return Boolean: True if the sensory interface exists, False otherwise.
        */
        virtual bool fetchSensoryInterface( uint32 sensory_interface_id, SensoryInterface & sensory_interface ) const = 0;

        //! Check the compatibility between the sensory interface of a given taxon and a given sensory interface.
        /*!
            \param taxon_id Identifier of the taxon.
            \param sensory_interface Sensory interface.
            \return Boolean: True if compatibility is ok, False otherwise.
        */
        virtual bool checkSensoryInterfaceCompatibility( uint32 taxon_id, const SensoryInterface & sensory_interface ) const = 0;


        //-------------------------------------------------------------------------------
        //  Cognitive system management
        //-------------------------------------------------------------------------------

        //! Check whether a given file represents a valid cognitive system.
        /*!
         * \param file_name Name of the file containing the parameters of the cognitive system.
         * \param taxon_id Identifier of the taxon of the recognized cognitive system.
         * \return Boolean: True if the file is a valid cognitive system, False otherwise.
        */
        virtual bool fetchCognitiveSystemTaxonId( const char* file_name, uint32 & taxon_id ) = 0;


        //-------------------------------------------------------------------------------
        //  Agent management
        //-------------------------------------------------------------------------------

        //! Set the agent manager.
        /*!
            \param sc_pointer Pointer of the agent manager.
        */
        virtual void setAgentManager( AbstractAgentManager* sc_pointer ) = 0;

        //! Add an agent into the environment.
        /*!
         * \param agent_id Agent id.
         * \param taxon_id Id of the taxon of the agent.
         * \return Boolean: True if the agent has been added into the environment.
        */
        virtual bool addAgent( uint32 agent_id, uint32 taxon_id ) = 0;

        //! Add an agent into the environment.
        /*!
         * \param taxon_id Id of the taxon of the agent.
         * \return Boolean: True if the agent has been added into the environment.
        */
        virtual bool addAgent( uint32 taxon_id ) = 0;

        //! Remove an agent.
        /*!
         * \param agent_id Agent identifier.
         * \return Boolean: True if the agent has been removed from the environment.
        */
        virtual bool removeAgent( uint32 agent_id ) = 0;

        //! Get the number of agents inhabiting the environment in the current scenario.
        /*!
         * \return Integer.
        */
        virtual uint32 getNbrAgents() const = 0;

        //! Get the agent whose \a id is given.
        /*!
         * \param agent_id Agent identifier.
         * \return Pointer to the agent.
        */
        virtual AbstractAgent* getAgent( uint32 agent_id ) = 0;

        //! Clear the agents.
        /*!
        */
        virtual void clearAgents() = 0;

        //! Is the agent idle?
        /*!
         * \param agent_id Agent identifier.
         * \return Boolean.
        */
        virtual bool isIdle( uint32 agent_id ) const = 0;

        //! Get the current sensation of an agent.
        /*!
         * \param agent_id Agent identifier.
         * \return Stimulus.
        */
        virtual const Stimulus & getCurrentStimulus( uint32 agent_id ) = 0;

        //! Add an agent's influence.
        /*!
         * \param agent_id Agent identifier.
         * \param influence Response of the agent.
        */
        virtual void addInfluence( uint32 agent_id, const Response & influence ) = 0;

        //! Is the episode successfully ended by a given agent.
        /*!
         * \param agent_id Agent identifier.
         * \return Boolean.
        */
        virtual bool isScenarioSucceededByAgent( uint32 agent_id ) = 0;

        //! Get the current agent performance.
        /*!
         * \param agent_id Agent identifier.
         * \return Vector of scalars.
        */
        virtual const PerformanceMeasure & getAgentPerformance( uint32 agent_id ) = 0;


        //-------------------------------------------------------------------------------
        //  System Scheduling management
        //-------------------------------------------------------------------------------

        //! Set the system scheduling.
        /*!
         * \param system_scheduling Pointer to the system scheduling.
        */
        virtual void setSystemScheduling( AbstractSystemScheduling* system_scheduling ) = 0;


        //-------------------------------------------------------------------------------
        //  Inputs
        //-------------------------------------------------------------------------------

        //! May update the environment when a key is pressed.
        /*!
         * \param ev Key event.
         * \return Boolean: True if the key event has been taken into account.
        */
        virtual bool pressKey( QKeyEvent* ev ) = 0;


        //-------------------------------------------------------------------------------
        //  Rendering
        //-------------------------------------------------------------------------------

        //! Initialize the renderer of the environment.
        /*!
        */
        virtual void initRenderer() = 0;

        //! Quit the renderer.
        /*!
        */
        virtual void quitRenderer() = 0;

        //! Number of the main rendering windows of the environment.
        /*!
          \return Integer.
        */
        virtual uint32 getNbrMainRenderingWindows() const = 0;

        //! Number of rendering windows associated with a scenario.
        /*!
          \return Integer.
        */
        virtual uint32 getNbrScenarioRenderingWindows() const = 0;

        //! Get the name of a rendering window.
        /*!
         * \param window_id Identifier of the window.
         * \return String.
        */
        virtual const char* getWindowName( uint32 window_id ) = 0;

        //! Resize the window.
        /*!
         * This method is called whenever the window size changes.
         * \param window_id Identifier of the window.
         * \param w Window width.
         * \param h Window height.
        */
        virtual void resizeWindow( uint32 window_id, int32 w, int32 h ) = 0;

        //! Render an environment view in a given widget.
        /*!
         * \param widget Rendering widget.
         * \param window_id Identifier of the window.
        */
        virtual void renderWindow( uint32 window_id, QGLWidget* widget ) = 0;
};


extern AbstractEnvironment* g_Environment;


#endif
