#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , eye(0.0,5.0,5.0)
    , center(0.0,0.0,0.0)
    , up(0.0,1.0,0.0)
{

}

void MyGLWidget::initializeGL(){
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);

    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    timer = new QTimer(this);
    timer->connect(timer, SIGNAL(timeout()), this ,SLOT(updateGL()));
    timer->start(50);

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
