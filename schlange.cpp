#include "schlange.h"

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
        qWarning() << "Textur konnte nciht geladen werden";
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
    bool res = model.loadObjectFromFile("C:/Users/thomas/Documents/Projekte/Snake3D/models/sphere_low.obj");
    bool hasTextures = model.hasTextureCoordinates();

    if (res) {
        // Frage zu erwartende Array-Längen ab
        this->vboLength = model.lengthOfVBO(0,true,hasTextures);
        this->iboLength = model.lengthOfIndexArray();
        // Generiere VBO und Index-Array
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

void Schlange::render(){
    int attrVertices = 0;
    int attrTexCoords = 2;
    int unifMatrix = 0;

    QMatrix4x4 mvMatrix;
    mvMatrix.setToIdentity();
    mvMatrix.translate(0.0,0.0,-3.0);
    QVector3D eye(0.0,3.0,6.0);
    QVector3D center (0.0,0.0,0.0);
    QVector3D up(0.0,0.0,1.0);
    mvMatrix.lookAt(eye,center, up);
    mvMatrix.frustum(-0.5,0.5,-0.5,0.5,0.1,100);

    shaderProgram.bind();
    vbo.bind();
    ibo.bind();


    shaderProgram.setUniformValue("texture",0);
    shaderProgram.setUniformValue(unifMatrix, mvMatrix);

    qtex->bind(0);

    size_t stride = 12 * sizeof(GLfloat);

    int offset =0;
    shaderProgram.setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
    offset += 8 * sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);

    shaderProgram.enableAttributeArray(attrVertices);
    shaderProgram.enableAttributeArray(attrTexCoords);


    glDrawElements(GL_POINTS, this->iboLength , GL_UNSIGNED_INT,0);

    vbo.release();
    ibo.release();
    shaderProgram.release();
}
