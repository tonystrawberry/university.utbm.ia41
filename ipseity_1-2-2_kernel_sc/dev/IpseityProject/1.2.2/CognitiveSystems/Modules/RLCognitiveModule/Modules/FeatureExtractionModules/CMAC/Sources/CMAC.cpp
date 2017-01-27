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


#include "CMAC.h"


#include "Math/RandomNumberGenerator.h"
#include "IO/XMLScanner.h"
#include "XMLTags_CMAC.h"


//*****************************************************************************


CMAC::CMAC()
{
    RandomNumberGenerator::initSeed( 0 );
}

CMAC::~CMAC()
{
    for( uint32 i = 0; i < m_Tilings.size(); i++ )
    {
        SafeDelete( m_Tilings[i] );
    }
}


const char*
CMAC::getBaseName() const
{
    return "CMAC";
}


const char*
CMAC::getDescription() const
{
    return "Cerebellar Model Articulation Controller";
}


const char*
CMAC::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
CMAC::defineParameters()
{
}


bool
CMAC::attachView( uint32 view_id, QWidget* )
{
    return false;
}


bool
CMAC::init( const char* file_name, const char* base_path )
{
    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        return false;
    }

    XMLScanner::Token t;
    uint32 var_id, nbr_intervals;
    Tiling<Ipseity::Scalar>* main_tiling;
    uint32 nbr_tilings;
    Ipseity::String name;
    bool def_var = false;
    bool dynamic_var;

//    cout << "Loading CMAC parameters from '" << base_path << file_name << "'..." << endl;

    m_ExtractFromStimulusOnly = true;
    m_DynamicVariables = false;

    while( scanner.nextToken( &t ) )
    {
        switch( t.kind )
        {
            case XMLScanner::Token::Tag:
                t.queryTagId( g_Tags );

                switch( t.tagId )
                {
                    case TAG_FEM:
                        def_var = true;
                        break;

                    case TAG_TILING:
                    {
                        main_tiling = new Tiling<Ipseity::Scalar>();
                        nbr_tilings = 1;
                        break;
                    }
                }
                break;

            case XMLScanner::Token::Attribute:
                switch( t.tagId )
                {
                    case TAG_VARIABLE:
                        if (t.keyIs( "Name" ))
                        {
                            name = t.stringValue();
                            dynamic_var = false;
                        }
                        else if (t.keyIs( "NbrIntervals" ))
                        {
                            nbr_intervals = t.intValue();
                        }
                        else if (t.keyIs( "Type" ))
                        {
                            if (!strcmp( t.stringValue(), "Dynamic" ))
                            {
                                dynamic_var = true;
                            }
                        }
                        else if (t.keyIs( "Cardinal" ))
                        {
                            m_NbrDynamicVariableSpaceCardinal = t.intValue();
                        }
                        break;

                    case TAG_TILING:
                        if (t.keyIs( "Repeat" ))
                        {
                            nbr_tilings = t.intValue();
                        }
                        break;
                }
                break;

            case XMLScanner::Token::Close:
                t.queryTagId( g_Tags );

                switch( t.tagId )
                {
                    case TAG_FEM:
                        def_var = false;
                        break;

                    case TAG_VARIABLE:
                        if (def_var)
                        {
                            if (m_DataDescription && m_DataDescription->query( name.c_str(), var_id ))
                            {
                                if (dynamic_var)
                                {
                                    m_DynamicVariables = true;
                                    m_DynamicVariableId = var_id;

//                                    cout << "Dynamic Variable: " << var_id << endl;
                                }
                                else
                                {
                                    TilingDimension<Ipseity::Scalar> tiling_dim( var_id, (Ipseity::ScalarDomain*) &(*m_DataDescription)[var_id], nbr_intervals );

                                    main_tiling->addDimension( tiling_dim );

                                    if (var_id >= m_StimulusDescription->getDimension())
                                    {
                                        m_ExtractFromStimulusOnly = false;
                                    }
                                }
                            }
                        }
                        break;

                    case TAG_TILINGS:
                        prepare();
                        break;

                    case TAG_TILING:
//						cout << "Create " << nbr_tilings << " tilings" << endl;
                        createTilings( nbr_tilings, main_tiling );
                        delete main_tiling;
                        break;
                }
                break;
        }
    }

    return true;
}


void
CMAC::prepareFeatures( const SensorySignal & x, const Response & u )
{
    uint32 nbr = 0;
    uint32 index1;

    if (extractFromStimulusOnly())
    {
        uint32 index2;

        index2 = m_ResponseDescription->getIndex( u )*m_NbrTotalFeatures;

        if (m_DynamicVariables)
        {
            uint32 dynamic_var_index;

            if (m_DynamicVariable.find( x[m_DynamicVariableId] ) == m_DynamicVariable.end())
            {
                dynamic_var_index = m_DynamicVariable.size();
                m_DynamicVariable[x[m_DynamicVariableId]] = dynamic_var_index;
            }
            else
            {
                dynamic_var_index = m_DynamicVariable[x[m_DynamicVariableId]];
            }

//			cout << x[m_DynamicVariableId] << " -> " << dynamic_var_index << endl;

            index2 += dynamic_var_index*m_NbrTotalFeatures*2;
        }

        for( uint32 i = 0; i < m_NbrActivatedFeatures; i++ )
        {
            if (m_Tilings[i]->query( x, index1 ))
            {
                index1 += nbr;
                m_ActivatedWeightIds[i] = index1+index2;
                m_ActivatedFeatureIds[i] = index1;
            }

            nbr += m_Tilings[i]->getNbrTiles();
        }
    }
    else
    {
        m_SR = x.concat( u );

        for( uint32 i = 0; i < m_NbrActivatedFeatures; i++ )
        {
            if (m_Tilings[i]->query( m_SR, index1 ))
            {
                index1 += nbr;
                m_ActivatedWeightIds[i] = index1;
                m_ActivatedFeatureIds[i] = index1;
            }

            nbr += m_Tilings[i]->getNbrTiles();
        }
    }
}


bool
CMAC::update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate )
{
    return false;
}


bool
CMAC::load( FILE* file )
{
    for( uint32 i = 0; i < m_Tilings.size(); ++i )
    {
        m_Tilings[i]->load( file );
    }

    return true;
}

bool
CMAC::save( FILE* file )
{
    for( uint32 i = 0; i < m_Tilings.size(); ++i )
    {
        m_Tilings[i]->save( file );
    }

    return true;
}


Ipseity::Scalar
CMAC::getFeature( uint32 ) const
{
    return 1.0;
}


//*******************************************************************
// Protected functions
//*******************************************************************


void
CMAC::prepare()
{
    m_NbrActivatedFeatures = m_Tilings.size();

    m_NbrTotalFeatures = 0;
    for( uint32 i = 0; i < m_NbrActivatedFeatures; i++ )
    {
        m_NbrTotalFeatures += m_Tilings[i]->getNbrTiles();
    }

    m_ActivatedWeightIds.resize( m_NbrActivatedFeatures );
    m_ActivatedFeatureIds.resize( m_NbrActivatedFeatures );

    if (extractFromStimulusOnly())
    {
        Cardinality c;

        c = m_ResponseDescription->getCardinality();

        if (c.Class == CARDINALITY_FINITE)
        {
//            cout << "Response Space Cardinality: " << c.Count << endl;

            m_NbrWeights = m_NbrTotalFeatures*c.Count;
        }
        else
        {
//            cout << "Infinite Response Space!" << endl;

            m_NbrWeights = m_NbrTotalFeatures;
        }
    }
    else
    {
        m_NbrWeights = m_NbrTotalFeatures;
    }

    if (m_DynamicVariables)
    {
        m_NbrWeights *= m_NbrDynamicVariableSpaceCardinal;
    }
}


void
CMAC::createTilings( uint32 nbr_tilings, Tiling<Ipseity::Scalar>* main_tiling )
{
    Tiling<Ipseity::Scalar>* tiling;

    for( uint32 i = 0; i < nbr_tilings; ++i )
    {
        tiling = new Tiling<Ipseity::Scalar>();
        tiling->copyFrom( *main_tiling );
        m_Tilings.push_back( tiling );
    }
}
