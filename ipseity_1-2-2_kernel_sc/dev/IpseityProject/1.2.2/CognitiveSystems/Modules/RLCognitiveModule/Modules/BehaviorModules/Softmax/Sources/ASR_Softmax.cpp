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


#include "ASR_Softmax.h"

#include "Types.h"
#include "AL/RLCognitiveSystem/RLTypes.h"
#include "Math/RandomNumberGenerator.h"
#include "IO/VFiles.h"
#include "IO/XMLScanner.h"
#include "XMLTags.h"

#include <iostream>

using namespace std;


const Ipseity::Scalar cstDefaultTau = 0.1;


ASR_Softmax::ASR_Softmax() :
	m_Policy( NULL ),
	m_FixedTau( true ),
	m_Tau( cstDefaultTau ),
    m_Probabilities( NULL ),
    m_Layout( NULL )
{
    RandomNumberGenerator::initSeed( 0 );
}


ASR_Softmax::~ASR_Softmax()
{
	SafeDelete( m_RouletteWheel );
    deleteWidgets();
}


const char*
ASR_Softmax::getBaseName() const
{
	return "Softmax";
}


const char*
ASR_Softmax::getDescription() const
{
    return "";
}


const char*
ASR_Softmax::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
ASR_Softmax::defineParameters()
{
    m_Layout = new QGridLayout();

    m_CheckBoxFixedTau = new ParameterCheckBox( "FlagUseFixedTau", "Tau:" );
    m_CheckBoxFixedTau->setChecked( m_FixedTau );
    m_CheckBoxFixedTau->addToLayout( m_Layout, 0,0 );

    m_SpinboxTau = new ParameterDoubleSpinBox( "Tau", "" );
    m_SpinboxTau->setMinimum( 0.0 );
    m_SpinboxTau->setMaximum( 1.0 );
    m_SpinboxTau->setSingleStep( 0.1 );
    m_SpinboxTau->setValue( m_Tau );
    m_SpinboxTau->addToLayout( m_Layout, 0,1 );

    connect( m_CheckBoxFixedTau, SIGNAL(toggled(bool)), this, SLOT(onToggleCheckboxFixedTau(bool)) );
}


bool
ASR_Softmax::attachView( uint32 view_id, QWidget* widget )
{
    widget->setLayout( m_Layout );

    return true;
}

void
ASR_Softmax::deleteWidgets()
{
    if (m_Layout)
    {
		delete m_CheckBoxFixedTau;
		delete m_SpinboxTau;
        delete m_Layout;
    }
}


void
ASR_Softmax::init( uint32 data_channel_id, AbstractPolicy* policy, AbstractQFunction* q_function )
{
    m_DataChannelId = data_channel_id;
    m_Policy = policy;
	m_QFunction = q_function;

    Cardinality c = q_function->getResponseSignalProperties()->getCardinality();

    if (c.Class == CARDINALITY_FINITE)
	{
        m_Probabilities = new float64 [c.Count];
	}
}


void
ASR_Softmax::loadParameters( const char* file_name, const char* base_path )
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
						if ( t.keyIs( "Tau" ))
                        {
							m_FixedTau = true;
							setTau( t.floatValue() );
                        }
                        break;
                }
                break;
        }
    }
}


const Response &
ASR_Softmax::selectResponse( const Stimulus & x )
{
	Ipseity::Scalar p;

	p = RandomNumberGenerator::getRandomNumber( 1.0 );


	Ipseity::Scalar total_q = 0.0;
	uint32 response_id;
	uint32 nbr_responses;

	response_id = 0;
    m_Response.setToMinVector( m_QFunction->getResponseSignalProperties() );
	do
	{
		m_Probabilities[response_id] = exp( m_QFunction->getValue( x.getSensoryData( m_DataChannelId ), m_Response ) / m_Tau );
		total_q += m_Probabilities[response_id];
		++response_id;
	}
    while( m_Response.inc( m_QFunction->getResponseSignalProperties() ) );
	nbr_responses = response_id;

	Ipseity::Scalar last_p = 0.0;

    m_Response.setToMinVector( m_QFunction->getResponseSignalProperties() );
	for( response_id = 0; response_id < nbr_responses; ++response_id )
	{
		m_Probabilities[response_id] /= total_q;
		m_Probabilities[response_id] += last_p;

		if (p < m_Probabilities[response_id])
		{
			break;
		}

		last_p = m_Probabilities[response_id];
        m_Response.inc( m_QFunction->getResponseSignalProperties() );
	}

	if (m_Policy)
	{
		m_BestResponseSelected = (m_Policy->selectBestResponse( x.getSensoryData( m_DataChannelId ) ) == m_Response);
	}
	else
	{
		m_BestResponseSelected = false;
	}

	return m_Response;
}


bool
ASR_Softmax::isBestResponseSelected() const
{
    return m_BestResponseSelected;
}


void
ASR_Softmax::setTau( Ipseity::Scalar e )
{
	m_Tau = e;
}


void
ASR_Softmax::onToggleCheckboxFixedTau( bool toggle )
{
	m_SpinboxTau->setEnabled( toggle );
}
