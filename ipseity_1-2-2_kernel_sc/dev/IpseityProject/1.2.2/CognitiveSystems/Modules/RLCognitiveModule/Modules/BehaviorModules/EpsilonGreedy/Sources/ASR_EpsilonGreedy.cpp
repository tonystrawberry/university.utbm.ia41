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


#include "ASR_EpsilonGreedy.h"

#include "Types.h"
#include "AL/RLCognitiveSystem/RLTypes.h"
#include "Math/RandomNumberGenerator.h"
#include "IO/VFiles.h"
#include "IO/XMLScanner.h"
#include "XMLTags.h"

#include <iostream>

using namespace std;


const Ipseity::Scalar cstDefaultEpsilon = 0.1;


ASR_EpsilonGreedy::ASR_EpsilonGreedy() :
	m_Policy( NULL ),
	m_FixedEpsilon( true ),
    m_Epsilon( cstDefaultEpsilon ),
    m_Layout( NULL )
{
    RandomNumberGenerator::initSeed( 0 );
}


ASR_EpsilonGreedy::~ASR_EpsilonGreedy()
{
    deleteWidgets();
}


const char*
ASR_EpsilonGreedy::getBaseName() const
{
    return "Epsilon-Greedy";
}


const char*
ASR_EpsilonGreedy::getDescription() const
{
    return "";
}


const char*
ASR_EpsilonGreedy::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
ASR_EpsilonGreedy::defineParameters()
{
    m_Layout = new QGridLayout();

    m_CheckBoxFixedEpsilon = new ParameterCheckBox( "FlagUseFixedEpsilon", "Epsilon:" );
    m_CheckBoxFixedEpsilon->setChecked( m_FixedEpsilon );
    m_CheckBoxFixedEpsilon->addToLayout( m_Layout, 0,0 );

    m_SpinboxEpsilon = new ParameterDoubleSpinBox( "Epsilon", "" );
    m_SpinboxEpsilon->setMinimum( 0.0 );
    m_SpinboxEpsilon->setMaximum( 1.0 );
    m_SpinboxEpsilon->setSingleStep( 0.1 );
    m_SpinboxEpsilon->setValue( m_Epsilon );
    m_SpinboxEpsilon->addToLayout( m_Layout, 0,1 );

    connect( m_CheckBoxFixedEpsilon, SIGNAL(toggled(bool)), this, SLOT(onToggleCheckboxFixedEpsilon(bool)) );
}


bool
ASR_EpsilonGreedy::attachView( uint32 view_id, QWidget* widget )
{
    widget->setLayout( m_Layout );

    return true;
}

void
ASR_EpsilonGreedy::deleteWidgets()
{
    if (m_Layout)
    {
        delete m_CheckBoxFixedEpsilon;
        delete m_SpinboxEpsilon;

        delete m_Layout;
    }
}


void
ASR_EpsilonGreedy::init( uint32 data_channel_id, AbstractPolicy* policy, AbstractQFunction* q_function )
{
	m_DataChannelId = data_channel_id;
	m_Policy = policy;
	m_QFunction = q_function;
}


void
ASR_EpsilonGreedy::loadParameters( const char* file_name, const char* base_path )
{
    XMLScanner scanner( file_name, base_path );
    XMLScanner::Token t;

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
                        if ( t.keyIs( "Epsilon" ))
                        {
                            m_FixedEpsilon = true;
                            setEpsilon( t.floatValue() );
                        }
                        break;
                }
                break;
        }
    }
}


const Response &
ASR_EpsilonGreedy::selectResponse( const Stimulus & x )
{
    Ipseity::Scalar p;

	p = RandomNumberGenerator::getRandomNumber( 1.0 );

	if (p < m_SpinboxEpsilon->value())
    {
		m_BestResponseSelected = false;

//		cout << "Random response" << endl;
		return m_Policy->selectRandomResponse( x.getSensoryData( m_DataChannelId ) );
    }
    else
    {
        if (m_Policy)
        {
            m_BestResponseSelected = true;

//			cout << "Best response" << endl;
            return m_Policy->selectBestResponse( x.getSensoryData( m_DataChannelId ) );
        }
        else
        {
            m_BestResponseSelected = false;

			return Response::VoidResponse;
        }
    }
}


bool
ASR_EpsilonGreedy::isBestResponseSelected() const
{
    return m_BestResponseSelected;
}


void
ASR_EpsilonGreedy::setEpsilon( Ipseity::Scalar e )
{
    m_Epsilon = e;
}


void
ASR_EpsilonGreedy::onToggleCheckboxFixedEpsilon( bool toggle )
{
    m_SpinboxEpsilon->setEnabled( toggle );
}
