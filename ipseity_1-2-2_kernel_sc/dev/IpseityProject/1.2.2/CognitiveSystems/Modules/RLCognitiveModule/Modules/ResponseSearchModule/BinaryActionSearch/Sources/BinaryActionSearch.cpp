#include "BinaryActionSearch.h"
#include "AL/AbstractQFunction.h"
#include "XMLTags_BinaryActionSearch.h"
#include "AL/RLCognitiveSystem/RLTypes.h"
#include "Math/RandomNumberGenerator.h"
#include "IO/XMLScanner.h"
#include <stdio.h>
#include <math.h>


BinaryActionSearch::BinaryActionSearch() :
	m_Policy( NULL )
{
    RandomNumberGenerator::initSeed( 0 );
}


BinaryActionSearch::~BinaryActionSearch()
{
}


const char*
BinaryActionSearch::getName() const
{
    return "BinaryActionSearch";
}


const char*
BinaryActionSearch::getDescription() const
{
    return "";
}


const char*
BinaryActionSearch::getAuthors() const
{
    return "Fabrice Lauri (fabrice.lauri@utbm.fr) & Jiawei Zhu";
}


void
BinaryActionSearch::attachToWidget( QWidget* )
{
}


void
BinaryActionSearch::setResponseDescription( const ResponseProperties & rd )
{
    m_ResponseDescription = rd;
}


ResponseProperties*
BinaryActionSearch::getDecisionDescription()
{
    return &m_BinaryDecisionDescription;
}


const Response &
BinaryActionSearch::selectBestResponse( const SensoryData & stimulus )
{
    if (m_Policy)
    {
        IpseityPrecisionType delta;

        m_Response[0] = (m_ResponseMax[0] + m_ResponseMin[0]) / 2.0;
        delta = m_Delta0;

        for( uint32 i = 0; i < m_NbrDecisions; ++i )
        {
            m_BinaryDecisionSequence.setTemporaryResponse( i, m_Response );
            m_BinaryDecisionSequence.setDecision( i, m_Policy->selectBestResponse( stimulus.concat( m_Response ) ) );
			delta *= 0.5;   // divide by 2
			m_Response[0] += m_BinaryDecisionSequence.getDecision( i )[0] * delta;
        }
    }

    return m_Response;
}


const Response &
BinaryActionSearch::selectRandomResponse( const SensoryData & stimulus )
{
    if (m_Policy)
    {
        IpseityPrecisionType delta;

        m_Response[0] = (m_ResponseMax[0] + m_ResponseMin[0]) / 2.0;
        delta = m_Delta0;

        for( uint32 i = 0; i < m_NbrDecisions; ++i )
        {
            m_BinaryDecisionSequence.setTemporaryResponse( i, m_Response );
            m_BinaryDecisionSequence.setDecision( i, m_Policy->selectRandomResponse( stimulus.concat( m_Response ) ) );
			delta *= 0.5;   // divide by 2
			m_Response[0] += m_BinaryDecisionSequence.getDecision( i )[0] * delta;
        }
    }

    return m_Response;
}


bool
BinaryActionSearch::init( const char *file_name, const char *base_path )
{
    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        return false;
    }

    XMLScanner::Token t;

    uint32 nbr_decisions = 8;

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
                    case TAG_RESPONSE_SEARCH_MODULE:
                        if ( t.keyIs( "Resolution" ))
                        {
                            nbr_decisions = t.intValue();
                        }
                        break;
                }
                break;
        }
    }

    m_BinaryDecisionDescription.setName( "BinaryDecision" );
    m_BinaryDecisionDescription.setDimension( 1 );

    ComponentProperty cp;

    cp.setRange( -1.0, 1.0 );
    cp.setInterval( 2.0 );

    m_BinaryDecisionDescription.setProperty( 0, cp );

    m_Response.setDimension( m_ResponseDescription.getDimension() );
    m_ResponseMin.setDimension( m_ResponseDescription.getDimension() );
    m_ResponseMax.setDimension( m_ResponseDescription.getDimension() );

    m_ResponseMin[0] = m_ResponseDescription[0].Min;
    m_ResponseMax[0] = m_ResponseDescription[0].Max;

    setNbrDecisions( nbr_decisions );
    cout << "Resolution bits: " << m_NbrDecisions << endl;

    m_BinaryDecisionSequence.setSize( m_NbrDecisions, m_ResponseDescription.getDimension() );
}


void
BinaryActionSearch::setPolicy( AbstractPolicy* policy )
{
    m_Policy = policy;
}


uint32
BinaryActionSearch::getNbrDecisions() const
{
    return m_NbrDecisions;
}


void
BinaryActionSearch::setNbrDecisions( uint32 nbr_decisions )
{
    m_NbrDecisions = nbr_decisions;

    m_Delta0 = (m_ResponseMax[0] - m_ResponseMin[0]);
    m_Delta0 *= pow( 2.0, m_NbrDecisions - 1 );
    m_Delta0 /= pow( 2.0, m_NbrDecisions ) - 1;
}


const BinaryDecisionSequence &
BinaryActionSearch::getBinaryDecisionSequence() const
{
    return m_BinaryDecisionSequence;
}
