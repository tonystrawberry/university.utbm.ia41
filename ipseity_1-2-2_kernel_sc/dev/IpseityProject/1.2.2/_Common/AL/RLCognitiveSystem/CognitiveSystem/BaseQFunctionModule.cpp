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


#include "BaseQFunctionModule.h"

#include "IO/XMLScanner.h"
#include "AL/RLCognitiveSystem/CognitiveSystem/XMLTags_BaseQValuesMemoryModule.h"


BaseQFunctionModule::BaseQFunctionModule() :
    m_AlphaScheduleModule( NULL  ),
    m_ConstantAlpha( 0.1 )
{
}


BaseQFunctionModule::~BaseQFunctionModule()
{
	m_PcAsm.free();
}


void
BaseQFunctionModule::setDataDescription( SensorySignalProperties* stimulus_description, ResponseSignalProperties* response_description )
{
    m_StimulusSpace = stimulus_description;
    m_ResponseSpace = response_description;
    if (stimulus_description)
    {
        m_Description = stimulus_description->concat( *response_description );
//        cout << "Agent Data Space: " << m_Description << endl;
    }
    else
    {
//        cout << "Unknown Agent Data Space" << endl;
    }

}


bool
BaseQFunctionModule::loadParameters( const char* file_name, const char* base_path )
{
	XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        return false;
    }

//	cout << "Load Qfunction module from '" << file_name << "' from " << base_path << endl;

	XMLScanner::Token t;

    while( scanner.nextToken( &t ) )
    {
        switch( t.kind )
        {
            case XMLScanner::Token::Tag:
                t.queryTagId( g_Tags_BQVM );
                break;

            case XMLScanner::Token::Attribute:
                switch( t.tagId )
                {
					case TAG_PARAMETER_SCHEDULE_MODULE:
//                        if (t.keyIs( "Name" ))
//                        {
//							m_PcAsm.setParent( this );
//                            if (m_PcAsm.load( g_Tags_BQVM[TAG_PARAMETER_SCHEDULE_MODULE], base_path, t.stringValue(), NULL ))
//                            {
//                                m_AlphaScheduleModule = reinterpret_cast<AbstractParameterScheduleModule*>(m_PcAsm.get());
//                                cout << "Alpha schedule loaded!" << endl;
//                            }
//                        }
//                        else
                        if (t.keyIs( "ConstantValue" ))
                        {
                            m_ConstantAlpha = t.floatValue();
//                            cout << "Constant Alpha: " << m_ConstantAlpha << endl;
                        }
                        break;
                }
                break;

            case XMLScanner::Token::Close:
                    t.queryTagId( g_Tags_BQVM );
                    break;
        }
    }

    return true;
}


Ipseity::Scalar
BaseQFunctionModule::getAlpha( uint32 nbr_visits, Ipseity::Scalar td ) const
{
    if (m_AlphaScheduleModule)
    {
        return m_AlphaScheduleModule->getValue( nbr_visits, td );
    }
    else
    {
        return m_ConstantAlpha;
    }
}


bool
BaseQFunctionModule::load( const char* file_name )
{
    FILE* file;

    file = fopen( file_name, "rb" );
    if (file)
    {
        load( file );
        fclose( file );

        return true;
    }
    else
    {
        return false;
    }
}


bool
BaseQFunctionModule::save( const char* file_name )
{
    FILE* file;

    file = fopen( file_name, "wb" );
    if (file)
    {
        save( file );
        fclose( file );

        return true;
    }
    else
    {
        return false;
    }
}
