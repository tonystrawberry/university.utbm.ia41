# -------------------------------------------------
# Project created by QtCreator 2009-04-27T23:27:38
# -------------------------------------------------

CONFIG += console release
QT += core gui opengl

TARGET = Delirium2Editor
TEMPLATE = app

DEFINES -= UNICODE

EDITOR_DIR = ../../../../../
COMMON_PATH = $$EDITOR_DIR/Common/
INCLUDEPATH += $$COMMON_PATH inc/

OBJECTS_DIR = release/

TARGET_DIR = environments/Delirium2/
DESTDIR = ../$$EDITOR_DIR/app/Ipseity/data/$$TARGET_DIR

HEADERS += \
	$$COMMON_PATH/*.h \
	$$COMMON_PATH/AL/*.h \
	$$COMMON_PATH/IO/*.h \
	$$COMMON_PATH/Math/*.h \
	inc/*.h

SOURCES += src/main.cpp \
	src/MainWindow.cpp \
	src/GLWidget.cpp \
	src/MyXMLTags.cpp \
	src/ItemInfos.cpp \
	src/GlErrors.cpp \
	src/BeyondGLTexturesManager.cpp \
	src/BeyondGLResources.cpp \
	src/BeyondGLMesh.cpp \
	src/BeyondGLLoader3ds.cpp \
	src/DeliriumStaticCell.cpp \
	src/StringCollection.cpp \
	src/GameDifficultyInfos.cpp \
	$$COMMON_PATH/StringFunctions.cpp \
	$$COMMON_PATH/LF_String.cpp \
	$$COMMON_PATH/Math/Quaternion.cpp \
	$$COMMON_PATH/IO/FileSearch.cpp \
	$$COMMON_PATH/IO/VFiles.cpp \
	$$COMMON_PATH/IO/XMLScanner.cpp \
    src/CellGrid.cpp
