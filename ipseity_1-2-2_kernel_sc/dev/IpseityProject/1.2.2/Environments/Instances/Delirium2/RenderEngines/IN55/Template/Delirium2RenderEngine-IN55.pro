#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T20:57:21
#
#-------------------------------------------------

TARGET = Delirium2RenderEngine-IN55

QT += opengl
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT

RLTOOLBOX_DIR = ../../../../../../../
COMMON_PATH = $$RLTOOLBOX_DIR/Common/
INCLUDEPATH += $$COMMON_PATH ../../../Common/ ../../../Common/Rendering/ Sources/

win32 {
	APP_DIR = app_win
}

unix {
	APP_DIR = app_unix
}

OBJECTS_DIR = release/
DESTDIR = ../$$RLTOOLBOX_DIR/$$APP_DIR/Ipseity/Environments/Delirium2/


HEADERS += \
	$$COMMON_PATH/*.h \
	$$COMMON_PATH/AL/*.h \
	$$COMMON_PATH/Math/*.h \
	$$COMMON_PATH/IO/*.h \
	$$COMMON_PATH/GL/*.h \
	../../../Common/Rendering/*.h \
	Sources/*.h

SOURCES += \
	$$COMMON_PATH/LF_String.cpp \
	$$COMMON_PATH/StringFunctions.cpp \
	$$COMMON_PATH/Math/RandomNumberGenerator.cpp \
	$$COMMON_PATH/Math/MathToolBox_Misc.cpp \
	$$COMMON_PATH/Math/Quaternion.cpp \
	$$COMMON_PATH/IO/XMLScanner.cpp \
	$$COMMON_PATH/IO/VFiles.cpp \
	../../../Common/Rendering/BaseRenderEngine.cpp \
	Sources/main.cpp \
	Sources/RenderEngine.cpp
