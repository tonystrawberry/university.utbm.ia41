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


#ifndef AbstractSupervisionModule_h
#define AbstractSupervisionModule_h


#include "Types.h"
#include "IO/AbstractPlugin.h"


class AbstractSupervisionModule : public AbstractPlugin
{
    public:
        virtual bool canSimulate() = 0;
        virtual void setSimulationStatus( bool ) = 0;

        virtual void start() = 0;
        virtual bool isEnded() const = 0;

        virtual bool isNewTrial() const = 0;
        virtual uint32 getTrialId() const = 0;

        virtual uint32 getEpisodeId() const = 0;

        virtual uint32 getCurrentScenarioId() const = 0;
        virtual uint64 getCurrentScenarioUid() const = 0;
        virtual const char* getCurrentScenarioFileName() const = 0;

        virtual bool isLearningMode() const = 0;

        virtual bool readScenarii( const char* base_path, const char* file_name ) = 0;
        virtual void clearScenarii() = 0;
        virtual void addScenario( const char* name ) = 0;
        virtual void setIterations( uint32 scenario_id, uint32 nbr_iterations, uint32 max_iterations ) = 0;
        virtual bool selectNextScenario() = 0;

        virtual void restartCurrentScenario() = 0;
        virtual void gotoPreviousScenario() = 0;
        virtual void gotoNextScenario() = 0;
};


#define INTERFACE_ABSTRACT_SUPERVISION_MODULE \
    bool canSimulate(); \
    void setSimulationStatus( bool ); \
    void start(); \
    bool isEnded() const; \
    bool isNewTrial() const; \
    uint32 getTrialId() const; \
    uint32 getEpisodeId() const; \
    uint32 getCurrentScenarioId() const; \
    uint64 getCurrentScenarioUid() const; \
    const char* getCurrentScenarioFileName() const; \
    bool isLearningMode() const; \
    bool readScenarii( const char* base_path, const char* file_name ); \
    void clearScenarii(); \
    void addScenario( const char* name ); \
    void setIterations( uint32 scenario_id, uint32 nbr_iterations, uint32 max_iterations ); \
    bool selectNextScenario(); \
    void restartCurrentScenario(); \
    void gotoPreviousScenario(); \
    void gotoNextScenario(); \


#endif
