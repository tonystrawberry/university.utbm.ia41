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


#ifndef PerformanceStats_h
#define PerformanceStats_h


#include "IpseityString.h"
#include "AL/IpseityTypes.h"

#include <map>
#include <list>
#include <ostream>


class ScenarioInfos
{
    public:
        ScenarioInfos();

        void reset();

        void load( FILE* );
        void save( FILE* ) const;

        friend std::ostream & operator<<( std::ostream & os, const ScenarioInfos & );


    public:
        uint32 Count;
        uint32 Idleness;
        uint32 NbrSuccesses;
        std::list<bool> Successes;
        uint32 MinNbrDecisions;
        uint32 CumulatedNbrDecisions;
        std::list<uint32> NbrDecisions;
        Ipseity::Scalar BestDiscountedReturn;
        Ipseity::Scalar CumulatedDiscountedReturns;
        std::list<Ipseity::Scalar> DiscountedReturns;
};


typedef union
{
    struct {
        uint32	Learning : 1,
                Succeeded : 1;
    };
    uint32 Value;
} EpisodeStatus;

class EpisodeInfos
{
    public:
        EpisodeInfos();

        void load( FILE* );
        void save( FILE* ) const;

        friend std::ostream & operator<<( std::ostream & os, const EpisodeInfos & );


    public:
        uint64 ScenarioUid;
        EpisodeStatus Status;
        Ipseity::Scalar DiscountedReturn;
        uint32 NbrDecisions;
};


class PerformanceStats
{
    public:
        PerformanceStats();

        void init( const std::map<uint64,Ipseity::String>* );

        uint32 getExperimentId() const;
        uint64 getNbrTotalDecisions() const;
        uint32 getNbrTotalEpisodes() const;

        void addScenario( uint64 scenario_uid );

        void startEpisode( uint64 scenario_uid );
        void makeDecision();
        void endEpisode( bool is_success, uint32 nbr_decisions, Ipseity::Scalar discounted_return );

        uint32 getMinNbrEpisodes() const;
        float64 getTotalSuccessPercentage() const;
        float64 getSuccessPercentage( uint32 scenario_id ) const;
        float64 getMinSuccessPercentage() const;

        void clear();
        void reset();

        bool load( const char* base_name, const char* base_path );
        bool save( const char* base_name, const char* base_path );

        void load( FILE* file );
        void save( FILE* file );

        void generateFigures( const char* base_name, const char* base_path );

        friend std::ostream & operator<<( std::ostream & os, PerformanceStats & );

    protected:
        void incIdlenesses();

        uint32 getMaxIdleness() const;

        const char* getExperimentIdString();


    protected:
        uint32 m_ExperimentId;
        uint64 m_NbrTotalDecisions;
        uint32 m_NbrTotalEpisodes;
        uint32 m_NbrSucceededEpisodes;
        std::list<EpisodeInfos> m_EpisodesInfos;
        std::map<uint64,Ipseity::String>* m_Scenarii;
        std::map<uint64,ScenarioInfos> m_ScenariiInfos;

    private:
        bool m_Updated;
        bool m_ScenarioStarted;
        uint64 m_ScenarioUid;
        char m_ExperimentIdString[32];
};


#endif
