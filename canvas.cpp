#include "Canvas.h"

#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QTimeLine>
#include <cmath>

#ifdef QT_DEBUG
    #include <QDebug>
#endif

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setStyleSheet("border: 0");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCacheMode(QGraphicsView::CacheBackground);

    installEventFilter(this);

    // Grimey!
    zoomInFactor = 1.25;
    zoomClamp = false;
    zoom = 10;
    zoomStep = 1;
    zoomRange = {5, 16};

//    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

Canvas::~Canvas()
{
//    delete graphicsScene;
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        setDragMode(QGraphicsView::NoDrag);
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void Canvas::scalingTime(qreal x)
{
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 300.0;
    scale(factor, factor);
}

void Canvas::animFinished() {
    if (_numScheduledScalings > 0) _numScheduledScalings--;
    else _numScheduledScalings++;
    sender()->~QObject();
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    auto angleDelta = event->angleDelta();

#ifdef SMOOTHZOOM
    int numDegrees = angleDelta.y() / 8;

    int numSteps = numDegrees / 15; // see QWheelEvent documentation
    _numScheduledScalings += numSteps;
    if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
        _numScheduledScalings = numSteps;

    QTimeLine *anim = new QTimeLine(200, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(animFinished()));
    anim->start();
#else
    float zoomOutFactor = 1 / zoomInFactor;

    qreal zoomFactor;

    if (angleDelta.y() > 0) {
        zoomFactor = zoomInFactor;
        zoom += zoomStep;
    } else {
        zoomFactor = zoomOutFactor;
        zoom -= zoomStep;
    }

    bool clamped = false;

    if (zoom < zoomRange[0]) { zoom= zoomRange[0]; clamped = true; }
    if (zoom > zoomRange[1]) { zoom= zoomRange[1]; clamped = true; }

    if (!clamped) scale(zoomFactor, zoomFactor);
#endif
}

// Qt's graphics view wants to drag with left click only by default, override this
bool Canvas::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

      if (mouseEvent->button() == Qt::MiddleButton) {
          // Temporarly enable dragging mode
          this->setDragMode(QGraphicsView::ScrollHandDrag);
          // Emit a left mouse click (the default button for the drag mode)
          QMouseEvent* pressEvent = new QMouseEvent(QEvent::GraphicsSceneMousePress,
                                    mouseEvent->pos(), Qt::MouseButton::LeftButton,
                                    Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier);

          this->mousePressEvent(pressEvent);
      }
      else if (event->type() == QEvent::MouseButtonRelease) {
        // Disable drag mode if dragging is finished
        this->setDragMode(QGraphicsView::DragMode::NoDrag);
      }
    }

  Q_UNUSED(object)
  return false;
}
