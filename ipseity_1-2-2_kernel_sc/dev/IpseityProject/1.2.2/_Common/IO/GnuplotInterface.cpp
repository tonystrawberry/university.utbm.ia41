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


#include "GnuplotInterface.h"

#include "Types.h"
#include "IO/ShellCommand.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>

using namespace std;


FILE* GnuplotInterface::m_Fp;
Ipseity::String GnuplotInterface::g_Path;
Ipseity::String GnuplotInterface::g_PlotFileName;
std::list<Ipseity::String> GnuplotInterface::g_GeneratedFigures;

const char* g_GnuplotName = "wgnuplot";
const char* GnuplotInterface::g_FontName = "Arial";
uint32 GnuplotInterface::g_FontSize = 8;
const char* g_Extension = "pdf";


void
GnuplotInterface::create( const char* path, const char* prefix_name )
{
    Ipseity::String fig_name;

    g_GeneratedFigures.clear();

    setPath( path );
    setPlotFileName( prefix_name );

    fig_name = g_Path.c_str();
    fig_name += g_PlotFileName.c_str();

    m_Fp = fopen( fig_name.c_str(), "w" );

    fprintf( m_Fp, "reset\n" );
    fprintf( m_Fp, "set datafile missing \"-\"\n" );
    fprintf( m_Fp, "\n" );
    fprintf( m_Fp, "set terminal pdfcairo enhanced color font \"%s,%d\"\n", g_FontName, g_FontSize );
    fprintf( m_Fp, "\n" );
//	fprintf( m_Fp, "set lmargin 0\n" );
//	fprintf( m_Fp, "set rmargin 0\n" );
//	fprintf( m_Fp, "set bmargin 0\n" );
//	fprintf( m_Fp, "set tmargin 0\n" );
//	fprintf( m_Fp, "\n" );
}


void
GnuplotInterface::addLine( const char* text, ... )
{
    va_list lst;

    va_start( lst, text );
    vfprintf( m_Fp, text, lst );
    va_end( lst );
    fprintf( m_Fp, "\n" );
}


void
GnuplotInterface::addNewLine()
{
    fprintf( m_Fp, "\n" );
}


void
GnuplotInterface::addFigure( const char* text, ... )
{
    char name[256];
    va_list lst;

    va_start( lst, text );
    vsprintf( name, text, lst );
    va_end( lst );

    g_GeneratedFigures.push_back( name );
    fprintf( m_Fp, "set output '%s.%s'\n", name, g_Extension );
}


void
GnuplotInterface::close()
{
    fprintf( m_Fp, "set output\n" );
    fprintf( m_Fp, "set terminal pop\n" );

    fclose( m_Fp );

    generatePdfFigures();
}


const std::list<Ipseity::String> &
GnuplotInterface::getGeneratedFigures()
{
    return g_GeneratedFigures;
}


//*****************************************************************************


void
GnuplotInterface::setPath( const char* path )
{
    g_Path = path;
}

void
GnuplotInterface::setPlotFileName( const char* name )
{
    g_PlotFileName = name;
    g_PlotFileName += ".dem";
}


void
GnuplotInterface::generatePdfFigures()
{
    ShellCommand::exec( g_GnuplotName, g_PlotFileName.c_str(), g_Path.c_str() );
}
