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


#include "FittedQLearning.h"

#include <Python.h> // Must appear first
#include <iostream>
#include <cstdio>
#include "IO/ParseData.h"
#include <time.h>
#include <math.h>

#include "Math/RandomNumberGenerator.h"


#include <ostream>
#include <QDebug>


FittedQLearning::FittedQLearning() :
    m_NbrTotalSamples( 0 ),
    m_Learn( true ),
    m_LoadNewData( true ),
    m_TreeEnsemble( NULL ),
    m_Input( NULL ),
    m_Layout( NULL )
{
    RandomNumberGenerator::initSeed( 0 );

    m_NextResponse.setDimension( 1 );
    m_Response.setDimension( 1 );
}

FittedQLearning::~FittedQLearning()
{
    if (m_Layout)
    {
        clearParameters();

        delete m_Layout;
    }

    if (m_NbrTotalSamples > 0)
    {
        delete [] m_MatrixStatesActions;
        delete [] m_StatesActionsMins;
        delete [] m_StatesActionsMaxs;
        delete [] m_MatrixStates;
        delete [] m_MatrixActions;
        delete [] m_MatrixNextStates;
        delete [] m_MatrixRewards;
        delete [] m_SampleQValues;
    }

    SafeDelete( m_Input );
    SafeDelete( m_TreeEnsemble );
}


const char*
FittedQLearning::getBaseName() const
{
    return "FittedQ";
}


const char*
FittedQLearning::getDescription() const
{
    return "";
}


const char*
FittedQLearning::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
FittedQLearning::defineParameters()
{
    // Init GUI
    m_Layout = new QGridLayout();
    m_Layout->setAlignment( Qt::AlignTop | Qt::AlignLeft );

    m_DoubleSpinboxGamma = new ParameterDoubleSpinBox( "Gamma", "Gamma" );
    m_DoubleSpinboxGamma->setRange( 0.0, 1.0 );
    m_DoubleSpinboxGamma->setValue( 0.90 );
    m_DoubleSpinboxGamma->setSingleStep( 0.1 );
    m_DoubleSpinboxGamma->addToLayout( m_Layout, 0,0 );

    m_SpinboxNbrIterations = new ParameterSpinBox( "NbrIterations", "Number of iterations" );
    m_SpinboxNbrIterations->setFixedWidth( 120 );
    m_SpinboxNbrIterations->setRange( 0, 10000000 );
    m_SpinboxNbrIterations->setValue( 1 );
    m_SpinboxNbrIterations->setSingleStep( 10 );
    m_SpinboxNbrIterations->addToLayout( m_Layout, 1,0 );

    m_SpinboxNbrTrees = new ParameterSpinBox( "NbrTrees", "Number of trees" );
    m_SpinboxNbrTrees->setFixedWidth( 120 );
    m_SpinboxNbrTrees->setRange( 1, 100000 );
    m_SpinboxNbrTrees->setValue( 1 );
    m_SpinboxNbrTrees->setSingleStep( 1 );
    m_SpinboxNbrTrees->addToLayout( m_Layout, 2,0 );

    m_SpinboxNbrCuts = new ParameterSpinBox( "NbrCuts", "Number of cuts" );
    m_SpinboxNbrCuts->setFixedWidth( 120 );
    m_SpinboxNbrCuts->setRange( 1, 100000 );
    m_SpinboxNbrCuts->setValue( 100 );
    m_SpinboxNbrCuts->setSingleStep( 1 );
    m_SpinboxNbrCuts->addToLayout( m_Layout, 3,0 );

    m_SpinboxMinSamples = new ParameterSpinBox( "MinSamples", "Minimum number of samples" );
    m_SpinboxMinSamples->setFixedWidth( 120 );
    m_SpinboxMinSamples->setRange( 2, 100000000 );
    m_SpinboxMinSamples->setValue( 2 );
    m_SpinboxMinSamples->setSingleStep( 1 );
    m_SpinboxMinSamples->addToLayout( m_Layout, 4,0 );

    m_SpinboxMaxLevels = new ParameterSpinBox( "MaxLevels", "Maximum number of levels" );
    m_SpinboxMaxLevels->setFixedWidth( 120 );
    m_SpinboxMaxLevels->setRange( 0, 100000 );
    m_SpinboxMaxLevels->setValue( 1000 );
    m_SpinboxMaxLevels->setSingleStep( 10 );
    m_SpinboxMaxLevels->addToLayout( m_Layout, 5,0 );

    m_LineEditExpertFilename = new ParameterLineEdit( "ExpertData", "Expert data", "expert" );
    m_LineEditExpertFilename->setFixedWidth( 240 );
    m_LineEditExpertFilename->addToLayout( m_Layout, 6,0 );

    m_LineEditNonExpertFilename = new ParameterLineEdit( "NonExpertData", "Non expert data", "random" );
    m_LineEditNonExpertFilename->setFixedWidth( 240 );
    m_LineEditNonExpertFilename->addToLayout( m_Layout, 7,0 );

    m_SpinboxNbrTotalSamples = new ParameterSpinBox( "NbrTotalSamples", "Total number of samples:" );
    m_SpinboxNbrTotalSamples->setFixedWidth( 120 );
    m_SpinboxNbrTotalSamples->setRange( 1, 100000000 );
    m_SpinboxNbrTotalSamples->setSingleStep( 10 );
    m_SpinboxNbrTotalSamples->addToLayout( m_Layout, 8,0 );

    m_SliderPercentageNonExpertSamples = new ParameterSlider( "PercentageNonExpertSamples", "Percentage of non expert samples:\n%1 \%", Qt::Horizontal );
    m_SliderPercentageNonExpertSamples->setRange( 0, 100 );
    m_SliderPercentageNonExpertSamples->setTickInterval( 5 );
    m_SliderPercentageNonExpertSamples->setTickPosition( QSlider::TicksBelow );
    connect( m_SliderPercentageNonExpertSamples, SIGNAL(valueChanged(int)), this, SLOT(onEditChange()) );
    m_SliderPercentageNonExpertSamples->addToLayout( m_Layout, 9,0 );

    connect( m_DoubleSpinboxGamma, SIGNAL(valueChanged(double)), this, SLOT(onParamChange()) );
    connect( m_SpinboxNbrIterations, SIGNAL(valueChanged(int)), this, SLOT(onParamChange()) );
    connect( m_SpinboxNbrTrees, SIGNAL(valueChanged(int)), this, SLOT(onParamChange()) );
    connect( m_SpinboxNbrCuts, SIGNAL(valueChanged(double)), this, SLOT(onParamChange()) );
    connect( m_SpinboxMinSamples, SIGNAL(valueChanged(int)), this, SLOT(onParamChange()) );
    connect( m_LineEditExpertFilename, SIGNAL(textChanged(QString)), this, SLOT(onEditChange()) );
    connect( m_LineEditNonExpertFilename, SIGNAL(textChanged(QString)), this, SLOT(onEditChange()) );
    connect( m_SpinboxNbrTotalSamples, SIGNAL(valueChanged(int)), this, SLOT(onEditChange()) );
}


bool
FittedQLearning::attachView( uint32 view_id, QWidget* widget )
{
    widget->setLayout( m_Layout );

    return true;
}


//*****************************************************************************


bool
FittedQLearning::init( const char* base_path, const char* file_name, const Taxon & taxon, AbstractInputManager*  )
{
    return true;
}


void
FittedQLearning::setMode( bool learning_mode )
{
}


bool
FittedQLearning::canReset() const
{
    return false;
}


void
FittedQLearning::reset()
{
}


void
FittedQLearning::onEditChange()
{
    m_LoadNewData = true;
}


void
FittedQLearning::onParamChange()
{
    m_Learn = true;
}


void
FittedQLearning::startNewSimulation()
{
}

void
FittedQLearning::endSimulation()
{
}


void
setMinMaxValues( float64* matrix,
                 uint32 nbr_samples, uint32 vector_size,
                 float64* mins, float64* maxs, std::vector<uint32>* valid_entries )
{
    float64 v, min, max;
    uint32 i;

    valid_entries->clear();

    for( uint32 k = 0; k < vector_size; ++k )
    {
        min = max = matrix[k];

        for( uint32 sample_id = 1, i = k+vector_size; sample_id < nbr_samples; ++sample_id, i += vector_size )
        {
            v = matrix[i];

            if (v < min)
            {
                min = v;
            }

            if (v > max)
            {
                max = v;
            }
        }

        mins[k] = min;
        maxs[k] = max;

        if (min != max)
        {
            valid_entries->push_back( k );
            cout << k << ": " << min << "," << max << endl;
        }
    }

    cout << "# valid entries: " << valid_entries->size() << endl;
}


void
FittedQLearning::startNewEpisode( uint64 scenario_uid )
{
    if (m_LoadNewData)
    {
        m_LoadNewData = false;
        m_Learn = true;

        // Loading the data
        char path[256];
        char filename[256];
        char de[256];
        char dp[256];

        sprintf( de, "%s", (char*) m_LineEditExpertFilename->toString() );
        sprintf( dp, "%s", (char*) m_LineEditNonExpertFilename->toString() );

        sprintf( path, "%s/../", m_VolatileVariables.m_Workspace.c_str() );

        if (m_NbrTotalSamples > 0)
        {
            delete [] m_MatrixStatesActions;
            delete [] m_StatesActionsMins;
            delete [] m_StatesActionsMaxs;
            delete [] m_MatrixStates;
            delete [] m_MatrixActions;
            delete [] m_MatrixNextStates;
            delete [] m_MatrixRewards;
            delete [] m_SampleQValues;
        }


        m_NbrTotalSamples = m_SpinboxNbrTotalSamples->value();
        m_NbrNonExpertSamples = (m_NbrTotalSamples*m_SliderPercentageNonExpertSamples->value())/100;
        m_NbrExpertSamples = m_NbrTotalSamples-m_NbrNonExpertSamples;


        int* tab_actions[1] = { &m_ActionVectorDimension };
        int* tab_states[2] = { &m_StateVectorDimension, &m_RewardVectorDimension };


        sprintf( filename, "%s%s_actions.txt", path, dp );
        first_parse( filename, m_NbrTotalSamples, NULL, tab_actions );
        cout << "ActionVectorDim: " << m_ActionVectorDimension << endl;
        m_MatrixActions = new int[m_NbrTotalSamples*m_ActionVectorDimension];
        int* mat_actions[] = { m_MatrixActions };
        int mat_actions_shifts[] = { 0 };
        sprintf( filename, "%s%s_actions.txt", path, de );
        parse_data( filename, m_NbrExpertSamples, mat_actions, mat_actions_shifts );
        sprintf( filename, "%s%s_actions.txt", path, dp );
        mat_actions_shifts[0] = m_NbrExpertSamples*m_ActionVectorDimension;
        parse_data( filename, m_NbrNonExpertSamples, mat_actions, mat_actions_shifts );

        sprintf( filename, "%s%s_states.txt", path, dp );
        first_parse( filename, m_NbrTotalSamples, NULL, tab_states );
        cout << "StateVectorDim: " << m_StateVectorDimension << endl;
        cout << "RewardVectorDim: " << m_RewardVectorDimension << endl;

        ExtremelyRandomizedTree::g_NbrSamples = m_NbrTotalSamples;
        ExtremelyRandomizedTree::g_InputSize = m_StateVectorDimension+m_ActionVectorDimension;

        m_MatrixStatesActions = new double[m_NbrTotalSamples*ExtremelyRandomizedTree::g_InputSize];
        m_StatesActionsMins = new double[ExtremelyRandomizedTree::g_InputSize];
        m_StatesActionsMaxs = new double[ExtremelyRandomizedTree::g_InputSize];
        m_MatrixStates = new double[m_NbrTotalSamples*m_StateVectorDimension];
        double* mat_states[] = { m_MatrixStates, NULL };
        int mat_states_shifts[] = { 0, 0 };
        sprintf( filename, "%s%s_states.txt", path, de );
        parse_data( filename, m_NbrExpertSamples, mat_states, mat_states_shifts );
        sprintf( filename, "%s%s_states.txt", path, dp );
//        int mat_non_expert_states_shifts[] = { 0, 0 };
        mat_states_shifts[0] = m_NbrExpertSamples*m_StateVectorDimension;
//        mat_states_shifts[1] = m_NbrExpertSamples*m_ActionVectorDimension;
        parse_data( filename, m_NbrNonExpertSamples, mat_states, mat_states_shifts );

        m_MatrixNextStates = new double[m_NbrTotalSamples*m_StateVectorDimension];
        m_MatrixRewards = new double[m_NbrTotalSamples*m_RewardVectorDimension];
        double* mat_next_states[] = { m_MatrixNextStates, m_MatrixRewards };
        int mat_next_states_shifts[] = { 0, 0 };
        sprintf( filename, "%s%s_next_states.txt", path, de );
        parse_data( filename, m_NbrExpertSamples, mat_next_states, mat_next_states_shifts );
        sprintf( filename, "%s%s_next_states.txt", path, dp );
        mat_next_states_shifts[0] = m_NbrExpertSamples*m_StateVectorDimension;
        mat_next_states_shifts[1] = m_NbrExpertSamples*m_RewardVectorDimension;
        parse_data( filename, m_NbrNonExpertSamples, mat_next_states, mat_next_states_shifts );

        // Create outputs
        m_SampleQValues = new double[m_NbrTotalSamples*m_RewardVectorDimension];

        float64* stac_ptr = m_MatrixStatesActions;
        float64* st_ptr = m_MatrixStates;
        int* ac_ptr = m_MatrixActions;


        // Concatenate the state matrix and the action matrix into one big matrix
        cout << "Create state-action matrix" << endl;
        for( uint32 i = 0; i < m_NbrTotalSamples; ++i )
        {
            memcpy( stac_ptr, st_ptr, sizeof( float64 )*m_StateVectorDimension );
            stac_ptr += m_StateVectorDimension;
            for( int j = 0; j < m_ActionVectorDimension; ++j )
            {
                *(stac_ptr++) = (float64) *(ac_ptr++);
            }
            st_ptr += m_StateVectorDimension;
        }
        setMinMaxValues( m_MatrixStatesActions, m_NbrTotalSamples, ExtremelyRandomizedTree::g_InputSize, m_StatesActionsMins, m_StatesActionsMaxs, &m_ValidEntries );

        // Get the action space cardinality
        n_a = 0;
        for( uint32 i = 0; i < m_NbrTotalSamples; ++i )
        {
            if (m_MatrixActions[i] > n_a)
            {
                n_a = m_MatrixActions[i];
            }
        }
        cout << "# actions: " << n_a << endl;

        ExtremelyRandomizedTree::g_Inputs = m_MatrixStatesActions;
        ExtremelyRandomizedTree::g_InputMins = m_StatesActionsMins;
        ExtremelyRandomizedTree::g_InputMaxs = m_StatesActionsMaxs;
        ExtremelyRandomizedTree::g_Outputs = m_SampleQValues;
        ExtremelyRandomizedTree::g_ValidEntries = &m_ValidEntries;

        m_State.setDimension( m_StateVectorDimension, false );

        SafeDelete( m_Input );
        m_Input = new float64 [m_StateVectorDimension+m_ActionVectorDimension];
    }

    if (m_Learn && m_NbrTotalSamples > 0)
    {
        m_Learn = false;

        SafeDelete( m_TreeEnsemble );
        m_TreeEnsemble = new TreeEnsemble( m_SpinboxNbrTrees->value() );

        cout << "Starting classification..." << endl;

        for( uint32 i = 0; i < m_SpinboxNbrIterations->value(); ++i )
        {
            cout << "Itr: " << i << " / " << m_SpinboxNbrIterations->value() << endl;
            oneStepVI();
        }

        cout << "Classification done!" << endl;
    }
}


void
FittedQLearning::endEpisode( bool is_success, uint32 nbr_decisions )
{
}


void
FittedQLearning::observe( const Sensation & x )
{
    m_CurrentStimulus = x;
}


const Response &
FittedQLearning::selectResponse()
{
    uint32 best_action;

    getV( m_CurrentStimulus.getSensoryData( 0 ), best_action );

    m_NextResponse[0] = best_action;

    return m_NextResponse;
}


void
FittedQLearning::suggestResponse( const Response & response )
{
}


void
FittedQLearning::oneStepVI()
{
    float64* ptr;
    uint32 best_action;
    float64 gamma = m_DoubleSpinboxGamma->value();

    ptr = m_MatrixNextStates;
    for( uint32 i = 0; i < m_NbrTotalSamples; ++i )
    {
        m_State = ptr;

        m_SampleQValues[i] = m_MatrixRewards[i] + gamma * getV( m_State, best_action );

//        cout << "  " << i << ": " << m_SampleQValues[i] << endl;

        ptr += m_StateVectorDimension;
    }

    findNewQFunction();
}


float64
FittedQLearning::getV( const SensorySignal & x, uint32 & best_action )
{
    float64 v, max_v;

    m_Response[0] = 1;
    best_action = 0;
    max_v = getValue( x, m_Response );
    for( uint32 i = 1; i < n_a; ++i )
    {
        m_Response[0] = i+1;
        v = getValue( x, m_Response );
        if (v > max_v)
        {
            best_action = i;
            max_v = v;
        }
    }

    return max_v;
}


float64
FittedQLearning::getValue( const SensorySignal & x, const Response & u )
{
    memcpy( m_Input, &x[0], sizeof( float64 )*m_StateVectorDimension );
    memcpy( m_Input+m_StateVectorDimension , &u[0], sizeof( float64 )*m_ActionVectorDimension );

    return m_TreeEnsemble->getLabel( m_Input );
}


void
FittedQLearning::findNewQFunction()
{
    m_TreeEnsemble->create( m_SpinboxNbrCuts->value(), m_SpinboxMinSamples->value(), m_SpinboxMaxLevels->value() );
}


void
FittedQLearning::loadMemory()
{
}

void
FittedQLearning::saveMemory()
{
}


void
FittedQLearning::loadStats()
{
}

void
FittedQLearning::saveStats()
{
}


//*****************************************************************************
