#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T20:57:21
#
#-------------------------------------------------

TARGET = Delirium2RenderEngine-IN55-2012-01

QT += opengl
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT

RLTOOLBOX_DIR = ../../../../../../../../
COMMON_PATH = $$RLTOOLBOX_DIR/Common/
INCLUDEPATH += Sources/ ./ $$COMMON_PATH ../../../../Common/ ../../../../Common/Rendering/ "$$RLTOOLBOX_DIR/API/glew-mingw/include"
win32:LIBS += -L"$$RLTOOLBOX_DIR/API/glew-mingw/lib" -lglew32 C:/MinGW/lib/tinystr.o -Llibs -ltinyxml  -ltinyxmlerror  -ltinyxmlparser

OBJECTS_DIR = release/
DESTDIR = ../$$RLTOOLBOX_DIR/app/Ipseity/Environments/Delirium2/

SOURCES += \
	$$COMMON_PATH/LF_String.cpp \
	$$COMMON_PATH/StringFunctions.cpp \
	$$COMMON_PATH/Math/RandomNumberGenerator.cpp \
	$$COMMON_PATH/Math/MathToolBox_Misc.cpp \
	$$COMMON_PATH/Math/Quaternion.cpp \
	$$COMMON_PATH/IO/XMLScanner.cpp \
	$$COMMON_PATH/IO/VFiles.cpp \
	Sources/BaseRenderEngine.cpp \
	Sources/main.cpp \
	Sources/RenderEngine.cpp \
    Sources/Entity3d.cpp \
    Sources/Camera.cpp \
    Sources/tga.cpp \
    Sources/Entity3dManager.cpp \
	Sources/GlFramework.cpp \
    Sources/TextRenderer.cpp \
    Sources/QuatCamera.cpp \
    Sources/monster.cpp


HEADERS += \
	$$COMMON_PATH/*.h \
	$$COMMON_PATH/AL/*.h \
	$$COMMON_PATH/Math/*.h \
	$$COMMON_PATH/IO/*.h \
	$$COMMON_PATH/GL/*.h \
	../../../../Common/Rendering/*.h \
	Sources/*.h \
#    Sources/Entity3d.h \
#    Sources/globals.h \
#    Sources/tga.h \
#    Sources/Entity3dManager.h \
#    Sources/TextRenderer.h \
#    Sources/QuatCamera.h


symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE5066609
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = DLL.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}





