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


#include <QApplication>

#include "MainWindow.h"
#include "Types.h"

#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int instance_id = 0;

    if (argc > 1)
    {
        instance_id = atoi( argv[1] );
    }
    else
    {
        QSharedMemory shared_memory( "ipseity" );
        int* ptr;
        bool ret_sm;

        ret_sm = shared_memory.create( sizeof( int ) );
//        shared_memory.setKey( "ipseity" );
//        if (shared_memory.attach( QSharedMemory::ReadWrite ))
        {
            shared_memory.lock();
            ptr = (int*) shared_memory.data();

            if (ret_sm)
            {
                cout << "0" << endl;
    //            shared_memory.lock();
                *ptr = 0;
    //            shared_memory.unlock();
            }
            else
            {
                cout << "> 0" << endl;
    //            shared_memory.lock();
//                ++(*ptr);
    //            shared_memory.unlock();
            }


            instance_id = *ptr;
            shared_memory.unlock();

//            shared_memory.detach();
        }

//        shared_memory.lock();
//        shared_memory.unlock();

    }

    MainWindow w( instance_id );
    w.show();

    return a.exec();
}
