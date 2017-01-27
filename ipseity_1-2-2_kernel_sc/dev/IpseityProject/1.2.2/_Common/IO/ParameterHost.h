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


#ifndef ParameterHost_h
#define ParameterHost_h


#include "Types.h"
#include "IpseityString.h"

#include <list>


class ParameterRepository;
class AbstractControl;
class AbstractParameter;
class QSettings;


class ParameterHost
{
    public:
        ParameterHost();
        ~ParameterHost();

        //! Get the name of the parameter host
        const char* getName() const;

        ParameterRepository* getParameterRepository() const;
        void setParameterRepository( ParameterRepository* parameter_factory );

        void addControl( AbstractControl* control );
        void removeControl( AbstractControl* control );

        void addParameter( AbstractParameter* parameter );
        void removeParameter( AbstractParameter* parameter );
        AbstractParameter* findParameter( const char* parameter_name );

        void closeSettings();

    protected:
        void openSettings( const char* workspace_path );
        void initParametersSettings();
        void clearParameters();


    protected:
        Ipseity::String m_Name;

        // Repository of all parameters
        ParameterRepository* m_ParameterRepository;

        // Current Settings
        QSettings* m_Settings;

        // Parameters
        std::list<AbstractParameter*> m_Parameters;
};


#endif
