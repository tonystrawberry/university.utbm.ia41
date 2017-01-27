#include "GSBFN.h"


#include "Math/RandomNumberGenerator.h"
#include "IO/XMLScanner.h"
#include "XMLTags_GSBFN.h"


//*****************************************************************************


GSBFN::GSBFN() :
    m_StimulusDescription( NULL ),
    m_ResponseDescription( NULL ),
    m_DataDescription( NULL ),
    m_EMax( 0.031622 ),     // e^2 = 0.001
    m_AMin( 0.37 )          // e^-1
{
    RandomNumberGenerator::initSeed( 0 );
}

GSBFN::~GSBFN()
{
}


void
GSBFN::setDataDescription( SensoryProperties* stimulus_description, ResponseProperties* response_description, AgentDataProperties* data_description )
{
    m_StimulusDescription = stimulus_description;
    m_ResponseDescription = response_description;
    m_DataDescription = data_description;
}


const char*
GSBFN::getName() const
{
    return "GSBFN";
}


const char*
GSBFN::getDescription() const
{
    return "Gaussian Softmax Basis Function Network";
}


const char*
GSBFN::getAuthors() const
{
    return "Fabrice Lauri (fabrice.lauri@utbm.fr)";
}


void
GSBFN::attachToWidget( QWidget* )
{
}


bool
GSBFN::init( const char* file_name, const char* base_path )
{
    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        return false;
    }

    XMLScanner::Token t;
    lf::String name;
    uint32 count;
    IpseityPrecisionType width;
    uint32 var_id;
    GSBFN_Grid<IpseityPrecisionType>* grid;
    bool def_var = false;

    cout << "Loading GSBFN parameters from '" << base_path << file_name << "'..." << endl;

    m_ExtractFromStimulusOnly = true;

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

                    case TAG_GRID:
                    {
                        grid = new GSBFN_Grid<IpseityPrecisionType>();
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
                            count = 1;
                            width = 0.0;
                        }
                        else if (t.keyIs( "Count" ))
                        {
                            count = t.intValue();
                        }
                        else if (t.keyIs( "Width" ))
                        {
                            width = t.floatValue();
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
                            if (m_DataDescription && m_DataDescription->queryFromName( name.c_str(), var_id ))
                            {
                                GSBFN_GridDimension<IpseityPrecisionType> dim( var_id, (VectorVariableProperty<IpseityPrecisionType>*) &(*m_DataDescription)[var_id], count, width );

                                grid->addDimension( dim );

                                if (var_id >= m_StimulusDescription->getDimension())
                                {
                                    m_ExtractFromStimulusOnly = false;
                                }
                            }
                        }
                        break;

                    case TAG_GRID:
                        grid->prepare();
                        m_Grids.push_back( grid );
                        break;
                }
                break;
        }
    }

    prepare();

    return true;
}


void
GSBFN::computeFeatures( const SensoryData & x, const Response & u )
{
    IpseityPrecisionType max_a;

    if (extractFromStimulusOnly())
    {
        m_ResponseIndex = m_ResponseDescription->getIndex( u )*m_NbrTotalFeatures;

        m_MaxA = 0.0;
        for( uint32 i = 0; i < m_Grids.size(); i++ )
        {
            max_a = m_Grids[i]->query( x );

            if (max_a > m_MaxA)
            {
                m_MaxA = max_a;
            }
        }
    }
    else
    {
        m_ResponseIndex = 0;

        m_SR = x.concat( u );

        m_MaxA = 0.0;
        for( uint32 i = 0; i < m_Grids.size(); i++ )
        {
            max_a = m_Grids[i]->query( m_SR );

            if (max_a > m_MaxA)
            {
                m_MaxA = max_a;
            }
        }
    }
}


uint32
GSBFN::getNbrTotalCells() const
{
    return m_NbrCells;
}

uint32
GSBFN::getNbrTotalFeatures() const
{
    return m_NbrTotalFeatures;
}

uint32
GSBFN::getNbrActivatedFeatures() const
{
    uint32 count = 0;

    for (uint32 i = 0; i < m_Grids.size(); ++i )
    {
        count += m_Grids[i]->getNbrActivatedFeatures();
    }

    return count;
}

uint32
GSBFN::getActivatedCellId( uint32 feature_nr ) const
{
    return m_ResponseIndex+m_Grids[0]->getActivatedFeatureId( feature_nr );
}

uint32
GSBFN::getActivatedFeatureId( uint32 feature_nr ) const
{
    return m_Grids[0]->getActivatedFeatureId( feature_nr );
}


IpseityPrecisionType
GSBFN::getFeatureFactor( uint32 feature_id ) const
{
    return m_Grids[0]->getFeatureFactor ( feature_id );
}


IpseityPrecisionType
GSBFN::getFeatureFactors() const
{
    IpseityPrecisionType sum = 0.0;

    for (uint32 i = 0; i < m_Grids.size(); ++i )
    {
        sum += m_Grids[i]->getFeatureFactors();
    }

    return sum;
}


bool
GSBFN::update( const SensoryData & x, const Response & u, IpseityPrecisionType old_estimate, IpseityPrecisionType new_estimate )
{
    IpseityPrecisionType td = new_estimate-old_estimate;

    if (fabs(td) > m_EMax && m_MaxA < m_AMin)
    {
        cout << "add new f" << endl;
    }

    return false;
}


bool
GSBFN::load( FILE* file )
{
    return true;
}

bool
GSBFN::save( FILE* file )
{
    return true;
}


//*******************************************************************
// Protected functions
//*******************************************************************


bool
GSBFN::extractFromStimulusOnly() const
{
    return m_ExtractFromStimulusOnly;
}


void
GSBFN::prepare()
{
    m_NbrTotalFeatures = 0;
    for( uint32 i = 0; i < m_Grids.size(); i++ )
    {
        m_NbrTotalFeatures += m_Grids[i]->getNbrFunctions();
    }

    if (extractFromStimulusOnly())
    {
        Cardinality c;

        c = m_ResponseDescription->getCardinality();

        if (c.Type == CARDINALITY_FINITE)
        {
            cout << "Response Space Cardinality: " << c.Cardinal << endl;

            m_NbrCells = m_NbrTotalFeatures*c.Cardinal;
        }
        else
        {
            cout << "Infinite Response Space!" << endl;

            m_NbrCells = m_NbrTotalFeatures;
        }
    }
    else
    {
        m_NbrCells = m_NbrTotalFeatures;
    }
}
