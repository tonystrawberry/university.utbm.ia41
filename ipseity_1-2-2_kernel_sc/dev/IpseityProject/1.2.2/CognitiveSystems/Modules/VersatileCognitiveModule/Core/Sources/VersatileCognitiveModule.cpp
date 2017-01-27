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


#include "VersatileCognitiveModule.h"

#include "IO/XMLScanner.h"
#include "XMLTags_CognitiveSystem.h"

#include <QDebug>


const char*
VersatileCognitiveModule::getBaseName() const
{
    return "Versatile";
}


const char*
VersatileCognitiveModule::getDescription() const
{
    return "Hard-coded cognitive module.";
}


const char*
VersatileCognitiveModule::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
VersatileCognitiveModule::defineParameters()
{
}


bool
VersatileCognitiveModule::attachView( uint32 view_id, QWidget* widget )
{
    return false;
}


//*****************************************************************************


VersatileCognitiveModule::VersatileCognitiveModule() :
    m_ChannelId( 0 ),
    m_BehaviorModule( NULL )
{
    m_SensoryInterface.add( "Percepts" );
}


VersatileCognitiveModule::~VersatileCognitiveModule()
{
    m_PcBehaviorModule.free();
}


const char*
VersatileCognitiveModule::getFileName() const
{
    return m_FileName.c_str();
}


const char*
VersatileCognitiveModule::getBasePath() const
{
    return m_BasePath.c_str();
}


const SensoryInterface &
VersatileCognitiveModule::getSensoryInterface() const
{
    return m_SensoryInterface;
}


bool
VersatileCognitiveModule::init( const char* base_path, const char* file_name, const Taxon & taxon, AbstractInputManager* )
{
    m_Taxon = taxon;

    m_FileName = file_name;

    qDebug() << "Loading" << file_name << "from" << base_path;

    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        qCritical() << "CognitiveSystem NOT initialized!";
        qCritical() << "File '" << file_name << "' from" << base_path << "not loaded!";

        return false;
    }

    m_BasePath = scanner.path;

    qDebug() << file_name << "loaded from" << m_BasePath.c_str();

    XMLScanner::Token t;
    Ipseity::String command;
    Ipseity::String source;
    Ipseity::String output;
    Ipseity::String bm_name;
    Ipseity::String line_command;
    Ipseity::String output_path;
    Ipseity::String environment_name;

    while( scanner.nextToken( &t ) )
    {
        switch( t.kind )
        {
            case XMLScanner::Token::Tag:
                t.queryTagId( g_Tags );
                break;

            case XMLScanner::Token::Attribute:
                switch( t.tagId )
                {
                    case TAG_BEHAVIOR_MODULE:
                        if (t.keyIs( "Name" ))
                        {
                            bm_name = t.stringValue();
                        }
                        else if (t.keyIs( "Command" ))
                        {
                            command = t.stringValue();
                        }
                        else if (t.keyIs( "Source" ))
                        {
                            source = t.stringValue();
                        }
                        else if (t.keyIs( "Output" ))
                        {
                            output = t.stringValue();
                        }
                        else if (t.keyIs( "Environment" ))
                        {
                            environment_name = t.stringValue();
                        }
                        break;
                }
                break;

            case XMLScanner::Token::Close:
                break;
        }
    }

    if (strlen( command.c_str() ) &&
            strlen( source.c_str() ) &&
            strlen( output.c_str() ))
    {
        output_path = m_VolatileVariables.m_SearchRootPath.c_str();
        output_path += environment_name;
        output_path += "/";

        qDebug() << "Create dir:";
        qDebug() << output_path.c_str() << "...";
        QDir().mkpath( output_path.c_str() );

        line_command = m_VolatileVariables.m_SearchRootPath.c_str();
        line_command += command;
        line_command += " \"";
        line_command += m_BasePath;
        line_command += source;
        line_command += "\" \"";
        line_command += output_path.c_str();
        line_command += output.getSubString( 0, output.findLastChar( '.' )-1 );
        line_command += PLUGIN_EXTENSION;
        line_command += "\"";
        //command += " \"";
        //command += m_BasePath;
        //command += output.getSubString( 0, output.findLastChar( '.' )-1 );
        //command += ".txt";
        //command += "\"";

        qDebug() << "Executing command:";
        qDebug() << line_command.c_str() << "...";

        int res = system( line_command.c_str() );

        qDebug() << "Return:" << res;

        if (res == 0)
        {
            bm_name = output;
        }
        else
        {
            return false;
        }
    }

    if (strlen( bm_name.c_str() ) > 0)
    {
        qDebug() << "Searching Behavior Module '" << bm_name.c_str() << "'...";

        m_PcBehaviorModule.setParent( this );
        if (m_PcBehaviorModule.load( "VersatileBehaviorModule", bm_name.c_str() ))
        {
            m_BehaviorModule = reinterpret_cast<AbstractVersatileBehaviorModule*>(m_PcBehaviorModule.get());
            qDebug() << "Module '" << bm_name.c_str() << "' loaded!";
            qDebug() << "ChannelId: " << m_ChannelId;
            m_BehaviorModule->init( bm_name.c_str(), m_ChannelId, NULL );
            m_BehaviorModule->loadParameters( file_name, base_path );
        }
    }

    return true;
}


void
VersatileCognitiveModule::setMode( bool learning_mode )
{
}


bool
VersatileCognitiveModule::canReset() const
{
    return false;
}


void
VersatileCognitiveModule::reset()
{
}


void
VersatileCognitiveModule::startNewSimulation()
{
}

void
VersatileCognitiveModule::endSimulation()
{
}


void
VersatileCognitiveModule::startNewEpisode( uint64 scenario_uid )
{
}

void
VersatileCognitiveModule::endEpisode( bool is_success, uint32 nbr_decisions )
{
}


void
VersatileCognitiveModule::observe( const Sensation & x )
{
    m_CurrentSensation = x;
}


const Response &
VersatileCognitiveModule::selectResponse()
{
    if (m_BehaviorModule)
    {
        return m_BehaviorModule->selectResponse( m_CurrentSensation );
    }
    else
    {
        return Response::VoidResponse;
    }
}


void
VersatileCognitiveModule::suggestResponse( const Response & )
{
}


void
VersatileCognitiveModule::loadMemory()
{
}


void
VersatileCognitiveModule::saveMemory()
{
}


void
VersatileCognitiveModule::loadStats()
{
}


void
VersatileCognitiveModule::saveStats()
{
}


//*****************************************************************************
