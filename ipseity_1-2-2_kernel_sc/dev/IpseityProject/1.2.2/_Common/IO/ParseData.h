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

 
#ifndef parse_H
#define parse_H


#include <fstream>
#include <iostream>
#include <cstdio>


template <class T>
void
print_mat( int w, int h, T* mat )
{
    for(int i = 0 ; i < w ; ++i)
    {
        for(int j = 0 ; j < h ; ++j)
        {
            std::cout << mat[i*h + j] << " ";
        }
        std::cout << std::endl;
    }
}


int first_parse( char* fname, int max_nbr_samples, int* NComponents, int** NDimensions );


template <class T>
int
parse_data( char* fname, int nbr_samples, T** mat, int* shift )
{
    std::ifstream infile(fname);

    std::cout << "  Loading " << fname << std::endl;
    if (infile.is_open())
    {
        int nbr_total_samples;
        int nbr_channels;
        int* sizes;
        T** mat_ptr;

        infile >> nbr_total_samples;
        infile >> nbr_channels;

        sizes = new int [nbr_channels];
        mat_ptr = new T* [nbr_channels];

        for( int k = 0; k < nbr_channels; ++k )
        {
            infile >> sizes[k];
            mat_ptr[k] = mat[k];
            if (mat_ptr[k])
            {
                mat_ptr[k] += shift[k];
            }
        }

        if (nbr_samples < 0)
        {
            nbr_samples = nbr_total_samples;
        }

        T v;
        for( int i = 0 ; i < nbr_samples ; ++i )
        {
            for( int k = 0; k < nbr_channels; ++k )
            {
                if (mat_ptr[k])
                {
                    for( int j = 0 ; j < sizes[k] ; ++j )
                    {
                        infile >> *(mat_ptr[k]++);
                    }
                }
                else
                {
                    for( int j = 0 ; j < sizes[k] ; ++j )
                    {
                        infile >> v;
                    }
                }
            }
        }

        delete [] mat_ptr;
        delete [] sizes;
    }

    return 0;
}


#endif
