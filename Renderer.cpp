#include "Renderer.h"

#include <QApplication>
#include <QFile>
#include <QTimer>

// float clamp(float value, float lower, float upper) {
//     return std::min(std::max(value, lower), upper);
// }

Renderer::Renderer(QWidget *parent) : QOpenGLWidget(parent)
{
    // similar to GLFW's window hints, we select a profile and set
    // properties on the default surface that opengl will draw to
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
//    format.setVersion(4, 1);
    format.setSamples(4);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapInterval(0);
    setFormat(format);

//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(1000.f / 60.f);

//    setAcceptDrops(true);

//    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

//    initShaders();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000.f / 60.f);

    _elapsedTimer.start();
}

Renderer::~Renderer()
{
}

void Renderer::recompileFragShader(QString slot)
{
    QFile shaderFile(":/frag.shader");
    shaderFile.open(QFile::ReadOnly);

    QString shaderBasePlusSlot = QString(shaderFile.readAll()).arg(slot);

//    qDebug() << shaderBasePlusSlot;
//    m_program->release();
    m_program->removeShader(frag);
//    delete frag;
    frag = new QOpenGLShader(QOpenGLShader::Fragment);
    frag->compileSourceCode(shaderBasePlusSlot);

    m_program->addShader(frag);

//    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, shaderBasePlusSlot);
//    m_program->link();
//    m_program->bind();
}

void Renderer::update()
{
    // Schedule a redraw
    QOpenGLWidget::update();
}

void Renderer::initializeGL()
{
    initializeOpenGLFunctions(); // obvious

    const float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    static const char *vshader =
            "#version 330 core\n"
            "layout(location=0) in vec2 posAttr;\n"
            "layout(location=1) in vec2 texCoord;\n"
            "out vec2 TexCoord;\n"
            "void main() {\n"
            "TexCoord = texCoord;\n"
            "gl_Position = vec4(posAttr, 0.0, 1.0); }";

    static const char *fshader =
            "#version 330 core\n"
            "out vec4 col;\n"
            "uniform vec2 iResolution;\n"
            "uniform float u_time;\n"
            "in vec2 TexCoord;\n"
            "void main() {\n"
            "vec2 st = gl_FragCoord.xy / iResolution.xy;\n"
            "vec3 color = vec3(0.);\n"
            "color = vec3(st.x,st.y,abs(sin(u_time)));\n"
            "col = vec4(color, 1.0); }";

      // Create Shader (Do not release until VAO is created)

       frag = new QOpenGLShader(QOpenGLShader::Fragment);
       frag->compileSourceFile(":/default_frag.shader");

      m_program = new QOpenGLShaderProgram();
      m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader);
//      m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader);
//      m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/frag.shader");
      m_program->addShader(frag);
      m_program->link();
      m_program->bind();

      iResolution = m_program->uniformLocation("iResolution");
      u_time = m_program->uniformLocation("u_time");

    vao = new QOpenGLVertexArrayObject;
    vao->create();
    vao->bind();

    vertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertices->create();
    vertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertices->bind();
    vertices->allocate(quadVertices, sizeof(quadVertices) * sizeof (GLfloat));

    vao->release();
}

void Renderer::paintGL()
{
    float timeElapsed = _elapsedTimer.nsecsElapsed() / (1000.f * 1000.f * 1000.f);
    // restart this timer to get delta time!
//    _elapsedTimer.restart();


//    glClearColor(0.3, 0.5, 0.9, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    // On 4k so need to scale to the monitor DPI
    QVector2D paneSize(width() * devicePixelRatio(), height() * devicePixelRatio());
    glViewport(0, 0, paneSize.x(), paneSize.y());

    vao->bind();
    m_program->bind();

    m_program->setUniformValue(iResolution, paneSize);
    m_program->setUniformValue(u_time, timeElapsed);

    m_program->bindAttributeLocation("posAttr", 0);
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 4);

    m_program->bindAttributeLocation("texCoord", 1);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_UNSIGNED_BYTE, 2, 4);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_program->release();
    vao->release();
}

void Renderer::resizeGL(int w, int h)
{
    QVector2D paneSize(width() * devicePixelRatio(), height() * devicePixelRatio());
    glViewport(0, 0, paneSize.x(), paneSize.y());

    update();
}

void Renderer::initShaders()
{
//    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.shader");
//    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/frag.shader");
//    m_program.link();
//    m_program.bind();

//    m_vertex.create();
//   m_vertex.bind();
//   m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
//   m_vertex.allocate(quadVertices, sizeof(quadVertices));

//   // Create Vertex Array Object
//   m_object.create();
//   m_object.bind();
//   m_program.enableAttributeArray(0);
////   m_program.enableAttributeArray(1);
//   m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3);
////   m_program.setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

//   // Release (unbind) all
//   m_object.release();
//   m_vertex.release();
//   m_program.release();
}
