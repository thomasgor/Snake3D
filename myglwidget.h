#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <GL/glut.h>
#include <QMatrix4x4>
#include <schlange.h>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT

private:
    QMatrix4x4 pMatrix;
    Schlange * snake;

    //Buffer
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    GLfloat *vboData;
    GLuint *iboData;
    unsigned int vboLength;
    unsigned int iboLength;


public:
    explicit MyGLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
signals:

public slots:
};

#endif // MYGLWIDGET_H
