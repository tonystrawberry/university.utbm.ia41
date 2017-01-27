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


#include "ExtremelyRandomizedTree.h"


uint32 ExtremelyRandomizedTree::g_NbrSamples = 0;
uint32 ExtremelyRandomizedTree::g_InputSize = 0;
float64* ExtremelyRandomizedTree::g_Inputs = NULL;
float64* ExtremelyRandomizedTree::g_InputMins = NULL;
float64* ExtremelyRandomizedTree::g_InputMaxs = NULL;
float64* ExtremelyRandomizedTree::g_Outputs = NULL;
std::vector<uint32>* ExtremelyRandomizedTree::g_ValidEntries = NULL;


TreeNode::TreeNode( uint32 level ) :
    Left( NULL ),
    Right( NULL ),
    Level( level ),
    Label( 0.0 )
{
}

TreeNode::~TreeNode()
{
//    cout << "delete: " << Level << endl;
    SafeDelete( Left );
    SafeDelete( Right );
}


ExtremelyRandomizedTree::ExtremelyRandomizedTree() :
    m_RootNode( NULL ),
    m_NbrCuts( 0 ),
    m_Cuts( NULL ),
    m_MinSamples( 0 ),
    m_NbrMaxLevels( 100 )
{
}

ExtremelyRandomizedTree::~ExtremelyRandomizedTree()
{
    SafeDeleteArray( m_Cuts );
    SafeDelete( m_RootNode );
}


void
ExtremelyRandomizedTree::create( uint32 nbr_cuts, uint32 min_samples, uint32 max_levels )
{
    SafeDeleteArray( m_Cuts );
    SafeDelete( m_RootNode );

    m_NbrCuts = nbr_cuts;
    m_MinSamples = min_samples;
    m_NbrMaxLevels = max_levels;

    m_Cuts = new TypeCut [m_NbrCuts];

    m_RootNode = new TreeNode( 0 );
    for( uint32 i = 0; i < g_NbrSamples; ++i )
    {
        m_RootNode->Samples.push_back( i );
    }

    buildTree( m_RootNode );
}


float64
ExtremelyRandomizedTree::getLabel( float64* x )
{
    return getLabel( x, m_RootNode );
}


float64
ExtremelyRandomizedTree::getLabel( float64* x, TreeNode* tree_node )
{
    if (tree_node)
    {
        if (tree_node->Left == NULL && tree_node->Right == NULL)
        {
            return tree_node->Label;
        }
        else
        {
            if (x[tree_node->Cut.Direction] < tree_node->Cut.Point && tree_node->Left)
            {
                return getLabel( x, tree_node->Left );
            }
            else if (tree_node->Right)
            {
                return getLabel( x, tree_node->Right );
            }
        }
    }

    return 0.0;
}


void
ExtremelyRandomizedTree::buildTree( TreeNode* tree_node )
{
    if (tree_node->Samples.size() >= m_MinSamples && (m_NbrMaxLevels == 0 || tree_node->Level < m_NbrMaxLevels))
    {
//        qDebug() << "# s: " << tree_node->Samples.size();

        if (existTests( &tree_node->Samples ))
        {
            uint32 cut_id = selectTest( &tree_node->Samples );
            tree_node->Cut = m_Cuts[cut_id];
    //        cout << "l: " << tree_node->Level << endl;
            tree_node->Left = new TreeNode( tree_node->Level+1 );
            tree_node->Right = new TreeNode( tree_node->Level+1 );

            split( &tree_node->Samples, &tree_node->Cut, &tree_node->Left->Samples, &tree_node->Right->Samples );

            if (tree_node->Left->Samples.size() == 0)
            {
                delete tree_node->Left;
                tree_node->Left = NULL;
            }
            else
            {
    //            qDebug() << "***  Left";
                buildTree( tree_node->Left );
            }

            if (tree_node->Right->Samples.size() == 0)
            {
                delete tree_node->Right;
                tree_node->Right = NULL;
            }
            else
            {
    //            qDebug() << "***  Right";
                buildTree( tree_node->Right );
            }
        }
    }
    else
    {
        computeLabel( tree_node );
    }
}


bool
ExtremelyRandomizedTree::existTests( std::list<uint32>* samples )
{
    uint32 d, k;
    float64 min, max;
    float64 v;
    std::list<uint32>::const_iterator itr;

    for( uint32 i = 0; i < g_ValidEntries->size(); ++i )
    {
        d = g_ValidEntries->at( i );
        itr = samples->begin();
        k = d+(*itr)*g_InputSize;
        min = max = g_Inputs[k];
        ++itr;

        for( ; itr != samples->end(); ++itr )
        {
            k = d+(*itr)*g_InputSize;
            v = g_Inputs[k];

            if (v < min)
            {
                min = v;
            }

            if (v > max)
            {
                max = v;
            }
        }

        if (min != max)
        {
            return true;
        }
    }

    return false;
}


uint32
ExtremelyRandomizedTree::selectTest( std::list<uint32>* samples )
{
    uint32 d, k;
    float64 min, max;
    float64 v;
    std::list<uint32>::const_iterator itr;

    // Define the cuts
    for( uint32 i = 0; i < m_NbrCuts; ++i )
    {
        do
        {
            d = g_ValidEntries->at( RandomNumberGenerator::getRandomNumber( (int32) g_ValidEntries->size() ) );

            itr = samples->begin();
            k = d+(*itr)*g_InputSize;
            min = max = g_Inputs[k];
            ++itr;

            for( ; itr != samples->end(); ++itr )
            {
                k = d+(*itr)*g_InputSize;
                v = g_Inputs[k];

                if (v < min)
                {
                    min = v;
                }

                if (v > max)
                {
                    max = v;
                }
            }
        }
        while( min == max);

        m_Cuts[i].Direction = d;
        m_Cuts[i].Point = min+(RandomNumberGenerator::getFloat64OpenClosed()*(max-min));

//        cout << i << "," << d << ": " << min << "," << max << " -> " << m_Cuts[i].Point;
    }

    // Select the best cut
    float64 score;
    float64 best_score;
    uint32 best_cut;

    best_score = getScore( &m_Cuts[0], samples );
    best_cut = 0;
    qDebug() << best_cut << ": " << best_score;
    for( uint32 i = 1; i < m_NbrCuts; ++i )
    {
        score = getScore( &m_Cuts[i], samples );

        qDebug() << i << ": " << score;

        if (score > best_score)
        {
            best_score = score;
            best_cut = i;
        }
    }
    qDebug() << "  --> " << best_cut << ": " << best_score;

    return best_cut;
}


void
ExtremelyRandomizedTree::split( std::list<uint32>* samples,
                                TypeCut* cut,
                                std::list<uint32>* left_samples,
                                std::list<uint32>* right_samples )
{
    uint32 sample_id;

    for( std::list<uint32>::const_iterator itr = samples->begin(); itr != samples->end(); ++itr )
    {
        sample_id = (*itr);
        if (g_Inputs[sample_id*g_InputSize+cut->Direction] < cut->Point)
        {
            left_samples->push_back( sample_id );
        }
        else
        {
            right_samples->push_back( sample_id );
        }
    }
}


float64
ExtremelyRandomizedTree::getScore( TypeCut* cut, std::list<uint32>* samples )
{
    float64 var = getVariance( samples );

    if (var > 0.0)
    {
        std::list<uint32> left_samples;
        std::list<uint32> right_samples;

        split( samples, cut, &left_samples, &right_samples );

        float64 frac_nbr_samples = 1.0 / (float64) g_NbrSamples;
        float64 left_var = frac_nbr_samples*left_samples.size()*getVariance( &left_samples );
        float64 right_var = frac_nbr_samples*right_samples.size()*getVariance( &right_samples );

        return (var-left_var-right_var)/var;
    }
    else
    {
        return 0.0;
    }
}


float64
ExtremelyRandomizedTree::getVariance( std::list<uint32>* samples )
{
    if (samples->size())
    {
        float64 mean;
        float64 frac_samples = 1.0 / (float64) samples->size();
        float64 square_sum;
        uint32 sample_id;

        square_sum = 0.0;
        mean = 0.0;
        for( std::list<uint32>::const_iterator itr = samples->begin(); itr != samples->end(); ++itr )
        {
            sample_id = (*itr);
            square_sum += g_Outputs[sample_id]*g_Outputs[sample_id];
            mean += g_Outputs[sample_id];
        }
        mean *= frac_samples;

        return frac_samples*square_sum - mean*mean;
    }
    else
    {
//        cout << "oops" << endl;
        return 0.0;
    }
}


void
ExtremelyRandomizedTree::computeLabel( TreeNode* tree_node )
{
    if (tree_node)
    {
        float64 y;
        float64 frac_nbr = 1.0 / (float64) tree_node->Samples.size();

        y = 0.0;
        for( std::list<uint32>::const_iterator itr = tree_node->Samples.begin(); itr !=  tree_node->Samples.end(); ++itr )
        {
            y += g_Outputs[(*itr)];
        }
        y *= frac_nbr;

        tree_node->Label = y;
    }
}
