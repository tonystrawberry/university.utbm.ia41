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


#ifndef BasePluginCognitiveSystem_h
#define BasePluginCognitiveSystem_h


#include "AL/AbstractCognitiveModule.h"
#include "AL/AbstractEnvironment.h"
#include "AL/ResponseModule.h"


class BasePluginCognitiveModule : public AbstractCognitiveModule
{
    public:
        const char* getFileName() const;
        const char* getBasePath() const;
        const SensoryInterface & getSensoryInterface() const;


    protected:
        Ipseity::String m_FileName;
        Ipseity::String m_BasePath;
        SensoryInterface m_SensoryInterface;

        ResponseSignalProperties* m_ResponseDescription;
        ResponseModule m_ResponseModule;
};


#define INTERFACE_BASE_PLUGIN_COGNITIVE_MODULE \
    bool init( const char* base_path, const char* name, const Taxon & taxon, AbstractInputManager* ); \
    void setMode( bool learning_mode ); \
    bool canBeShared() const; \
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
