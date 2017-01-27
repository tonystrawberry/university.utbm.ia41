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


#ifndef CreateAlPlugin_h
#define CreateAlPlugin_h


#include "IO/CreatePlugin.h"


#define CreatePluginEnvironment( object_class, version_number ) \
        CreateDllPlugin( "Environment", version_number, object_class, AbstractEnvironment )

#define CreatePluginEnvironmentInstance( object_class, version_number ) \
        CreateDllPlugin( "EnvironmentInstance", version_number, object_class, AbstractMultiAgentEnvironmentInstance )

#define CreatePluginSystemScheduling( object_class, version_number ) \
        CreateDllPlugin( "SystemScheduling", version_number, object_class, AbstractSystemScheduling )

#define CreatePluginScenarioSupervision( object_class, version_number ) \
        CreateDllPlugin( "ScenarioSupervision", version_number, object_class, AbstractSupervisionModule )

#define CreatePluginCognitiveSystem( object_class, version_number ) \
        CreateDllPlugin( "CognitiveSystem", version_number, object_class, AbstractCognitiveSystem )

#define CreatePluginCognitiveModule( object_class, version_number ) \
        CreateDllPlugin( "CognitiveModule", version_number, object_class, AbstractCognitiveModule )

#define CreatePluginStimulusIntegrationFunction( object_class, version_number ) \
        CreateDllPlugin( "StimulusIntegrationFunction", version_number, object_class, AbstractStimulusIntegrationFunction )

#define CreatePluginBehaviorModule( object_class, bm_class_name, version_number, abstract_object_class ) \
        CreateDllPlugin( bm_class_name, version_number, object_class, abstract_object_class )

#define CreatePluginQFunction( object_class, version_number ) \
        CreateDllPlugin( "QValueMemory", version_number, object_class, AbstractQFunctionModule )

#define CreatePluginParameterScheduleModule( object_class, version_number ) \
		CreateDllPlugin( "ParameterScheduleModule", version_number, object_class, AbstractParameterScheduleModule )

#define CreatePluginFeatureExtractionModule( object_class, version_number ) \
        CreateDllPlugin( "FeatureExtractionModule", version_number, object_class, AbstractFeatureExtractionModule )

#define CreatePluginLearningModule( object_class, version_number ) \
        CreateDllPlugin( "LearningModule", version_number, object_class, AbstractLearningModule )

#define CreatePluginContinuousPolicy( object_class, version_number ) \
        CreateDllPlugin( "ContinuousPolicy", version_number, object_class, AbstractContinuousPolicy )


#endif
