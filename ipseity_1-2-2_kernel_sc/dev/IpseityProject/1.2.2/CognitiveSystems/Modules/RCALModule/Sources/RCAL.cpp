/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2014, 2013 Ipseity Core Developers
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


#include "RCAL.h"

#include <Python.h> // Must appear first
#include <iostream>
#include <cstdio>
#include "IO/ParseData.h"
#include <time.h>
#include <math.h>

#include "Math/RandomNumberGenerator.h"

#include <ostream>
#include <QDebug>


RCAL::RCAL() :
    m_NbrExpertSamples( 0 ),
    m_Classify( true ),
    m_LoadNewData( true ),
    RCAL_classifier( NULL ),
    m_Layout( NULL )
{
    RandomNumberGenerator::initSeed( 0 );

    m_NextResponse.setDimension( 1 );
}

RCAL::~RCAL()
{
    SafeDelete( RCAL_classifier );

    if (m_Layout)
    {
        clearParameters();

        delete m_Layout;
    }

    delete [] m_MatrixExpertStates;
    delete [] m_MatrixExpertActions;
    delete [] m_MatrixExpertNextStates;
    delete [] m_MatrixNonExpertStates;
    delete [] m_MatrixNonExpertActions;
    delete [] m_MatrixNonExpertNextStates;
}


const char*
RCAL::getBaseName() const
{
    return "RCAL";
}


const char*
RCAL::getDescription() const
{
    return "";
}


const char*
RCAL::getAuthors() const
{
    return "Piot, B. and " IPSEITY_CREATOR;
}


void
RCAL::defineParameters()
{
    // Init GUI
    m_Layout = new QGridLayout();
    m_Layout->setAlignment( Qt::AlignTop | Qt::AlignLeft );

    m_LineEditExpertFilename = new ParameterLineEdit( "ExpertData", "Expert data", "expert" );
    m_LineEditExpertFilename->setFixedWidth( 240 );
    m_LineEditExpertFilename->addToLayout( m_Layout, 0,0 );

    m_LineEditNonExpertFilename = new ParameterLineEdit( "NonExpertData", "Non expert data", "random" );
    m_LineEditNonExpertFilename->setFixedWidth( 240 );
    m_LineEditNonExpertFilename->addToLayout( m_Layout, 1,0 );

    m_SpinboxNbrTotalSamples = new ParameterSpinBox( "TotalNbrSamples", "Total number of samples:" );
    m_SpinboxNbrTotalSamples->setRange( 1, 100000000 );
    m_SpinboxNbrTotalSamples->setSingleStep( 10 );
    m_SpinboxNbrTotalSamples->addToLayout( m_Layout, 2,0 );

    m_SliderPercentageNonExpertSamples = new ParameterSlider( "PercentageNonExpertSamples", "Percentage of non expert samples:\n%1", Qt::Horizontal );
    m_SliderPercentageNonExpertSamples->setRange( 0, 100 );
    m_SliderPercentageNonExpertSamples->setTickInterval( 5 );
    m_SliderPercentageNonExpertSamples->setTickPosition( QSlider::TicksBelow );
    m_SliderPercentageNonExpertSamples->addToLayout( m_Layout, 3,0 );

    m_SpinboxLambda = new ParameterDoubleSpinBox( "Lambda", "Lambda" );
    m_SpinboxLambda->setRange( 0.0, 1.0 );
    m_SpinboxLambda->setSingleStep( 0.05 );
    m_SpinboxLambda->setValue( 0.1 );
    m_SpinboxLambda->addToLayout( m_Layout, 4,0 );

    m_SpinboxNbrLearners = new ParameterSpinBox( "NbrLearners", "Number of trees" );
    m_SpinboxNbrLearners->setMinimumHeight( 30 );
    m_SpinboxNbrLearners->setMinimum( 1 );
    m_SpinboxNbrLearners->setMaximum( 100000 );
    m_SpinboxNbrLearners->setValue( 50 );
    m_SpinboxNbrLearners->addToLayout( m_Layout, 5,0 );

    connect( m_LineEditExpertFilename, SIGNAL(textChanged(QString)), this, SLOT(onEditChange()) );
    connect( m_LineEditNonExpertFilename, SIGNAL(textChanged(QString)), this, SLOT(onEditChange()) );
    connect( m_SpinboxNbrTotalSamples, SIGNAL(valueChanged(int)), this, SLOT(onEditChange()) );
    connect( m_SliderPercentageNonExpertSamples, SIGNAL(valueChanged(int)), this, SLOT(onEditChange()) );
    connect( m_SpinboxLambda, SIGNAL(valueChanged(double)), this, SLOT(onParamClassifChange()) );
    connect( m_SpinboxNbrLearners, SIGNAL(valueChanged(int)), this, SLOT(onParamClassifChange()) );
}


bool
RCAL::attachView( uint32 view_id, QWidget* widget )
{
    widget->setLayout( m_Layout );

    return true;
}


//*****************************************************************************


bool
RCAL::init( const char* base_path, const char* file_name, const Taxon & taxon, AbstractInputManager*  )
{
    return true;
}


void
RCAL::setMode( bool learning_mode )
{
}


bool
RCAL::canReset() const
{
    return false;
}


void
RCAL::reset()
{
}


void
RCAL::onEditChange()
{
    m_LoadNewData = true;
}


void
RCAL::onParamClassifChange()
{
    m_Classify = true;
}


void
RCAL::startNewSimulation()
{
}

void
RCAL::endSimulation()
{
}


void
RCAL::startNewEpisode( uint64 scenario_uid )
{
    if (m_LoadNewData)
    {
        m_LoadNewData = false;
        m_Classify = true;

        // Loading the data
        char path[256];
        char filename[256];
        char de[256];
        char dp[256];

        sprintf( de, "%s", (char*) m_LineEditExpertFilename->toString() );
        sprintf( dp, "%s", (char*) m_LineEditNonExpertFilename->toString() );

        sprintf( path, "%s/../", m_VolatileVariables.m_Workspace.c_str() );

        if (m_NbrExpertSamples > 0)
        {
            delete [] m_MatrixExpertStates;
            delete [] m_MatrixExpertActions;
            delete [] m_MatrixExpertNextStates;
            delete [] m_MatrixNonExpertStates;
            delete [] m_MatrixNonExpertActions;
            delete [] m_MatrixNonExpertNextStates;
        }


        int nbr_total_samples;
        int max_nbr_expert_samples;
        int max_nbr_non_expert_samples;

        nbr_total_samples = m_SpinboxNbrTotalSamples->value();
        max_nbr_non_expert_samples = (nbr_total_samples*m_SliderPercentageNonExpertSamples->value())/100;
        max_nbr_expert_samples = nbr_total_samples-max_nbr_non_expert_samples;


        int* tab_actions[1] = { &m_ActionVectorDimension };
        int* tab_states[2] = { &m_StateVectorDimension, NULL };

        //parsing of .txt files which gives us the matrix needed
        sprintf( filename, "%s%s_actions.txt", path, de );
        first_parse( filename, max_nbr_expert_samples, &m_NbrExpertSamples, tab_actions );
        cout << "ActionVectorDim: " << m_ActionVectorDimension << endl;
        cout << "NbrExpertSamples: " << m_NbrExpertSamples << endl;
        m_MatrixExpertActions = new int [m_NbrExpertSamples*m_ActionVectorDimension];
        int* mat_expert_actions[1] = { m_MatrixExpertActions };
        int actions_shift[] = { 0 };
        parse_data( filename, m_NbrExpertSamples, mat_expert_actions, actions_shift );

        sprintf( filename, "%s%s_states.txt", path, de );
        first_parse( filename, max_nbr_expert_samples, &m_NbrExpertSamples, tab_states );
        cout << "StateVectorDim: " << m_StateVectorDimension << endl;
        m_MatrixExpertStates = new double [m_NbrExpertSamples*m_StateVectorDimension];
        double* mat_expert_states[2] = { m_MatrixExpertStates, NULL };
        int states_shifts[] = { 0, 0 };
        parse_data( filename, m_NbrExpertSamples, mat_expert_states, states_shifts );

        sprintf( filename, "%s%s_next_states.txt", path, de );
        m_MatrixExpertNextStates = new double [m_NbrExpertSamples*m_StateVectorDimension];
        double* mat_expert_next_states[2] = { m_MatrixExpertNextStates, NULL };
        parse_data( filename, m_NbrExpertSamples, mat_expert_next_states, states_shifts );


        sprintf( filename, "%s%s_actions.txt", path, dp );
        first_parse( filename, max_nbr_non_expert_samples, &m_NbrNonExpertSamples, NULL );
        cout << "NbrNonExpertSamples: " << m_NbrNonExpertSamples << endl;
        m_NbrTotalSamples = m_NbrNonExpertSamples+m_NbrExpertSamples;
        m_MatrixNonExpertActions = new int[m_NbrTotalSamples*m_ActionVectorDimension];
        int* mat_nonexpert_actions[1] = { m_MatrixNonExpertActions };
        memcpy( m_MatrixNonExpertActions, m_MatrixExpertActions, m_NbrExpertSamples*m_ActionVectorDimension*sizeof( int ) );
        sprintf( filename, "%s%s_actions.txt", path, dp );
        actions_shift[0] = m_NbrExpertSamples*m_ActionVectorDimension;
        parse_data( filename, m_NbrNonExpertSamples, mat_nonexpert_actions, actions_shift );

        sprintf( filename, "%s%s_states.txt", path, dp );
        m_MatrixNonExpertStates = new double[m_NbrTotalSamples*m_StateVectorDimension];
        double* mat_nonexpert_states[2] = { m_MatrixNonExpertStates, NULL };
        memcpy( m_MatrixNonExpertStates, m_MatrixExpertStates, m_NbrExpertSamples*m_StateVectorDimension*sizeof( double ) );
        sprintf( filename, "%s%s_states.txt", path, dp );
        states_shifts[0] = m_NbrExpertSamples*m_StateVectorDimension;
        parse_data( filename, m_NbrNonExpertSamples, mat_nonexpert_states, states_shifts );

        sprintf( filename, "%s%s_next_states.txt", path, dp );
        m_MatrixNonExpertNextStates = new double[m_NbrTotalSamples*m_StateVectorDimension];
        double* mat_nonexpert_next_states[2] = { m_MatrixNonExpertNextStates, NULL };
        memcpy( m_MatrixNonExpertNextStates, m_MatrixExpertNextStates, m_NbrExpertSamples*m_StateVectorDimension*sizeof( double ) );
        sprintf( filename, "%s%s_next_states.txt", path, dp );
//        states_shifts[0] = m_NbrExpertSamples*m_StateVectorDimension;
        parse_data( filename, m_NbrNonExpertSamples, mat_nonexpert_next_states, states_shifts );

        n_a = 0;
        for( uint32 i = 0; i < m_NbrTotalSamples; ++i )
        {
            if (m_MatrixNonExpertActions[i] > n_a)
            {
                n_a = m_MatrixNonExpertActions[i];
            }
        }
        cout << "# actions: " << n_a << endl;
    }

    if (m_Classify && m_NbrExpertSamples > 0)
    {
        m_Classify = false;

        N_base = m_SpinboxNbrLearners->value();

        cout << "Starting classification..." << endl;
        SafeDelete( RCAL_classifier );
        RCAL_classifier = new BilalClassifier();

        RCAL_classifier->classify( m_MatrixExpertStates, m_MatrixExpertActions, m_MatrixExpertNextStates,
                                  m_MatrixNonExpertStates, m_MatrixNonExpertActions, m_MatrixNonExpertNextStates,
                                  n_a, N_base,
                                  m_NbrExpertSamples, m_NbrTotalSamples, m_StateVectorDimension,
                                  m_SpinboxLambda->value() );

        cout << "Classification done!" << endl;
    }
}


void
RCAL::endEpisode( bool is_success, uint32 nbr_decisions )
{
}


void
RCAL::observe( const Sensation & x )
{
    m_CurrentStimulus = x;
}


const Response &
RCAL::selectResponse()
{
    if (RCAL_classifier)
    {
        int predictedLabels;
        float64* state;

        state = &m_CurrentStimulus.getSensoryData( 0 )[0];

        RCAL_classifier->predict(
            state,
            1,
            n_a,
            N_base,
            m_StateVectorDimension,
            &predictedLabels );
        m_NextResponse[0] = predictedLabels-1;
    }

    return m_NextResponse;
}


void
RCAL::suggestResponse( const Response & response )
{
}


void
RCAL::loadMemory()
{
}

void
RCAL::saveMemory()
{
}


void
RCAL::loadStats()
{
}

void
RCAL::saveStats()
{
}


//*****************************************************************************
