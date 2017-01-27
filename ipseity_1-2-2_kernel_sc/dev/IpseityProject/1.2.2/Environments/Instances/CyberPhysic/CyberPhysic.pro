#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T20:57:21
#
#-------------------------------------------------

TARGET = CyberPhysic

QT += opengl
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT

RLTOOLBOX_DIR = ../../../../
COMMON_PATH = $$RLTOOLBOX_DIR/Common/

win32 {
	OS = win
}

unix {
	OS = unix
}

INCLUDEPATH += $$COMMON_PATH $$RLTOOLBOX_DIR/API/$$OS/glew-mingw/include/
LIBS += -L$$RLTOOLBOX_DIR/API/$$OS/glew-mingw/lib/ -lglew32

OBJECTS_DIR = release/

TARGET_DIR = Environments/$$TARGET/
DESTDIR = ../$$RLTOOLBOX_DIR/app/Ipseity/binary/$$OS/plugins/$$TARGET_DIR


HEADERS += \
	$$COMMON_PATH/*.h \
	$$COMMON_PATH/AL/*.h \
	$$COMMON_PATH/Math/*.h \
	$$COMMON_PATH/IO/*.h \
	$$COMMON_PATH/GL/*.h \
	Sources/*.h

SOURCES += \
	$$COMMON_PATH/LF_String.cpp \
	$$COMMON_PATH/Math/RandomNumberGenerator.cpp \
	$$COMMON_PATH/StringFunctions.cpp \
	$$COMMON_PATH/AL/Stimulus.cpp \
	$$COMMON_PATH/AL/Response.cpp \
	$$COMMON_PATH/AL/EnvironmentInstance.cpp \
	$$COMMON_PATH/IO/XMLScanner.cpp \
	$$COMMON_PATH/IO/VFiles.cpp \
        Sources/CyberPhysic.cpp \
	Sources/main.cpp \
