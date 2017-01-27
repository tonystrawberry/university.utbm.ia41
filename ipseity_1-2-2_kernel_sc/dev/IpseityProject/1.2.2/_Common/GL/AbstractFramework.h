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


#ifndef AbstractFramework_h
#define AbstractFramework_h


#include "GL/OpenGL.h"
#include "Types.h"


struct GLMatrix
{
    union
    {
        GLfloat m[4][4];
        GLfloat data[16];
    };

    void setIdentity();
    void display();

    friend GLMatrix operator*(const GLMatrix &, const GLMatrix &);
	friend GLMatrix inverse( const GLMatrix &, bool = false );
};


class AbstractFramework
{
    public:
        /*!
        *\brief Utilisation d'un shader.
        */
        virtual bool useShader( GLint shader_id ) = 0;
        virtual bool useShader( const char* shader_name ) = 0;

        virtual void fetchUniformVariables( uint32 nbr_variables, const char**, GLint* ) = 0;
        virtual void fetchAttributeVariables( uint32 nbr_variables, const char**, GLint* ) = 0;

        /*!
        *\brief Id du shader actuel.
        */
        virtual GLint getCurrentShaderId( bool gl_id = true ) const = 0;

        /*!
        *\brief Transmet la matrice MVP = P*V*M au shader actif.
        */
        virtual void transmitMVP( GLuint var_id ) = 0;

        /*!
        *\brief Transmet la matrice MV = V*M au shader actif.
        */
        virtual void transmitMV( GLuint var_id ) = 0;

        /*!
        *\brief Transmet la transposée inverse de la matrice MV = ((V*M)^-1)T au shader actif.
        */
        virtual void transmitNM( GLuint var_id ) = 0;

        /*!
        *\brief Transmet la matrice M au shader actif.
        */
        virtual void transmitM( GLuint var_id ) = 0;

        /*!
        *\brief Transmet la matrice V au shader actif.
        */
        virtual void transmitV( GLuint var_id ) = 0;

        /*!
        *\brief Transmet la matrice P au shader actif.
        */
        virtual void transmitP( GLuint var_id ) = 0;

        /*!
        *\brief Enregistre la matrice de transformation courante sur le sommet de la
        *pile de matrices.
        */
        virtual void pushMatrix() = 0;

        /*!
        *\brief Remplace la matrice de transformation courante par celle se trouvant
        *au sommet de la pile de matrices.
        */
        virtual void popMatrix() = 0;

        /*!
        *\brief Remplace la matrice de transformation courante par une matrice
        *identité.
        */
        virtual void loadIdentity() = 0;

        /*!
        *\brief Multiplie la matrice de transformation courante par une matrice de
        *translation utilisant le vecteur de coordonnées \a x, \a y et \a z.
        */
        virtual void translate(GLfloat x, GLfloat y, GLfloat z) = 0;

        /*!
        *\brief Multiplie la matrice de transformation courante par une matrice de
        *rotation utilisant l'angle \a angle (en degrés) et l'axe représenté par le
        *vecteur de coordonnées \a x, \a y et \a z.
        */
        virtual void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) = 0;

        /*!
        *\brief Multiplie la matrice de transformation courante par une matrice de
        *changement d'échelle utilisant les facteurs \a x, \a y et \a z.
        */
        virtual void scale(GLfloat x, GLfloat y, GLfloat z) = 0;

        virtual void multiply( const GLMatrix & m ) = 0;

        virtual void computeAncillaryMatrices() = 0;

        virtual GLuint createTexture( const char* filename ) = 0;
};


#define ABSTRACT_FRAMEWORK_INTERFACE \
    bool useShader( GLint shader_id ); \
    bool useShader( const char* shader_name ); \
    void fetchUniformVariables( uint32 nbr_variables, const char**, GLint* ); \
    void fetchAttributeVariables( uint32 nbr_variables, const char**, GLint* ); \
    GLint getCurrentShaderId( bool gl_id = true ) const; \
    void transmitMVP( GLuint var_id ); \
    void transmitMV( GLuint var_id ); \
    void transmitNM( GLuint var_id ); \
    void transmitM( GLuint var_id ); \
    void transmitV( GLuint var_id ); \
    void transmitP( GLuint var_id ); \
    void pushMatrix(); \
    void popMatrix(); \
    void loadIdentity(); \
    void translate(GLfloat x, GLfloat y, GLfloat z); \
    void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z); \
    void scale(GLfloat x, GLfloat y, GLfloat z); \
    void multiply( const GLMatrix & m ); \
    void computeAncillaryMatrices(); \
    GLuint createTexture( const char* filename ); \


#endif
