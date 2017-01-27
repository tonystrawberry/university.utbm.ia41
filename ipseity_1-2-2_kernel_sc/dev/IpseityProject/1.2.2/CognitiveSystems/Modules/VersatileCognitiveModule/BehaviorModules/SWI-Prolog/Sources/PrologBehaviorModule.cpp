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


/*#########################################################################

    MAIN: Prolog-based Artificial Intelligence Module

    ---------------------------------------------------------------------

    Author(s) : Fabrice LAURI
    Creation Date : 2008/11/10
    Last Update : 2011/10/22

#########################################################################*/


#include "PrologBehaviorModule.h"

#include <QDebug>


PrologBehaviorModule::PrologBehaviorModule()
{
#ifdef PluginWrapper_h
    m_PrologInterface.setParent( this );
#endif
}


PrologBehaviorModule::~PrologBehaviorModule()
{
}


const char*
PrologBehaviorModule::getBaseName() const
{
    return "SWI-Prolog";
}


const char*
PrologBehaviorModule::getDescription() const
{
    return "";
}


const char*
PrologBehaviorModule::getAuthors() const
{
    return "Lauri, F. (fabrice.lauri@utbm.fr)";
}


void
PrologBehaviorModule::defineParameters()
{
}


bool
PrologBehaviorModule::attachView( uint32 view_id, QWidget* )
{
    return false;
}


void
PrologBehaviorModule::init( const char* program_name, uint32 data_channel_id, AbstractResponseModule* )
{
    m_PrologInterface.init( program_name );
    m_DataChannelId = data_channel_id;

    if (m_PrologInterface.start( "startThinking", 1 ))
    {
        if (m_PrologInterface.call())
        {
            qDebug() << "Thinking initialized!";
        }
        else
        {
            qDebug() << "Thinking NOT initialized!";
        }

        m_PrologInterface.finish();
    }
}


void
PrologBehaviorModule::loadParameters( const char*, const char* )
{
}


const Response &
PrologBehaviorModule::selectResponse( const Stimulus & stimulus )
{
    const uint32 cNbrParams = 2;

    if (m_PrologInterface.start( "think", cNbrParams ))
    {
        term_t stimulus_list = (m_PrologInterface.funcNewTermRef)();
        term_t hResponse;
        int32 stimulus_size;

        hResponse = m_PrologInterface.FirstTerm+1;

        m_PrologInterface.cleanList( m_PrologInterface.FirstTerm );

        stimulus_size = stimulus.getSensoryData( m_DataChannelId ).getDimension();

        m_StimulusRawData = new float64 [stimulus_size];
        for( int32 i = 0; i < stimulus_size; ++i )
        {
            m_StimulusRawData[i] = stimulus.getSensoryData( m_DataChannelId )[i];
        }

        m_PrologInterface.putList( stimulus_list, stimulus_size, m_StimulusRawData );

        m_PrologInterface.consList( m_PrologInterface.FirstTerm, stimulus_list );
        SafeDeleteArray( m_StimulusRawData );

        if (m_PrologInterface.call())
        {
            std::vector<float64> response;

            m_PrologInterface.getList( hResponse, response );

            m_Response.setDimension( response.size() );
            for( uint32 i = 0; i < response.size(); ++i )
            {
                m_Response[i] = response[i];
            }
        }

        m_PrologInterface.finish();
    }

    return m_Response;
}
