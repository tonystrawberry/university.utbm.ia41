#include "RBF.h"


#include "IO/XMLScanner.h"
#include "XMLTags_RBF.h"


//*****************************************************************************


RBF::RBF()
{
}

RBF::~RBF()
{
    SafeDeleteArray( m_CenterDatas );
}


const char*
RBF::getName() const
{
    return "RBF";
}


const char*
RBF::getDescription() const
{
    return "";
}


const char*
RBF::getAuthors() const
{
    return "Fabrice Lauri (fabrice.lauri@utbm.fr)";
}


void
RBF::attachToWidget( QWidget* )
{
}


bool
RBF::init( const char* file_name, const char* base_path )
{
    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        return false;
    }

    XMLScanner::Token t;
    uint32 varId;

    char name[64];
    uint32* EachDimNbr;

    IpseityPrecisionType* Shifts;

//    cout <<"Umwelt is " << endl;
//    cout << (*m_StimulusUmwelt)[1].Name<< endl;

    m_CenterDimension = m_StimulusUmwelt->getDimension();
    CreateSafeArray( EachDimNbr, uint32,  m_CenterDimension);
    CreateSafeArray( Shifts, IpseityPrecisionType,  m_CenterDimension);

    cout << "Loading RBF parameters from '" << base_path << file_name << "'..." << endl;

    m_ExtractFromStimulusOnly = true;

    varId = 0;

    while( scanner.nextToken( &t ) )
    {
        switch( t.kind )
        {
            case XMLScanner::Token::Tag:
                t.queryTagId( g_Tags );

                switch( t.tagId )
                {
                    case TAG_VARIABLES:
                        break;
                }
                break;

            case XMLScanner::Token::Attribute:
                switch( t.tagId )
                {
                    case TAG_VARIABLE:
                        if (t.keyIs( "Name" ))
                        {
                            strcpy( name, t.stringValue() );
                        }
                        else if (t.keyIs( "NbrIntervals" ))
                        {
                            EachDimNbr[varId] = t.intValue();
                            ++varId;
                        }
                        break;

                    case TAG_PARAMETER:
                        if (t.keyIs( "Name" ))
                        {
                            strcpy( name, t.stringValue() );
                        }
                        else if (t.keyIs( "Value" ))
                        {
                            if(!strcmp(name,"Width"))
                            {
                                m_Width = t.intValue();
                                cout << "Width is " << m_Width << endl;
                            }
                            else if(!strcmp(name,"Height"))
                            {
                                m_Height = t.intValue();
                                cout << "Height is " << m_Height << endl;
                            }
                            else
                            {
                                cout << "*************** Wrong Input!!!!!!!!!!*******" << endl;
                            }
                        }
                        break;
                }
                break;

            case XMLScanner::Token::Close:
                t.queryTagId( g_Tags );

                switch( t.tagId )
                {
                    case TAG_VARIABLES:
                        m_CenterNbr = 1;
                        for(int i = 0; i < m_CenterDimension; i++)
                        {
//                            cout << EachDimNbr[i] << endl;
                            m_CenterNbr *= EachDimNbr[i];

                            Shifts[i] = ((*m_StimulusUmwelt)[i].Max - (*m_StimulusUmwelt)[i].Min) / EachDimNbr[i];
                        }
                        cout << "+++ Total nbr of Centers is " << m_CenterNbr << endl;

                        CreateSafeArray(m_CenterDatas, IpseityPrecisionType, m_CenterNbr * m_CenterDimension );

                        if(m_CenterDimension == 2)
                        {
                            for(int i = 0; i < EachDimNbr[0]; i ++)
                            {
                                for(int j = 0; j < EachDimNbr[1]; j++)
                                {
                                    m_CenterDatas[i*EachDimNbr[1]*2+j*2] = (Shifts[0]/2.0)*(2*i+1) + (*m_StimulusUmwelt)[0].Min;
                                    cout<< "( " << m_CenterDatas[i*EachDimNbr[1]*2+j*2] << " , " ;
                                    m_CenterDatas[i*EachDimNbr[1]*2+j*2+1] = (Shifts[1]/2.0)*(2*j+1) + (*m_StimulusUmwelt)[1].Min;
                                    cout<< m_CenterDatas[i*EachDimNbr[1]*2+j*2+1] << " )" << endl;
                                }
                            }
                        }
                        else
                        {
                            cout << "false: m_CenterDimension is not 2!" << endl;
                            return false;
                        }


                        cout << "++++++ End of TAG_VARIABLES-------------------  dim=" << m_CenterDimension <<endl;
                        break;
                }
                break;
        }
    }
    prepare();
    SafeDeleteArray(EachDimNbr);
    SafeDeleteArray(Shifts);

    return true;
}


void
RBF::computeFeatures( const SensoryData & s )
{
    IpseityPrecisionType temp;
    for( uint32 i = 0; i < m_CenterNbr; i++ )
    {
        temp = 0.0;
        for(uint32 j = 0; j < m_CenterDimension; j++)
        {
            temp += pow((s[j] - m_CenterDatas[i*m_CenterDimension+j]),2);
        }
        m_Features[i] = m_Height * exp(-(temp/m_Width));
//        cout << "s = " << s << endl;
//        cout << "m_Features[i] = " << m_Features[i] << endl;
    }
}


void
RBF::computeFeatures( const SensoryData & s, const Response & a )
{
//    GenericVector<IpseityPrecisionType> sa;
//    uint32 nbr;
//    uint32 index;

//    sa = s.concat( a );

//    nbr = 0;
//    for( uint32 i = 0; i < m_NbrActivatedFeatures; i++ )
//    {
//        if( m_Tilings[i]->query( sa, &index ) )
//        {
//            index += nbr;
//            m_ActivatedFeatureIds[i] = index;
//        }

//        nbr += m_Tilings[i]->getNbrTiles();
//    }
}


//*******************************************************************
// Protected functions
//*******************************************************************


void
RBF::prepare()
{
    m_NbrActivatedFeatures = m_CenterNbr;

    m_NbrTotalFeatures = m_CenterNbr;

    CreateSafeArray( m_Features, IpseityPrecisionType, m_NbrTotalFeatures );
    CreateSafeArray( m_ActivatedFeatureIds, uint32, m_NbrActivatedFeatures );

    for( uint32 i = 0; i < m_NbrTotalFeatures; i++ )
    {
        m_Features[i] = 0.0;
        m_ActivatedFeatureIds[i] = i;
    }
}


void
RBF::defineNRegularTilings()
{
//    Tiling<IpseityPrecisionType,IpseityPrecisionType>* tiling;

////	cout << "Define regular tilings" << endl;
//    for( uint32 i = 0; i < nbrTilings; i++ )
//    {
//        tiling = new Tiling<IpseityPrecisionType,IpseityPrecisionType>( *mainTiling );
//        tiling->disturbFrom( i, nbrTilings );
//        m_Tilings.push_back( tiling );
//    }
}


void
RBF::defineNRandomTilings()
{
//    Tiling<IpseityPrecisionType,IpseityPrecisionType>* tiling;

////	cout << "Define random tilings" << endl;
//    for( uint32 i = 0; i < nbrTilings; i++ )
//    {
//        tiling = new Tiling<IpseityPrecisionType,IpseityPrecisionType>( *mainTiling );
//        tiling->disturbRandomlyFrom( nbrTilings );
//        m_Tilings.push_back( tiling );
//    }
}
