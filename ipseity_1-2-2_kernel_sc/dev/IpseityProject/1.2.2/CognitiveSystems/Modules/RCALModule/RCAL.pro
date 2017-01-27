#/*
# * $Id$
# *
# * Ipseity is an open-source platform dedicated to the development and
# * validation of Artificial Intelligence techniques in Multiagent Systems.
# * It is developed in C++ with the Qt framework (http://qt-project.org).
# * More details on <http://www.ipseity-project.com>
# * Copyright (C) 2014, 2013 Ipseity Core Developers
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


#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T20:57:21
#
#-------------------------------------------------

TARGET = RCAL
TARGET_DIR = CognitiveSystems/CM_RL/

PROJECT_ROOTDIR = ../../../

include($$PROJECT_ROOTDIR/common.pro)

INCLUDEPATH += Sources/
INCLUDEPATH += C:/Tools/Python27/include/ C:\Tools\Python27\Lib\site-packages\numpy\core\include\ C:\Tools\GSL\include/
#INCLUDEPATH += "C:\Program Files\MATLAB\R2011b\extern\include/"

LIBS += -LC:\Tools\GSL\lib -lgsl -LC:\Tools\Python27\libs -lpython27
#LIBS += -L"C:\Program Files\MATLAB\R2011b\extern\lib\win64\microsoft/" -llibeng -llibmat -llibmx

include($$PROJECT_ROOTDIR/common_plugin.pro)

HEADERS += \
    $$COMMON_PATH/AL/Stimulus.h \
    $$COMMON_PATH/AL/Response.h \
    $$COMMON_PATH/AL/BasePluginCognitiveModule.h \
    $$COMMON_PATH/AL/ResponseModule.h \
    $$COMMON_PATH/Math/Cardinality.h \
    $$COMMON_PATH/IO/ShellCommand.h \
    $$COMMON_PATH/IO/GnuplotInterface.h \
    $$COMMON_PATH/IO/ParseData.h \
    Sources/*.h \

SOURCES += \
    $$COMMON_PATH/AL/Stimulus.cpp \
    $$COMMON_PATH/AL/Response.cpp \
    $$COMMON_PATH/AL/BasePluginCognitiveModule.cpp \
    $$COMMON_PATH/AL/ResponseModule.cpp \
    $$COMMON_PATH/Math/Cardinality.cpp \
    $$COMMON_PATH/IO/ShellCommand.cpp \
    $$COMMON_PATH/IO/GnuplotInterface.cpp \
    $$COMMON_PATH/IO/ParseData.cpp \
    Sources/decisionTreesClassifier.cc \
    Sources/bilal_classifier.cc \
    Sources/main.cpp \
    Sources/RCAL.cpp
