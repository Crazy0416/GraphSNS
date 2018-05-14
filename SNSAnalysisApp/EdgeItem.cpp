#include "EdgeItem.h"
#include <QtWidgets>

void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
}

void EdgeItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
}

void EdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
}

EdgeItem::EdgeItem(double x1, double y1, double x2, double y2, QColor color, int width, int start, int end)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->start_nodeIndex = start;
	this->end_nodeIndex = end;

	this->color = color;
	this->width = width;
	setZValue(0);	//노드 앞 가리지 않도록 ZValue 설정

					/*setFlags(ItemIsSelectable | ItemIsMovable);
					setAcceptHoverEvents(true);*/
}
EdgeItem::EdgeItem(EdgeItem& src)
	: x1(src.x1), y1(src.y1), x2(src.x2), y2(src.y2), width(src.width), color(src.color),
	start_nodeIndex(src.start_nodeIndex), end_nodeIndex(src.end_nodeIndex), Label(src.Label) {}

QRectF EdgeItem::boundingRect() const
{
	double startX, startY;
	double height = abs(y1 - y2);
	double width = abs(x1 - x2);

	if (x1 > x2)
		startX = x2;
	else
		startX = x1;
	if (y1 > y2)
		startY = y2;
	else
		startY = y1;

	return QRectF(startX, startY, width, height);
}

QPainterPath EdgeItem::shape() const
{
	QPainterPath path;
	return path;
}

void EdgeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);

	QPen oldPen = painter->pen();
	QPen pen = oldPen;
	pen.setWidth(width);
	pen.setColor(color);
	painter->setPen(pen);
	painter->drawLine(QLineF(x1, y1, x2, y2));
	painter->drawText(QPoint((x1 + x2) / 2, (y1 + y2) / 2),Label);
}
