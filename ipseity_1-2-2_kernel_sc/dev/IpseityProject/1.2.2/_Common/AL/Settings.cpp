#include "AL/Settings.h"


#include "MemDefns.h"


Settings::Settings() :
    m_SettingId( NULL )
{
}


Settings::~Settings()
{
    SafeDelete( m_SettingId );
}


void
Settings::init( const char* settings_name )
{
//    if (m_CheckboxUseParameterSettingFile->isChecked())
//    {
//        std::fstream file;
//        Ipseity::String file_name;
//        char text[256];
//        QString var_name;
//        QStringList tmp_list;
//        QStringList list;
//        QString str;
//        QRegExp re( "[\[({]" );

//        file_name = m_VolatileVariables.m_Workspace;
//        file_name += m_LineEditParameterSettingFile->toString();

//        m_SettingName.clear();
//        m_SettingsList.clear();

//        file.open( file_name.c_str(), ios_base::in );
//        if (file.is_open())
//        {
//            cout << "Reading experiment file " << file_name << "..." << endl;
//            while( !file.eof() )
//            {
//                file.getline( text, 256 );
//                list = QString( text ).split( "=", QString::SkipEmptyParts );
//                if (list.size() > 0)
//                {
//                    var_name = list.at( 0 );

//    //                        cout << var_name.toStdString().c_str() << endl;

//                    if (list.at( 1 ).contains( re ))
//                    {
//                        str = list.at( 1 ).mid( 1, list.at( 1 ).length()-2 );
//                    }
//                    else
//                    {
//                        str = list.at( 1 );
//                    }

//    //                        cout << str.toStdString().c_str() << endl;

//                    if (str.contains( ":" ))
//                    {
//                        tmp_list = str.split( ":", QString::SkipEmptyParts );

//                        int32 first, step, last;

//                        first = atoi( tmp_list.at( 0 ).toStdString().c_str() );
//                        step = atoi( tmp_list.at( 1 ).toStdString().c_str() );
//                        last = atoi( tmp_list.at( 2 ).toStdString().c_str() );

//                        list.clear();
//    //                            cout << "Value: ";
//                        for( int32 i = first; i <= last; i += step )
//                        {
//                            list << QString( "%1" ).arg( i );
//    //                                cout << i << " ";
//                        }
//    //                            cout << endl;
//                    }
//                    else
//                    {
//                        list = str.split( " ", QString::SkipEmptyParts );
//                    }

//                    m_SettingName.push_back( var_name );
//                    m_SettingsList.push_back( list );
//                }
//            }

//            file.close();
//        }


//        SafeDelete( m_SettingId );
//        m_SettingId = new VectorNumber( m_SettingName.size() );

//        for( uint32 i = 0; i < m_SettingName.size(); ++i )
//        {
//            if (findParameter( m_SettingName[i].toStdString().c_str() ))
//            {
//                m_SettingId->setCardinality( i, m_SettingsList[i].size() );
//            }
//        }


//        m_NbrSettings = m_SettingId->getCardinality();

//        m_SpinBoxParameterSettingId->setMaximum( m_NbrSettings );
//        m_SettingId->selectNumber( m_SpinBoxParameterSettingId->value() );

//        m_NbrSettings -= m_SpinBoxParameterSettingId->value();
//        if (m_NbrSettings <= 0)
//        {
//            m_NbrSettings = 1;
//        }

//        cout << "# settings: " << m_NbrSettings << endl;
//    }
//    else
//    {
//        m_NbrSettings = 1;
//        SafeDelete( m_SettingId );
//    }
}


void
Settings::initParameters()
{
//    if (m_SettingId)
//    {
//        m_LabelNbrParameterSettings->setText( QString( "TotalSettings: %1 - RemainingSettings: %2" ).arg( m_SpinBoxParameterSettingId->maximum() ).arg( m_SpinBoxParameterSettingId->maximum()-m_SpinBoxParameterSettingId->value() ) );

//        AbstractParameter* param;
//        const std::vector<int32> & experiment_id = m_SettingId->getNumber();

//        cout << "****  Set values  ****" << endl;
//        for( uint32 i = 0; i < m_SettingName.size(); ++i )
//        {
//            param = findParameter( m_SettingName[i].toStdString().c_str() );

//            if (param)
//            {
//                cout << "Set value of " << m_SettingName[i].toStdString().c_str() << " to ";
//                cout << m_SettingsList[i].at( experiment_id[i] ).toStdString().c_str() << endl;

//                param->setValue( m_SettingsList[i].at( experiment_id[i] ).toStdString().c_str() );
//            }
//        }
//    }
}
