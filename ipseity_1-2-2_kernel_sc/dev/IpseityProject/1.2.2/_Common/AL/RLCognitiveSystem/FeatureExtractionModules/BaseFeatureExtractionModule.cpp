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


#include "BaseFeatureExtractionModule.h"


BaseFeatureExtractionModule::BaseFeatureExtractionModule() :
    m_ExtractFromStimulusOnly( true ),
    m_NbrTotalFeatures( 0 ),
    m_NbrActivatedFeatures( 0 )
{
}


BaseFeatureExtractionModule::~BaseFeatureExtractionModule()
{
}


void
BaseFeatureExtractionModule::setDataDescription( SensorySignalProperties* stimulus_description, ResponseSignalProperties* response_description, AgentDataProperties* data_description )
{
    m_StimulusDescription = stimulus_description;
    m_ResponseDescription = response_description;
    m_DataDescription = data_description;
}


uint32
BaseFeatureExtractionModule::getNbrTotalWeights() const
{
    return m_NbrWeights;
}


uint32
BaseFeatureExtractionModule::getNbrTotalFeatures() const
{
    return m_NbrTotalFeatures;
}


uint32
BaseFeatureExtractionModule::getNbrActivatedFeatures() const
{
    return m_NbrActivatedFeatures;
}


uint32
BaseFeatureExtractionModule::getActivatedWeightId( uint32 i ) const
{
    return m_ActivatedWeightIds[i];
}


uint32
BaseFeatureExtractionModule::getActivatedFeatureId( uint32 i ) const
{
    return m_ActivatedFeatureIds[i];
}


Ipseity::Scalar
BaseFeatureExtractionModule::getNormalizedFeature( uint32 id ) const
{
    return getFeature( id )*m_Norm;
}


void
BaseFeatureExtractionModule::computeFeatures( const SensorySignal & sig, const Response & res )
{
    prepareFeatures( sig, res );

    if (getNbrActivatedFeatures())
    {
        Ipseity::Scalar norm = 0.0;
        for( uint32 i = 0; i < getNbrActivatedFeatures(); ++i )
        {
            norm  += getFeature( getActivatedFeatureId( i ) );
        }
        m_Norm = 1.0 / norm;
    }
    else
    {
        m_Norm = 1.0;
    }
}


//*****************************************************************************


bool
BaseFeatureExtractionModule::extractFromStimulusOnly() const
{
    return m_ExtractFromStimulusOnly;
}
