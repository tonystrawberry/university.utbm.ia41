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


#ifndef SingleAgentEnvironmentInstance_h
#define SingleAgentEnvironmentInstance_h


#include "AL/EnvironmentInstance.h"


class SingleAgentEnvironmentInstance : public EnvironmentInstance
{
    public:
        SingleAgentEnvironmentInstance( uint32 response_size );

        bool isEpisodeEnded() const;
        bool isIdle( uint32 agent_id ) const;

        const Stimulus & getCurrentStimulus( uint32 agent_id );

        void addInfluence( uint32 agent_id, const Response & influence );

        const PerformanceMeasure & getAgentPerformance( uint32 agent_id );


    protected:
        Stimulus m_Stimulus;
        Response m_Response;
        bool m_Idle;
        PerformanceMeasure m_Performance;
};


#define INTERFACE_SINGLE_AGENT_ENVIRONMENT \
        bool sendCommand( const char* command ); \
        bool pressKey( QKeyEvent* ev ); \
        void initialize( const char* config_file_name ); \
        void applyLaws( float64 elapsed_milliseconds ); \
        void onEndEpisode(); \
        bool requireScenarioFile() const; \
        const char* getScenarioName( uint32 scenario_id ); \
        const char* getScenarioDescription( uint32 scenario_id ); \
        uint64 getScenarioUid( uint32 scenario_id ) const; \
        uint32 getMaxIterations() const; \
        void load( const char* scenario_filename ); \
        void disturb(); \
		void saveStats( FILE* ); \
		void saveEpisodeStats( FILE* ); \
        void initRenderer(); \
        void quitRenderer(); \
        uint32 getNbrMainRenderingWindows() const; \
        uint32 getNbrScenarioRenderingWindows() const; \
        const char* getWindowName( uint32 window_id ); \
        void renderWindow( uint32 window_id, QGLWidget* ); \


#endif
