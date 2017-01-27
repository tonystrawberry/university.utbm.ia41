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


#include "WidgetRenderer.h"

#include "Defines.h"

#include <QtGui>
#include <QDebug>


WidgetRenderer::WidgetRenderer( uint32 window_id, const char* environment_name, const QString & name, QWidget* parent, QGLWidget* shared_widget ) :
    QGLWidget( QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::StencilBuffer),
        parent,
        shared_widget,
        Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint ),
    m_Parent( parent )
{
    setFocusPolicy( Qt::StrongFocus );
    if (window_id == 0)
    {
        QGLFormat fmt;
        fmt.setVersion( 5, 0 );

        setFormat( fmt );
    }

    m_EnvironmentName = environment_name;
    m_RenderingWindowId = window_id;

    setWindowTitle( name );
}


WidgetRenderer::~WidgetRenderer()
{
    g_Settings.setValue( tr("RenderingWindowGeometry_%1-%2").arg(QString(m_Environment->getName())).remove(' ').arg(m_RenderingWindowId), this->geometry() );

    if (m_Environment && m_RenderingWindowId == 0)
    {
        m_Environment->quitRenderer();
    }
}


void
WidgetRenderer::init( AbstractEnvironment* env, vector<AbstractCognitiveSystem*>* cs )
{
    m_Environment = env;
    m_CognitiveSystems = cs;

    QRect geom( 100+m_RenderingWindowId*100, 100+m_RenderingWindowId*100, cstDefaultRenderWidth, cstDefaultRenderHeight );
    if (g_Settings.contains( tr("RenderingWindowGeometry_%1-%2").arg(QString(m_Environment->getName())).remove(' ').arg(m_RenderingWindowId) ))
    {
        geom = QRect( g_Settings.value( tr("RenderingWindowGeometry_%1-%2").arg(QString(m_Environment->getName())).remove(' ').arg(m_RenderingWindowId) ).toRect() );
    }
    this->setGeometry(geom);
}


void
WidgetRenderer::initializeGL()
{
    if (m_RenderingWindowId == 0)
    {
        glewExperimental = GL_TRUE;

        GLenum error = glewInit();
        if(error != GLEW_OK)
        {
            qCritical() << "Echec de l'initialization de GLEW" << (char*) glewGetErrorString(error);
        }

        qDebug() << "OpenGL Version:" << (char*) glGetString( GL_VERSION );
        qDebug() << "OpenGL Vendor:" << (char*) glGetString( GL_VENDOR );
        qDebug() << "OpenGL Renderer:" << (char*) glGetString( GL_RENDERER );
        qDebug() << "GLEW Version:" << (char*) glewGetString( GLEW_VERSION );

        qDebug() << "****  GL initialized!";

        if (m_Environment)
        {
            m_Environment->initRenderer();
        }
        qDebug() << "****  Renderer initialized for window #" << m_RenderingWindowId;
    }
}


void
WidgetRenderer::render()
{
    updateGL();
}


void
WidgetRenderer::paintGL()
{
    if (m_Environment)
    {
        m_Environment->renderWindow( m_RenderingWindowId, this );
    }
}


void
WidgetRenderer::resizeGL( int w, int h )
{
    if (m_Environment)
    {
        m_Environment->resizeWindow( m_RenderingWindowId, w,h );
    }
}


void
WidgetRenderer::focusInEvent( QFocusEvent* )
{
}

void
WidgetRenderer::closeEvent( QCloseEvent* )
{
    emit closed( m_RenderingWindowId );
}


void
WidgetRenderer::keyPressEvent( QKeyEvent* ev )
{
    switch( ev->key() )
    {
        case Qt::Key_Minus:
            emit previousScenario();
            break;

        case Qt::Key_Plus:
            emit nextScenario();
            break;

        case Qt::Key_Enter:
            emit retryScenario();
            break;

        case Qt::Key_Tab:
            m_Parent->activateWindow();
            break;

        case Qt::Key_F10:
            setWindowState( windowState() ^ Qt::WindowMaximized );
            break;
    }

    if (m_Environment)
    {
        if (!m_Environment->pressKey( ev ) && !ev->isAutoRepeat())
        {
            for( vector<AbstractCognitiveSystem*>::const_iterator itr = m_CognitiveSystems->begin(); itr != m_CognitiveSystems->end(); ++itr )
            {
                if ((*itr))
                {
                    (*itr)->pressKey( ev->key() );
                }
            }
        }
    }
}


void
WidgetRenderer::keyReleaseEvent( QKeyEvent* ev )
{
    if (m_Environment)
    {
        if (!ev->isAutoRepeat())
        {
            for( vector<AbstractCognitiveSystem*>::const_iterator itr = m_CognitiveSystems->begin(); itr != m_CognitiveSystems->end(); ++itr )
            {
                if ((*itr))
                {
                    (*itr)->releaseKey( ev->key() );
                }
            }
        }
    }
}
