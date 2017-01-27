#/*
# * $Id$
# *
# * Ipseity is a rich-client platform developed in C++ with the Qt
# * framework (http://qt-project.org).
# * More details on <http://www.ipseity-project.com>
# * Copyright (C) 2013 Ipseity Core Developers
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
# */

# -------------------------------------------------
# Project created by QtCreator 2010-10-10T18:15:51
# -------------------------------------------------

TARGET = test
TEMPLATE = app

DEFINES -= UNICODE
DEFINES += QT

CONFIG += console release
QT += core gui opengl xml network

PROJECT_ROOTDIR = ../

include($$PROJECT_ROOTDIR/common.pro)

COMMON_PATH = $$PROJECT_ROOTDIR/_Common/
INCLUDEPATH += $$COMMON_PATH Sources/

DESTDIR = $$APP_ROOTDIR/IpseityProject/binary/$$APP_VERSION/$$OS/


HEADERS += \
    $$COMMON_PATH/StringFunctions.h \
    $$COMMON_PATH/IpseityString.h \
#    $$COMMON_PATH/IO/Parameter.h \
    $$COMMON_PATH/Math/BoundedVariable.h \
    $$COMMON_PATH/Math/Cardinality.h \
    $$COMMON_PATH/Math/PeriodicVariable.h \
    $$COMMON_PATH/Math/RandomNumberGenerator.h \
    $$COMMON_PATH/Math/MathToolBox_Misc.h \
    $$COMMON_PATH/Math/ScalarDomain.h \
    $$COMMON_PATH/Math/Vector.h \
    $$COMMON_PATH/Math/VectorDomain.h \
#    $$COMMON_PATH/Math/Variable.h \
#    $$COMMON_PATH/Math/VariableBank.h \


SOURCES += \
    $$COMMON_PATH/StringFunctions.cpp \
    $$COMMON_PATH/IpseityString.cpp \
#    $$COMMON_PATH/IO/Parameter.cpp \
    $$COMMON_PATH/Math/BoundedVariable.inl \
    $$COMMON_PATH/Math/Cardinality.cpp \
    $$COMMON_PATH/Math/PeriodicVariable.inl \
    $$COMMON_PATH/Math/RandomNumberGenerator.cpp \
    $$COMMON_PATH/Math/MathToolBox_Misc.cpp \
    $$COMMON_PATH/Math/ScalarDomain.inl \
    $$COMMON_PATH/Math/Vector.inl \
    $$COMMON_PATH/Math/VectorDomain.inl \
#    $$COMMON_PATH/Math/Variable.cpp \
#    $$COMMON_PATH/Math/VariableBank.cpp \
    Sources/main.cpp \
