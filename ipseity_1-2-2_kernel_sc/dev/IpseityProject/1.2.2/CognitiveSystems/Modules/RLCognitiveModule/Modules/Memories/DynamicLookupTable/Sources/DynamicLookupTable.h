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


#ifndef DynamicLookupTable_h
#define DynamicLookupTable_h


#include "AL/RLCognitiveSystem/CognitiveSystem/BaseQFunctionModule.h"

#include <map>
#include <QtGui>


class DynamicLookupTable : public BaseQFunctionModule
{
	public:
		DynamicLookupTable();
		~DynamicLookupTable();

		INTERFACE_ABSTRACT_PLUGIN
		INTERFACE_BASE_QVMM

    protected:
        void init();
        Ipseity::Scalar getPercentageVisitedCells();
        void updateLabelPercentageVisitedCells();

        void deleteWidgets();


	protected:
        typedef std::map<SensorySignal, uint32> TIndices;
		TIndices m_Indices;

		typedef std::vector<Ipseity::Scalar> TValues;
		TValues m_Values;
		TValues m_Eligibilities;

		std::vector<uint32> m_ActivatedCells;

		// Gui
		QGridLayout* m_Layout;
		QLabel* m_Label;
};


#endif
