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


#ifndef GnuplotInterface_h
#define GnuplotInterface_h


#include "Types.h"
#include "IpseityString.h"

#include <list>


class GnuplotInterface
{
    public:
        static void create( const char*, const char* );
        static void addLine( const char*, ... );
        static void addNewLine();
        static void addFigure( const char*, ... );
        static void close();

        static const std::list<Ipseity::String> & getGeneratedFigures();


    protected:
        static void setPath( const char* );
        static void setPlotFileName( const char* );
        static void generatePdfFigures();


    public:
        static const char* g_FontName;
        static uint32 g_FontSize;

    protected:
        static FILE* m_Fp;
        static Ipseity::String g_Path;
        static Ipseity::String g_PlotFileName;
        static std::list<Ipseity::String> g_GeneratedFigures;
};


#endif
