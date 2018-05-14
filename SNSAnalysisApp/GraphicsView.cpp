#include "GraphicsView.h"
#include <qmath.h>
#include <qdebug.h>
#include <QKeyEvent>

//View
View::View(const QString& name, QWidget *parent)
	: QFrame(parent)
{
	//GraphicsView settings
	graphicsView = new GraphicsView(this);
	graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	//graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate); // this is legacy
	graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	graphicsView->scale(0.1, 0.1);
	//layout
	QGridLayout *topLayout = new QGridLayout;
	topLayout->addWidget(graphicsView, 0, 0);
	setLayout(topLayout);
	setupMatrix();
}

QGraphicsView* View::view() const
{
	return static_cast<QGraphicsView *>(graphicsView);
}

void View::setupMatrix()
{
	qreal scale = qPow(qreal(1), qreal(1));

	QMatrix matrix;
	matrix.scale(scale, scale);
	matrix.rotate(qreal(0));

	graphicsView->setMatrix(matrix);
}

#ifndef QT_NO_WHEELEVENT
void GraphicsView::wheelEvent(QWheelEvent * event)
{
	scaleView(pow((double)2, event->delta() / 240.0));
}
#endif

void GraphicsView::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;
	qDebug() << "scale : " << scaleFactor;
	scale(scaleFactor, scaleFactor);
}
