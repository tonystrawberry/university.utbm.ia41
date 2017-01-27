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


#ifndef AbstractBaseEnvironment_h
#define AbstractBaseEnvironment_h


#include "AL/AbstractEnvironment.h"
#include "AL/AbstractSystemScheduling.h"

#include "IpseityString.h"

#include <vector>
#include <fstream>


//! States of the Machine State upon which is based an environment
enum
{
    SS_START	= 0,
    SS_END,

    SS_NEW_SCENARIO,

    SS_FACE_CURRENT_SCENARIO,

    SS_PREPARE_NEW_EPISODE,

    SS_NEW_INTERACTION_STEP,
    SS_ADD_INFLUENCES,
    SS_AGENTS_INTERACT,
    SS_AGENT_PERCEIVE,
    SS_AGENT_ADD_INFLUENCE,
    SS_AGENT_REST,

    SS_INTEGRATE_INFLUENCES,

    SS_NEXT_EPISODE,
    SS_AGENT_END_EPISODE,
    SS_END_EPISODE,

    SS_SIMULATION_HALTED
};


//! Abstract base environment.
/*!
    \brief Manage the scenarios of an environment and the start and end of a simulation.
    \author Fabrice LAURI
*/

class AbstractBaseEnvironment : public AbstractEnvironment
{
    public: // Dll interface
        AbstractBaseEnvironment();
        ~AbstractBaseEnvironment();

        //! Get the path where is located the environment.
        /*!
         * \return String.
        */
        const char* getBasePath() const;

        //! Set the name of the file containing the parameters of the environment.
        /*!
         * \param file_name File name containing the parameters.
        */
        void setParameterFileName( const char* file_name );

        //! Get the name of the file containing the parameters of the environment.
        /*!
         * \return String.
        */
        const char* getParameterFileName() const;

        //! Get the maximum number of iterations per episode.
        /*!
         * \return Integer.
        */
        uint32 getMaxIterations() const;

        //! Set the maximum number of iterations for all episodes.
        /*!
         * \param max_ticks Maximum number of iterations.
        */
        void setMaxIterations( uint32 );

        //! Get the maximum simulation time (in milliseconds) for the current episode.
        /*!
         * \return 64-bit float.
        */
        float64 getMaxSimulationTime() const;

        //! Set the maximum simulation time (in milliseconds) for the current episode.
        /*!
         * \param max_time Maximum time.
        */
        void setMaxSimulationTime( float64 );

        //! Get the total number of iterations performed.
        /*!
         * \return Integer.
        */
        uint32 getNbrIterations() const;


        //-------------------------------------------------------------------------------
        //  Simulation management
        //-------------------------------------------------------------------------------

        //! Set the information of the current experiment.
        /*!
         * \param trial_id ID of the trial.
         * \param episode_id ID of the episode under progress.
         * \param scenario_uid UID of the current scenario.
         * \param scenario_filename Filename of the current scenario to load.
        */
        void setExperiment( uint32 trial_id, uint32 episode_id, uint64 scenario_uid, const char* scenario_filename, bool learning_mode );

        //! Start a simulation.
        /*!
        */
        void startSimulation();

        //! Stop the simulation.
        /*!
         * \param halted Halted by the user (True) or not (False).
        */
        void stopSimulation( bool halted );

        //! Enable/Disable step-by-step simulation.
        /*!
         * \param flag If True, every iteration of the simulation stops
        */
        void enableStepByStep( bool flag );

        //! Simulate one time step of the environment.
        /*!
         * \param elapsed_milliseconds Elapsed milliseconds.
         * \return Environmental state.
        */
        uint32 simulate( float64 elapsed_millisecond );

        //! Does the simulation continue?
        /*!
         * \return Boolean: False if the simulation is finished, True otherwise.
        */
        bool continueSimulation();

        //! Select the identifier of the current agent perceiving or acting in the simulation.
        /*!
         * \return Integer.
        */
        uint32 getSimulationAgentId() const;

        //! Is the simulation terminated?
        /*!
         * \return Boolean.
        */
        bool isSimulationTerminated() const;

        //! Is the episode ended?
        /*!
         * \return Boolean.
        */
        bool isEpisodeEnded() const;


        //-------------------------------------------------------------------------------
        //  System Scheduling management
        //-------------------------------------------------------------------------------

        //! Set the system scheduling.
        /*!
         * \param system_scheduling Pointer to the system scheduling.
        */
        void setSystemScheduling( AbstractSystemScheduling* system_scheduling );


    protected: // Abstract functions to implement in the plugged environment.
        //! Feed the environment with the data of the current scenario.
        /*!
        */
        virtual void load() = 0;

        //! Is the current agent conscious?
        /*!
         * A conscious agent is not idle.
         * \return Boolean.
        */
        virtual bool isCurrentAgentConscious() = 0;

        //! Integrate the agent influences to update the environment.
        /*!
         * \param elapsed_milliseconds Elapsed time in milliseconds.
        */
        virtual void integrateInfluences( float64 elapsed_milliseconds ) = 0;

        //! Called when an episode has ended.
        /*!
        */
        virtual void onEndEpisode() = 0;

    protected:
        //! Get the status of the simulation.
        /*!
         * \return Integer.
        */
        uint32 getSimulationStatus() const;

        //! Face the current scenario.
        /*! It consists in:
         *  - feeding the environment with the data of the current scenario,
         *  - informing the agents a new episode has just started,
         *  - incrementing the number of episodes.
         *  \see load
         *  \see prepareAgents
        */
        void faceCurrentScenario();

        //! Select a new agent that perceives or acts.
        /*!
         * \return Boolean: True if it exists, False otherwise.
        */
        bool selectNewAgent();


    protected:
        Ipseity::String m_BasePath;
        Ipseity::String m_ParameterFileName;
        uint32 m_SimulationStatus;

        // Experiment data
        uint32 m_TrialId;
        uint32 m_EpisodeId;
        Ipseity::String m_ScenarioFileName;
        uint64 m_ScenarioUid;
        bool m_LearningMode;

        bool m_CurrentScenarioLoaded;

        // Ticks
		uint32 m_MaxTicks;
		uint32 m_NbrTicks;

        // Time
        float64 m_MaxTime;
        float64 m_CurrentTime;	// in milliseconds

        bool m_EpisodeEnded;
        bool m_StepByStep;
        bool m_SimulationHalted;

        AbstractSystemScheduling* m_SystemSchedulingModule;
        uint32 m_AgentId;
        uint32 m_InteractionSignal;
        std::list<uint32>::const_iterator m_AgentItr;
        std::list<uint32> m_AgentSet[IPSEITY_ENUM_AGENT_NBR_SIGNALS];

		FILE* m_EpisodeInteractionsFile;
};


#endif
