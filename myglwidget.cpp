#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent) : QGLWidget(parent)
{
    snake = new Schlange();
}



void MyGLWidget::initializeGL(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
void MyGLWidget::resizeGL(int width, int height){
    glDraw();
}

void MyGLWidget::paintGL(){
    this->snake->render();
}