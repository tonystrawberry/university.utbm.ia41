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


#ifndef WidgetEnvironments_h
#define WidgetEnvironments_h


#include "Types.h"
#include "IpseityString.h"

#include <QtGui>

#include <vector>
#include <map>
#include <set>


typedef struct
{
    Ipseity::String Name;
    Ipseity::String ConfigFileName;
    Ipseity::String FileName;
    Ipseity::String PathName;
    std::map<Ipseity::String,Ipseity::String> Keywords;
    QIcon Icon;
} EnvironmentDescription;


class WidgetEnvironments : public QObject
{
    Q_OBJECT

    public:
        WidgetEnvironments( const char* plugin_path );
        ~WidgetEnvironments();

        void attachView( uint32 view_id, QWidget* widget );

        void setEnabled( bool );

        const char* getCurrentEnvironmentName() const;
        int32 getCurrentEnvironmentId() const;

        const char* getEnvironmentName( int32 env_id ) const;
        const char* getEnvironmentConfigFileName( int32 env_id ) const;
        const char* getEnvironmentFileName( int32 env_id ) const;
        const char* getEnvironmentPathName( int32 env_id ) const;

        bool existsEnvironment( const char* environment_name, int32 & env_id );

        void setSelectedEnvironment( int32 env_id );

    signals:
        void updateWorkspaces();

    private slots:
        void updateEnvironmentSet();
        void setSelectedEnvironment( QAction* );


    public:
        const char* m_PluginPath;

        std::vector<QLabel*> m_Labels;
        std::vector<QComboBox*> m_ComboBoxes;
        std::vector<QLabel*> m_Descriptions;

        std::vector<int32> m_EnvironmentSubset;
        std::vector<EnvironmentDescription> m_Environments;

    protected:
        int32 m_SelectedEnvironmentId;

    protected:
        QGroupBox* m_GroupboxKeywords;
        QGroupBox* m_GroupboxEnvironmentDescription;
        QGroupBox* m_GroupboxEnvironmentSet;
        QGridLayout* m_LayoutKeywords;
        QGridLayout* m_LayoutEnvironmentDescription;
        QGridLayout* m_LayoutEnvironmentSet;
        QActionGroup* m_EnvironmentActionGroup;
        QToolBar* m_EnvironmentBar;
};


#endif
