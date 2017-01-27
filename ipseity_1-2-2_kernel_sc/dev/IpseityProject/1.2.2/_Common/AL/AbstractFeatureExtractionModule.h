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


#ifndef AbstractFeatureExtractionModule_h
#define AbstractFeatureExtractionModule_h


#include "AL/IpseityTypes.h"
#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "IO/AbstractPlugin.h"


class AbstractFeatureExtractionModule : public AbstractPlugin
{
    public:
        virtual void setDataDescription( SensorySignalProperties* stimulus_description, ResponseSignalProperties* response_description, AgentDataProperties* data_description ) = 0;
        virtual bool init( const char*, const char* ) = 0;
        virtual void computeFeatures( const SensorySignal &, const Response & ) = 0;
        virtual uint32 getNbrTotalWeights() const = 0;
        virtual uint32 getNbrTotalFeatures() const = 0;
        virtual uint32 getNbrActivatedFeatures() const = 0;
        virtual uint32 getActivatedWeightId( uint32 feature_nr ) const = 0;
        virtual uint32 getActivatedFeatureId( uint32 feature_nr ) const = 0;
        virtual Ipseity::Scalar getFeature( uint32 feature_id ) const = 0;
        virtual Ipseity::Scalar getNormalizedFeature( uint32 feature_id ) const = 0;

        virtual bool update( const SensorySignal & x, const Response & u, Ipseity::Scalar old_estimate, Ipseity::Scalar new_estimate ) = 0;

        virtual bool load( FILE* ) = 0;
        virtual bool save( FILE* ) = 0;
};


#define INTERFACE_ABSTRACT_FEATURE_EXTRACTION_MODULE \
    void setDataDescription( SensoryProperties* stimulus_description, ResponseProperties* response_description, AgentDataProperties* data_description ); \
    bool init( const char*, const char* ); \
    void computeFeatures( const SensoryData &, const Response & ); \
    uint32 getNbrTotalCells() const; \
    uint32 getNbrTotalFeatures() const; \
    uint32 getNbrActivatedFeatures() const; \
    uint32 getActivatedCellId( uint32 feature_nr ) const; \
    uint32 getActivatedFeatureId( uint32 feature_nr ) const; \
    IpseityPrecisionType getFeature( uint32 id ) const; \
    IpseityPrecisionType getNormalizedFeature( uint32 id ) const; \
    bool update( const SensoryData & x, const Response & u, IpseityPrecisionType old_estimate, IpseityPrecisionType new_estimate ); \
    bool load( FILE* ); \
    bool save( FILE* ); \


#endif
