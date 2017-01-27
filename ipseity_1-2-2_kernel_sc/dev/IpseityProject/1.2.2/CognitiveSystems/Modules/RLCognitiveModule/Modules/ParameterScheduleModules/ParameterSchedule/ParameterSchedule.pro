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


#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T20:57:21
#
#-------------------------------------------------

TARGET = PS
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT

PROJECT_ROOTDIR = ../../../../../../
COMMON_PATH = $$PROJECT_ROOTDIR/_Common/
INCLUDEPATH += $$COMMON_PATH Sources/

include($$PROJECT_ROOTDIR/common.pro)

DEV_ROOTDIR = ../../../../../../../../

OBJECTS_DIR = release/

TARGET_DIR = CognitiveSystems/CM_RL/
DESTDIR = ../$$DEV_ROOTDIR/app/IpseityProject/binary/$$APP_VERSION/$$OS/plugins/$$TARGET_DIR


HEADERS += \
    $$COMMON_PATH/IpseityString.h \
    Sources/*.h

SOURCES += \
    $$COMMON_PATH/IpseityString.cpp \
    Sources/main.cpp \
    Sources/ParameterSchedule.cpp



