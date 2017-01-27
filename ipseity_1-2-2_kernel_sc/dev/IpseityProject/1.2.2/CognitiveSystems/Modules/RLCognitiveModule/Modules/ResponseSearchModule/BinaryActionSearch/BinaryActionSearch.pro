TARGET = BAS

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
    $$COMMON_PATH/*.h \
    $$COMMON_PATH/AL/*.h \
    $$COMMON_PATH/AL/RLCognitiveSystem/FeatureExtractionModules/*.h \
    $$COMMON_PATH/Math/*.h \
    $$COMMON_PATH/IO/*.h \
    Sources/*.h \

SOURCES += \
    $$COMMON_PATH/StringFunctions.cpp \
    $$COMMON_PATH/LF_String.cpp \
    $$COMMON_PATH/AL/Stimulus.cpp \
    $$COMMON_PATH/AL/Response.cpp \
    $$COMMON_PATH/Math/Cardinality.cpp \
    $$COMMON_PATH/Math/RandomNumberGenerator.cpp \
    $$COMMON_PATH/IO/VFiles.cpp \
    $$COMMON_PATH/IO/XMLScanner.cpp \
    $$COMMON_PATH/AL/RLCognitiveSystem/Policy/BinaryDecisionSequence.cpp \
    Sources/main.cpp \
    Sources/BinaryActionSearch.cpp \
    Sources/XMLTags_BinaryActionSearch.cpp
