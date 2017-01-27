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


#include "PerformanceStats.h"


#include "IpseityString.h"
#include "IO/GnuplotInterface.h"

#include <iostream>
#include <fstream>

#include <QDir>


using namespace std;


ScenarioInfos::ScenarioInfos()
{
    Count = 0;
    Idleness = 0;
    NbrSuccesses = 0;
    MinNbrDecisions = UINT32_MAX_VALUE;
    CumulatedNbrDecisions = 0;
    BestDiscountedReturn = FLOAT64_MIN_VALUE;
    CumulatedDiscountedReturns = 0.0;
}


void
ScenarioInfos::reset()
{
    Count = 0;
    Idleness = 0;
    NbrSuccesses = 0;
    Successes.clear();
    MinNbrDecisions = UINT32_MAX_VALUE;
    CumulatedNbrDecisions = 0;
    NbrDecisions.clear();
    BestDiscountedReturn = FLOAT64_MIN_VALUE;
    CumulatedDiscountedReturns = 0.0;
    DiscountedReturns.clear();
}


void
ScenarioInfos::load( FILE* file )
{
    bool success;
    uint32 nbr_decisions;
    Ipseity::Scalar discounted_return;

    fread( &Count, sizeof( uint32 ), 1, file );
    fread( &Idleness, sizeof( uint32 ), 1, file );

    // Read the successes
    fread( &NbrSuccesses, sizeof( uint32 ), 1, file );

    Successes.clear();
    for( uint32 i = 0; i < Count; ++i )
    {
        fread( &success, sizeof( bool ), 1, file );
        Successes.push_back( success );
    }

    // Read the set of decisions
    fread( &MinNbrDecisions, sizeof( uint32 ), 1, file );
    fread( &CumulatedNbrDecisions, sizeof( uint32 ), 1, file );

    NbrDecisions.clear();
    for( uint32 i = 0; i < Count; ++i )
    {
        fread( &nbr_decisions, sizeof( uint32 ), 1, file );
        NbrDecisions.push_back( nbr_decisions );
    }

    // Read the discounted returns
    fread( &BestDiscountedReturn, sizeof( Ipseity::Scalar ), 1, file );
    fread( &CumulatedDiscountedReturns, sizeof( Ipseity::Scalar ), 1, file );

    DiscountedReturns.clear();
    for( uint32 i = 0; i < Count; ++i )
    {
        fread( &discounted_return, sizeof( Ipseity::Scalar ), 1, file );
        DiscountedReturns.push_back( discounted_return );
    }
}


void
ScenarioInfos::save( FILE* file ) const
{
    std::list<bool>::const_iterator itr_s;
    std::list<uint32>::const_iterator itr_n;
    std::list<Ipseity::Scalar>::const_iterator itr_r;

    fwrite( &Count, sizeof( uint32 ), 1, file );
    fwrite( &Idleness, sizeof( uint32 ), 1, file );

    fwrite( &NbrSuccesses, sizeof( uint32 ), 1, file );

    for( itr_s = Successes.begin(); itr_s != Successes.end(); ++itr_s )
    {
        fwrite( &(*itr_s), sizeof( bool ), 1, file );
    }

    fwrite( &MinNbrDecisions, sizeof( uint32 ), 1, file );
    fwrite( &CumulatedNbrDecisions, sizeof( uint32 ), 1, file );

    for( itr_n = NbrDecisions.begin(); itr_n != NbrDecisions.end(); ++itr_n )
    {
        fwrite( &(*itr_n), sizeof( uint32 ), 1, file );
    }

    fwrite( &BestDiscountedReturn, sizeof( Ipseity::Scalar ), 1, file );
    fwrite( &CumulatedDiscountedReturns, sizeof( Ipseity::Scalar ), 1, file );

    for( itr_r = DiscountedReturns.begin(); itr_r != DiscountedReturns.end(); ++itr_r )
    {
        fwrite( &(*itr_r), sizeof( Ipseity::Scalar ), 1, file );
    }
}


std::ostream & operator<<( std::ostream & os, const ScenarioInfos & si )
{
    os << si.Count;
    os << " ";
    os << si.NbrSuccesses;
    os << " ";
    if (si.Count)
    {
        os << ((float64) si.NbrSuccesses) / si.Count;
    }
    else
    {
        os << "0";
    }
    os << " ";
    os << si.MinNbrDecisions;
    os << " ";
    if (si.Count)
    {
        os << si.CumulatedNbrDecisions / si.Count;
    }
    else
    {
        os << "0";
    }
    os << " ";
    os << si.BestDiscountedReturn;
    os << " ";
    if (si.Count)
    {
        os << si.CumulatedDiscountedReturns / si.Count;
    }
    else
    {
        os << "0.0";
    }

    return os;
}


//*****************************************************************************


EpisodeInfos::EpisodeInfos()
{
    Status.Value = 0;
}


void
EpisodeInfos::load( FILE* file )
{
    fread( &ScenarioUid, sizeof( uint64 ), 1, file );
    fread( &Status, sizeof( EpisodeStatus ), 1, file );
    fread( &DiscountedReturn, sizeof( Ipseity::Scalar ), 1, file );
    fread( &NbrDecisions, sizeof( uint32 ), 1, file );
}


void
EpisodeInfos::save( FILE* file ) const
{
    fwrite( &ScenarioUid, sizeof( uint64 ), 1, file );
    fwrite( &Status, sizeof( EpisodeStatus ), 1, file );
    fwrite( &DiscountedReturn, sizeof( Ipseity::Scalar ), 1, file );
    fwrite( &NbrDecisions, sizeof( uint32 ), 1, file );
}


std::ostream & operator<<( std::ostream & os, const EpisodeInfos & si )
{
    os << si.ScenarioUid;
    os << " ";
    os << si.DiscountedReturn;
    os << " ";
    os << si.NbrDecisions;
    os << " ";
    if (si.Status.Succeeded)
    {
        os << "1";
    }
    else
    {
        os << "0";
    }
    os << " ";
    if (si.Status.Learning)
    {
        os << "1";
    }
    else
    {
        os << "0";
    }

    return os;
}


//*****************************************************************************


PerformanceStats::PerformanceStats() :
    m_ExperimentId( 1 ),
    m_NbrTotalDecisions( 0 ),
    m_NbrTotalEpisodes( 0 ),
    m_NbrSucceededEpisodes( 0 ),
    m_Updated( false ),
    m_ScenarioStarted( false )
{
}


uint32
PerformanceStats::getExperimentId() const
{
    return m_ExperimentId;
}


uint64
PerformanceStats::getNbrTotalDecisions() const
{
    return m_NbrTotalDecisions;
}

uint32
PerformanceStats::getNbrTotalEpisodes() const
{
    return m_NbrTotalEpisodes;
}


void
PerformanceStats::addScenario( uint64 scenario_uid )
{
    if (m_ScenariiInfos.find( scenario_uid ) == m_ScenariiInfos.end())
    {
        ScenarioInfos si;

        si.Idleness = getMaxIdleness();

        m_ScenariiInfos[scenario_uid] = si;
    }
}


void
PerformanceStats::startEpisode( uint64 scenario_uid )
{
    addScenario( scenario_uid );

    m_ScenarioStarted = true;
    m_ScenarioUid = scenario_uid;
}


void
PerformanceStats::makeDecision()
{
    m_Updated = true;
    ++m_NbrTotalDecisions;
}


void
PerformanceStats::endEpisode( bool is_success, uint32 nbr_decisions, Ipseity::Scalar discounted_return )
{
    if (m_ScenarioStarted && m_ScenariiInfos.find( m_ScenarioUid ) != m_ScenariiInfos.end())
    {
        m_Updated = true;
        ++m_NbrTotalEpisodes;
        ++m_ScenariiInfos[m_ScenarioUid].Count;

        EpisodeInfos ei;

        ei.ScenarioUid = m_ScenarioUid;
        ei.Status.Learning = false;
        ei.Status.Succeeded = is_success;
        ei.DiscountedReturn = discounted_return;
        ei.NbrDecisions = nbr_decisions;

        qDebug() << "Episode:" << m_NbrTotalEpisodes << ei.Status.Learning << ei.Status.Succeeded;

        m_EpisodesInfos.push_back( ei );

        if (is_success)
        {
            ++m_NbrSucceededEpisodes;
            ++m_ScenariiInfos[m_ScenarioUid].NbrSuccesses;

            if (nbr_decisions < m_ScenariiInfos[m_ScenarioUid].MinNbrDecisions)
            {
                m_ScenariiInfos[m_ScenarioUid].MinNbrDecisions = nbr_decisions;
            }

            if (discounted_return > m_ScenariiInfos[m_ScenarioUid].BestDiscountedReturn)
            {
                m_ScenariiInfos[m_ScenarioUid].BestDiscountedReturn = discounted_return;
            }
        }

        m_ScenariiInfos[m_ScenarioUid].Successes.push_back( is_success );

        m_ScenariiInfos[m_ScenarioUid].NbrDecisions.push_back( nbr_decisions );
        m_ScenariiInfos[m_ScenarioUid].CumulatedNbrDecisions += nbr_decisions;

        m_ScenariiInfos[m_ScenarioUid].DiscountedReturns.push_back( discounted_return );
        m_ScenariiInfos[m_ScenarioUid].CumulatedDiscountedReturns += discounted_return;

        incIdlenesses();
        m_ScenariiInfos[m_ScenarioUid].Idleness = 0;

        m_ScenarioStarted = false;
    }
}


uint32
PerformanceStats::getMinNbrEpisodes() const
{
    if (m_ScenariiInfos.size())
    {
        std::map<uint64,ScenarioInfos>::const_iterator itr;
        uint32 nbr, min_ne;

        itr = m_ScenariiInfos.begin();
        min_ne = (*itr).second.Count;

        if (m_ScenariiInfos.size() > 1)
        {
            ++itr;

            for( ; itr != m_ScenariiInfos.end(); ++itr )
            {
                nbr = (*itr).second.Count;
                if (nbr < min_ne)
                {
                    min_ne = nbr;
                }
            }
        }

        return min_ne;
    }
    else
    {
        return 0;
    }
}


float64
PerformanceStats::getTotalSuccessPercentage() const
{
    float64 percentage = 0.0;

    if (m_NbrTotalEpisodes)
    {
        percentage = m_NbrSucceededEpisodes;
        percentage /= m_NbrTotalEpisodes;
    }

    return percentage;
}

float64
PerformanceStats::getSuccessPercentage( uint32 scenario_id ) const
{
    float64 percentage = 0.0;

    if (scenario_id < m_ScenariiInfos.size())
    {
        std::map<uint64,ScenarioInfos>::const_iterator itr;
        uint32 i;

        for( i = 0, itr = m_ScenariiInfos.begin(); i < scenario_id; ++i, ++itr );

        if ((*itr).second.Count)
        {
            percentage = (*itr).second.NbrSuccesses;
            percentage /= (*itr).second.Count;
        }
    }

    return percentage;
}

float64
PerformanceStats::getMinSuccessPercentage() const
{
    float64 perc, minp;

    minp = getSuccessPercentage( 0 );
    for( uint32 i = 1; i < m_ScenariiInfos.size(); ++i )
    {
        perc = getSuccessPercentage( i );
        if (perc < minp)
        {
            minp = perc;
        }
    }

    return minp;
}


void
PerformanceStats::clear()
{
    m_NbrTotalDecisions = 0;
    m_NbrTotalEpisodes = 0;
    m_NbrSucceededEpisodes = 0;
    m_EpisodesInfos.clear();
    m_ScenariiInfos.clear();

    m_Updated = true;
    m_ScenarioStarted = false;
}


void
PerformanceStats::reset()
{
    ++m_ExperimentId;
    m_NbrTotalDecisions = 0;
    m_NbrTotalEpisodes = 0;
    m_NbrSucceededEpisodes = 0;

    m_EpisodesInfos.clear();

    std::map<uint64,ScenarioInfos>::iterator itr;

    for( itr = m_ScenariiInfos.begin(); itr != m_ScenariiInfos.end(); ++itr )
    {
        (*itr).second.reset();
    }

    m_Updated = true;
    m_ScenarioStarted = false;
}


const char*
PerformanceStats::getExperimentIdString()
{
    sprintf( m_ExperimentIdString, "%05d", m_ExperimentId );

    return m_ExperimentIdString;
}


bool
PerformanceStats::load( const char* base_name, const char* base_path )
{
    FILE* file;
    Ipseity::String ln;

//    // Experiment id
//    ln = base_path;
//    ln += "data";

//    if (file = fopen( ln.c_str(), "rb" ))
//    {
//        fread( &m_ExperimentId, sizeof( uint32 ), 1, file );
//        fclose( file );
//    }

    // Data
    ln = base_path;
//    ln += getExperimentIdString();
//    ln += "/";
    ln += base_name;
    ln += ".stats";

    file = fopen( ln.c_str(), "rb" );
    if (file)
    {
        load( file );
        fclose( file );

        return true;
    }
    else
    {
        return false;
    }
}

bool
PerformanceStats::save( const char* base_name, const char* base_path )
{
    if (m_Updated)
    {
        FILE* file;
        Ipseity::String ln;

        QDir().mkpath( base_path );

        ln = base_path;
        ln += base_name;
        ln += ".stats";

        file = fopen( ln.c_str(), "wb" );
        if (file)
        {
            save( file );
            fclose( file );

            return true;
        }
    }
    else
    {
        return false;
    }
}


void
PerformanceStats::load( FILE* file )
{
    fread( &m_NbrTotalDecisions, sizeof( uint64 ), 1, file );
    fread( &m_NbrTotalEpisodes, sizeof( uint32 ), 1, file );
    fread( &m_NbrSucceededEpisodes, sizeof( uint32 ), 1, file );
    fread( &m_ScenarioStarted, sizeof( bool ), 1, file );
    fread( &m_ScenarioUid, sizeof( uint32 ), 1, file );

    uint32 nbr;
    EpisodeInfos ei;

    // Read the episodes infos
    fread( &nbr, sizeof( uint32 ), 1, file );

    m_EpisodesInfos.clear();
    for( uint32 i = 0; i < nbr; ++i )
    {
        ei.load( file );
        m_EpisodesInfos.push_back( ei );
    }

    uint64 scenario_uid;
    ScenarioInfos si;

    // Read the scenarii infos
    fread( &nbr, sizeof( uint32 ), 1, file );

    m_ScenariiInfos.clear();
    for( uint32 i = 0; i < nbr; ++i )
    {
        fread( &scenario_uid, sizeof( uint64 ), 1, file );
        si.load( file );
        m_ScenariiInfos[scenario_uid] = si;
    }

    m_Updated = false;
}


void
PerformanceStats::save( FILE* file )
{
    fwrite( &m_NbrTotalDecisions, sizeof( uint64 ), 1, file );
    fwrite( &m_NbrTotalEpisodes, sizeof( uint32 ), 1, file );
    fwrite( &m_NbrSucceededEpisodes, sizeof( uint32 ), 1, file );
    fwrite( &m_ScenarioStarted, sizeof( bool ), 1, file );
    fwrite( &m_ScenarioUid, sizeof( uint32 ), 1, file );

    uint32 nbr;
    std::list<EpisodeInfos>::const_iterator itr_ei;
    std::map<uint64,ScenarioInfos>::const_iterator itr_si;

    // Write the episodes infos
    nbr = m_EpisodesInfos.size();
    fwrite( &nbr, sizeof( uint32 ), 1, file );
    for( itr_ei = m_EpisodesInfos.begin(); itr_ei != m_EpisodesInfos.end(); ++itr_ei )
    {
        (*itr_ei).save( file );
    }

    // Write the scenarii infos
    nbr = m_ScenariiInfos.size();
    fwrite( &nbr, sizeof( uint32 ), 1, file );
    for( itr_si = m_ScenariiInfos.begin(); itr_si != m_ScenariiInfos.end(); ++itr_si )
    {
        fwrite( &(*itr_si).first, sizeof( uint64 ), 1, file );
        (*itr_si).second.save( file );
    }
}


void
PerformanceStats::generateFigures( const char* base_name, const char* base_path )
{
    if (m_Updated)
    {
        std::ofstream of;
        Ipseity::String path;
        Ipseity::String fname;

        path = base_path;
//        path += getExperimentIdString();
//        path += "/";

        fname = path;
        fname += base_name;
        fname += ".txt";

        of.open( fname.c_str(), std::ios_base::out );
        of << (*this) << endl;
        of.close();


        char* block = "every :::1::1"; // I:J:K:L:M:N with I = Line increment, J = Data block increment, K = first line, L = first data block, M = last line, N = last data block
        char* line_format = "with lines ls 1 notitle";

        fname = base_name;

        GnuplotInterface::create( path.c_str(), fname.c_str() );

        GnuplotInterface::addLine( "set xlabel \"Episodes\"" );

        GnuplotInterface::addFigure( "fig_%s_discounted_rewards", base_name );
        GnuplotInterface::addLine( "set ylabel \"Discounted rewards\"" );
        GnuplotInterface::addLine( "plot '%s.txt' %s using 1:3 %s", fname.c_str(), block, line_format );
        GnuplotInterface::addNewLine();

        GnuplotInterface::addFigure( "fig_%s_success_percentage", base_name );
        GnuplotInterface::addLine( "set ylabel \"%% successes\"" );
        GnuplotInterface::addLine( "plot '%s.txt' %s using 1:8 %s", fname.c_str(), block, line_format );
        GnuplotInterface::addNewLine();

        GnuplotInterface::addFigure( "fig_%s_nbr_decisions", base_name );
        GnuplotInterface::addLine( "set ylabel \"Number of decisions\"" );
        GnuplotInterface::addLine( "plot '%s.txt' %s using 1:4 %s", fname.c_str(), block, line_format );
        GnuplotInterface::addNewLine();

        GnuplotInterface::close();
    }
}


std::ostream &
operator<<( std::ostream & os, PerformanceStats & ps )
{
    os << "# Nbr Total Decisions: " << endl;
    os << ps.m_NbrTotalDecisions;
    os << endl;
    os << "# Nbr Total Succeeded episodes: " << endl;
    os << ps.m_NbrSucceededEpisodes;
    os << endl;
    os << "# Nbr Total Episodes: " << endl;
    os << ps.m_NbrTotalEpisodes;
    os << endl << endl;

    if (ps.m_EpisodesInfos.size())
    {
        std::list<EpisodeInfos>::iterator itr_ei;
        uint32 i;
        uint32 nbr = 0;

        os << "# Episodes:" << endl;
        os << "# EpisodeId EpisodeUid DiscountedReturn NbrDecisions Success LearningEpisode CumulatedSuccess SuccessPercentage";
        os << endl;

        for( i = 0, itr_ei = ps.m_EpisodesInfos.begin(); itr_ei != ps.m_EpisodesInfos.end(); ++i, ++itr_ei )
        {
            os << i+1;
            os << " ";
            os << (*itr_ei);
            os << " ";
            nbr += (*itr_ei).Status.Succeeded;
            os << nbr;
            os << " ";
            os << ((float64) nbr) / (i+1);
            os << endl;
        }
        os << endl;

        std::map<uint64,ScenarioInfos>::iterator itr;
        std::list<uint32>::iterator itr_d;

        os << "# Scenarii infos (" << ps.m_ScenariiInfos.size() << " episode";
        if (ps.m_ScenariiInfos.size() > 1)
        {
            os << "s";
        }
        os << ")" << endl;
        os << "# ScenarioUid Count NbrSuccesses SuccessPercentage MinNbrDecisions AverageNbrDecisions BestDiscountedReturn AverageDiscountedReturn" << endl;

        for( itr = ps.m_ScenariiInfos.begin(); itr != ps.m_ScenariiInfos.end(); ++itr )
        {
            os << (*itr).first;
            os << " ";
            os << (*itr).second;
            os << endl;
        }
    }

    return os;
}


//*****************************************************************************


void
PerformanceStats::incIdlenesses()
{
    std::map<uint64,ScenarioInfos>::iterator itr;

    for( itr = m_ScenariiInfos.begin(); itr != m_ScenariiInfos.end(); ++itr )
    {
        ++(*itr).second.Idleness;
    }
}


uint32
PerformanceStats::getMaxIdleness() const
{
    std::map<uint64,ScenarioInfos>::const_iterator itr;
    uint32 max_idleness = 0;

    for( itr = m_ScenariiInfos.begin(); itr != m_ScenariiInfos.end(); ++itr )
    {
        max_idleness += (*itr).second.Count;
    }

    return max_idleness;
}
