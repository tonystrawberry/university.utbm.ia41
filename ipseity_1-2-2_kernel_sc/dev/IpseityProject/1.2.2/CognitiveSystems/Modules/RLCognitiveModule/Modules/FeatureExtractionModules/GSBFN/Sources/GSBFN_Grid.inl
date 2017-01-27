#include "GSBFN_Grid.h"


template <class TPrecision>
GSBFN_GridDimension<TPrecision>::GSBFN_GridDimension( uint32 var_id, VectorVariableProperty<TPrecision>* var_property, uint32 count, TPrecision width )
{
    VariableId = var_id;
    VariableProperty = var_property;
    Count = count;

    m_Min = VariableProperty->Min;
    m_Step = (VariableProperty->Max-VariableProperty->Min)/(count+1);	// divide in (count+1) intervals

    if (width > 0)
    {
        Width = width;
    }
    else
    {
        Width = m_Step;
    }
}


template <class TPrecision>
TPrecision
GSBFN_GridDimension<TPrecision>::getCenter( uint32 index ) const
{
    return m_Min + m_Step * ((TPrecision) (index+1));
}


template <class TPrecision>
TPrecision
GSBFN_GridDimension<TPrecision>::getWidth() const
{
    return Width;
}


template <class TPrecision>
bool
GSBFN_GridDimension<TPrecision>::load( FILE* file )
{
    fread( &Count, sizeof( uint32 ), 1, file );

    return true;
}


template <class TPrecision>
bool
GSBFN_GridDimension<TPrecision>::save( FILE* file )
{
    fwrite( &Count, sizeof( uint32 ), 1, file );

    return true;
}


//*****************************************************************************


template <class TPrecision>
GSBFN_Grid<TPrecision>::GSBFN_Grid() :
    m_NbrFunctions( 1 )
{
}


template <class TPrecision>
uint32
GSBFN_Grid<TPrecision>::getNbrFunctions() const
{
    return m_NbrFunctions;
}


template <class TPrecision>
bool
GSBFN_Grid<TPrecision>::addDimension( GSBFN_GridDimension<TPrecision> dim )
{
    m_Dimensions.push_back( dim );
    m_NbrFunctions *= dim.Count;

    return true;
}


template <class TPrecision>
void
GSBFN_Grid<TPrecision>::prepare()
{
    Vector<TPrecision> center;
    Vector<TPrecision> radius;
    Vector<TPrecision> inverse_radius;
    uint32 nbr_dimensions = m_Dimensions.size();
    uint32* tab_indices;
    uint32 i;

    tab_indices = new uint32 [nbr_dimensions];
    memset( tab_indices, 0, sizeof( uint32 )*nbr_dimensions );

    center.setDimension( nbr_dimensions );
    radius.setDimension( nbr_dimensions );
    inverse_radius.setDimension( nbr_dimensions );

    cout << m_NbrFunctions << " basis functions" << endl;

    m_ActivatedFeatureIds.resize( m_NbrFunctions );
    m_FeatureFactors.resize( m_NbrFunctions );

    for( uint32 k = 0; k < m_NbrFunctions; ++k )
    {
//		cout << k << ": ";
        for( i = 0; i < nbr_dimensions; ++i )
        {
//			cout << tab_indices[i] << ",";
            center[i] = m_Dimensions[i].getCenter( tab_indices[i] );
            radius[i] = m_Dimensions[i].getWidth()*m_Dimensions[i].getWidth();
            inverse_radius[i] = 1.0 / (2.0*radius[i]);
        }
//		cout << endl;

        m_Centers.push_back( center );
        m_Radius.push_back( radius );
        m_InverseRadius.push_back( inverse_radius );

//		cout << k << ": " << center << endl;

        i = 0;
        do
        {
            ++tab_indices[i];
            if (tab_indices[i] < m_Dimensions[i].Count)
            {
                break;
            }
            else
            {
                tab_indices[i] = 0;
                ++i;
            }
        }
        while( i < nbr_dimensions );
    }
}


template <class TPrecision>
void
GSBFN_Grid<TPrecision>::addFunction( const Vector<TPrecision> & s )
{
}


template <class TPrecision>
TPrecision
GSBFN_Grid<TPrecision>::query( const Vector<TPrecision> & s )
{
    TPrecision term, sum, f;
    TPrecision max_a;
    uint32 nbr_dimensions = m_Dimensions.size();
    uint32 i;

    m_NbrActivatedFeatures = 0;
    m_FeatureFactorsSum = 0.0;
    max_a = 0.0;
    for( uint32 k = 0; k < m_NbrFunctions; ++k )
    {
        sum = 0.0;
        for( i = 0; i < nbr_dimensions; ++i )
        {
            term = s[m_Dimensions[i].VariableId]-m_Centers[k][i];
            term *= term;

            if (term > m_Radius[k][i])
            {
                break;
            }
            else
            {
                term *= m_InverseRadius[k][i];
                sum -= term;
            }
        }

        if (i == nbr_dimensions)
        {
            f = exp( sum );

            if (f > max_a)
            {
                max_a = f;
            }

            m_FeatureFactors[k] = f;
            m_FeatureFactorsSum += f;

            m_ActivatedFeatureIds[m_NbrActivatedFeatures] = k;
            ++m_NbrActivatedFeatures;
        }
        else
        {
            m_FeatureFactors[k] = 0.0;
        }
    }

    return max_a;

//	cout << m_NbrActivatedFeatures << " activated functions" << endl;
}


template <class TPrecision>
uint32
GSBFN_Grid<TPrecision>::getNbrActivatedFeatures() const
{
    return m_NbrActivatedFeatures;
}


template <class TPrecision>
uint32
GSBFN_Grid<TPrecision>::getActivatedFeatureId( uint32 feature_nr ) const
{
    return m_ActivatedFeatureIds[feature_nr];
}


template <class TPrecision>
TPrecision
GSBFN_Grid<TPrecision>::getFeatureFactor( uint32 index ) const
{
    return m_FeatureFactors[index];
}


template <class TPrecision>
TPrecision
GSBFN_Grid<TPrecision>::getFeatureFactors() const
{
    return m_FeatureFactorsSum;
}


template <class TPrecision>
bool
GSBFN_Grid<TPrecision>::load( FILE* file )
{
    for( uint32 k = 0; k < m_Dimensions.size(); k++ )
    {
        m_Dimensions[k].load( file );
    }

    return true;
}


template <class TPrecision>
bool
GSBFN_Grid<TPrecision>::save( FILE* file )
{
    for( uint32 k = 0; k < m_Dimensions.size(); k++ )
    {
        m_Dimensions[k].save( file );
    }

    return true;
}
