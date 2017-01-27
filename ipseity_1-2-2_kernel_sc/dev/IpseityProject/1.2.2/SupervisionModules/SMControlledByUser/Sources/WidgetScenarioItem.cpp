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


#include "WidgetScenarioItem.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDebug>

#include <iostream>

using namespace std;


const uint32 cstLineHeight = 35;
const uint32 cstNameCellWidth = 160;
const int cstMaximumEpisodeCount = 100000000;


WidgetScenarioItem::WidgetScenarioItem( uint32 scenario_id, QString _name, QTreeWidget* _parent ) :
    QTreeWidgetItem(_parent),
    m_CurrentEpisode( 1 )
{
//	this->setSizeHint(0,QSize(150,30));
//	this->setSizeHint(1,QSize(80,30));
//	this->setSizeHint(2,QSize(80,30));
//	this->setSizeHint(3,QSize(240,30));

	m_ScenarioId = scenario_id;
    m_ScenarioLabel = new QLabel(_name);
    m_ScenarioLabel->setAlignment( Qt::AlignJustify | Qt::AlignVCenter );
    m_ScenarioLabel->setFixedHeight( cstLineHeight );

    Ipseity::String name;

    name = "NbrEpisodes-S";
    name += scenario_id;

    m_NbrEpisodesSpinBox = new ParameterSpinBox( name.c_str(), "" );
    m_NbrEpisodesSpinBox->setRange(0, cstMaximumEpisodeCount);
    m_NbrEpisodesSpinBox->setSingleStep(1);
    m_NbrEpisodesSpinBox->setValue(1);
    m_NbrEpisodesSpinBox->setFixedHeight( cstLineHeight );
    connect(m_NbrEpisodesSpinBox,SIGNAL(valueChanged(int)),this,SLOT(spinboxNbrEpisodesValueChanged(int)));

    name = "LearnEvery-S";
    name += scenario_id;

    m_LearningEpisodeSpinBox = new ParameterSpinBox( name.c_str(), "" );
    m_LearningEpisodeSpinBox->setRange(0, cstMaximumEpisodeCount);
    m_LearningEpisodeSpinBox->setSingleStep(1);
    m_LearningEpisodeSpinBox->setValue(0);
    m_LearningEpisodeSpinBox->setFixedHeight( cstLineHeight );

    name = "TestEvery-S";
    name += scenario_id;

    m_TestEpisodeSpinBox = new ParameterSpinBox( name.c_str(), "" );
    m_TestEpisodeSpinBox->setRange(0, cstMaximumEpisodeCount);
    m_TestEpisodeSpinBox->setSingleStep(1);
    m_TestEpisodeSpinBox->setValue(0);
    m_TestEpisodeSpinBox->setFixedHeight( cstLineHeight );

    m_ProgressWidget = new QWidget;
    m_ProgressWidget->setFixedHeight( cstLineHeight );

    m_ProgressLayout = new QHBoxLayout(m_ProgressWidget);

    m_NbrEpisodes = new QLabel(tr("0"));
    m_ProgressLayout->addWidget(m_NbrEpisodes);

    m_EpisodeProgress = new QProgressBar;
    m_EpisodeProgress->setMinimum(0);
    m_EpisodeProgress->setMaximum(1);
    m_ProgressLayout->addWidget(m_EpisodeProgress);


    m_LabelIterations = new QLabel("");
    m_LabelIterations->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    m_LabelIterations->setFixedHeight( cstLineHeight );


    if (_parent != NULL)
    {
        this->treeWidget()->setItemWidget(this, 0, m_ScenarioLabel);
        this->treeWidget()->setItemWidget(this, 1, m_NbrEpisodesSpinBox);
        this->treeWidget()->setItemWidget(this, 2, m_LearningEpisodeSpinBox);
        this->treeWidget()->setItemWidget(this, 3, m_TestEpisodeSpinBox);
        this->treeWidget()->setItemWidget(this, 4, m_ProgressWidget);
        this->treeWidget()->setItemWidget(this, 5, m_LabelIterations);
    }
}


WidgetScenarioItem::~WidgetScenarioItem()
{
    delete m_ScenarioLabel;
    delete m_LabelIterations;
    delete m_LearningEpisodeSpinBox;
    delete m_NbrEpisodes;
    delete m_EpisodeProgress;
    delete m_ProgressLayout;
    delete m_ProgressWidget;
}


void
WidgetScenarioItem::displayWidgetInList(QTreeWidget* _list)
{
    _list->setItemWidget(this, 0, m_ScenarioLabel);
    _list->setItemWidget(this, 1, m_LearningEpisodeSpinBox);
    _list->setItemWidget(this, 2, m_ProgressWidget);
    _list->setItemWidget(this, 3, m_LabelIterations);
}


void
WidgetScenarioItem::setVisible( bool visible )
{
    m_LearningEpisodeSpinBox->setVisible( visible );
    m_EpisodeProgress->setVisible( visible );
}


void
WidgetScenarioItem::updateIterations( uint32 nbr_iterations, uint32 max_iterations )
{
    if (max_iterations)
    {
        m_LabelIterations->setText(tr("%1/%2").arg(nbr_iterations).arg(max_iterations));
    }
    else
    {
        m_LabelIterations->setText(tr("%1").arg(nbr_iterations));
    }
}


void
WidgetScenarioItem::increaseEpisodeCount()
{
    ++m_CurrentEpisode;
    m_LearningEpisodeSpinBox->stepUp();
    m_EpisodeProgress->setMaximum(m_LearningEpisodeSpinBox->value());
    m_NbrEpisodes->setText(tr("%1").arg(m_CurrentEpisode));
    m_EpisodeProgress->setValue( m_CurrentEpisode );
}

void
WidgetScenarioItem::decreaseEpisodeCount()
{
    m_LearningEpisodeSpinBox->stepDown();
    m_EpisodeProgress->setMaximum(m_LearningEpisodeSpinBox->value());
}


void
WidgetScenarioItem::setEpisodeCount(int value)
{
    m_NbrEpisodesSpinBox->setValue(value);
    m_EpisodeProgress->setMaximum(value);
}


void
WidgetScenarioItem::setLearningEpisodeCount(int value)
{
    m_LearningEpisodeSpinBox->setValue(value);
}


void
WidgetScenarioItem::setTestEpisodeCount(int value)
{
    m_TestEpisodeSpinBox->setValue(value);
}


uint32
WidgetScenarioItem::getScenarioId() const
{
    return m_ScenarioId;
}

uint32
WidgetScenarioItem::getCurrentEpisode() const
{
    return m_CurrentEpisode;
}

uint32
WidgetScenarioItem::getEpisodeCount() const
{
    return m_NbrEpisodesSpinBox->value();
}

uint32
WidgetScenarioItem::getLearnEveryEpisode() const
{
    return m_LearningEpisodeSpinBox->value();
}

uint32
WidgetScenarioItem::getTestEveryEpisode() const
{
    return m_TestEpisodeSpinBox->value();
}


void
WidgetScenarioItem::setEnabledEpisodeCount( bool toggled )
{
    m_LearningEpisodeSpinBox->setEnabled( toggled );
    m_NbrEpisodes->setText( "0" );
    if (toggled)
    {
        m_EpisodeProgress->setMaximum(m_LearningEpisodeSpinBox->value());
    }
    else
    {
        m_EpisodeProgress->setMaximum( 1 );
    }
}

QString
WidgetScenarioItem::scenarioName() const
{
    return m_ScenarioLabel->text();
}

void
WidgetScenarioItem::resetProgressBar() const
{
    m_EpisodeProgress->reset();
}

void
WidgetScenarioItem::spinboxValueChanged(int value)
{
    m_NbrEpisodes->setText( "0" );

    if(value <= 0)
    {
        m_EpisodeProgress->setEnabled(false);
    }
    else
    {
        m_EpisodeProgress->setEnabled(true);
        m_EpisodeProgress->setMaximum(value);
    }

    if (this->treeWidget() != NULL)
    {
        emit scenarioNbrEpisodesChanged(this->treeWidget()->indexOfTopLevelItem(this), value);
    }
}

void
WidgetScenarioItem::nextEpisode()
{
    ++m_CurrentEpisode;
    m_NbrEpisodes->setText(tr("%1").arg(m_CurrentEpisode));
    m_EpisodeProgress->setValue(m_CurrentEpisode);
}

void
WidgetScenarioItem::resetCurrentEpisode()
{
    m_NbrEpisodes->setText( "1" );
    m_CurrentEpisode = 1;
}
