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


TARGET = Delirium2GameEngine
TARGET_DIR = Environments/Delirium2/

RESOURCES = Delirium2GameEngine.qrc

PROJECT_ROOTDIR = ../../../../

include($$PROJECT_ROOTDIR/mamdp_environment_plugin.pro)

INCLUDEPATH += ../Common/


HEADERS += \
    $$COMMON_PATH/Timer.h \
    $$COMMON_PATH/Math/MathToolBox_Misc.h \
    $$COMMON_PATH/Math/Quaternion.h \
    $$COMMON_PATH/IO/GnuplotInterface.h \
    $$COMMON_PATH/IO/LogFile.h \
    $$COMMON_PATH/IO/ShellCommand.h \
    ../Common/Environment/*.h \
    ../Common/Environment/*.inl \

SOURCES += \
    $$COMMON_PATH/Timer.cpp \
    $$COMMON_PATH/Math/MathToolBox_Misc.cpp \
    $$COMMON_PATH/Math/Quaternion.cpp \
    $$COMMON_PATH/IO/GnuplotInterface.cpp \
    $$COMMON_PATH/IO/LogFile.cpp \
    $$COMMON_PATH/IO/ShellCommand.cpp \
    ../Common/Environment/CardinalDirections.cpp \
    ../Common/Environment/DeliriumAgent.cpp \
    ../Common/Environment/Inventory.cpp \
    ../Common/Environment/DiscreteWorld.cpp \
    ../Common/Environment/Memory.cpp \
    ../Common/Environment/DeliriumCell.cpp \
    ../Common/Environment/DeliriumCellProperties.cpp \
    ../Common/Environment/DiscreteWorldCell.cpp \
    ../Common/Environment/DiscreteWorldCellProperties.cpp \
    ../Common/Environment/GraphicalCell.cpp \
    ../Common/Environment/ItemProperties.cpp \
    ../Common/Environment/ItemsGroups.cpp \
    ../Common/Environment/StaticCell.cpp \
    ../Common/Environment/StaticCellProperties.cpp \
    ../Common/Environment/EnvXMLTags.cpp \
