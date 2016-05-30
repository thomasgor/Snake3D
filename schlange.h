#ifndef SCHLANGE_H
#define SCHLANGE_H

#include <QList>
#include <QDir>
#include <Qdebug>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <modelloader.h>


class Schlange
{
private:
    //TODO: Dynamische Datenstruktur für Kontrollpunkte des Spline
    QOpenGLTexture *qtex;
    unsigned int laenge=0;
    QOpenGLShaderProgram shaderProgram;

    //Buffer
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    GLfloat *vboData;
    GLuint* iboData;
    unsigned int vboLength;
    unsigned int iboLength;

public:
    void initShader();
    void render();
    void loadModel();


public:
    Schlange();
};

#endif // SCHLANGE_H
