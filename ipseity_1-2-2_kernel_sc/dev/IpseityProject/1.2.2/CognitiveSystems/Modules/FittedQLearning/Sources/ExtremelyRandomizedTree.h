/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2014, 2013 Ipseity Core Developers
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


#ifndef ExtremelyRandomizedTree_h
#define ExtremelyRandomizedTree_h


#include "AL/IpseityTypes.h"


typedef struct
{
    int32 Direction;
    float64 Point;
} TypeCut;


class TreeNode
{
    public:
        TreeNode( uint32 level );
        ~TreeNode();

    public:
        std::list<uint32> Samples;
        TypeCut Cut;
        uint32 Level;
        float64 Label;
        TreeNode* Left;
        TreeNode* Right;
};


class ExtremelyRandomizedTree
{
    public:
        static uint32 g_NbrSamples;
        static uint32 g_InputSize;
        static float64* g_Inputs;
        static float64* g_InputMins;
        static float64* g_InputMaxs;
        static float64* g_Outputs;
        static std::vector<uint32>* g_ValidEntries;

    public:
        ExtremelyRandomizedTree();
        ~ExtremelyRandomizedTree();

        void create( uint32 nbr_cuts, uint32 min_samples, uint32 max_levels );

        float64 getLabel( float64* x );


    protected:
        void buildTree( TreeNode* tree_node );
        bool existTests( std::list<uint32>* samples );
        uint32 selectTest( std::list<uint32>* samples );
        void split( std::list<uint32>* samples, TypeCut* cut, std::list<uint32>* left_samples, std::list<uint32>* right_samples );
        float64 getScore( TypeCut* cut, std::list<uint32>* samples );
        float64 getVariance( std::list<uint32>* samples );
        float64 getLabel( float64* x, TreeNode* tree_node );
        void computeLabel( TreeNode* tree_node );


    protected:
        TreeNode* m_RootNode;

        uint32 m_NbrCuts;
        TypeCut* m_Cuts;
        uint32 m_MinSamples;
        uint32 m_NbrMaxLevels;
};


#endif
