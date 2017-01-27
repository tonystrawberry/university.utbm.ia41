#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T20:57:21
#
#-------------------------------------------------

TARGET = GSBFN
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

TARGET_DIR = CognitiveSystems/CM_RL/
DESTDIR = ../$$RLTOOLBOX_DIR/app/Ipseity/binary/$$OS/plugins/$$TARGET_DIR


HEADERS += \
    $$COMMON_PATH/StringFunctions.h \
    $$COMMON_PATH/LF_String.h \
    $$COMMON_PATH/AL/Stimulus.h \
    $$COMMON_PATH/AL/Response.h \
    $$COMMON_PATH/AL/RLCognitiveSystem/FeatureExtractionModules/BaseFeatureExtractionModule.h \
    $$COMMON_PATH/Math/Cardinality.h \
    $$COMMON_PATH/Math/RandomNumberGenerator.h \
    $$COMMON_PATH/IO/VFiles.h \
    $$COMMON_PATH/IO/XMLScanner.h \
    Sources/GSBFN.h \
    Sources/XMLTags_GSBFN.h \


SOURCES += \
    $$COMMON_PATH/StringFunctions.cpp \
    $$COMMON_PATH/LF_String.cpp \
    $$COMMON_PATH/AL/Stimulus.cpp \
    $$COMMON_PATH/AL/Response.cpp \
    $$COMMON_PATH/AL/RLCognitiveSystem/FeatureExtractionModules/BaseFeatureExtractionModule.cpp \
    $$COMMON_PATH/Math/Cardinality.cpp \
    $$COMMON_PATH/Math/RandomNumberGenerator.cpp \
    $$COMMON_PATH/IO/VFiles.cpp \
    $$COMMON_PATH/IO/XMLScanner.cpp \
    Sources/GSBFN.cpp \
    Sources/XMLTags_GSBFN.cpp \
    Sources/main.cpp \
