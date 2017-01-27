#include "FEM_Monoides.h"

#include "Math/MathToolBox_Misc.h"
#include "IO/XMLScanner.h"
#include "XMLTags_FEM_Monoides.h"


//*****************************************************************************


FEM_Monoides::FEM_Monoides()
{
}

FEM_Monoides::~FEM_Monoides()
{
}


const char*
FEM_Monoides::getName() const
{
	return "FEM_Monoides";
}


const char*
FEM_Monoides::getDescription() const
{
    return "";
}


const char*
FEM_Monoides::getAuthors() const
{
    return "Fabrice Lauri (fabrice.lauri@utbm.fr) & Jiawei Zhu";
}


void
FEM_Monoides::attachToWidget( QWidget* )
{
}


bool
FEM_Monoides::init( const char* file_name, const char* base_path )
{
    XMLScanner scanner( file_name, base_path );

    if (!scanner.isLoaded())
    {
        return false;
    }

    XMLScanner::Token t;
	lf::String parameter_name;

	cout << "Loading FEM_Monoides parameters from '" << base_path << file_name << "'..." << endl;

    m_ExtractFromStimulusOnly = true;
    m_MonoideDegree = 2;

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
					case TAG_PARAMETER:
						if (t.keyIs( "Name" ))
						{
							parameter_name = t.stringValue();
						}
						else if (t.keyIs( "Value" ))
						{
							if (!strcmp( parameter_name.c_str(), "IncludeResponse" ))
							{
								if (!strcmp( t.stringValue(), "yes" ))
								{
									m_ExtractFromStimulusOnly = false;
								}
								else
								{
									m_ExtractFromStimulusOnly = true;
								}
							}
							else if (!strcmp( parameter_name.c_str(), "Degree" ))
							{
								m_MonoideDegree = t.intValue();
							}
						}
						break;
                }
                break;

            case XMLScanner::Token::Close:
                t.queryTagId( g_Tags );
				break;
        }
    }

    if (m_MonoideDegree > m_StimulusDescription->getDimension())
    {
        m_MonoideDegree = m_StimulusDescription->getDimension();
    }

    if (m_ExtractFromStimulusOnly)
    {
        m_NbrTotalFeatures = 1+Cnk( m_StimulusDescription->getDimension()+m_MonoideDegree-1, m_MonoideDegree );

        m_CWR.init( m_StimulusDescription->getDimension(), m_MonoideDegree );
    }
    else
    {
        m_NbrTotalFeatures = 1+Cnk( m_DataDescription->getDimension()+m_MonoideDegree-1, m_MonoideDegree );

        m_CWR.init( m_DataDescription->getDimension(), m_MonoideDegree );
    }

    m_Features = new IpseityPrecisionType [m_NbrTotalFeatures];

    m_NbrActivatedFeatures = m_NbrTotalFeatures;
    m_ActivatedFeatureIds = new uint32 [m_NbrActivatedFeatures];
    for( uint32 i = 0; i < m_NbrActivatedFeatures; ++i )
    {
        m_ActivatedFeatureIds[i] = i;
    }

    return true;
}


void
FEM_Monoides::computeFeatures( const SensoryData & x )
{
    m_Features[0] = 1.0;

    uint32 k = 1;
    uint32 nbrS = m_StimulusDescription->getDimension();

    m_CWR.reset();
    do
    {
        m_Features[k] = 1;
        for( uint32 i = 0; i < nbrS; ++i )
        {
            for( uint32 n = 0; n < m_CWR.Counts[i]; ++n )
            {
                m_Features[k] *= x[i];
            }
        }
        ++k;
    }
    while( m_CWR.next() );
}


void
FEM_Monoides::computeFeatures( const SensoryData & x, const Response & u )
{
    m_Features[0] = 1.0;

    uint32 k = 1;
    uint32 nbrS = m_StimulusDescription->getDimension();
    uint32 nbrR = m_ResponseDescription->getDimension();

    m_CWR.reset();
    do
    {
        m_Features[k] = 1;
        for( uint32 i = 0; i < nbrS; ++i )
        {
            for( uint32 n = 0; n < m_CWR.Counts[i]; ++n )
            {
                m_Features[k] *= x[i];
            }
        }
        for( uint32 i = 0; i < nbrR; ++i )
        {
            for( uint32 n = 0; n < m_CWR.Counts[nbrS+i]; ++n )
            {
                m_Features[k] *= u[i];
            }
        }
        ++k;
    }
    while( m_CWR.next() );
}
