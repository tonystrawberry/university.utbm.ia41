/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2015, 2014, 2013 Ipseity Core Developers
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


#ifndef GlFramework_h
#define GlFramework_h


#include "AbstractFramework.h"

#include <QLinkedList>
#include <vector>
#include <string>


class GlFramework : public AbstractFramework
{
    public: // methods
        GlFramework();
        ~GlFramework();

        ABSTRACT_FRAMEWORK_INTERFACE


        /*!
        *\brief Fonction de création d'un shader.
        */
        GLint createShader( const char* shader_prefix );

        /*!
        *\brief Recalcule la matrice de projection en utilisant les valeurs \a fovY
        *c'est à dire l'angle d'ouverture de la caméra selon l'axe Y (en degrés), le
        *\a ratio entre la longueur et la largeur de la caméra, et les distances
        *\a zNear et \a zFar correspondant respectivement aux distances de clipping
        *proches et éloignées.
        */
        void setPerspective(GLfloat fovY, GLfloat ratio, GLfloat zNear, GLfloat zFar);

        void setOrtho(GLfloat fovY, GLfloat ratio, GLfloat zNear, GLfloat zFar);

        /*!
        *\brief Recalcule la matrive de vue en utilisant les coordonnées \a eyeX,
        *\a eyeY et \a eyeZ correspondant à la position de la caméra, les
        *coordonnées \a targetX, \a targetY et \a targetZ du point visé par la
        *caméra et les coordonnées \a upX, \a upY et \a upZ du vecteur représentant
        *l'axe vertical de la caméra.
        */
        void lookAt(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat targetX, GLfloat targetY, GLfloat targetZ, GLfloat upX = 0.0f, GLfloat upY = 1.0f, GLfloat upZ = 0.0f);


    protected:
        /*!
        *\brief Fonction de chargement d'un shader.
        */
        char* loadShader( const char* file_name );

        /*!
        *\brief Fonction d'initialization d'OpenGL, appelée à la création du
        *contexte de rendu.
        */
        bool init();

    public:
        GLMatrix pProjMatrix, pViewMatrix;

    protected:
        GLMatrix pModelMatrix;
        GLMatrix pModelViewMatrix, pNormalMatrix, pMVPMatrix;
        QLinkedList<GLMatrix> pMatrixStack;
        std::vector<std::string> m_ShaderNames;
        std::vector<GLint> m_Shader;
        GLint m_CurrentShader;
};


#endif
