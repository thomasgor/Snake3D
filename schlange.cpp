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

    qtex = new QOpenGLTexture(QImage(":/textures/schlange2.jpg").mirrored());
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

QVector3D Schlange::kubischerBezierPunkt(float t, int i){
    float k1 = (1 - t) * (1 - t) * (1 - t);
    float k2 = 3 * (1 - t) * (1 - t) * t;
    float k3 = 3 * (1 - t) * t * t;
    float k4 = t * t * t;

    QVector3D erg = k1 * kontrollPunkte.at(i) +
                    k2 * kontrollPunkte.at(i+1) +
                    k3 * kontrollPunkte.at(i+2) +
                    k4 * kontrollPunkte.at(i+3);
    return erg;
}

void Schlange::loadBezier(){
    kontrollPunkte.append( QVector3D(-1.0, 0.5, 2.0));
    kontrollPunkte.append( QVector3D(-4.0, 3.0, 3.0));
    kontrollPunkte.append( QVector3D(-2.5, 5.5, 4.0));
    kontrollPunkte.append( QVector3D(-0.5, 5.5, 2.0));
    kontrollPunkte.append( QVector3D(1.5, 5.5, 0.0));
    kontrollPunkte.append( QVector3D(3.0, 3.0, -2.0));
    kontrollPunkte.append( QVector3D(5.5, 4.5, -4.0));
    kontrollPunkte.append( QVector3D(8.0, 6.0, -2.0));
    kontrollPunkte.append( QVector3D(8.0, 6.0, 1.0));
    kontrollPunkte.append( QVector3D(7.0,0.5,3.0));
}

void Schlange::generiereBezierCurve()
{
    double schritt = 0.01;
    qWarning() << "Schrittweite: " << schritt;
    int segmente = 20;
    float r = 0.5;

    vboLength= (1.0/schritt) * 3 * 4 * (segmente) * 4;
    iboLength= vboLength;

    vboData = new GLfloat[vboLength];
    iboData = new GLuint[iboLength];


    unsigned int vertex_index = 0;
    unsigned int index_index = 0;
    unsigned int aktuellerPunkt = 0;
    for (int i = 0; i < kontrollPunkte.length() - 3; i+=3){
        for(float t= 0; t < 1; t+=schritt)
        {
            //Interpolierte Bezier Punkte  -> Mittelpunkte der Schlange
            QVector3D erg = kubischerBezierPunkt(t,i);
            QVector3D erg1 = kubischerBezierPunkt(t+schritt,i);

            //Normale / Tangente / binormale -> Lokales KoordinatenSystem
            QVector3D erg_tangente  =  erg1 - erg;
            QVector3D erg_normal = erg.normalized();
            //QVector3D erg_binormal = erg.normalized().crossProduct(erg_tangente, erg_normal);
            QVector3D erg_binormal = erg.crossProduct(erg_tangente, erg_normal);


            //Vektoren auf Länge 1 bringen
            erg_tangente *= 1./erg_tangente.length();
            erg_binormal *= 1./erg_binormal.length();

            //Vertex und Index Einträge für das Mesh generieren
            //TODO Hier sollten noch die NOrmalen der Punkte eingespeichert werden
            //TODO Wahrscheinlich werden hier auch viel zu viele Dreiecke generiert
            for(int n =0; n < segmente; n++)
            {
                float alpha = n * 2 * M_PI  / float(segmente);
                QVector3D tmp = erg + (erg_normal * cos(alpha) + erg_binormal * sin(alpha)) * r;
                vboData[vertex_index++] = tmp.x();
                vboData[vertex_index++] = tmp.y();
                vboData[vertex_index++] = tmp.z();
                vboData[vertex_index++] = 1.0;

                //Zwei dreiecke Speichern. Das ergibt ein Viereck
                iboData[index_index++] = aktuellerPunkt;
                iboData[index_index++] = aktuellerPunkt + segmente;
                iboData[index_index++] = aktuellerPunkt + 1;

                iboData[index_index++] = aktuellerPunkt +1;
                iboData[index_index++] = aktuellerPunkt + segmente;
                iboData[index_index++] = aktuellerPunkt + segmente + 1;
                aktuellerPunkt++;

                //WENN MAN NUR DIE PUNKTE SEHEN MÖCHTE +  GL_POINTS aktivieren
                //iboData[index_index] = index_index++;
            }

        }
    }


    qWarning() << "Vertex Größe:" << vertex_index;
    qWarning() << "Index Größe:" << index_index;

    qWarning() << "Erstellung des IBO: " << ibo.create();
    qWarning() << "Erstellung des VBO: " << vbo.create();

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

    //Temporäre Animation
    mvMatrix.translate(-2.0,-1.0,-13.0);
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
    size_t stride = 4 * sizeof(GLfloat);

    shaderProgram.setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
    //offset += 4 * sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);

    shaderProgram.enableAttributeArray(attrVertices);
    shaderProgram.enableAttributeArray(attrTexCoords);

    //FÜR DIE FLÄCHE GL_TRIANGLES VERWENDEN, GL_LINES FÜR DAS MESH
    glDrawElements(GL_TRIANGLES, this->iboLength , GL_UNSIGNED_INT,0);

    qtex->release(0);
    vbo.release();
    ibo.release();
    shaderProgram.release();
}
