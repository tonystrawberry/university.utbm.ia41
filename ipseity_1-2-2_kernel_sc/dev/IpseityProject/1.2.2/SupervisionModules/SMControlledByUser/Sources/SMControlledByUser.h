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


#ifndef SMControlledByUser_h
#define SMControlledByUser_h


#include <QtGui>

#include "Math/VectorNumber.h"
#include "AL/IpseityTypes.h"
#include "AL/AbstractSupervisionModule.h"
#include "AL/Scenarii.h"
#include "AL/Settings.h"
#include <IO/ParameterCheckBox.h>
#include <IO/ParameterLineEdit.h>
#include <IO/ParameterSpinBox.h>

#include "WidgetScenariiList.h"
#include "WidgetScenarioItem.h"

#include <map>


enum {
	STATE_NORMAL = 0,
	STATE_RESTART,
	STATE_PREVIOUS_SCENARIO,
	STATE_NEXT_SCENARIO
};


enum {
    MODE_TEST   = 0,
    MODE_LEARN
};


class SMControlledByUser : public QObject, public AbstractSupervisionModule
{
    Q_OBJECT

    public: // Dll interface
        SMControlledByUser();
        ~SMControlledByUser();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_SUPERVISION_MODULE

    protected:
		void initScenarioId();
		void countCurrentScenario();
        uint32 getNbrTotalEpisodes() const;

        bool isLearning() const;
        bool isTesting() const;

    private slots:
        void setEpisodeCount();
        void setLearningEpisodes();
        void setTestEpisodes();
        void onScenariiListSelectionChanged();
        void onUseExperimentFile( bool toggle );


    protected:
        Scenarii m_Scenarii;
        uint32 m_NbrRemainingLoops;
        uint32 m_ItemId;
        bool m_NewTrial;
        uint32 m_TrialId;
        uint32 m_ScenarioId;
        uint32 m_EpisodeRuns;
        uint32 m_EpisodeId[2];
        uint32 m_Mode;
        bool m_Ended;

        // GUI
//        QGridLayout* m_LayoutParameterSetting;
        QGridLayout* m_LayoutScenarios;

        QWidget* m_AncillaryWidget;
        QHBoxLayout* m_AncillaryLayout;
        QHBoxLayout* m_LayoutScenarioEpisodes;

//        ParameterCheckBox* m_CheckboxUseParameterSettingFile;
//        ParameterLineEdit* m_LineEditParameterSettingFile;
//        ParameterSpinBox* m_SpinBoxParameterSettingId;
//        QLabel* m_LabelNbrParameterSettings;
        ParameterSpinBox* m_SpinboxNbrTrials;
        ParameterSpinBox* m_SpinboxNbrLoops;

        QPushButton* m_ButtonSetEpisodeCount;
        QPushButton* m_ButtonSetLearningEpisodeCount;
        QPushButton* m_ButtonSetTestEpisodeCount;

        QGroupBox* m_GroupBoxScenarioEpisodes;
        ParameterSpinBox* m_SpinboxEpisodeCount;
        WidgetScenariiList* m_WidgetScenariiList;
        QProgressBar* m_ProgressBarGlobal;
};


#endif
