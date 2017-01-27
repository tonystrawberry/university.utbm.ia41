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


#ifndef VersatileCognitiveModule_h
#define VersatileCognitiveModule_h


#include "Types.h"
#include "IpseityString.h"

#include "AL/AbstractCognitiveModule.h"
#include "AL/AbstractEnvironment.h"
#include "AL/AbstractVersatileBehaviorModule.h"
#include "AL/Response.h"

#include "IO/PluginWrapper.h"


class VersatileCognitiveModule : public AbstractCognitiveModule
{
    public:
        VersatileCognitiveModule();
        ~VersatileCognitiveModule();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_COGNITIVE_MODULE


    protected:
        Taxon m_Taxon;

        Ipseity::String m_FileName;
        Ipseity::String m_BasePath;
        SensoryInterface m_SensoryInterface;

        PluginWrapper<AbstractVersatileBehaviorModule> m_PcBehaviorModule;
        AbstractVersatileBehaviorModule* m_BehaviorModule;

        Stimulus m_CurrentSensation;

        uint32 m_ChannelId;
};


#endif
