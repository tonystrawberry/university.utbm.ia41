/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2014, 2013 Ipseity Core Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef InputDevices_h
#define InputDevices_h


#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <vector>
#include <map>
#include <string>

#include "Types.h"
#include "Math/Vectors.h"

#include "IO/ScanCodes.h"


const int MaxJoysticks = 8;
const uint32 MaxKeys = 1024;


class InputDevices
{
    public:
        static void setGeneralActions( uint32 nbrGeneralActions, CodeName* generalActions );
        static void setCommonActions( uint32 nbrCommonActions, CodeName* commonActions );
        static void setPlayerActions( uint32 nbrPlayers, uint32 nbrPlayerActions, CodeName* playerActions );

        static void loadBindings( const char* configfile, const char* base_path );

        static HRESULT init( HWND hDlg, bool fs );
        static void destroy();
        static void clear();

        static void setJoystickSensibility( int, int );
        static void setPlayerJoystick( int, int32 );

        static void acquireInputDevices();
        static void reAcquireInputDevices();

        static bool isGeneralActionOccured( int32 actionId, bool kst = false, bool countKeysDown = true );
        static bool isCommonActionOccured( int32 actionId, bool kst = false, bool countKeysDown = true );
        static bool isPlayerActionOccured( uint32 nrp, int32 actionId, bool kst = false, bool countKeysDown = true );

    protected:
        static int32 getAvatarActionId( uint32 nrp, int32 actionId );
        static int32 getActionId( const char* );

        static bool isKeyDown( uint32 k );
        static int32 getKeyCode( char* );

        static uint32 getNbrKeysDown();

        static void setKeys( uint32 nr, int32 actionId, char* keyset );
        static void setKey( uint32 nr, bool v );

        static bool isSingleActionOccured( uint32 nrb, int32 actionId, bool );
        static bool isActionOccured( int32 actionId, bool kst = false, bool countKeysDown = true );

        static void acquireKeyboardData();
        static void acquireMouseData();
        static HRESULT acquireJoystickData( uint32 nr );

        static HRESULT configureJoystick( HWND hDlg, uint32 nr );
        static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
        static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );


    public:
        static LPDIRECTINPUT8 g_pDI;

        static LPDIRECTINPUTDEVICE8 g_pJoystick[MaxJoysticks];
        static LPDIRECTINPUTDEVICE8 g_Keyboard;
        static LPDIRECTINPUTDEVICE8 g_Mouse;

        static uint32 g_NbrJoysticks, g_NrJoystick;
        static int g_JoystickSensibility[MaxJoysticks];
        static int32 g_PlayerJoystick[MaxJoysticks];

        static Vec2i g_MouseRelativePosition;

        // Arrays used for the Keyboard Routines
        static unsigned char g_Keys[MaxKeys];
        static uint32 g_PlayerId;

        static uint32 g_NbrGeneralActions;
        static CodeName* g_GeneralActions;

        static uint32 g_NbrCommonActions;
        static CodeName* g_CommonActions;

        static uint32 g_NbrPlayers;
        static uint32 g_NbrPlayerActions;
        static CodeName* g_PlayerActions;

        static std::vector<int32>* g_Bindings[2];
        static uint32 g_NbrActions;
        static bool* g_PressedInputActions;
};


#endif
