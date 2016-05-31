#include "schlange.h"

std::string modelPath = "H:/Computergrafik_SS16/Snake3D/models/sphere_low.obj";

Schlange::Schlange()
    :vbo(QOpenGLBuffer::VertexBuffer)
    ,ibo(QOpenGLBuffer::IndexBuffer)
{
    this->loadModel();

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/default330.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/default330.frag");
    shaderProgram.link();

    qtex = new QOpenGLTexture(QImage(":/textures/schlange.jpg").mirrored());
    qtex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    qtex->setMagnificationFilter(QOpenGLTexture::Linear);

    if(this->qtex->textureId() == 0)
        qWarning() << "Textur konnte nicht geladen werden";
}

void Schlange::loadModel()
{
    qWarning() << ibo.create();
    qWarning() << vbo.create();

    ibo.bind();
    vbo.bind();

    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    ModelLoader  model;
    bool res = model.loadObjectFromFile(modelPath);
    bool hasTextures = model.hasTextureCoordinates();

    if (res) {
        this->vboLength = model.lengthOfVBO(0,true,hasTextures);
        this->iboLength = model.lengthOfIndexArray();
        this->vboData = new GLfloat[this->vboLength];
        this->iboData = new GLuint[this->iboLength];
        model.genVBO(this->vboData,0,true,hasTextures);
        model.genIndexArray(this->iboData);
    }
    else {
        // Modell konnte nicht geladen werden
        qWarning() << QDir::currentPath();
        qWarning() << "Model konnte nicht geladen werden";
    }

    vbo.allocate(this->vboData, sizeof(GLfloat) * vboLength);
    ibo.allocate(this->iboData, sizeof(GLuint) * iboLength);

    vbo.release();
    ibo.release();
}
void Schlange::render(QMatrix4x4 pMatrix){
    mvMatrix.setToIdentity();
    mvMatrix.translate(0.0,0.0,-10.0);

    int attrVertices = 0;
    int attrTexCoords = 3;
    int unifMatrix = 0;
    int unifPmatrix = 1;

    shaderProgram.bind();
    vbo.bind();
    ibo.bind();
    qtex->bind(0);

    shaderProgram.setUniformValue("texture",0);
    shaderProgram.setUniformValue(unifPmatrix, pMatrix);
    shaderProgram.setUniformValue(unifMatrix, mvMatrix);

    int offset = 0;
    size_t stride = 12 * sizeof(GLfloat);

    shaderProgram.setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
    offset += 8 * sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);

    shaderProgram.enableAttributeArray(attrVertices);
    shaderProgram.enableAttributeArray(attrTexCoords);

    glDrawElements(GL_TRIANGLES, this->iboLength , GL_UNSIGNED_INT,0);

    qtex->release(0);
    vbo.release();
    ibo.release();
    shaderProgram.release();
}
