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


#ifndef EnvironmentInstance_h
#define EnvironmentInstance_h


#include "AbstractMultiAgentEnvironmentInstance.h"
#include "SensoryInterface.h"

#include <vector>
#include <QPixmap>


class EnvironmentInstance : public AbstractMultiAgentEnvironmentInstance
{
    public:
		EnvironmentInstance();

        const QPixmap & getPixmap() const;

        void setParentEnvironment( AbstractEnvironment* );

		uint32 getNbrTaxons() const;
        const Taxon & getTaxon( uint32 taxon_id );

        uint32 getNbrSensoryInterfaces() const;
        const SensoryInterface & getSensoryInterface( uint32 sensory_interface_id );

        void resizeWindow( uint32 window_id, int32 width, int32 height );

	protected:
        uint32 addTaxon( const char* name, uint32 status, uint32 sensory_interface_id = 0 );


    protected:
        QPixmap m_Pixmap;

        AbstractEnvironment* m_ParentEnvironment;

        // Set of taxons
		std::vector<Taxon> m_Taxons;
        std::vector<SensoryInterface> m_SensoryInterfaces;

        std::vector<int32> m_Width;
        std::vector<int32> m_Height;
};


#define INTERFACE_ENVIRONMENT_INSTANCE \
    bool sendCommand( const char* command ); \
    bool pressKey( QKeyEvent* ev ); \
    void initialize( const char* config_file_name ); \
    void applyLaws( float64 elapsed_milliseconds ); \
    bool isEpisodeEnded() const; \
    void onEndEpisode(); \
    bool requireScenarioFile() const; \
    uint32 getMaxIterations() const; \
    void load( const char* scenario_filename ); \
    void disturb(); \
    bool isIdle( uint32 agent_id ) const; \
    const Stimulus & getCurrentStimulus( uint32 agent_id ); \
    void addInfluence( uint32 agent_id, const Response & influence ); \
    const PerformanceMeasure & getAgentPerformance( uint32 agent_id ); \
    void saveStats( FILE* ); \
    void saveEpisodeStats( FILE* ); \
    void initRenderer(); \
    void quitRenderer(); \
    uint32 getNbrMainRenderingWindows() const; \
    uint32 getNbrScenarioRenderingWindows() const; \
    const char* getWindowName( uint32 window_id ); \
    void renderWindow( uint32 window_id, QGLWidget* ); \


#endif
