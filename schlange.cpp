#include "schlange.h"

std::string modelPath = "C:/Users/thomas/Documents/Projekte/Snake3D/models/Snake_By_Swp.obj";
//std::string modelPath = "H:/Computergrafik_SS16/Snake3D/models/sphere_low.obj";

Schlange::Schlange()
    :vbo(QOpenGLBuffer::VertexBuffer)
    ,ibo(QOpenGLBuffer::IndexBuffer)
{
    loadBezier();
    generiereBezierCurve();
    this->tmp = 0;

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/default330.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/default330.frag");
    //shaderProgram.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shader/bezier330.geom");
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

void Schlange::loadBezier(){

//    iboLength = 10;
//    vboLength = 6 * 4;
//    vboData = new GLfloat[vboLength];
//    iboData = new GLuint[iboLength];

//    //1.Punkt
//    vboData[0] = (0.0f);
//    vboData[1]=(15.0f);
//    vboData[2]=(0.0f);
//    vboData[3]=(1.0f);
//    //2.Punkt
//    vboData[4]=(5.0f);
//    vboData[5]=(11.0f);
//    vboData[6]=(7.0f);
//    vboData[7]=(1.0f);
//    //3.Punkt
//    vboData[8]=(9.0f);
//    vboData[9]=(15.0f);
//    vboData[10]=(11.0f);
//    vboData[11]=(1.0f);
//    //4.Punkt
//    vboData[12]=(12.0f);
//    vboData[13]=(10.0f);
//    vboData[14]=(14.0f);
//    vboData[15]=(1.0f);
//    //5.Punkt
//    vboData[16]=(12.0f);
//    vboData[17]=(4.0f);
//    vboData[18]=(0.0f);
//    vboData[19]=(1.0f);
//    //6.Punkt
//    vboData[20]=(6.0f);
//    vboData[21]=(-2.0f);
//    vboData[22]=(9.0f);
//    vboData[23]=(1.0f);

//    iboData[0]=(0);
//    iboData[1]=(1);

//    iboData[2]=(1);
//    iboData[3]=(2);

//    iboData[4]=(2);
//    iboData[5]=(3);

//    iboData[6]=(3);
//    iboData[7]=(4);

//    iboData[8]=(4);
//    iboData[9]=(5);

//    qWarning() << ibo.create();
//    qWarning() << vbo.create();

//    ibo.bind();
//    vbo.bind();

//    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

//    vbo.allocate(vboData, sizeof(GLfloat) * vboLength);
//    ibo.allocate(iboData, sizeof(GLuint) * iboLength);

//    vbo.release();
//    ibo.release();

    kontrollPunkte.append( QVector3D(-1.0, 0.5, 2.0));
    kontrollPunkte.append( QVector3D(-4.0, 3.0, 3.0));
    kontrollPunkte.append( QVector3D(-2.5, 5.5, 4.0));
    kontrollPunkte.append( QVector3D(-0.5, 5.5, 2.0));
    kontrollPunkte.append( QVector3D(1.5, 5.5, 0.0));
    kontrollPunkte.append( QVector3D(3.0, 3.0, -2.0));
    kontrollPunkte.append( QVector3D(5.5, 4.5, -4.0));
    kontrollPunkte.append( QVector3D(8.0, 6.0, -3.0));
    kontrollPunkte.append( QVector3D(8.0, 3.0, 1.0));
    kontrollPunkte.append( QVector3D(5.0,0.5,3.0));
}
void Schlange::generiereBezierCurve()
{
    float schritt = 0.1;

    vboLength= (1.0/0.1) * 3 * 6 * 4;
    iboLength= vboLength;

    vboData = new GLfloat[vboLength];
    iboData = new GLuint[iboLength];


    unsigned int vertex_index = 0;
    unsigned int index_index = 0;
    for (int i = 0; i < kontrollPunkte.length() - 3; i+=3){
        for(float t= 0; t < 1; t+=0.1)
        {
            float k1 = (1 - t) * (1 - t) * (1 - t);
            float k2 = 3 * (1 - t) * (1 - t) * t;
            float k3 = 3 * (1 - t) * t * t;
            float k4 = t * t * t;

            QVector3D erg = k1 * kontrollPunkte.at(i) +
                            k2 * kontrollPunkte.at(i+1) +
                            k3 * kontrollPunkte.at(i+2) +
                            k4 * kontrollPunkte.at(i+3);

            vboData[vertex_index++] = erg.x();
            vboData[vertex_index++] = erg.y();
            vboData[vertex_index++] = erg.z();
            vboData[vertex_index++] = 1.0;

            //Normale / Tangente
            QVector3D erg_normal = erg.normalized();
            vboData[vertex_index++] = erg_normal.x();
            vboData[vertex_index++] = erg_normal.y();
            vboData[vertex_index++] = erg_normal.z();
            vboData[vertex_index++] = 1.0;


//            int segmente = 5;
//            float r = 0.5;
//            for(int n =0; n <= segmente; n++)
//            {
//                float alpha = n * M_2_PI  / float(segmente);
//                vboData[vertex_index++] = erg.x() * cos(alpha) * r;
//                vboData[vertex_index++] = erg.y() * sin(alpha) * r;
//                vboData[vertex_index++] = erg.z();
//                vboData[vertex_index++] = 1.0;
//                iboData[index_index] = index_index++;

//            }
            //qDebug() << vertex_index;
            //qDebug() << index_index;
            //qDebug() << erg;

            iboData[index_index] = index_index++;
        }
    }

    qWarning() << ibo.create();
    qWarning() << vbo.create();

    ibo.bind();
    vbo.bind();

    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    vbo.allocate(vboData, sizeof(GLfloat) * vboLength);
    ibo.allocate(iboData, sizeof(GLuint) * iboLength);

    vbo.release();
    ibo.release();

}
void Schlange::render(QMatrix4x4 pMatrix){

    mvMatrix.setToIdentity();

    mvMatrix.translate(-2.0,-3.0,-13.0);
    //mvMatrix.rotate(tmp/2,0.0,1.0,0.0);
    mvMatrix.rotate(tmp/2,1.0,0.0,0.0);
    tmp ++;

    int attrVertices = 0;
    int attrTexCoords = 3;
    int unifMatrix = 0;
    int unifPmatrix = 1;
    int attrColor=2;

    shaderProgram.bind();
    vbo.bind();
    ibo.bind();
    qtex->bind(0);

    shaderProgram.setUniformValue("texture",0);
    shaderProgram.setUniformValue(unifPmatrix, pMatrix);
    shaderProgram.setUniformValue(unifMatrix, mvMatrix);
    shaderProgram.setAttributeValue(attrColor, QVector4D(0.0,1.0,0.0,1.0));

    int offset = 0;
    size_t stride = 8 * sizeof(GLfloat);

    shaderProgram.setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
    //offset += 8 * sizeof(GLfloat);
    //shaderProgram.setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);

    shaderProgram.enableAttributeArray(attrVertices);
    //shaderProgram.enableAttributeArray(attrTexCoords);

    glDrawElements(GL_LINE_STRIP, this->iboLength , GL_UNSIGNED_INT,0);

    //Paint Normals
    shaderProgram.setAttributeValue(attrColor, QVector4D(1.0,0.0,0.0,1.0));
    shaderProgram.setAttributeBuffer(attrVertices, GL_FLOAT, 4 * sizeof(GLfloat), 4, stride);
    glDrawElements(GL_POINTS, this->iboLength , GL_UNSIGNED_INT,0);

    //Paint Zylinder
    //shaderProgram.setAttributeValue(attrColor, QVector4D(1.0,1.0,0.0,1.0));
    //shaderProgram.setAttributeBuffer(attrVertices, GL_FLOAT, 8 * sizeof(GLfloat), 4, stride);
    //glDrawElements(GL_POINTS, this->iboLength , GL_UNSIGNED_INT,0);

    qtex->release(0);
    vbo.release();
    ibo.release();
    shaderProgram.release();
}
