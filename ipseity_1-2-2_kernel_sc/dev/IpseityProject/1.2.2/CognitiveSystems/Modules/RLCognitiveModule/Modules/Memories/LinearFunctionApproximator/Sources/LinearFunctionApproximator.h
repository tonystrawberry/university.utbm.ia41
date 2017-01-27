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


#ifndef LinearFunctionApproximator_h
#define LinearFunctionApproximator_h


#include "IO/PluginWrapper.h"
#include "AL/RLCognitiveSystem/CognitiveSystem/BaseQFunctionModule.h"
#include "AL/AbstractFeatureExtractionModule.h"

#include <vector>
#include <map>

#include <QtGui>


class Cell
{
	public:
		Cell();

		void reset();


	public:
		Ipseity::Scalar Weight;
		Ipseity::Scalar Feature;
		uint32 NbrVisits;
};


class LinearFunctionApproximator : public BaseQFunctionModule
{
    public:
        LinearFunctionApproximator();
        ~LinearFunctionApproximator();

        INTERFACE_ABSTRACT_PLUGIN

        bool loadParameters( const char* file_name, const char* base_path );

        INTERFACE_BASE_QVMM

    protected:
        void init();
        Ipseity::Scalar getPercentageVisitedCells();
        void updateLabelPercentageVisitedCells();

        void deleteWidgets();


    protected:
        PluginWrapper<AbstractFeatureExtractionModule> m_PcFem;
        AbstractFeatureExtractionModule* m_FeatureExtractionModule;

        uint32 m_NbrCells;
        uint32 m_NbrVisitedCells;

        std::map<uint32,Ipseity::Scalar> m_EligibilityTraces;

		std::vector<Cell> m_Cells;

        // Gui
        QGridLayout* m_Layout;
        QLabel* m_Label;
};


#endif
