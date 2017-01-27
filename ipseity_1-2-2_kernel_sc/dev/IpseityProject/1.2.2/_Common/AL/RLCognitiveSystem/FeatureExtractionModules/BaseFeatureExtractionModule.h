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


#ifndef BaseFeatureExtractionModule_h
#define BaseFeatureExtractionModule_h


#include "AL/AbstractFeatureExtractionModule.h"


#include <vector>


class BaseFeatureExtractionModule : public AbstractFeatureExtractionModule
{
    public:
        BaseFeatureExtractionModule();
        ~BaseFeatureExtractionModule();

        void setDataDescription( SensorySignalProperties* stimulus_description, ResponseSignalProperties* response_description, AgentDataProperties* data_description );

        uint32 getNbrTotalWeights() const;
        uint32 getNbrTotalFeatures() const;
        uint32 getNbrActivatedFeatures() const;
        uint32 getActivatedWeightId( uint32 feature_nr ) const;
        uint32 getActivatedFeatureId( uint32 feature_nr ) const;

        Ipseity::Scalar getNormalizedFeature( uint32 id ) const;

        void computeFeatures( const SensorySignal &, const Response & );

    protected:
        virtual void prepareFeatures( const SensorySignal &, const Response & ) = 0;
        bool extractFromStimulusOnly() const;

    protected:
        SensorySignalProperties* m_StimulusDescription;
        ResponseSignalProperties* m_ResponseDescription;
        AgentDataProperties* m_DataDescription;

        bool m_ExtractFromStimulusOnly;
        uint32 m_NbrTotalFeatures;
        std::vector<Ipseity::Scalar> m_Features;
        Ipseity::Scalar m_Norm;

        uint32 m_NbrWeights;
        uint32 m_NbrActivatedFeatures;
        std::vector<uint32> m_ActivatedWeightIds;
        std::vector<uint32> m_ActivatedFeatureIds;
};


#define INTERFACE_BASE_FEATURE_EXTRACTION_MODULE \
    bool init( const char*, const char* ); \
    void prepareFeatures( const SensorySignal &, const Response & ); \
    Ipseity::Scalar getFeature( uint32 id ) const; \
    bool update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate ); \
    bool load( FILE* ); \
    bool save( FILE* ); \


#endif
