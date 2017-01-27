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


#include "LM_VIBased.h"

#include "IO/XMLScanner.h"
#include "AL/RLCognitiveSystem/CognitiveSystem/XMLTags_CognitiveSystem.h"


LearningModule_VIBased::LearningModule_VIBased() :
    m_RewardComponentId( 0 ),
    m_AccumulateEligibilities( false ),
    m_Layout( NULL )
{
}

LearningModule_VIBased::~LearningModule_VIBased()
{
    deleteWidgets();
}


void
LearningModule_VIBased::defineParameters()
{
    m_Layout = new QGridLayout();

    m_ButtonAccumulateTraces = new QRadioButton( "Accumulate eligibility traces" );
    m_ButtonReplaceTraces = new QRadioButton( "Replace eligibility traces" );

    m_ButtonGroup = new QButtonGroup();
    m_ButtonGroup->addButton( m_ButtonAccumulateTraces );
    m_ButtonGroup->addButton( m_ButtonReplaceTraces );
    m_ButtonGroup->setExclusive( true );

    m_ButtonAccumulateTraces->setChecked( true ); //m_AccumulateEligibilities );

    m_Layout->addWidget( m_ButtonAccumulateTraces, 0,0,1,2 );
    m_Layout->addWidget( m_ButtonReplaceTraces, 1,0,1,2 );

    m_SpinboxLambda = new ParameterDoubleSpinBox( "Lambda", "Lambda:" );
    m_SpinboxLambda->setMinimum( 0.0 );
    m_SpinboxLambda->setMaximum( 1.0 );
    m_SpinboxLambda->setSingleStep( 0.1 );
    m_SpinboxLambda->addToLayout( m_Layout, 2,0 );
}


bool
LearningModule_VIBased::attachView( uint32 view_id, QWidget* widget )
{
    m_ButtonAccumulateTraces->setParent( widget );
    m_ButtonReplaceTraces->setParent( widget );
    m_SpinboxLambda->setValue( m_Lambda );

    widget->setLayout( m_Layout );

    return true;
}


bool
LearningModule_VIBased::loadParameters( const char* file_name, const char* base_path )
{
    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        return false;
    }

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
                    case TAG_PARAMETERS:
                        if (t.keyIs( "RewardComponentId" ))
                        {
                            m_RewardComponentId = t.intValue();
                        }
                        else if (t.keyIs( "AccumulateEligibilities" ))
                        {
                            if (!strcmp( t.stringValue(), "yes" ))
                            {
                                m_AccumulateEligibilities = true;
                            }
                            else
                            {
                                m_AccumulateEligibilities = false;
                            }
                        }
                        else if (t.keyIs( "Lambda" ))
                        {
                            m_Lambda = t.floatValue();
//                            printf( "Lambda: %.2f\n", m_Lambda );
                        }
                        break;
                }
                break;
        }
    }

    return true;
}


void
LearningModule_VIBased::deleteWidgets()
{
    if (m_Layout)
    {
        delete m_ButtonAccumulateTraces;
        delete m_ButtonReplaceTraces;
        delete m_ButtonGroup;

        clearParameters();

        delete m_Layout;
    }
}
