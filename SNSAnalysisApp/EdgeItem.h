#ifndef EDGEITEM_H
#pragma once

#include <QColor>
#include <QGraphicsItem>

class EdgeItem
	: public QGraphicsItem
{
private:
	double x1, y1, x2, y2;
	int width;
	QColor color;
	int start_nodeIndex;
	int end_nodeIndex;
	QString Label;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public:
	EdgeItem(double x1, double y1, double x2, double y2, QColor color, int width, int start = -1, int end = -1);

	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	qreal getX1() { return x1; }
	qreal getX2() { return x2; }
	qreal getY1() { return y1; }
	qreal getY2() { return y2; }
	void setStartNodeIndex(int start) { this->start_nodeIndex = start; }
	void setEndNodeIndex(int end) { this->end_nodeIndex = end; }
	void setWidth(int width) { this->width = width; }
	int getStartNodeIndex() { return start_nodeIndex; }
	int getEndNodeIndex() { return end_nodeIndex; }
	void setLabel(QString label) { this->Label = label; }

	void setColor(QColor color) { this->color = color; }
};

#endif // EDGEITEM_H