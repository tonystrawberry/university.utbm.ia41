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


#ifndef BitVector_h
#define BitVector_h


#include "Types.h"


class BitVector
{
    public:
        BitVector();
        BitVector( int32 );
        ~BitVector();

        void setSize( int32 );

        bool isSet( int32 ) const;

        void reset();
        void fill();

        void set( int32 );
        void unset( int32 );
        void flip( int32 );

        void display();


    protected:
        int32 m_NbrBits;
        int32 m_Size;
        uint8* m_Field;
};


#endif
