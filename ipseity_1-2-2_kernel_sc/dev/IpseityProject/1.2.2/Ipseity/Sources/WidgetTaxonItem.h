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


#ifndef WidgetTaxonItem_h
#define WidgetTaxonItem_h


#include <QtGui>
#include <QCheckBox>
#include <QComboBox>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QLineEdit>

#include "Types.h"
#include "AL/Taxon.h"


class QLabel;
class ParameterCheckBox;
class ParameterComboBox;


class WidgetTaxonItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT

    public:
        WidgetTaxonItem( const QString & role_name, uint32 status, QTreeWidget* _parent );
        ~WidgetTaxonItem();

        void addCognitiveSystem( const char* name );
        void setCognitiveSystemNameEnabled( bool );
        void toggleShareCognitiveSystem();

        void changeIndex( int index );

        uint32 getStatus() const;

    signals:
        void updateTaxonStatusControllable( int, bool );
        void currentNameIndexChanged( int, int );

    private slots:
        void updateTaxonStatusControllable( bool );
        void currentNameIndexChanged( int );

    private:
        // GUI
        QLabel* m_TaxonName;
        QCheckBox* m_TaxonStatusControllable;
        QCheckBox* m_TaxonStatusSupervisor;
        ParameterCheckBox* m_TaxonStatusSharedCognitiveSystem;
        ParameterComboBox* m_CognitiveSystemName;
};


#endif
