#ifndef GSBFN_Grid_h
#define GSBFN_Grid_h


#include "Types.h"
#include "Math/Vector.h"

#include <vector>


template <class T>
class GSBFN_GridDimension;

template <class T>
std::ostream & operator<<( std::ostream &, const GSBFN_GridDimension<T> & );


template <class TPrecision>
class GSBFN_GridDimension
{
    public:
        GSBFN_GridDimension( uint32 var_id, VectorVariableProperty<TPrecision>* var_property, uint32 count, TPrecision width );

        TPrecision getCenter( uint32 ) const;
        TPrecision getWidth() const;

        bool load( FILE* );
        bool save( FILE* );

        friend std::ostream & operator<< <>( std::ostream &, const GSBFN_GridDimension<TPrecision> & );


    public:
        uint32 VariableId;
        VectorVariableProperty<TPrecision>* VariableProperty;
        uint32 Count;
        TPrecision Width;

    protected:
        TPrecision m_Min;
        TPrecision m_Step;
};


template <class TPrecision>
class GSBFN_Grid;

template <class TPrecision>
std::ostream & operator<<( std::ostream & os, const GSBFN_Grid<TPrecision> & v );


template <class TPrecision>
class GSBFN_Grid
{
    public:
        GSBFN_Grid();

        uint32 getNbrFunctions() const;

        bool addDimension( GSBFN_GridDimension<TPrecision> );
        void prepare();

        void addFunction( const Vector<TPrecision> & s );

        TPrecision query( const Vector<TPrecision> & s );

        uint32 getNbrActivatedFeatures() const;
        uint32 getActivatedFeatureId( uint32 feature_nr ) const;
        TPrecision getFeatureFactor( uint32 index ) const;
        TPrecision getFeatureFactors() const;

        bool load( FILE* );
        bool save( FILE* );

        friend std::ostream & operator<< <>( std::ostream & os, const GSBFN_Grid<TPrecision> & v );


    protected:
        std::vector<GSBFN_GridDimension<TPrecision> > m_Dimensions;
        uint32 m_NbrFunctions;
        std::vector<Vector<TPrecision> > m_Centers;
        std::vector<Vector<TPrecision> > m_Radius;
        std::vector<Vector<TPrecision> > m_InverseRadius;
        std::vector<TPrecision> m_FeatureFactors;
        TPrecision m_FeatureFactorsSum;
        uint32 m_NbrActivatedFeatures;
        std::vector<uint32> m_ActivatedFeatureIds;
};


#include "GSBFN_Grid.inl"


#endif
