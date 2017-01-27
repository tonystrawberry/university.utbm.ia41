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


#include "LinearFunctionApproximator.h"

#include "IO/XMLScanner.h"
#include "XMLTags_LinearFunctionApproximator.h"

#include <stdio.h>


//*****************************************************************************


Cell::Cell() :
    Weight( 0.0 ), NbrVisits( 0 )
{
}


void
Cell::reset()
{
    Weight = 0.0;
    NbrVisits = 0;
}


//*****************************************************************************


LinearFunctionApproximator::LinearFunctionApproximator() :
    m_FeatureExtractionModule( NULL ),
    m_NbrCells( 0 ),
    m_NbrVisitedCells( 0 )
{
    m_Layout = new QGridLayout();
    m_Label = new QLabel();
    m_Layout->addWidget( m_Label, 1,1,1,1 );
}


LinearFunctionApproximator::~LinearFunctionApproximator()
{
	m_PcFem.free();

    deleteWidgets();
}


const char*
LinearFunctionApproximator::getBaseName() const
{
    return "LinearFunctionApproximator";
}


const char*
LinearFunctionApproximator::getDescription() const
{
    return "Approximate the Q-Value Function using a linear combination of weights and features.";
}


const char*
LinearFunctionApproximator::getAuthors() const
{
    return IPSEITY_CREATOR;
}


void
LinearFunctionApproximator::defineParameters()
{
}


bool
LinearFunctionApproximator::attachView( uint32 view_id, QWidget* widget )
{
    updateLabelPercentageVisitedCells();

    widget->setLayout( m_Layout );

    return true;
}


void
LinearFunctionApproximator::deleteWidgets()
{
    if (m_Layout)
    {
        delete m_Label;
        SafeDelete( m_Layout );
    }
}


bool
LinearFunctionApproximator::isDynamic() const
{
    return false;
}


bool
LinearFunctionApproximator::loadParameters( const char* file_name, const char* base_path )
{
//    cout << "Load function approximator parameters from " << file_name << " in " << base_path << endl;
    BaseQFunctionModule::loadParameters( file_name, base_path );

    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        return false;
    }

    m_NbrCells = 0;

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
                    case TAG_FEATURE_EXTRACTION_MODULE:
                        if (t.keyIs( "Name" ))
                        {
                            m_PcFem.setParent( this );
							if (m_PcFem.load( "FeatureExtractionModule", t.stringValue() ))
                            {
//                                cout << "Feature extraction module: " << t.stringValue() << " loaded!" << endl;
                                m_FeatureExtractionModule = reinterpret_cast<AbstractFeatureExtractionModule*>(m_PcFem.get());
                                if (m_FeatureExtractionModule)
                                {
//                                    cout << "SD (" << m_StimulusSpace->getDimension() << ")" << endl;
//                                    cout << "RD (" << m_ResponseSpace->getDimension() << ")" << endl;
//                                    cout << "DD (" << m_Description.getDimension() << ")" << endl;
                                    m_FeatureExtractionModule->setDataDescription( m_StimulusSpace, m_ResponseSpace, &m_Description );
//                                    cout << "Load Feature extraction module parameters" << endl;
                                    m_FeatureExtractionModule->init( file_name, base_path );

                                    init();
                                }
                            }
                        }
                        break;
                }
                break;

            case XMLScanner::Token::Close:
                t.queryTagId( g_Tags );
                break;
        }
    }

    return true;
}


void
LinearFunctionApproximator::init()
{
    m_NbrCells = m_FeatureExtractionModule->getNbrTotalWeights();
    m_Cells.resize( m_NbrCells );

//    cout << m_NbrCells << " weights." << endl;
//    cout << m_FeatureExtractionModule->getNbrTotalFeatures() << " features." << endl;

    reset();
}


Ipseity::Scalar
LinearFunctionApproximator::getPercentageVisitedCells()
{
    if (m_NbrCells)
    {
        return (100.0*(((Ipseity::Scalar) m_NbrVisitedCells) / (Ipseity::Scalar) m_NbrCells));
    }
    else
    {
        return 0.0;
    }
}


void
LinearFunctionApproximator::updateLabelPercentageVisitedCells()
{
    QString text( "Visited cells: %1 \% (%2/%3)" );

    m_Label->setText( text.arg( getPercentageVisitedCells(), 0, 'f', 2 ).arg( m_NbrVisitedCells ).arg( m_NbrCells ) );
}


bool
LinearFunctionApproximator::canReset() const
{
    return (m_NbrVisitedCells > 0);
}


void
LinearFunctionApproximator::reset()
{
    m_NbrVisitedCells = 0;
    for( uint32 i = 0; i < getNbrCells(); ++i )
    {
        m_Cells[i].reset();
    }
    m_EligibilityTraces.clear();
    updateLabelPercentageVisitedCells();
}


uint32
LinearFunctionApproximator::getNbrCells() const
{
    return m_NbrCells;
}


Ipseity::Scalar
LinearFunctionApproximator::getValue( const SensorySignal & x, const Response & u )
{
    Ipseity::Scalar v = 0.0;

    if (m_FeatureExtractionModule)
    {
        m_FeatureExtractionModule->computeFeatures( x, u );

        uint32 cell_id;
        uint32 feature_id;

        for( uint32 i = 0; i < m_FeatureExtractionModule->getNbrActivatedFeatures(); ++i )
        {
            feature_id = m_FeatureExtractionModule->getActivatedFeatureId( i );
            cell_id = m_FeatureExtractionModule->getActivatedWeightId( i );
            v += m_Cells[cell_id].Weight*m_FeatureExtractionModule->getNormalizedFeature( feature_id );
        }
    }

    return v;
}


uint32
LinearFunctionApproximator::getNbrVisits( const SensorySignal & x, const Response & u )
{
    if (m_FeatureExtractionModule)
    {
        uint32 nbr_visits = 0;
        uint32 cell_id;

        m_FeatureExtractionModule->computeFeatures( x, u );

        for( uint32 i = 0; i < m_FeatureExtractionModule->getNbrActivatedFeatures(); ++i )
        {
            cell_id = m_FeatureExtractionModule->getActivatedWeightId( i );

            nbr_visits += m_Cells[cell_id].NbrVisits;
        }
        nbr_visits /= m_FeatureExtractionModule->getNbrActivatedFeatures();

        return nbr_visits;
    }
    else
    {
        return 0;
    }
}


void
LinearFunctionApproximator::update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate )
{
    uint32 feature_id;
    uint32 cell_id;
    Ipseity::Scalar td;
    Ipseity::Scalar delta_weight;

    td = new_estimate-old_estimate;

//    cout << "Update with: " << td << endl;
    m_FeatureExtractionModule->computeFeatures( x, u );

    for( uint32 i = 0; i < m_FeatureExtractionModule->getNbrActivatedFeatures(); ++i )
    {
        feature_id = m_FeatureExtractionModule->getActivatedFeatureId( i );
        cell_id = m_FeatureExtractionModule->getActivatedWeightId( i );
        delta_weight = getAlpha( m_Cells[cell_id].NbrVisits, td )*td;
        m_Cells[cell_id].Weight += delta_weight*m_FeatureExtractionModule->getNormalizedFeature( feature_id );
    }

//    for( std::map<uint32,Ipseity::Scalar>::const_iterator itr = m_EligibilityTraces.begin(); itr != m_EligibilityTraces.end(); ++itr )
//    {
//        cell_id = (*itr).first;
//        delta_weight = getAlpha( m_Cells[cell_id].NbrVisits, td )*td*m_Cells[cell_id].Feature*(*itr).second;

////        cout << "D:" << delta_weight << endl;
//        m_Cells[cell_id].Weight += delta_weight;
//    }

    updateLabelPercentageVisitedCells();

    if (m_FeatureExtractionModule)
    {
        if (m_FeatureExtractionModule->update( x, u, old_estimate, new_estimate ))
        {
            uint32 new_nbr_cells = m_FeatureExtractionModule->getNbrTotalWeights();

            if (new_nbr_cells > m_NbrCells)
            {
                Cell cell;

                cell.Weight = new_estimate;
                cell.NbrVisits = 0;

                m_Cells.resize( new_nbr_cells, cell );

//                m_NbrVisitedCells += new_nbr_cells-m_NbrCells;
                m_NbrCells = new_nbr_cells;
            }
        }
    }
}


void
LinearFunctionApproximator::clearEligibilityTraces()
{
    m_EligibilityTraces.clear();
}


void
LinearFunctionApproximator::decayEligibilityTraces( Ipseity::Scalar factor )
{
    for( std::map<uint32,Ipseity::Scalar>::iterator itr = m_EligibilityTraces.begin(); itr != m_EligibilityTraces.end(); ++itr )
    {
        (*itr).second *= factor;
    }
}


void
LinearFunctionApproximator::replaceEligibilityTraces( const SensorySignal & x, const Response & u )
{
    if (m_FeatureExtractionModule)
    {
        uint32 cell_id;
//        uint32 feature_id;
//        IpseityPrecisionType norm_factor;

//        m_FeatureExtractionModule->computeFeatures( x, u );
//        norm_factor = 1.0 / m_FeatureExtractionModule->getFeatureFactors();

        for( uint32 i = 0; i < m_FeatureExtractionModule->getNbrActivatedFeatures(); ++i )
        {
            cell_id = m_FeatureExtractionModule->getActivatedWeightId( i );
//            feature_id = m_FeatureExtractionModule->getActivatedFeatureId( i );

            m_EligibilityTraces[cell_id] = 1.0; //m_FeatureExtractionModule->getFeatureFactor( feature_id )*norm_factor;

            if (m_Cells[cell_id].NbrVisits == 0)
            {
                ++m_NbrVisitedCells;
            }
            ++m_Cells[cell_id].NbrVisits;
        }
    }
}


void
LinearFunctionApproximator::accumulateEligibilityTraces( const SensorySignal & x, const Response & u )
{
    if (m_FeatureExtractionModule)
    {
        uint32 cell_id;
//        uint32 feature_id;
//        IpseityPrecisionType norm_factor;

//        m_FeatureExtractionModule->computeFeatures( x, u );
//        norm_factor = 1.0 / m_FeatureExtractionModule->getFeatureFactors();

        for( uint32 i = 0; i < m_FeatureExtractionModule->getNbrActivatedFeatures(); ++i )
        {
            cell_id = m_FeatureExtractionModule->getActivatedWeightId( i );
//            feature_id = m_FeatureExtractionModule->getActivatedFeatureId( i );

            if (m_EligibilityTraces.find( cell_id ) != m_EligibilityTraces.end())
            {
                m_EligibilityTraces[cell_id] += 1.0; //m_FeatureExtractionModule->getFeatureFactor( feature_id )*norm_factor;
            }
            else
            {
                m_EligibilityTraces[cell_id] = 1.0; //m_FeatureExtractionModule->getFeatureFactor( feature_id )*norm_factor;
            }

            if (m_Cells[cell_id].NbrVisits == 0)
            {
                ++m_NbrVisitedCells;
            }
            ++m_Cells[cell_id].NbrVisits;
        }
    }
}


bool
LinearFunctionApproximator::load( FILE* file )
{
    if (m_FeatureExtractionModule && m_FeatureExtractionModule->load( file ))
    {
        uint32 nbr_cells;

        fread( &nbr_cells, sizeof( uint32 ), 1, file );

        if (nbr_cells == m_NbrCells)
        {
            m_NbrVisitedCells = 0;
            for( uint32 i = 0; i < nbr_cells; ++i )
            {
                fread( &m_Cells[i].Weight, sizeof( Ipseity::Scalar ), 1, file );
                fread( &m_Cells[i].NbrVisits, sizeof( uint32 ), 1, file );

                if (m_Cells[i].NbrVisits > 0)
                {
                    ++m_NbrVisitedCells;
                }
            }

            return true;
        }
        else
        {
            cout << "The size of the memory does not match!" << endl;

            return false;
        }
    }
    else
    {
        return false;
    }
}


bool
LinearFunctionApproximator::save( FILE* file )
{
    if (m_FeatureExtractionModule)
    {
        m_FeatureExtractionModule->save( file );
    }

    fwrite( &m_NbrCells, sizeof( uint32 ), 1, file );
    for( uint32 i = 0; i < m_NbrCells; ++i )
    {
        fwrite( &m_Cells[i].Weight, sizeof( Ipseity::Scalar ), 1, file );
        fwrite( &m_Cells[i].NbrVisits, sizeof( uint32 ), 1, file );
    }

    return true;
}
