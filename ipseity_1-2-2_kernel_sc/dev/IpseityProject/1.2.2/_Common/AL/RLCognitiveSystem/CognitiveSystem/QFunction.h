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


#ifndef QValueMemory_h
#define QValueMemory_h


#include "AL/IpseityTypes.h"
#include "AL/Stimulus.h"
#include "AL/Response.h"
#include "AL/AbstractQFunction.h"
#include "IO/PluginWrapper.h"
#include "AbstractQFunctionModule.h"
#include "AL/AbstractResponseModule.h"


#include <vector>
#include <string>


//! Q-value Memory.
/*!
    \author Fabrice LAURI
*/

class QFunction : public AbstractQFunction
{
    public:
        //! Initialize a Q-value Memory.
        /*!
            \param memory_name
            \param base_path
            \param name
            \param env
        */
        QFunction( const char* memory_name, const char* base_path, const char* name );
        ~QFunction();

        INTERFACE_ABSTRACT_PLUGIN
        INTERFACE_ABSTRACT_QFUNCTION

        bool canMemorize() const;

    protected:
		PluginWrapper<AbstractQFunctionModule> m_QFunction;
        AbstractQFunctionModule* m_Memory;

        SensorySignalProperties* m_SensoryProperties;
        ResponseSignalProperties* m_ResponseProperties;

        std::string m_MemoryName;
        std::string m_Path;
        std::string m_ConfigFileName;

        Response m_Response;
};


#endif
