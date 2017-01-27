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


#include "WidgetEnvironments.h"


#include "Defines.h"

#include "AL/AbstractMultiAgentEnvironmentInstance.h"
#include "AL/XMLTags_Environments.h"
#include "IO/FileSearch.h"
#include "IO/PluginWrapper.h"
#include "IO/XMLScanner.h"


const char* g_EnvironmentsPath = DATA_PATH "Environments/";

const char* g_EnvironmentExt = ".environment.xml";


WidgetEnvironments* g_WidgetEnvironments = NULL;


bool
fetchEnvironmentInformation( const char* env_name, const char* file_name, const char* xml_file_name, const char* base_path )
{
    PluginWrapper<AbstractMultiAgentEnvironmentInstance> p;
    AbstractMultiAgentEnvironmentInstance* env;
    bool ret;

    p.setPluginSearchRootPath( g_WidgetEnvironments->m_PluginPath );
    ret = p.check( "EnvironmentInstance", file_name );

    if (ret)
    {
        QFileInfo name( file_name );

        EnvironmentDescription env_desc;

        env_desc.Name = env_name;
        env_desc.ConfigFileName = xml_file_name;
        env_desc.PathName = base_path;
        env_desc.FileName = name.fileName().toStdString().c_str();
        env = reinterpret_cast<AbstractMultiAgentEnvironmentInstance*>(p.get());
        env_desc.Keywords = env->getInformation();

        QString icon_name;

        icon_name = base_path;
        icon_name += "/Icon.png";

        env_desc.Icon = QIcon( icon_name );

        if (env_desc.Icon.isDetached())
        {
            env_desc.Icon = QIcon( env->getPixmap() );
        }

        g_WidgetEnvironments->m_Environments.push_back( env_desc );
    }

    return ret;
}


bool
fetchEnvironment( const char* xml_file_name )
{
    XMLScanner scanner( xml_file_name, "./" );
    Ipseity::String env_name;

    env_name = xml_file_name;
    env_name = env_name.getSubString( env_name.findLastChar( '/' )+1, env_name.getLength() );
    env_name = env_name.getSubString( 0, env_name.findFirstChar( '.' )-1 );

    if (scanner.isLoaded())
    {
        XMLScanner::Token t;

        while( scanner.nextToken( &t ) )
        {
            switch( t.kind )
            {
                case XMLScanner::Token::Tag:
                        t.queryTagId( g_EnvironmentTags );
                        break;

                case XMLScanner::Token::Attribute:
                        switch( t.tagId )
                        {
                            case TAG_ENVIRONMENT:
                                if (t.keyIs( "Name" ))
                                {
                                    env_name = t.stringValue();
                                }
                                else if (t.keyIs( "Plugin" ))
                                {
                                    return fetchEnvironmentInformation( env_name.c_str(), t.stringValue(), xml_file_name, scanner.path.c_str() );
                                }
                                break;
                        }
                        break;

                case XMLScanner::Token::Close:
                    t.queryTagId( g_EnvironmentTags );
                    break;

                default:
                    break;
            }
        }
    }

    return false;
}


WidgetEnvironments::WidgetEnvironments( const char* plugin_path ) :
    m_PluginPath( plugin_path )
{
    g_WidgetEnvironments = this;

    cout << "From: " << plugin_path << endl;
}


WidgetEnvironments::~WidgetEnvironments()
{
}


void
WidgetEnvironments::setEnabled( bool toggle )
{
    for( uint32 id = 0; id < m_Labels.size(); ++id )
    {
        m_ComboBoxes[id]->setEnabled( toggle );
    }
    m_EnvironmentBar->setEnabled( toggle );
}


int32
WidgetEnvironments::getCurrentEnvironmentId() const
{
    if (m_SelectedEnvironmentId >= 0)
    {
        return m_EnvironmentSubset[m_SelectedEnvironmentId];
    }
    else
    {
        return -1;
    }
}


const char*
WidgetEnvironments::getCurrentEnvironmentName() const
{
    if (m_SelectedEnvironmentId >= 0)
    {
        return m_Environments[m_EnvironmentSubset[m_SelectedEnvironmentId]].Name.c_str();
    }
    else
    {
        return "";
    }
}


const char*
WidgetEnvironments::getEnvironmentName( int32 env_id ) const
{
    return m_Environments[env_id].Name.c_str();
}


const char*
WidgetEnvironments::getEnvironmentConfigFileName( int32 env_id ) const
{
    return m_Environments[env_id].ConfigFileName.c_str();
}


const char*
WidgetEnvironments::getEnvironmentFileName( int32 env_id ) const
{
    return m_Environments[env_id].FileName.c_str();
}


const char*
WidgetEnvironments::getEnvironmentPathName( int32 env_id ) const
{
    return m_Environments[env_id].PathName.c_str();
}


bool
WidgetEnvironments::existsEnvironment( const char* environment_name, int32 & env_id )
{
    for( env_id = 0; env_id < m_Environments.size(); ++env_id )
    {
        if (!strcmp( getEnvironmentName( env_id ), environment_name ))
        {
            return true;
        }
    }

    return false;
}


void
WidgetEnvironments::setSelectedEnvironment( int32 env_id )
{
    m_SelectedEnvironmentId = env_id;

    Ipseity::String label;

    m_Descriptions[0]->setText( getCurrentEnvironmentName() );
    for( uint32 label_id = 0; label_id < m_Labels.size(); ++label_id )
    {
        label = m_Labels[label_id]->text().toStdString().c_str();

        m_Descriptions[label_id+1]->setText( m_Environments[getCurrentEnvironmentId()].Keywords[label].c_str() );
    }

    emit updateWorkspaces();
}


void
WidgetEnvironments::attachView( uint32 view_id, QWidget* widget )
{
    QGridLayout* layout_environment = new QGridLayout( widget );
    layout_environment->setAlignment( Qt::AlignLeft );

    m_GroupboxKeywords = new QGroupBox(tr("Classification Criteria"));
    m_GroupboxEnvironmentDescription= new QGroupBox(tr("Description"));
    m_GroupboxEnvironmentSet = new QGroupBox(tr("Available Environments"));

    layout_environment->addWidget( m_GroupboxKeywords, 0,0 );
    layout_environment->addWidget( m_GroupboxEnvironmentDescription, 0,1 );
    layout_environment->addWidget( m_GroupboxEnvironmentSet, 1,0,1,2 );

    uint32 env_id;
    std::map<Ipseity::String,Ipseity::String>::const_iterator itr_kw;
    uint32 line = 0;

    m_EnvironmentActionGroup = NULL;

    m_LayoutKeywords = new QGridLayout( m_GroupboxKeywords );
    m_LayoutKeywords->setAlignment( Qt::AlignLeft );

    m_LayoutEnvironmentDescription = new QGridLayout( m_GroupboxEnvironmentDescription );
    m_LayoutEnvironmentDescription->setAlignment( Qt::AlignLeft );

    FileSearch::lookForFiles( g_EnvironmentsPath, FS_RECURSIVE_PATHS | FS_USE_LONGNAMES | FS_ITERATE_FILES, fetchEnvironment, g_EnvironmentExt, 1 );

    std::set<Ipseity::String> keywords;

    for( env_id = 0; env_id < m_Environments.size(); ++env_id )
    {
        for( itr_kw = m_Environments[env_id].Keywords.begin(); itr_kw != m_Environments[env_id].Keywords.end(); ++itr_kw )
        {
            keywords.insert( (*itr_kw).first.c_str() );
        }
    }

    std::set<Ipseity::String>::const_iterator itr_kwn;
    QLabel* label;
    QComboBox* combo_box;
    std::set<Ipseity::String> values;
    std::set<Ipseity::String>::const_iterator itr_v;

    label = new QLabel( "Name:" );
    m_LayoutEnvironmentDescription->addWidget( label, 0,0 );
    label = new QLabel();
    m_Descriptions.push_back( label );
    m_LayoutEnvironmentDescription->addWidget( label, 0,1 );

    for( itr_kwn = keywords.begin(); itr_kwn != keywords.end(); ++itr_kwn )
    {
        label = new QLabel( QString( "%1" ).arg((*itr_kwn).c_str() ) );
        m_Labels.push_back( label );

        combo_box = new QComboBox();
        combo_box->setMinimumHeight( 30 );
        m_ComboBoxes.push_back( combo_box );

        values.clear();
        for( env_id = 0; env_id < m_Environments.size(); ++env_id )
        {
            values.insert( m_Environments[env_id].Keywords[(*itr_kwn)] );
        }
        if (values.size() > 1)
        {
            combo_box->addItem( "*" );
        }
        for( itr_v = values.begin(); itr_v != values.end(); ++itr_v )
        {
            combo_box->addItem( (*itr_v).c_str() );
        }

        m_LayoutKeywords->addWidget( label, line,0 );
        m_LayoutKeywords->addWidget( combo_box, line,1,1,3 );

        label = new QLabel( QString( "%1:" ).arg((*itr_kwn).c_str() ) );
        m_LayoutEnvironmentDescription->addWidget( label, line+1,0 );

        label = new QLabel();
        m_Descriptions.push_back( label );
        m_LayoutEnvironmentDescription->addWidget( label, line+1,1 );

        ++line;
    }

    for( uint32 i = 0; i < m_Labels.size(); ++i )
    {
        connect( m_ComboBoxes[i],SIGNAL(currentIndexChanged(int)),this,SLOT(updateEnvironmentSet()) );
    }

    m_LayoutEnvironmentSet = new QGridLayout( m_GroupboxEnvironmentSet );
    m_LayoutEnvironmentSet->setAlignment( Qt::AlignLeft );

    m_EnvironmentBar = new QToolBar( m_GroupboxEnvironmentSet );

    m_LayoutEnvironmentSet->addWidget( m_EnvironmentBar, 0,0 );


    updateEnvironmentSet();
}


void
WidgetEnvironments::updateEnvironmentSet()
{
    int32 env_id;

    m_EnvironmentSubset.clear();
    for( env_id = 0; env_id < m_Environments.size(); ++env_id )
    {
        m_EnvironmentSubset.push_back( env_id );
    }

    std::vector<int32>::iterator itr_env;
    Ipseity::String label;

    for( uint32 label_id = 0; label_id < m_Labels.size(); ++label_id )
    {
        if (strcmp( m_ComboBoxes[label_id]->currentText().toStdString().c_str(), "*" ))
        {
            label = m_Labels[label_id]->text().toStdString().c_str();
            itr_env = m_EnvironmentSubset.begin();
            while( itr_env != m_EnvironmentSubset.end() )
            {
                env_id = (*itr_env);

                if (strcmp( m_Environments[env_id].Keywords[label].c_str(), m_ComboBoxes[label_id]->currentText().toStdString().c_str() ))
                {
                    m_EnvironmentSubset.erase( itr_env );
                }
                else
                {
                    ++itr_env;
                }
            }
        }
    }


    QAction* action;
    QToolButton* button;

    SafeDelete( m_EnvironmentActionGroup );
    m_EnvironmentActionGroup = new QActionGroup( m_GroupboxEnvironmentSet );
    m_EnvironmentActionGroup->setExclusive( true );

    m_EnvironmentBar->clear();

    m_SelectedEnvironmentId = -1;
    for( uint32 env_index = 0; env_index < m_EnvironmentSubset.size(); ++env_index )
    {
        env_id = m_EnvironmentSubset[env_index];

        action = new QAction( m_Environments[env_id].Icon, m_Environments[env_id].Name.c_str(), m_EnvironmentActionGroup );
        action->setCheckable( true );
        if (env_index == 0)
        {
            action->setChecked( true );
            setSelectedEnvironment( action );
        }
        action->setShortcut( tr( "%1" ).arg(env_index+1) );

        button = new QToolButton();
        button->setFixedSize( 52, 52 );
        button->setToolButtonStyle( Qt::ToolButtonIconOnly );
        button->setDefaultAction( action );
        button->setIconSize( QSize( 48, 48 ) );

        m_EnvironmentBar->addWidget( button );
        m_EnvironmentActionGroup->addAction( action );
    }

    connect( m_EnvironmentActionGroup, SIGNAL(selected(QAction*)), this, SLOT(setSelectedEnvironment(QAction*)) );


    emit updateWorkspaces();
}


void
WidgetEnvironments::setSelectedEnvironment( QAction* action )
{
    QList<QAction*> lists;
    int32 nr;

    lists = m_EnvironmentActionGroup->actions();

    nr = 0;
    while( lists[nr] != action && nr < lists.count() )
    {
        ++nr;
    }

    setSelectedEnvironment( nr );
}
