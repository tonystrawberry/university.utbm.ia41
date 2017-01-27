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


#ifndef CognitiveSystem_h
#define CognitiveSystem_h


#include "IpseityString.h"
#include "AL/Taxon.h"
#include "AL/AbstractCognitiveSystem.h"

#include "AL/AbstractEnvironment.h"
#include "AL/AbstractCognitiveModule.h"
#include "AL/AbstractStimulusIntegrationFunction.h"

#include "IO/PluginWrapper.h"
#include "IO/AbstractPlugin.h"
#include "IO/ParameterCheckBox.h"
#include "IO/ParameterSpinBox.h"
#include "IO/ParameterLineEdit.h"
#include "IO/IpseityInputManager.h"

#include <QtGui>


//! Cognitive system.
/*!
 *  \author Fabrice LAURI
*/


class CognitiveSystem : public QObject, public AbstractCognitiveSystem
{
    Q_OBJECT

    public:
        CognitiveSystem();
        ~CognitiveSystem();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_COGNITIVE_SYSTEM

    protected:
        //! Save the memory and the stats associated with the cognitive system.
        void save();

    protected slots:
        void controlAgent( int agent_id );
        void selectCognitiveModule( int module_id );
        void onChangeLearningMode(bool);
        void onChangeSaveEvery(bool);
        void onChangeSaveInteractions(bool);
        void onChangeMergeEpisodes(bool);


    protected:
        Ipseity::String m_BasePath;
        Ipseity::String m_PerformancePath;
        Ipseity::String m_Prefix;

        AbstractEnvironment* m_Environment;

        Taxon m_Taxon;
        uint32 m_ControlledAgentId;

        Response m_Response;

        std::vector<uint32> m_AgentIds;
        std::map<uint32,Stimulus> m_AgentOldSensation;
        std::map<uint32,Stimulus> m_AgentSensation;
        std::map<uint32,Stimulus> m_AgentModifiedSensation;
        std::map<uint32,uint32> m_AgentNbrDecisions;

        uint32 m_SelectedCognitiveModuleId;
        std::vector<Ipseity::String> m_ModuleNames;
        std::vector<PluginWrapper<AbstractCognitiveModule>*> m_PcCognitiveModules;
        std::vector<AbstractCognitiveModule*> m_CognitiveModules;

        PluginWrapper<AbstractStimulusIntegrationFunction>* m_PcStimulusIntegrationFunction;
        AbstractStimulusIntegrationFunction* m_StimulusIntegrationFunction;

        IpseityInputManager* m_InputManager;

    protected:
        uint32 m_NbrEpisodes;
        bool m_LearningMode;


        // Gui
        QGridLayout* m_Layout;
        QCheckBox* m_WidgetControlAgent;
        QComboBox* m_WidgetAgents;
        QLabel* m_LabelSelectedCS;
        ParameterCheckBox* m_CheckboxLearningMode;
        ParameterCheckBox* m_CheckboxSaveEvery;
        ParameterSpinBox* m_SpinboxNbrEpisodes;
        ParameterCheckBox* m_CheckboxUseSIF;
        ParameterCheckBox* m_CheckboxSavePerformance;
        ParameterLineEdit* m_LineEditInteractionFilePrefix;
        ParameterCheckBox* m_CheckboxResetInteractions;
        ParameterCheckBox* m_CheckboxSaveInteractions;
        ParameterCheckBox* m_CheckboxMergeEpisodes;
        ParameterCheckBox* m_CheckboxIgnoreUnproductiveTransitions;
        ParameterCheckBox* m_CheckboxIncrementActions;
        QComboBox* m_WidgetSelectedCS;
        QGroupBox* m_Controls;
};


#endif
