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


//*****************************************************************************
//
// Class: GlFramework
// Authors: Fabrice LAURI (fabrice.lauri@utbm.fr)
//          (Associate Professor at the
//          "Université de Technologie de Belfort-Montbéliard", France)
//          Jocelyn BUISSON (jocelyn.buisson@voxelia.com)
// Version: 1.0
// Last update: 2012-05-01
//
//*****************************************************************************


#include "GlFramework.h"

#include "GL/Shapes/AbstractShape.h"

#include <QGLWidget>

#include <QImage>
#include <QMessageBox>
#include <QDebug>
#include "OpenGL.h"
#include <limits>
#include <cmath>
#include "IpseityString.h"

#include <QFile>
#include <QDebug>

#include <iostream>

using namespace std;


void
GLMatrix::setIdentity()
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void
GLMatrix::display()
{
    for( uint32 j = 0; j < 4; ++j )
    {
        for( uint32 i = 0; i < 4; ++i )
        {
            qDebug() << m[j][i];
        }
        qDebug();
    }
}


GLMatrix
operator*( const GLMatrix & mat1, const GLMatrix & mat2 )
{
    GLMatrix result;

    for(int idx = 0; idx < 4; idx++)
    {
        for(int idy = 0; idy < 4; idy++)
        {
            result.m[idy][idx] = 0.0f;
            for(int idz = 0; idz < 4; idz++)
            {
                result.m[idy][idx] += mat1.m[idy][idz] * mat2.m[idz][idx];
            }
        }
    }
    return result;
}

GLfloat
det( const GLMatrix & mat )
{
    GLfloat det;

    det = mat.data[0] * (mat.data[5] * mat.data[10] - mat.data[6] * mat.data[9])
        - mat.data[1] * (mat.data[4] * mat.data[10] - mat.data[6] * mat.data[8])
        + mat.data[2] * (mat.data[4] * mat.data[9] - mat.data[5] * mat.data[8]);

    return det;
}


GLMatrix
inverse( const GLMatrix & mat, bool mat3 )
{
    GLMatrix result;

    if (mat3)
    {
        GLfloat d = det(mat);

        if (fabs(d) < 0.0005)
        {
            result.setIdentity();
        }
        else
        {
            d = 1.0f/d;

            result.data[0] =  (mat.data[5] * mat.data[10] - mat.data[6] * mat.data[9]) * d;
            result.data[3] = -(mat.data[1] * mat.data[10] - mat.data[9] * mat.data[2]) * d;
            result.data[6] =  (mat.data[1] * mat.data[6] - mat.data[5] * mat.data[2]) * d;

            result.data[1] = -(mat.data[4] * mat.data[10] - mat.data[6] * mat.data[8]) * d;
            result.data[4] =  (mat.data[0] * mat.data[10] - mat.data[2] * mat.data[8]) * d;
            result.data[7] = -(mat.data[0] * mat.data[6] - mat.data[2] * mat.data[4]) * d;

            result.data[2] =  (mat.data[4] * mat.data[9] - mat.data[5] * mat.data[8]) * d;
            result.data[5] = -(mat.data[0] * mat.data[9] - mat.data[1] * mat.data[8]) * d;
            result.data[8] =  (mat.data[0] * mat.data[5] - mat.data[1] * mat.data[4]) * d;
        }
    }
    else if (mat.data[12] == 0 &&
        mat.data[13] == 0 &&
        mat.data[14] == 0 &&
        mat.data[15] == 1)
    {
        GLfloat d = det(mat);

        if (fabs(d) < 0.0005)
        {
            result.setIdentity();
        }
        else
        {
            d = 1.0f/d;

            result.data[0] =  (mat.data[5] * mat.data[10] - mat.data[6] * mat.data[9]) * d;
            result.data[4] = -(mat.data[1] * mat.data[10] - mat.data[9] * mat.data[2]) * d;
            result.data[8] =  (mat.data[1] * mat.data[6] - mat.data[5] * mat.data[2]) * d;

            result.data[1] = -(mat.data[4] * mat.data[10] - mat.data[6] * mat.data[8]) * d;
            result.data[5] =  (mat.data[0] * mat.data[10] - mat.data[2] * mat.data[8]) * d;
            result.data[9] = -(mat.data[0] * mat.data[6] - mat.data[2] * mat.data[4]) * d;

            result.data[2] =  (mat.data[4] * mat.data[9] - mat.data[5] * mat.data[8]) * d;
            result.data[6] = -(mat.data[0] * mat.data[9] - mat.data[1] * mat.data[8]) * d;
            result.data[10] =  (mat.data[0] * mat.data[5] - mat.data[1] * mat.data[4]) * d;

            result.data[3] = -result.data[0]*mat.data[3]-result.data[1]*mat.data[7]-result.data[2]*mat.data[11];
            result.data[7] = -result.data[4]*mat.data[3]-result.data[5]*mat.data[7]-result.data[6]*mat.data[11];
            result.data[11] = -result.data[8]*mat.data[3]-result.data[9]*mat.data[7]-result.data[10]*mat.data[11];

            result.data[12] = result.data[13] = result.data[14] = 0.0f;
            result.data[15] = 1.0f;
        }
    }

    return result;
}


GlFramework::GlFramework()
{
    loadIdentity();
    pViewMatrix.setIdentity();

    AbstractShape::g_Framework = this;

    m_CurrentShader = -1;

    qDebug() << "GL Framework initialized!";
}

GlFramework::~GlFramework()
{
    for( GLuint i = 0; i < m_Shader.size(); ++i )
    {
        glDeleteProgram( m_Shader[i] );
    }
}


bool
GlFramework::init()
{
    qDebug() << "Init GLEW";

    glewExperimental = GL_TRUE;

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        qCritical() << (char*) glewGetErrorString(error);

        return false;
    }
    else
    {
        qDebug() << "OpenGL Version:" << (char*) glGetString( GL_VERSION );
        qDebug() << "OpenGL Vendor:" << (char*) glGetString( GL_VENDOR );
        qDebug() << "OpenGL Renderer:" << (char*) glGetString( GL_RENDERER );
        qDebug() << "GLEW Version:" << (char*) glewGetString( GLEW_VERSION );

        qDebug() << "****  GL initialized!";

        return true;
    }
}


char*
GlFramework::loadShader( const char* file_name )
{
    QFile file( file_name );

    char* content = NULL;
    long length = 0;

    if (file.open( QIODevice::ReadOnly ))
    {
        length = file.size();

        content = new char [length+1];
        file.read( content, length );
        content[length] = '\0';

        file.close();
    }

    return content;
}

GLint
GlFramework::createShader( const char* shader_prefix )
{
    char shader_name[128];

    sprintf( shader_name, "%s.vs", shader_prefix );
    const GLchar* vs_content = loadShader( shader_name );

    sprintf( shader_name, "%s.fs", shader_prefix );
    const GLchar* fs_content = loadShader( shader_name );

    if (vs_content == NULL)
    {
        qCritical() << shader_prefix << " not found!";
        return -1;
    }

    GLint status, logSize;
    QByteArray log;
    GLuint pProgram;

    pProgram = glCreateProgram();

    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vs_content, NULL);
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
        glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &logSize);
        log.resize(logSize - 1);
        glGetShaderInfoLog(vshader, logSize, &logSize, log.data());

        qCritical() << "Shader error: " << QString(log).toStdString().c_str();

        return -1;
    }
    glAttachShader(pProgram, vshader);

    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fs_content, NULL);
    glCompileShader(fshader);
    if(status != GL_TRUE)
    {
        glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &logSize);
        log.resize(logSize - 1);
        glGetShaderInfoLog(fshader, logSize, &logSize, log.data());

        qCritical() << "Shader error: " << QString(log).toStdString().c_str();

        return -1;
    }
    glAttachShader(pProgram, fshader);

    glLinkProgram(pProgram);
    glGetProgramiv(pProgram, GL_LINK_STATUS, &status);
    if(status != GL_TRUE)
    {
        glGetProgramiv(pProgram, GL_INFO_LOG_LENGTH, &logSize);
        log.resize(logSize - 1);
        glGetProgramInfoLog(pProgram, logSize, &logSize, log.data());

        qCritical() << "Shader error: " << QString(log).toStdString().c_str();

        return -1;
    }

    delete [] vs_content;
    delete [] fs_content;

    qDebug() << ">>> " << shader_prefix << " created!";
    m_ShaderNames.push_back( shader_prefix );
    m_Shader.push_back( pProgram );

    return m_Shader.size()-1;
}


bool
GlFramework::useShader( GLint shader_id )
{
    if (shader_id >= 0 && shader_id < m_Shader.size())
    {
        if (m_CurrentShader != shader_id)
        {
//            std::cout << "Use shader: " << shader_id << std::endl;
            m_CurrentShader = shader_id;
            glUseProgram( m_Shader[shader_id] );
        }

        return true;
    }
    else
    {
        m_CurrentShader = -1;
        glUseProgram( 0 );

        return false;
    }
}

bool
GlFramework::useShader( const char* shader_name )
{
    for( GLuint id = 0; id < m_ShaderNames.size(); ++id )
    {
        if (strstr( m_ShaderNames[id].c_str(), shader_name ))
        {
            if (m_CurrentShader != id)
            {
//                std::cout << "Use shader: " << id << std::endl;
                m_CurrentShader = id;
                glUseProgram( m_Shader[id] );
            }

            return true;
        }
    }

    m_CurrentShader = -1;
    glUseProgram( 0 );

    return false;
}


void
GlFramework::fetchUniformVariables( uint32 nbr_variables, const char** var, GLint* indices )
{
    GLint current_shader = m_Shader[m_CurrentShader];

    for( uint32 i = 0; i < nbr_variables; ++i )
    {
        indices[i] = glGetUniformLocation( current_shader, var[i] );
    }
}


void
GlFramework::fetchAttributeVariables( uint32 nbr_variables, const char** var, GLint* indices )
{
    GLint current_shader = m_Shader[m_CurrentShader];

    for( uint32 i = 0; i < nbr_variables; ++i )
    {
        indices[i] = glGetAttribLocation( current_shader, var[i] );
    }
}


GLint
GlFramework::getCurrentShaderId( bool gl_id ) const
{
    if (gl_id)
    {
        return m_Shader[m_CurrentShader];
    }
    else
    {
        return m_CurrentShader;
    }
}


void
GlFramework::transmitMVP( GLuint var_id )
{
    glUniformMatrix4fv( var_id, 1, GL_TRUE, pMVPMatrix.data );
}

void
GlFramework::transmitMV( GLuint var_id )
{
    glUniformMatrix4fv( var_id, 1, GL_TRUE, pModelViewMatrix.data );
}

void
GlFramework::transmitNM( GLuint var_id )
{
    glUniformMatrix3fv( var_id, 1, GL_TRUE, pNormalMatrix.data );
}

void
GlFramework::transmitM( GLuint var_id )
{
    glUniformMatrix4fv( var_id, 1, GL_TRUE, pModelMatrix.data );
}

void
GlFramework::transmitV( GLuint var_id )
{
    glUniformMatrix4fv( var_id, 1, GL_TRUE, pViewMatrix.data );
}

void
GlFramework::transmitP( GLuint var_id )
{
   glUniformMatrix4fv( var_id, 1, GL_TRUE, pProjMatrix.data );
}


void
GlFramework::setPerspective(GLfloat fovY, GLfloat ratio, GLfloat zNear, GLfloat zFar)
{
    GLfloat thetaY = 0.5f * (M_PI * fovY / 180.0f);
    GLfloat tanThetaY = tan(thetaY);
    GLfloat tanThetaX = tanThetaY * ratio;
    GLfloat halfW   =  tanThetaX * zNear;
    GLfloat halfH   =  tanThetaY * zNear;
    GLfloat left    = -halfW;
    GLfloat right   =  halfW;
    GLfloat bottom  = -halfH;
    GLfloat top     =  halfH;
    GLfloat iWidth  = 1.0f / (right - left);
    GLfloat iHeight = 1.0f / (top - bottom);
    GLfloat iDepth  = 1.0f / (zFar - zNear);

    pProjMatrix.m[0][0] = 2.0f * zNear * iWidth; pProjMatrix.m[0][1] = 0.0f;                   pProjMatrix.m[0][2] =  (right + left) * iWidth;  pProjMatrix.m[0][3] =  0.0f;
    pProjMatrix.m[1][0] = 0.0f;                  pProjMatrix.m[1][1] = 2.0f * zNear * iHeight; pProjMatrix.m[1][2] =  (top + bottom) * iHeight; pProjMatrix.m[1][3] =  0.0f;
    pProjMatrix.m[2][0] = 0.0f;                  pProjMatrix.m[2][1] = 0.0f;                   pProjMatrix.m[2][2] = -(zFar + zNear) * iDepth;  pProjMatrix.m[2][3] = -2.0f * (zFar * zNear) * iDepth;
    pProjMatrix.m[3][0] = 0.0f;                  pProjMatrix.m[3][1] = 0.0f;                   pProjMatrix.m[3][2] = -1.0f;                     pProjMatrix.m[3][3] =  0.0f;

//    cout << "ProjectionMatrix" << endl;
//	pProjMatrix.display();
}


void
GlFramework::setOrtho(GLfloat fovY, GLfloat ratio, GLfloat zNear, GLfloat zFar)
{
    GLfloat thetaY = 0.5f * (M_PI * fovY / 180.0f);
    GLfloat tanThetaY = tan(thetaY);
    GLfloat tanThetaX = tanThetaY * ratio;
    GLfloat halfW   =  tanThetaX * zNear;
    GLfloat halfH   =  tanThetaY * zNear;
    GLfloat left    = -halfW;
    GLfloat right   =  halfW;
    GLfloat bottom  = -halfH;
    GLfloat top     =  halfH;
    GLfloat iWidth  = 1.0f / (right - left);
    GLfloat iHeight = 1.0f / (top - bottom);
    GLfloat iDepth  = 1.0f / (zNear - zFar);

    pProjMatrix.m[0][0] = 2.0f * iWidth;    pProjMatrix.m[0][1] = 0.0f;             pProjMatrix.m[0][2] = 0.0f;             pProjMatrix.m[0][3] = -(right+left) * iWidth;
    pProjMatrix.m[1][0] = 0.0f;             pProjMatrix.m[1][1] = 2.0f * iHeight;   pProjMatrix.m[1][2] = 0.0f;             pProjMatrix.m[1][3] =  -(top+bottom) * iHeight;
    pProjMatrix.m[2][0] = 0.0f;             pProjMatrix.m[2][1] = 0.0f;             pProjMatrix.m[2][2] = 2.0f * iDepth;    pProjMatrix.m[2][3] = (zFar+zNear) * iDepth;
    pProjMatrix.m[3][0] = 0.0f;             pProjMatrix.m[3][1] = 0.0f;             pProjMatrix.m[3][2] = 0.0f;             pProjMatrix.m[3][3] =  1.0f;

//	cout << "ProjectionMatrix" << endl;
//	pProjMatrix.display();
}


void
GlFramework::lookAt(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat targetX, GLfloat targetY, GLfloat targetZ, GLfloat upX, GLfloat upY, GLfloat upZ)
{
    GLfloat forwardX = targetX - eyeX;
    GLfloat forwardY = targetY - eyeY;
    GLfloat forwardZ = targetZ - eyeZ;

    GLfloat forwardLen = std::sqrt(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
    if (forwardLen > std::numeric_limits<GLfloat>::epsilon())
    {
        forwardX /= forwardLen;
        forwardY /= forwardLen;
        forwardZ /= forwardLen;
    }

    GLfloat upLen = std::sqrt(upX * upX + upY * upY + upZ * upZ);
    if (upLen > std::numeric_limits<GLfloat>::epsilon())
    {
        upX /= upLen;
        upY /= upLen;
        upZ /= upLen;
    }

    GLfloat sX = forwardY * upZ - forwardZ * upY;
    GLfloat sY = forwardZ * upX - forwardX * upZ;
    GLfloat sZ = forwardX * upY - forwardY * upX;

    GLfloat uX = sY * forwardZ - sZ * forwardY;
    GLfloat uY = sZ * forwardX - sX * forwardZ;
    GLfloat uZ = sX * forwardY - sY * forwardX;

    pViewMatrix.m[0][0] = sX;        pViewMatrix.m[0][1] = sY;        pViewMatrix.m[0][2] = sZ;        pViewMatrix.m[0][3] = 0.0f;
    pViewMatrix.m[1][0] = uX;        pViewMatrix.m[1][1] = uY;        pViewMatrix.m[1][2] = uZ;        pViewMatrix.m[1][3] = 0.0f;
    pViewMatrix.m[2][0] = -forwardX; pViewMatrix.m[2][1] = -forwardY; pViewMatrix.m[2][2] = -forwardZ; pViewMatrix.m[2][3] = 0.0f;
    pViewMatrix.m[3][0] = 0.0f;      pViewMatrix.m[3][1] = 0.0f;      pViewMatrix.m[3][2] = 0.0f;      pViewMatrix.m[3][3] = 1.0f;

    GLMatrix t;
    t.m[0][0] = 1.0f; t.m[0][1] = 0.0f; t.m[0][2] = 0.0f; t.m[0][3] = -eyeX;
    t.m[1][0] = 0.0f; t.m[1][1] = 1.0f; t.m[1][2] = 0.0f; t.m[1][3] = -eyeY;
    t.m[2][0] = 0.0f; t.m[2][1] = 0.0f; t.m[2][2] = 1.0f; t.m[2][3] = -eyeZ;
    t.m[3][0] = 0.0f; t.m[3][1] = 0.0f; t.m[3][2] = 0.0f; t.m[3][3] = 1.0f;

    pViewMatrix = pViewMatrix * t;

//    cout << "ViewMatrix" << endl;
//    pViewMatrix.display();
}


void
GlFramework::pushMatrix()
{
    pMatrixStack.append(pModelMatrix);
}

void
GlFramework::popMatrix()
{
    pModelMatrix = pMatrixStack.takeLast();
}

void
GlFramework::loadIdentity()
{
    pModelMatrix.setIdentity();
}

void
GlFramework::translate(GLfloat x, GLfloat y, GLfloat z)
{
    GLMatrix t;
    t.m[0][0] = 1.0f; t.m[0][1] = 0.0f; t.m[0][2] = 0.0f; t.m[0][3] = x;
    t.m[1][0] = 0.0f; t.m[1][1] = 1.0f; t.m[1][2] = 0.0f; t.m[1][3] = y;
    t.m[2][0] = 0.0f; t.m[2][1] = 0.0f; t.m[2][2] = 1.0f; t.m[2][3] = z;
    t.m[3][0] = 0.0f; t.m[3][1] = 0.0f; t.m[3][2] = 0.0f; t.m[3][3] = 1.0f;
    pModelMatrix = pModelMatrix * t;
}

void
GlFramework::rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat rAngle = M_PI * angle / 180.0f;
    GLfloat c = std::cos(rAngle);
    GLfloat s = std::sin(rAngle);

    GLfloat axisLen = std::sqrt(x * x + y * y + z * z);
    if(axisLen > std::numeric_limits<GLfloat>::epsilon())
    {
        x /= axisLen;
        y /= axisLen;
        z /= axisLen;
    }

    GLMatrix r;
    r.m[0][0] = x * x * (1.0f - c) + c;     r.m[0][1] = x * y * (1.0f - c) - z * s; r.m[0][2] = x * z * (1.0f - c) + y * s; r.m[0][3] = 0.0f;
    r.m[1][0] = y * x * (1.0f - c) + z * s; r.m[1][1] = y * y * (1.0f - c) + c;     r.m[1][2] = y * z * (1.0f - c) - x * s; r.m[1][3] = 0.0f;
    r.m[2][0] = z * x * (1.0f - c) - y * s; r.m[2][1] = y * z * (1.0f - c) + x * s; r.m[2][2] = z * z * (1.0f - c) + c;     r.m[2][3] = 0.0f;
    r.m[3][0] = 0.0f;                       r.m[3][1] = 0.0f;                       r.m[3][2] = 0.0f;                       r.m[3][3] = 1.0f;
    pModelMatrix = pModelMatrix * r;
}

void
GlFramework::scale(GLfloat x, GLfloat y, GLfloat z)
{
    GLMatrix s;
    s.m[0][0] = x;    s.m[0][1] = 0.0f; s.m[0][2] = 0.0f; s.m[0][3] = 0.0f;
    s.m[1][0] = 0.0f; s.m[1][1] = y;    s.m[1][2] = 0.0f; s.m[1][3] = 0.0f;
    s.m[2][0] = 0.0f; s.m[2][1] = 0.0f; s.m[2][2] = z;    s.m[2][3] = 0.0f;
    s.m[3][0] = 0.0f; s.m[3][1] = 0.0f; s.m[3][2] = 0.0f; s.m[3][3] = 1.0f;
    pModelMatrix = pModelMatrix * s;
}

void
GlFramework::multiply( const GLMatrix & m )
{
    pModelMatrix = pModelMatrix*m;
}


void
GlFramework::computeAncillaryMatrices()
{
    pModelViewMatrix = pViewMatrix * pModelMatrix;
    pNormalMatrix = inverse( pModelViewMatrix, true );
    pMVPMatrix = pProjMatrix * pModelViewMatrix;
}


char* cubemaps[] =
{
  "_pX",
  "_nX",
  "_pY",
  "_nY",
  "_pZ",
  "_nZ"
};


GLuint
GlFramework::createTexture( const char* filename )
{
    GLuint texId = 0;
    QImage image;

    if (strstr( filename, "cubemap_" ))
    {
        Ipseity::String name;

        glGenTextures( 1, &texId );
        qDebug() << "Texture #" << texId << ":" << filename << "loaded!";

//		glEnable( GL_TEXTURE_CUBE_MAP );
        glBindTexture( GL_TEXTURE_CUBE_MAP, texId );
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for( uint32 i = 0; i < 6; ++i )
        {
            name = filename;
            name += cubemaps[i];

            if (image.load( name.c_str() ))
            {
                qDebug() << name.c_str() << "(" << image.width() << "," << image.height() << ") loaded!";

                image = QGLWidget::convertToGLFormat( image );
                glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );
            }
        }
    }
    else if (strstr( filename, "tex2d_" ))
    {
        if (image.load( filename ))
        {
            image = QGLWidget::convertToGLFormat( image );

            glGenTextures( 1, &texId );
            qDebug() << "Texture #" << texId << ":" << filename << "(" << image.width() << "," << image.height() << ") loaded!";

//			glEnable( GL_TEXTURE_2D );
            glBindTexture( GL_TEXTURE_2D, texId );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }
    else if (strstr( filename, "tex1d_" ))
    {
        if (image.load( filename ))
        {
            image = QGLWidget::convertToGLFormat( image );

            glGenTextures( 1, &texId );
            qDebug() << "Texture #" << texId << ":" << filename << " (" << image.width() << "," << image.height() << ") loaded!";

//			glDisable( GL_TEXTURE_CUBE_MAP );
//			glEnable( GL_TEXTURE_1D );
            glBindTexture( GL_TEXTURE_1D, texId );
            glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, image.width(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        }
    }

    return texId;
}
