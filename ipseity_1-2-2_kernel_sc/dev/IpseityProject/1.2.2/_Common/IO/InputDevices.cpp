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

 
#include "InputDevices.h"


#include <string>
#include <vector>

#include "MemDefns.h"

#include "Math/Vectors.h"
#include "IO/XMLScanner.h"

#include "IO/ApplicationLogFile.h"

#include <iostream>


using namespace std;


LPDIRECTINPUT8 InputDevices::g_pDI = NULL;

LPDIRECTINPUTDEVICE8 InputDevices::g_pJoystick[MaxJoysticks];
LPDIRECTINPUTDEVICE8 InputDevices::g_Keyboard = NULL;
LPDIRECTINPUTDEVICE8 InputDevices::g_Mouse = NULL;

uint32 InputDevices::g_NbrJoysticks = 0, InputDevices::g_NrJoystick;
int InputDevices::g_JoystickSensibility[MaxJoysticks];
int32 InputDevices::g_PlayerJoystick[MaxJoysticks];

Vec2i InputDevices::g_MouseRelativePosition;

unsigned char InputDevices::g_Keys[MaxKeys];
uint32 InputDevices::g_PlayerId;


uint32 InputDevices::g_NbrGeneralActions = 0;
CodeName* InputDevices::g_GeneralActions = NULL;

uint32 InputDevices::g_NbrCommonActions = 0;
CodeName* InputDevices::g_CommonActions = NULL;

uint32 InputDevices::g_NbrPlayers = 0;
uint32 InputDevices::g_NbrPlayerActions = 0;
CodeName* InputDevices::g_PlayerActions = NULL;

std::vector<int32>* InputDevices::g_Bindings[2] = { NULL, NULL };
uint32 InputDevices::g_NbrActions;
bool* InputDevices::g_PressedInputActions = NULL;


void
InputDevices::setGeneralActions( uint32 nbrGeneralActions, CodeName* generalActions )
{
	g_NbrGeneralActions = nbrGeneralActions;
	g_GeneralActions = generalActions;
}

void
InputDevices::setCommonActions( uint32 nbrCommonActions, CodeName* commonActions )
{
	g_NbrCommonActions = nbrCommonActions;
	g_CommonActions = commonActions;
}

void
InputDevices::setPlayerActions( uint32 nbrPlayers, uint32 nbrPlayerActions, CodeName* playerActions )
{
	g_NbrPlayers = nbrPlayers;
	g_NbrPlayerActions = nbrPlayerActions;
	g_PlayerActions = playerActions;

	for( uint32 i = 0; i < nbrPlayers; ++i )
	{
		InputDevices::setPlayerJoystick( i, -1 );
	}
}


void
InputDevices::loadBindings( const char* config_file, const char* base_path )
{
	g_NbrActions = g_NbrGeneralActions + g_NbrCommonActions + g_NbrPlayers*g_NbrPlayerActions;

	SafeDeleteArray( g_Bindings[0] );
	g_Bindings[0] = new std::vector<int32> [g_NbrActions];
	SafeDeleteArray( g_Bindings[1] );
	g_Bindings[1] = new std::vector<int32> [g_NbrActions];

	SafeDeleteArray( g_PressedInputActions );
	g_PressedInputActions = new bool [g_NbrActions];


	cout << "# actions: " << g_NbrActions << endl;
	cout << "# general actions: " << g_NbrGeneralActions << endl;
	cout << "# common actions: " << g_NbrCommonActions << endl;
	cout << "# player actions: " << g_NbrPlayerActions << endl;


	XMLScanner scanner( config_file, base_path );

	if (!scanner.isLoaded())
	{
		cout << "File " << config_file << " not loaded!" << endl;
		return;
	}

	XMLScanner::Token t;
	int32 actionId;
	char keyset[128];
	uint32 nrj;

	while( scanner.nextToken( &t ) )
	{
		switch( t.kind )
		{
			case XMLScanner::Token::Tag:
				break;

			case XMLScanner::Token::Attribute:
				if( t.keyIs( "Id" ) )
				{
					nrj = t.intValue()-1;
				}
				else if( t.keyIs( "Sensibility" ) )
				{
					InputDevices::setJoystickSensibility( nrj, t.intValue() );
				}
				else if( t.keyIs( "Player" ) )
				{
					InputDevices::setPlayerJoystick( t.intValue()-1, nrj );
				}
				else if( t.keyIs( "Name" ) )
				{
					actionId = getActionId( t.stringValue() );
				}
				else if( t.keyIs( "Key1" ) )
				{
					if( actionId >= 0 )
					{
						strncpy( keyset, t.value, t.valueSize );
						keyset[t.valueSize] = '\0';
						if( strlen( keyset ) )
						{
							setKeys( 0, actionId, keyset );
						}
					}
				}
				else if( t.keyIs( "Key2" ) )
				{
					if( actionId >= 0 )
					{
						strncpy( keyset, t.value, t.valueSize );
						keyset[t.valueSize] = '\0';
						if( strlen( keyset ) )
						{
							setKeys( 1, actionId, keyset );
						}
					}
				}
				break;
		}
	}
}


int32 InputDevices::getActionId( const char* actionName )
{
	uint32 i;
	int32 actionIdShift;

//	cout << actionName << ":";

	for( i = 0; i < g_NbrGeneralActions; ++i )
	{
		if( !strcmp( actionName, g_GeneralActions[i].Name ) )
		{
//			cout << "1:" << g_GeneralActions[i].Name << endl;
//			cout << g_GeneralActions[i].Code << endl;

			return g_GeneralActions[i].Code;
		}
	}

	actionIdShift = g_NbrGeneralActions;

	for( i = 0; i < g_NbrCommonActions; ++i )
	{
		if( !strcmp( actionName, g_CommonActions[i].Name ) )
		{
//			cout << "2:" << g_CommonActions[i].Name << endl;
//			cout << actionIdShift+g_CommonActions[i].Code << endl;

			return actionIdShift+g_CommonActions[i].Code;
		}
	}

	if( strstr( actionName, "Player" ) )
	{
		char actionSuffix[128];
//		int32 nrPlayer;

		sscanf( actionName, "Player%d%s", &g_PlayerId, actionSuffix );

		--g_PlayerId;
//		cout << "* " << nrPlayer << ", " << actionSuffix;

		if( g_PlayerId >= 0 )
		{
			actionIdShift += g_NbrCommonActions+g_PlayerId*g_NbrPlayerActions;

//			cout << " " << actionIdShift;

			for( i = 0; i < g_NbrPlayerActions; ++i )
			{
				if( !strcmp( actionSuffix, g_PlayerActions[i].Name ) )
				{
//					cout << " * ";
//					cout << actionIdShift+g_PlayerActions[i].Code << endl;

					return actionIdShift+g_PlayerActions[i].Code;
				}
			}
		}
	}

//	cout << " * " << -1 << endl;

	return -1;
}


bool InputDevices::isKeyDown( uint32 k )
{
/*
	if( (g_Keys[k] & 0x80) )
	{
//		cout << k << endl;
	}
*/
	return( (g_Keys[k] & 0x80) ? true : false );
}


int32 InputDevices::getKeyCode( char* keyname )
{
	int32 button;

	if( strstr( keyname, "MOUSE_BUTTON" ) )
	{
		if( sscanf( keyname, "MOUSE_BUTTON%d", &button ) == 1 )
		{
			return DIK_MOUSE_BUTTON+button;
		}
		else
		{
			return DIK_MOUSE_BUTTON+1;
		}
	}
	else if( strstr( keyname, "JOYPAD" ) )
	{
		int32 nrJoypad;

		if( sscanf( keyname, "JOYPAD%d", &nrJoypad ) == 1 )
		{
			--nrJoypad;
		}
		else
		{
			nrJoypad = g_PlayerJoystick[g_PlayerId];
		}

		keyname = strstr( keyname, "_" ) + 1;

		if( nrJoypad == -1 )
		{
			return -1;
		}

		int32 codeShift = nrJoypad*DIK_JOYPAD_INFOS_SIZE;

		if( !strcmp( keyname, "LEFT" ) )
		{
			return codeShift+DIK_JOYPAD1_LEFT;
		}
		else if( !strcmp( keyname, "RIGHT" ) )
		{
			return codeShift+DIK_JOYPAD1_RIGHT;
		}
		else if( !strcmp( keyname, "UP" ) )
		{
			return codeShift+DIK_JOYPAD1_UP;
		}
		else if( !strcmp( keyname, "DOWN" ) )
		{
			return codeShift+DIK_JOYPAD1_DOWN;
		}
		else
		{
			if( sscanf( keyname, "BUTTON%d", &button ) == 1 )
			{
				return codeShift+DIK_JOYPAD1_BUTTON+button;
			}
			else
			{
				return -1;
			}
		}
	}

	uint32 i = 0;

	while( strcmp( KeyNames[i].Name, "" ) && strcmp( KeyNames[i].Name, keyname ) )
	{
		i++;
	}

	return KeyNames[i].Code;
}


uint32 InputDevices::getNbrKeysDown()
{
	uint32 nbr = 0;

	for( uint32 i = 0; i < DIK_SPECIAL_FIRST_KEYS; i++ )
	{
		if( (g_Keys[i] & 0x80) )
		{
			nbr++;
		}
	}

	return nbr;
}


void InputDevices::setKey( uint32 nr, bool v )
{
	g_Keys[nr] = v ? 0x80 : 0x00;
}


void InputDevices::setKeys( uint32 nr, int32 actionId, char* keyset )
{
	char* key;
	char* ptr;
	int32 keyCode;

	key = keyset;
	do
	{
		ptr = strstr( key, "+" );

		if( ptr )
		{
			*ptr = '\0';
			ptr++;
		}

		keyCode = InputDevices::getKeyCode( key );

		if( keyCode > 0 )
		{
			g_Bindings[nr][actionId].push_back( keyCode );

//			cout << key << "=" << keyCode << " " << g_Bindings[nr][actionId].size() << endl;
		}


		key = ptr;
	}
	while( ptr );
}


bool InputDevices::isSingleActionOccured( uint32 nrb, int32 actionId, bool countKeysDown )
{
	std::vector<int32> & k = g_Bindings[nrb][actionId];
	uint32 nbKeys = k.size();

	if( nbKeys )
	{
		uint32 nb = 0;

		for( uint32 i = 0; i < nbKeys; i++ )
		{
			nb += (InputDevices::isKeyDown( k[i] ) ? 1 : 0);
		}

		if( countKeysDown )
		{

			return ((nb == nbKeys) && (getNbrKeysDown() == nbKeys));
		}
		else
		{
			return (nb == nbKeys);
		}
	}
	else
	{
		return false;
	}
}

bool InputDevices::isActionOccured( int32 actionId, bool kst, bool countKeysDown )
{
	bool kp;

	kp = (InputDevices::isSingleActionOccured( 0, actionId, countKeysDown ) || 
		  InputDevices::isSingleActionOccured( 1, actionId, countKeysDown ));

	if( kst )
	{
		if( kp )
		{
			if( !g_PressedInputActions[actionId] )
			{
				g_PressedInputActions[actionId] = true;

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			g_PressedInputActions[actionId] = false;

			return false;
		}
	}
	else
	{
		return kp;
	}
}


bool InputDevices::isGeneralActionOccured( int32 actionId, bool kst, bool countKeysDown )
{
	return isActionOccured( actionId, kst, countKeysDown );
}

bool InputDevices::isCommonActionOccured( int32 actionId, bool kst, bool countKeysDown )
{
	return isActionOccured( g_NbrGeneralActions + actionId, kst, countKeysDown );
}

bool InputDevices::isPlayerActionOccured( uint32 nrp, int32 actionId, bool kst, bool countKeysDown )
{
	int32 aId = g_NbrGeneralActions + g_NbrCommonActions + nrp*g_NbrPlayerActions + actionId;

	return isActionOccured( aId, kst, countKeysDown );
}


void InputDevices::setJoystickSensibility( int nr, int s )
{
	g_JoystickSensibility[nr] = s;
}

void InputDevices::setPlayerJoystick( int nr, int32 n )
{
	g_PlayerJoystick[nr] = n;
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK InputDevices::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick[g_NbrJoysticks++], NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_CONTINUE;
}


//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK InputDevices::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 

        // Set the range for the axis
        if( FAILED( g_pJoystick[g_NrJoystick]->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}


HRESULT InputDevices::configureJoystick( HWND hDlg, uint32 nr )
{
	HRESULT hr;

	// Make sure we got a joystick
    if( NULL == g_pJoystick[nr] )
    {
        return S_OK;
    }

    // Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    if( FAILED( hr = g_pJoystick[nr]->SetDataFormat( &c_dfDIJoystick2 ) ) )
        return hr;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if( FAILED( hr = g_pJoystick[nr]->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
        return hr;

    // Enumerate the joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
	g_NrJoystick = nr;
    if( FAILED( hr = g_pJoystick[nr]->EnumObjects( EnumObjectsCallback, NULL, DIDFT_ALL ) ) ) //(VOID*)hDlg
        return hr;

	InputDevices::setJoystickSensibility( nr, 500 );

	return S_OK;
}


HRESULT InputDevices::init( HWND hDlg, bool fs )
{
/*
	g_ApplicationLogFile->outsep();
	g_ApplicationLogFile->outln( "Initializing Input Devices..." );
*/
	memset( g_Keys, 0, MaxKeys );
	g_NbrJoysticks = 0;

	memset( g_PressedInputActions, 0, g_NbrActions*sizeof( bool ) );

	clear();

	HRESULT hr;
	int status;

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        return hr;


	// ------------- Keyboard
	//
    // Create and initialize the keyboard device
    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysKeyboard, &g_Keyboard, NULL ) ) )
    {
        return hr;
    }

    if( FAILED( hr = g_Keyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
    {
        return hr;
    }

	if( fs )
	{
		status = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
	}
	else
	{
		status = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY;
	}

    if( FAILED( hr = g_Keyboard->SetCooperativeLevel( hDlg, status ) ) )
    {
        return hr;
    }

    if( FAILED( hr = g_Keyboard->Acquire() ) )
    {
        return hr;
    }


	// ------------- Mouse
	//
    // Create and initialize the mouse device
    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysMouse, &g_Mouse, NULL ) ) )
    {
        return hr;
    }

    if( FAILED( hr = g_Mouse->SetDataFormat( &c_dfDIMouse2 ) ) )
    {
        return hr;
    }

	if( fs )
	{
		status = DISCL_FOREGROUND | DISCL_EXCLUSIVE;
	}
	else
	{
		status = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
	}

    if( FAILED( hr = g_Mouse->SetCooperativeLevel( hDlg, status ) ) )
    {
        return hr;
    }

    if( FAILED( hr = g_Mouse->Acquire() ) )
    {
        return hr;
    }


	// ------------- Joystick
	//
    // Look for the installed joysticks
    if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY ) ) )
        return hr;

//	g_ApplicationLogFile->outln( "  %d joystick(s) installed!", g_NbrJoysticks );

	for( uint32 nr = 0; nr < g_NbrJoysticks; nr++ )
	{
		InputDevices::configureJoystick( hDlg, nr );
	}
/*
	g_ApplicationLogFile->outsep();
	g_ApplicationLogFile->outln();
*/
    return S_OK;
}


void InputDevices::acquireKeyboardData()
{
	if( NULL == g_Keyboard )
	{
		return;
	}

	HRESULT hr;

	hr = g_Keyboard->Acquire();

	// Obtain input from the keyboard
    if( g_Keyboard->GetDeviceState( 256, (LPVOID) &g_Keys ) == DIERR_INPUTLOST )
    {
        while( hr == DIERR_INPUTLOST )
		{
            hr = g_Keyboard->Acquire();
		}

		g_Keyboard->GetDeviceState( 256, (LPVOID) &g_Keys );
	}

	g_Keys[DIK_SHIFT] = (g_Keys[DIK_LSHIFT] || g_Keys[DIK_RSHIFT]) ? 0x80 : 0;
	g_Keys[DIK_CONTROL] = (g_Keys[DIK_LCONTROL] || g_Keys[DIK_RCONTROL]) ? 0x80 : 0;
	g_Keys[DIK_MENU] = (g_Keys[DIK_LMENU] || g_Keys[DIK_RMENU]) ? 0x80 : 0;
}

void InputDevices::acquireMouseData()
{
	if( NULL == g_Mouse )
	{
		return;
	}

	HRESULT hr;
    DIMOUSESTATE2 dims2;

	hr = g_Mouse->Acquire();

    ZeroMemory( &dims2, sizeof( dims2 ) );
    hr = g_Mouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), &dims2 );
    if( FAILED( hr ) )
    {
		do
		{
			hr = g_Mouse->Acquire();
		}
		while( hr == DIERR_INPUTLOST );

		g_MouseRelativePosition.x = 0;
		g_MouseRelativePosition.y = 0;
    }
	else
	{
		g_MouseRelativePosition.x = dims2.lX;
		g_MouseRelativePosition.y = dims2.lY;

		for( int i = 0; i < 8; i++ )
		{
			InputDevices::setKey( DIK_MOUSE_BUTTON+i+1, (dims2.rgbButtons[i] & 0x80) );
		}
	}
}

HRESULT InputDevices::acquireJoystickData( uint32 nr )
{
    if( NULL == g_pJoystick[nr] )
	{
        return S_OK;
	}

	HRESULT     hr;
    DIJOYSTATE2 js;           // DInput joystick state 

	hr = g_pJoystick[nr]->Acquire();

	// Poll the device to read the current state
    hr = g_pJoystick[nr]->Poll(); 
    if( FAILED(hr) )  
    {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        do
		{
            hr = g_pJoystick[nr]->Acquire();
		}
		while( hr == DIERR_INPUTLOST );

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK; 
    }

    // Get the input's device state
    if( FAILED( hr = g_pJoystick[nr]->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) )
        return hr; // The device should have been acquired during the Poll()


	static int oldX = 0, oldY = 0;
	static int shiftX, shiftY;

	shiftX = js.lX; // - oldX;
	shiftY = js.lY; // - oldY;

	oldX = js.lX;
	oldY = js.lY;

	uint32 loc = DIK_JOYPAD_INFOS_SIZE*nr;

	if( shiftX >= g_JoystickSensibility[nr] )
	{
		InputDevices::setKey( DIK_JOYPAD1_RIGHT+loc, true );
	}
	else if( shiftX <= -g_JoystickSensibility[nr] )
	{
		InputDevices::setKey( DIK_JOYPAD1_LEFT+loc, true );
	}
	else
	{
		InputDevices::setKey( DIK_JOYPAD1_LEFT+loc, false );
		InputDevices::setKey( DIK_JOYPAD1_RIGHT+loc, false );
	}

	if( shiftY >= g_JoystickSensibility[nr] )
	{
		InputDevices::setKey( DIK_JOYPAD1_DOWN+loc, true );
	}
	else if( shiftY <= -g_JoystickSensibility[nr] )
	{
		InputDevices::setKey( DIK_JOYPAD1_UP+loc, true );
	}
	else
	{
		InputDevices::setKey( DIK_JOYPAD1_UP+loc, false );
		InputDevices::setKey( DIK_JOYPAD1_DOWN+loc, false );
	}

    for( int i = 0; i < DIK_JOYPAD_INFOS_SIZE-4; i++ )
    {
        if ( js.rgbButtons[i] & 0x80 )
		{
//			cout << "JP B" << i+1 << endl;
			InputDevices::setKey( loc+DIK_JOYPAD1_BUTTON+(i+1), true );
		}
		else
		{
			InputDevices::setKey( loc+DIK_JOYPAD1_BUTTON+(i+1), false );
		}
    }



    // Display joystick state to dialog
/*
    // Axes
    wsprintf( strText, TEXT("%ld"), js.lX ); 
    SetWindowText( GetDlgItem( hDlg, IDC_X_AXIS ), strText );
    wsprintf( strText, TEXT("%ld"), js.lY ); 
    SetWindowText( GetDlgItem( hDlg, IDC_Y_AXIS ), strText );
    wsprintf( strText, TEXT("%ld"), js.lZ ); 
    SetWindowText( GetDlgItem( hDlg, IDC_Z_AXIS ), strText );
    wsprintf( strText, TEXT("%ld"), js.lRx ); 
    SetWindowText( GetDlgItem( hDlg, IDC_X_ROT ), strText );
    wsprintf( strText, TEXT("%ld"), js.lRy ); 
    SetWindowText( GetDlgItem( hDlg, IDC_Y_ROT ), strText );
    wsprintf( strText, TEXT("%ld"), js.lRz ); 
    SetWindowText( GetDlgItem( hDlg, IDC_Z_ROT ), strText );

    // Slider controls
    wsprintf( strText, TEXT("%ld"), js.rglSlider[0] ); 
    SetWindowText( GetDlgItem( hDlg, IDC_SLIDER0 ), strText );
    wsprintf( strText, TEXT("%ld"), js.rglSlider[1] ); 
    SetWindowText( GetDlgItem( hDlg, IDC_SLIDER1 ), strText );

    // Points of view
    wsprintf( strText, TEXT("%ld"), js.rgdwPOV[0] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV0 ), strText );
    wsprintf( strText, TEXT("%ld"), js.rgdwPOV[1] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV1 ), strText );
    wsprintf( strText, TEXT("%ld"), js.rgdwPOV[2] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV2 ), strText );
    wsprintf( strText, TEXT("%ld"), js.rgdwPOV[3] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV3 ), strText );
 
   
    // Fill up text with which buttons are pressed
    str = strText;
    for( int i = 0; i < 128; i++ )
    {
        if ( js.rgbButtons[i] & 0x80 )
            str += wsprintf( str, TEXT("%02d "), i );
    }
    *str = 0;   // Terminate the string 

    SetWindowText( GetDlgItem( hDlg, IDC_BUTTONS ), strText );
*/
	return S_OK;
}


void InputDevices::acquireInputDevices()
{
	acquireKeyboardData();

	acquireMouseData();

	for( uint32 nr = 0; nr < g_NbrJoysticks; nr++ )
	{
		acquireJoystickData( nr );
	}
}


void InputDevices::reAcquireInputDevices()
{
	if( g_Keyboard )
	{
		g_Keyboard->Acquire();
	}

	if( g_Mouse )
	{
		g_Mouse->Acquire();
	}

	for( uint32 nr = 0; nr < g_NbrJoysticks; nr++ )
	{
		if( g_pJoystick[nr] )
		{
			g_pJoystick[nr]->Acquire();
		}
	}
}


//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
void InputDevices::clear()
{
    if( g_Keyboard )
	{
        g_Keyboard->Unacquire();
	    SafeRelease( g_Keyboard );
	}

    if( g_Mouse )
	{
        g_Mouse->Unacquire();
	    SafeRelease( g_Mouse );
	}

	for( uint32 nr = 0; nr < g_NbrJoysticks; nr++ )
	{
		if( g_pJoystick[nr] )
		{
		    g_pJoystick[nr]->Unacquire();
		}
	    SafeRelease( g_pJoystick[nr] );
	}

	SafeRelease( g_pDI );
}


void InputDevices::destroy()
{
	clear();

	SafeDeleteArray( g_Bindings[0] );
	SafeDeleteArray( g_Bindings[1] );
	SafeDeleteArray( g_PressedInputActions );
}
