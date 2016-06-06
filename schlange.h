#ifndef SCHLANGE_H
#define SCHLANGE_H

#include <QList>
#include <QDir>
#include <Qdebug>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <modelloader.h>
#include <QVector3D>


class Schlange
{
private:
    //TODO: Dynamische Datenstruktur für Kontrollpunkte des Spline
    QOpenGLTexture *qtex;
    unsigned int laenge=0;
    QOpenGLShaderProgram shaderProgram;
    QMatrix4x4 mvMatrix;

    QList<QVector3D> kontrollPunkte;

    //Buffer
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    GLfloat* vboData;
    GLuint* iboData;
    unsigned int vboLength = 0;
    unsigned int iboLength = 0;
    float tmp;

public:
    void initShader();
    void render(QMatrix4x4 pMatrix);
    void loadModel();
    void loadBezier();
    void generiereBezierCurve();
    QVector3D kubischerBezierPunkt(float t, int i);



public:
    Schlange();
};

#endif // SCHLANGE_H
