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


#ifndef WidgetRender_h
#define WidgetRender_h


#include <GL/glew.h>

#include <QGLWidget>

#include "AL/AbstractEnvironment.h"
#include "AL/AbstractCognitiveSystem.h"


class WidgetRenderer : public QGLWidget
{
    Q_OBJECT

    public:
        WidgetRenderer( uint32 window_id, const char* environment_name, const QString & window_name, QWidget* parent, QGLWidget* shared_widget );
        ~WidgetRenderer();

        void init( AbstractEnvironment*, vector<AbstractCognitiveSystem*>* );

        void render();

    signals:
        void retryScenario();
        void previousScenario();
        void nextScenario();
        void closed( uint32 );

    protected:
        virtual void initializeGL();
        virtual void paintGL();
        virtual void resizeGL(int w, int h);

        virtual void focusInEvent( QFocusEvent* );
        virtual void closeEvent( QCloseEvent* );

        void keyPressEvent( QKeyEvent* );
        void keyReleaseEvent( QKeyEvent* );


    private:
        QWidget* m_Parent;
        Ipseity::String m_EnvironmentName;
        uint32 m_RenderingWindowId;
        AbstractEnvironment* m_Environment;
        vector<AbstractCognitiveSystem*>* m_CognitiveSystems;
};


#endif
