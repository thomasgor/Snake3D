#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent) : QGLWidget(parent)
{

}

void MyGLWidget::initializeGL(){
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    snake = new Schlange();
}
void MyGLWidget::resizeGL(int width, int height){
    glViewport(0,0,width,height);
    this->pMatrix.setToIdentity();
    this->pMatrix.frustum(-0.05, 0.05, -0.05, 0.05, 0.1, 100.0);
}

void MyGLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->snake->render(pMatrix);
}
