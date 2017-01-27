/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2015, 2014, 2013 Ipseity Core Developers
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


#ifndef AbstractPluginMultiAgentEnvironment_h
#define AbstractPluginMultiAgentEnvironment_h


#include "Types.h"
#include "AL/AbstractEnvironment.h"
#include "AL/SensoryInterface.h"
#include "IO/AbstractPlugin.h"


class QGLWidget;
class QKeyEvent;
class QPixmap;


//! Abstract interface for implementing the interactions of a multi-agent environment
/*!
*/

class AbstractMultiAgentEnvironmentInstance : public AbstractPlugin
{
    public:
        //! Get the icon of the environment.
        /*!
        */
        virtual const QPixmap & getPixmap() const = 0;


        //! Store the pointer of the parent environment.
        /*!
        */
        virtual void setParentEnvironment( AbstractEnvironment* ) = 0;

        //-------------------------------------------------------------------------------
        //  Command
        //-------------------------------------------------------------------------------

        virtual bool sendCommand( const char* command ) = 0;

        //-------------------------------------------------------------------------------
        //  Inputs
        //-------------------------------------------------------------------------------

        virtual bool pressKey( QKeyEvent* ev ) = 0;

        //-------------------------------------------------------------------------------
        //  Simulation
        //-------------------------------------------------------------------------------

        //! Initialize the environment.
        /*!
            \param config_filename Name of the config file.
        */
        virtual void initialize( const char* config_filename ) = 0;

        //! Apply the environmental laws.
        /*!
            \param elapsed_milliseconds Time elapsed (in milliseconds) since the last call of the function.
        */
        virtual void applyLaws( float64 elapsed_milliseconds ) = 0;

        //! Is the episode ended?
        /*!
        */
        virtual bool isEpisodeEnded() const = 0;

        //! Called when the episode is ended.
        /*!
        */
        virtual void onEndEpisode() = 0;

        //-------------------------------------------------------------------------------
        //  Scenarii
        //-------------------------------------------------------------------------------

        //! Does the environment require a file for loading scenarii?
        /*!
        */
        virtual bool requireScenarioFile() const = 0;


        //! Specify the maximum number of iterations for the current scenario. 0 is infinity.
        /*!
        */
        virtual uint32 getMaxIterations() const = 0;

        //! Load the given scenario file.
        /*!
        */
        virtual void load( const char* scenario_filename ) = 0;

        //! Load the current scenario.
        /*!
        */
        virtual void disturb() = 0;


        //-------------------------------------------------------------------------------
        //  Taxons
        //-------------------------------------------------------------------------------

        //! Get the number of taxons.
        /*!
        */
        virtual uint32 getNbrTaxons() const = 0;

        //! Get a taxon.
        /*!
            \param taxon_id Identifier of a taxon.
            \return Taxon whose identifier is given.
        */
        virtual const Taxon & getTaxon( uint32 taxon_id ) = 0;


        //-------------------------------------------------------------------------------
        //  Sensory Interfaces
        //-------------------------------------------------------------------------------

        //! Get the number of sensory interfaces.
        /*!
        */
        virtual uint32 getNbrSensoryInterfaces() const = 0;

        //! Get a sensory interface.
        /*!
            \param sensory_interface_id Identifier of a sensory interface.
            \return Sensory interface whose identifier is given.
        */
        virtual const SensoryInterface & getSensoryInterface( uint32 sensory_interface_id ) = 0;


        //-------------------------------------------------------------------------------
        // Agent management
        //-------------------------------------------------------------------------------

        //! Is the agent idle?
        /*!
        */
        virtual bool isIdle( uint32 agent_id ) const = 0;

        //! Get the current sensation of an agent.
        /*!
        */
        virtual const Stimulus & getCurrentStimulus( uint32 agent_id ) = 0;

        //! Add the influence to the environment.
        /*!
            \param agent_id Agent id.
            \param influence Influence.
        */
        virtual void addInfluence( uint32 agent_id, const Response & influence ) = 0;

        //! Get the current agent performance.
        virtual const PerformanceMeasure & getAgentPerformance( uint32 agent_id ) = 0;


        //-------------------------------------------------------------------------------
        // Statistics
        //-------------------------------------------------------------------------------

        virtual void saveEpisodeStats( FILE* ) = 0;
        virtual void saveStats( FILE* ) = 0;


        //-------------------------------------------------------------------------------
        // Rendering
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


#define INTERFACE_ABSTRACT_MULTIAGENT_ENVIRONMENT_INSTANCE \
    const char* getInformation( const char* key ) const; \
    void setParentEnvironment( AbstractEnvironment* ); \
    bool sendCommand( const char* command ); \
    bool pressKey( QKeyEvent* ev ); \
    void initialize(); \
    void applyLaws( float64 elapsed_milliseconds ); \
    bool isEpisodeEnded() const; \
    void onEndEpisode(); \
    bool requireScenarioFile() const; \
    uint32 getMaxIterations() const; \
    void load( const char* scenario_filename ); \
    void disturb(); \
    uint32 getNbrTaxons() const; \
    const Taxon & getTaxon( uint32 taxon_id ); \
    bool fetchStimulusUmwelt( uint32 taxon_id, StimulusUmwelt & umwelt ) const; \
    bool fetchResponseUmwelt( uint32 taxon_id, ResponseUmwelt & umwelt ) const; \
    bool isIdle( uint32 agent_id ) const; \
    const Stimulus & getCurrentStimulus( uint32 agent_id ); \
    void addInfluence( uint32 agent_id, const Response & influence ); \
    void initRenderer(); \
    void quitRenderer(); \
    uint32 getNbrMainRenderingWindows() const; \
    uint32 getNbrScenarioRenderingWindows() const; \
    const char* getWindowName( uint32 window_id ); \
    void resizeWindow( uint32 window_id, int32, int32 ); \
    void renderWindow( uint32 window_id, QGLWidget* widget ); \


#endif
