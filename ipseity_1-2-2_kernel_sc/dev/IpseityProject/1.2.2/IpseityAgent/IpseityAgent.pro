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
# Project created by QtCreator 2012-11-20T14:50:54
#
#-------------------------------------------------

TARGET = IpseityAgent
TEMPLATE = app
VERSION = 1.0.0

APP_VERSION=1.0.0

RC_FILE += IpseityAgent.rc

CONFIG += release
QT     += core gui network

DEFINES -= UNICODE
DEFINES += QT

win32 {
	OS = win
	GLEW_LIB = glew32
}

unix {
	OS = unix
	GLEW_LIB = GLEW
}

DEV_ROOTDIR = ../../../
COMMON_PATH = $$DEV_ROOTDIR/Common/
INCLUDEPATH += $$COMMON_PATH Sources/

OBJECTS_DIR = release
DESTDIR = ../$$DEV_ROOTDIR/app/IpseityProject/binary/$$APP_VERSION/$$OS/


SOURCES += \
    $$COMMON_PATH/LF_String.cpp \
    $$COMMON_PATH/AL/Stimulus.cpp \
    $$COMMON_PATH/AL/Response.cpp \
    Sources/AgentClient.cpp \
    mainwindow.cpp \
    main.cpp \


HEADERS  += \
    $$COMMON_PATH/LF_String.h \
    $$COMMON_PATH/AL/Stimulus.h \
    $$COMMON_PATH/AL/Response.h \
    Sources/AgentClient.h \
    mainwindow.h \
