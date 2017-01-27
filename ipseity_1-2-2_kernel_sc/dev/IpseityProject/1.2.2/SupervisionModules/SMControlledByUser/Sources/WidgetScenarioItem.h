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

#ifndef WidgetScenarioItem_h
#define WidgetScenarioItem_h


#include <QtGui>

#include "IO/ParameterSpinBox.h"


class QSpinBox;
class QLabel;


class WidgetScenarioItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT

    public:
        WidgetScenarioItem( uint32 scenario_id, QString _name, QTreeWidget* _parent);
        ~WidgetScenarioItem();

        void displayWidgetInList(QTreeWidget* _list);

        void setVisible( bool );

        void updateIterations( uint32, uint32 );

        void increaseEpisodeCount();
        void decreaseEpisodeCount();
        void setEpisodeCount(int);
        void setLearningEpisodeCount(int);
        void setTestEpisodeCount(int);
        void resetProgressBar() const;

        uint32 getScenarioId() const;
        uint32 getCurrentEpisode() const;
        uint32 getEpisodeCount() const;
        uint32 getLearnEveryEpisode() const;
        uint32 getTestEveryEpisode() const;

        QString scenarioName() const;
        void nextEpisode();
        void resetCurrentEpisode();

        void setEnabledEpisodeCount( bool );

    signals:
        void scenarioNbrEpisodesChanged(int id, int nbEpisodes);

    private slots:
        void spinboxValueChanged(int);

    private:
        uint32 m_ScenarioId;
        uint32 m_CurrentEpisode;

        QLabel* m_ScenarioLabel;
        ParameterSpinBox* m_NbrEpisodesSpinBox;
        ParameterSpinBox* m_LearningEpisodeSpinBox;
        ParameterSpinBox* m_TestEpisodeSpinBox;
        QWidget* m_ProgressWidget;
        QHBoxLayout* m_ProgressLayout;
        QLabel* m_NbrEpisodes;
        QLabel* m_LabelIterations;
        QProgressBar* m_EpisodeProgress;
};


#endif
