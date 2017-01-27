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


#ifndef BaseQFunctionModule_h
#define BaseQFunctionModule_h


#include "IO/PluginWrapper.h"
#include "AL/RLCognitiveSystem/CognitiveSystem/AbstractQFunctionModule.h"
#include "AL/AbstractParameterScheduleModule.h"

#include <stdio.h>


class BaseQFunctionModule : public AbstractQFunctionModule
{
    public:
        BaseQFunctionModule();
        ~BaseQFunctionModule();

        void setDataDescription( SensorySignalProperties*, ResponseSignalProperties* );

        bool loadParameters( const char* name, const char* base_path );

        bool load( const char* );
        bool save( const char* );

        Ipseity::Scalar getAlpha( uint32 cell_id, Ipseity::Scalar td ) const;

    protected:
        virtual bool load( FILE* ) = 0;
        virtual bool save( FILE* ) = 0;


    protected:
		PluginWrapper<AbstractParameterScheduleModule> m_PcAsm;
        AbstractParameterScheduleModule* m_AlphaScheduleModule;
        Ipseity::Scalar m_ConstantAlpha;
        SensorySignalProperties* m_StimulusSpace;
        ResponseSignalProperties* m_ResponseSpace;
        AgentDataProperties m_Description;
};


#define INTERFACE_BASE_QVMM \
    bool isDynamic() const; \
    bool canReset() const; \
    void reset(); \
    uint32 getNbrCells() const; \
    Ipseity::Scalar getValue( const SensorySignal &, const Response & ); \
    uint32 getNbrVisits( const SensorySignal & x, const Response & u ); \
    void update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate ); \
    void clearEligibilityTraces(); \
    void decayEligibilityTraces( Ipseity::Scalar ); \
    void replaceEligibilityTraces( const SensorySignal &, const Response & ); \
    void accumulateEligibilityTraces( const SensorySignal &, const Response & ); \
    bool load( FILE* ); \
    bool save( FILE* ); \


#endif
