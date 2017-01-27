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


#ifndef AbstractCognitiveModule_h
#define AbstractCognitiveModule_h


#include "IpseityString.h"
#include "AL/Response.h"
#include "AL/Sensation.h"
#include "AL/Stimulus.h"
#include "AL/Taxon.h"
#include "AL/SensoryInterface.h"
#include "IO/AbstractPlugin.h"
#include "IO/AbstractInputManager.h"

#include <ostream>
#include <vector>


//! Abstraction of a cognitive module.
/*!
 *  \brief Provide to an agent the interface to interact with its environment.
 *  \author Fabrice LAURI
*/

class AbstractCognitiveModule : public AbstractPlugin
{
    public:
        //! Get the file name of the cognitive module.
        virtual const char* getFileName() const = 0;

        //! Get the path of the cognitive module.
        virtual const char* getBasePath() const = 0;

        //! Get the sensory interface of the cognitive module.
        virtual const SensoryInterface & getSensoryInterface() const = 0;

        //! Initialize the cognitive system.
        /*!
            \param base_path Base path.
            \param name Name of the brain to use.
            \param taxon Associated taxon.
            \param input_manager Input manager for handling the key events.
        */
        virtual bool init( const char* base_path, const char* name, const Taxon & taxon, AbstractInputManager* input_manager ) = 0;

        //! Set the mode of the cognitive module.
        /*!
         *  \param learning_mode If true, the cognitive module is learning, otherwise it is in the evaluation mode.
        */
        virtual void setMode( bool learning_mode ) = 0;

        //! Can the cognitive module be reset.
        /*!
         *  \brief In other words, is a memory used within it?
         *  \return True if a memory is used, false otherwise.
        */
        virtual bool canReset() const = 0;

        //! Reset the cognitive system.
        /*!
        */
        virtual void reset() = 0;

        //! A new simulation has just started.
        /*!
        */
        virtual void startNewSimulation() = 0;

        //! Indicate the end of a simulation.
        /*!
        */
        virtual void endSimulation() = 0;

        //! Inform the cognitive system a new episode has started.
        /*!
         *  \param scenario_id Id of the scenario for which a new episode has just started.
        */
        virtual void startNewEpisode( uint64 scenario_iud ) = 0;

        //! End the episode.
        /*!
         *  \param is_success Is the episode ended with success.
         *  \param nbr_decisions Total number of decisions taken by the cognitive module.
        */
        virtual void endEpisode( bool is_success, uint32 nbr_decisions ) = 0;

        //! Observe the current sensation.
        /*!
            \param x Current sensation.
        */
        virtual void observe( const Sensation & x ) = 0;

        //! Select the response to carry out.
        /*!
            \return Chosen response.
        */
        virtual const Response & selectResponse() = 0;

        //! Suggest a response to the cognitive system.
        /*!
            \param response Chosen response.
        */
        virtual void suggestResponse( const Response & response ) = 0;

        //! Load the memory linked to the cognitive system.
        /*!
        */
        virtual void loadMemory() = 0;

        //! Save the memory linked to the cognitive system.
        /*!
        */
        virtual void saveMemory() = 0;

        //! Load the statistics associated with the cognitive system.
        /*!
        */
        virtual void loadStats() = 0;

        //! Save the statistics associated with the cognitive system.
        /*!
        */
        virtual void saveStats() = 0;
};


#define INTERFACE_ABSTRACT_COGNITIVE_MODULE \
    const char* getFileName() const; \
    const char* getBasePath() const; \
    const SensoryInterface & getSensoryInterface() const; \
    bool init( const char* base_path, const char* name, const Taxon & taxon, AbstractInputManager* ); \
    void setMode( bool learning_mode ); \
    bool canReset() const; \
    void reset(); \
    void startNewSimulation(); \
    void endSimulation(); \
    void startNewEpisode( uint64 scenario_uid ); \
    void endEpisode( bool is_success, uint32 nbr_decisions ); \
    void observe( const Sensation & ); \
    const Response & selectResponse(); \
    void suggestResponse( const Response & ); \
    void loadMemory(); \
    void saveMemory(); \
    void loadStats(); \
    void saveStats(); \


#endif
