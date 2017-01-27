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


#ifndef Defines_h
#define Defines_h


#include <QSettings>


#define APP_NAME "Ipseity"
#define VERSION "1.2.2"
#define DATE "February 2015"

#define IRTES "Institut de Recherche sur les Transports, l'Energie et la Société (IRTES)"
#define SeT "Laboratoire Systèmes et Transports (SeT)"
#define UTBM "Université de Technologie de Belfort-Montbéliard (UTBM)"
#define AFFILIATION IRTES "<br>" SeT "<br>" UTBM "<br>FRANCE"

#define WINDOW_TITLE APP_NAME " " VERSION " by Fabrice LAURI (fabrice.lauri@utbm.fr)"


#define PLUGIN_PATH		"plugins/"
#define DATA_PATH		"../../../data/"


static const char* CONFIG_FILE_NAME  = APP_NAME ".ini";


//Définition des constantes utilisées pour paramétrer l'interface
static const int cstDefaultSpeed        = 50;
static const int cstMaxRecentFiles      = 5;
static const int cstDefaultRenderWidth  = 320;
static const int cstDefaultRenderHeight = 240;
static const int cstWindowMinimumWidth  = 800;
static const int cstWindowMinimumHeight = 600;

//Fichier de settings utilisé pour les paramètres persistants
static QSettings g_Settings( CONFIG_FILE_NAME, QSettings::IniFormat );


#endif
