#/*
# * $Id$
# *
# * Ipseity is an open-source platform dedicated to the development and
# * validation of Artificial Intelligence techniques in Multiagent Systems.
# * It is developed in C++ with the Qt framework (http://qt-project.org).
# * More details on <http://www.ipseity-project.com>
# * Copyright (C) 2015, 2014, 2013 Ipseity Core Developers
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
# *
# */

# -------------------------------------------------
# Project created by QtCreator 2010-10-10T18:15:51
# -------------------------------------------------

TARGET = Ipseity
TEMPLATE = app

RC_FILE += Ipseity.rc
RESOURCES = Ipseity.qrc

CONFIG += console release
QT += core gui opengl xml network

# DEFINES += USE_PYTHON

PROJECT_ROOTDIR = ../

include($$PROJECT_ROOTDIR/common.pro)

INCLUDEPATH += inc/ $$DEV_ROOTDIR/API/$$OS/glew-mingw/include/
LIBS += -L$$DEV_ROOTDIR/API/$$OS/glew-mingw/lib/ -l$$GLEW_LIB

contains( DEFINES, USE_PYTHON )
{
    INCLUDEPATH += C:/Tools/Python27/include/
    LIBS += -LC:\Tools\Python27\libs -lpython27
}

DESTDIR = $$APP_ROOTDIR/IpseityProject/binary/$$APP_VERSION/$$OS/

include($$PROJECT_ROOTDIR/common_lib.pro)


HEADERS += \
    $$COMMON_PATH/AL/Stimulus.h \
    $$COMMON_PATH/AL/Response.h \
    $$COMMON_PATH/AL/XMLTags_Environments.h \
    $$COMMON_PATH/Math/MathToolBox_Misc.h \
    Sources/DialogAbout.h \
    Sources/MainWindow.h \
    Sources/WidgetRenderer.h \
    Sources/WidgetTaxonsList.h \
    Sources/WidgetTaxonItem.h \
    Sources/WidgetAgentsList.h \
    Sources/WidgetAgentItem.h \
    Sources/WidgetEnvironments.h

SOURCES += \
    $$COMMON_PATH/AL/Stimulus.cpp \
    $$COMMON_PATH/AL/Response.cpp \
    $$COMMON_PATH/AL/XMLTags_Environments.cpp \
    $$COMMON_PATH/Math/MathToolBox_Misc.cpp \
    Sources/main.cpp \
    Sources/DialogAbout.cpp \
    Sources/MainWindow.cpp \
    Sources/WidgetRenderer.cpp \
    Sources/WidgetTaxonsList.cpp \
    Sources/WidgetTaxonItem.cpp \
    Sources/WidgetAgentsList.cpp \
    Sources/WidgetAgentItem.cpp \
    Sources/WidgetEnvironments.cpp
