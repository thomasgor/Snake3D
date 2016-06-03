#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <GL/glut.h>
#include <QMatrix4x4>
#include <schlange.h>
#include <QTimer>
#include <QVector3D>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT

private:
    QMatrix4x4 pMatrix;
    Schlange * snake;
    QTimer *timer;
    float time=0;

    QVector3D eye;
    QVector3D center;
    QVector3D up;

public:
    explicit MyGLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
signals:

public slots:
};

#endif // MYGLWIDGET_H
