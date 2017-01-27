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


//! Names and scan codes of several keys.
/*!
 *  \author Fabrice LAURI
 */


#ifndef ScanCodes_h
#define ScanCodes_h


#define SC_SPACE			0x20

#define SC_MULTIPLY			0x2A
#define SC_ADD				0x2B
#define SC_COMMA			0x2C
#define SC_SUBTRACT			0x2D
#define SC_PERIOD			0x2E
#define SC_DIVIDE			0x2F

#define SC_0				0x30
#define SC_1				SC_0+1
#define SC_2				SC_0+2
#define SC_3				SC_0+3
#define SC_4				SC_0+4
#define SC_5				SC_0+5
#define SC_6				SC_0+6
#define SC_7				SC_0+7
#define SC_8				SC_0+8
#define SC_9				SC_0+9

#define SC_A				0x41
#define SC_B				SC_A+1
#define SC_C				SC_A+2
#define SC_D				SC_A+3
#define SC_E				SC_A+4
#define SC_F				SC_A+5
#define SC_G				SC_A+6
#define SC_H				SC_A+7
#define SC_I				SC_A+8
#define SC_J				SC_A+9
#define SC_K				SC_A+10
#define SC_L				SC_A+11
#define SC_M				SC_A+12
#define SC_N				SC_A+13
#define SC_O				SC_A+14
#define SC_P				SC_A+15
#define SC_Q				SC_A+16
#define SC_R				SC_A+17
#define SC_S				SC_A+18
#define SC_T				SC_A+19
#define SC_U				SC_A+20
#define SC_V				SC_A+21
#define SC_W				SC_A+22
#define SC_X				SC_A+23
#define SC_Y				SC_A+24
#define SC_Z				SC_A+25

#define SC_ESCAPE			0x100
#define SC_TAB				0x101
#define SC_BACKSPACE		0x103
#define SC_RETURN			0x104
#define SC_ENTER			0x105
#define SC_INSERT			0x106
#define SC_DELETE			0x107
#define SC_PAUSE			0x108
#define SC_CLEAR			0x10B
#define SC_HOME				0x110
#define SC_END				0x111
#define SC_LEFT				0x112
#define SC_UP				0x113
#define SC_RIGHT			0x114
#define SC_DOWN				0x115
#define SC_PAGEUP			0x116
#define SC_PAGEDOWN			0x117
#define SC_SHIFT			0x120
#define SC_CONTROL			0x121
#define SC_ALT				0x123

#define SC_F1				0x130
#define SC_F2				SC_F1+1
#define SC_F3				SC_F1+2
#define SC_F4				SC_F1+3
#define SC_F5				SC_F1+4
#define SC_F6				SC_F1+5
#define SC_F7				SC_F1+6
#define SC_F8				SC_F1+7
#define SC_F9				SC_F1+8
#define SC_F10				SC_F1+9
#define SC_F11				SC_F1+10
#define SC_F12				SC_F1+11

#define SC_NUMPAD0			0x30
#define SC_NUMPAD1			SC_NUMPAD0+1
#define SC_NUMPAD2			SC_NUMPAD0+2
#define SC_NUMPAD3			SC_NUMPAD0+3
#define SC_NUMPAD4			SC_NUMPAD0+4
#define SC_NUMPAD5			SC_NUMPAD0+5
#define SC_NUMPAD6			SC_NUMPAD0+6
#define SC_NUMPAD7			SC_NUMPAD0+7
#define SC_NUMPAD8			SC_NUMPAD0+8
#define SC_NUMPAD9			SC_NUMPAD0+9


typedef struct
{
    const char* Name;
    int Code;
} CodeName;


extern "C" CodeName KeyNames[];


#endif
