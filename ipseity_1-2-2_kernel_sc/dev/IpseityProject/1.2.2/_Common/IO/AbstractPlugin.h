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


#ifndef AbstractPlugin_h
#define AbstractPlugin_h


#include "Types.h"
#include "IpseityString.h"
#include "AbstractParameter.h"
#include "ParameterHost.h"

#include <QWidget>

#include <map>
#include <list>


template <class T> class PluginWrapper;


class ParameterRepository;

class PluginVolatileVariables
{
    public:
        Ipseity::String m_SearchRootPath;
        Ipseity::String m_Workspace;
};


class AbstractPlugin : public ParameterHost
{
    template <class T> friend class PluginWrapper;

    public:
        AbstractPlugin();
        ~AbstractPlugin();

        //! Get some information about the plugin.
        /*!
        */
        const std::map<Ipseity::String,Ipseity::String> & getInformation() const;

        //! Get some information about the plugin.
        /*!
            \param key Class of the information to extract.
        */
        const char* getInformation( const char* key ) const;

        //! Add some information about the plugin.
        /*!
        */
        void addInformation( const char* key, const char* information );

        void setInstanceName( const char* instance_name );

        void setParent( AbstractPlugin* parent );
        void setSearchRootPath( const char* path );
        void setWorkspace( const char* workspace );


    public: // Abstract functions
        //! Get the base name of the plugin.
        virtual const char* getBaseName() const = 0;

        //! Get the description of the plugin.
        virtual const char* getDescription() const = 0;

        //! Get the authors of the plugin.
        virtual const char* getAuthors() const = 0;

        virtual void defineParameters() = 0;

        //! Attach one view of the plugin to the specified widget.
        virtual bool attachView( uint32 view_id, QWidget* widget ) = 0;


    protected:
        void setVolatileVariables( const PluginVolatileVariables & vv );


    protected:
        AbstractPlugin* m_Parent;
        PluginVolatileVariables m_VolatileVariables;

        // Keywords
        std::map<Ipseity::String,Ipseity::String> m_Keywords;
};


#define IPSEITY_CREATOR "Lauri, F. (fabrice.lauri@utbm.fr)"


#define INTERFACE_ABSTRACT_PLUGIN \
        const char* getBaseName() const; \
        const char* getDescription() const; \
        const char* getAuthors() const; \
        void defineParameters(); \
        bool attachView( uint32 view_id, QWidget* widget ); \


#endif
