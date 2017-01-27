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


TARGET = Delirium2RenderEngine-FL
TARGET_DIR = Environments/Delirium2/

QT += opengl

PROJECT_ROOTDIR = ../../../../../../

include($$PROJECT_ROOTDIR/common.pro)

INCLUDEPATH += ../../../Common/ ../../../Common/Rendering/ Sources/ "$$DEV_ROOTDIR/API/$$OS/glew-mingw/include"
LIBS += -L"$$DEV_ROOTDIR/API/$$OS/glew-mingw/lib" -l$$GLEW_LIB

include($$PROJECT_ROOTDIR/common_plugin.pro)

HEADERS += \
    $$COMMON_PATH/Math/MathToolBox_Misc.h \
    $$COMMON_PATH/Math/Quaternion.h \
    $$COMMON_PATH/GL/GlFramework.h \
    ../../../Common/Rendering/*.h \
    Sources/RenderEngine.h \

SOURCES += \
    $$COMMON_PATH/Math/MathToolBox_Misc.cpp \
    $$COMMON_PATH/Math/Quaternion.cpp \
    $$COMMON_PATH/GL/GlFramework.cpp \
    $$COMMON_PATH/GL/Shapes/AbstractShape.cpp \
    ../../../Common/Rendering/MyXMLTags.cpp \
    ../../../Common/Rendering/BaseRenderEngineFL.cpp \
    ../../../Common/Rendering/BeyondGLCamera.cpp \
    ../../../Common/Rendering/BeyondGLFont.cpp \
    ../../../Common/Rendering/BeyondGLGraphicalEntityManager.cpp \
    ../../../Common/Rendering/BeyondGLFunctions.cpp \
    ../../../Common/Rendering/BeyondGLLoader3ds.cpp \
    ../../../Common/Rendering/BeyondGLResources.cpp \
    ../../../Common/Rendering/BeyondGLTexturesManager.cpp \
    ../../../Common/Rendering/BeyondGL_GraphicalEntity.cpp \
    Sources/RenderEngine.cpp \
    Sources/main.cpp \
