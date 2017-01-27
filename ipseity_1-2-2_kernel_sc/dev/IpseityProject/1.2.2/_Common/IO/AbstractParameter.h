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


#ifndef AbstractParameter_h
#define AbstractParameter_h


#include "AL/IpseityTypes.h"

#include <QSettings>
#include <QGridLayout>


class ParameterHost;


class AbstractParameter
{
    public:
        static ParameterHost* g_Host;

    public:
        AbstractParameter( const char* name );
        virtual ~AbstractParameter();

        const char* getName() const;

        const char* getHostName() const;

        void init( QSettings* settings );

        virtual void addToLayout( QGridLayout* layout, int row, int column, bool caption_before_value ) = 0;

        void save();

        virtual const char* toString() = 0;

        virtual void setValue( bool value )             {}
        virtual void setValue( int value )              {}
        virtual void setValue( double value )           {}
        virtual void setValue( const char* value )      {}

    protected:
        virtual void initValue() = 0;
        virtual void saveValue() = 0;


    protected:
        Ipseity::String m_Name;
        ParameterHost* m_Host;
        char* m_ValueString;
        QSettings* m_Settings;
};


#define INTERFACE_ABSTRACT_PARAMETER \
    void addToLayout( QGridLayout* layout, int row, int column, bool caption_before_value = true ); \
    const char* toString(); \
    void initValue(); \
    void saveValue(); \


#endif
