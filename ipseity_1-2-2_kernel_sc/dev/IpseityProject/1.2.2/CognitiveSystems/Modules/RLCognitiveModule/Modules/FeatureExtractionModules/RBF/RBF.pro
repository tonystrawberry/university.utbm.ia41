#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T20:57:21
#
#-------------------------------------------------

TARGET = RBF
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT

RLTOOLBOX_DIR = ../../../../../../../
COMMON_PATH = $$RLTOOLBOX_DIR/Common/
INCLUDEPATH += $$COMMON_PATH Sources/

win32 {
	OS = win
}

unix {
	OS = unix
}

OBJECTS_DIR = release/

TARGET_DIR = CognitiveSystems/RLCognitiveSystem/
DESTDIR = ../$$RLTOOLBOX_DIR/app_$$OS/Ipseity/Plugins/$$TARGET_DIR


HEADERS += \
	$$COMMON_PATH/*.h \
	$$COMMON_PATH/AL/*.h \
	$$COMMON_PATH/AL/RLCognitiveSystem/FeatureExtractionModules/*.h \
	$$COMMON_PATH/Math/*.h \
	$$COMMON_PATH/IO/*.h \
	Sources/*.h

SOURCES += \
	$$COMMON_PATH/StringFunctions.cpp \
	$$COMMON_PATH/LF_String.cpp \
	$$COMMON_PATH/AL/Stimulus.cpp \
	$$COMMON_PATH/AL/Response.cpp \
	$$COMMON_PATH/AL/RLCognitiveSystem/FeatureExtractionModules/BaseFeatureExtractionModule.cpp \
	$$COMMON_PATH/Math/RandomNumberGenerator.cpp \
	$$COMMON_PATH/IO/VFiles.cpp \
	$$COMMON_PATH/IO/XMLScanner.cpp \
	Sources/main.cpp \
	Sources/RBF.cpp \
	Sources/XMLTags_RBF.cpp
