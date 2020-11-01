#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLTimerQuery>
#include <QElapsedTimer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public slots:
    void recompileFragShader(QString slot);

public:
    Renderer(QWidget *parent = Q_NULLPTR);
    ~Renderer();

    void update();

    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;
    void initShaders();
private:
    QElapsedTimer _elapsedTimer;

    QOpenGLBuffer *vertices;
    QOpenGLVertexArrayObject *vao;
    QOpenGLShaderProgram *m_program;
    QOpenGLShader *frag;

    int iResolution;
    qreal u_time;
};

#endif // RENDERER_H
