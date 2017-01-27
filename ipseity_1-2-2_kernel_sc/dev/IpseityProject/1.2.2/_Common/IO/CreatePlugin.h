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


#ifndef CreatePlugin_h
#define CreatePlugin_h


#include "MemDefns.h"
#include "IO/AbstractPlugin.h"

#include <vector>


#ifdef Q_WS_WIN
  #define DLL_EXPORT __declspec( dllexport )
#else
  #define DLL_EXPORT
#endif


#define CreateDllMain( object_class ) \
static std::vector<object_class*> g_ObjectList; \
static object_class* g_Object; \
static uint32 g_Id;


#define CreateDllVoidEntryFunction( object_class, abstract_object_class ) \
extern "C" DLL_EXPORT abstract_object_class* \
GetPlugin() \
{ \
    g_Object = new object_class(); \
    g_Id = g_ObjectList.size(); \
    g_ObjectList.push_back( g_Object ); \
    return g_Object; \
} \


#define CreateDllDestroyFunction() \
extern "C" DLL_EXPORT void \
DestroyPlugin( uint32 id ) \
{ \
    SafeDelete( g_ObjectList[id] ); \
} \


#define CreateDllVersionFunction( version_number ) \
extern "C" DLL_EXPORT uint32 \
GetPluginVersion() \
{ \
    return version_number; \
} \


#define CreateDllGroupNameFunction( group_name ) \
extern "C" DLL_EXPORT const char* \
GetPluginGroupName() \
{ \
    return group_name; \
} \


#define CreateDllGetInstanceId() \
extern "C" DLL_EXPORT uint32 \
GetInstanceId() \
{ \
    return g_Id; \
} \


#define CreateDllPlugin( group_name, version_number, object_class, abstract_object_class ) \
    CreateDllMain( object_class ) \
    CreateDllVoidEntryFunction( object_class, abstract_object_class ) \
    CreateDllDestroyFunction() \
    CreateDllVersionFunction( version_number ) \
    CreateDllGroupNameFunction( group_name ) \
    CreateDllGetInstanceId()


#endif
