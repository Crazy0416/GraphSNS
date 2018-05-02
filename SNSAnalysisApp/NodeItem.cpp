#include "NodeItem.h"
#include <QtWidgets>
#include <QMessageBox>

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::MouseButton::LeftButton && mousePressmode == 1) {											// 노드 상태 메세지박스로 출력
		std::string temp2 = std::to_string(weightSum);
		std::string temp3 = "";
		for (int i = 0; i < coauthorList.size(); i++)
			temp3 = temp3 + coauthorList[i] + ",";
		std::string temp = label.toStdString().append("\nedge count : ") + temp2;
		temp.append("\ncoauthor : ").append(temp3);
		int ret = QMessageBox::warning(NULL, QMessageBox::tr("Node info"), QMessageBox::tr(temp.c_str()), QMessageBox::Cancel, QMessageBox::Cancel);
	}
	else if (mousePressmode == 2){
		if (event->button() == Qt::MouseButton::LeftButton){
			this->setSelected(true);
			QMessageBox::warning(NULL, QMessageBox::tr("Node info"), QMessageBox::tr("selected"), QMessageBox::Cancel, QMessageBox::Cancel);
		}
	}
	
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
}

NodeItem::NodeItem(double x, double y, QColor color, QString label)
{
	this->x = x;
	this->y = y;
	this->color = color;
	this->label = label;
	setZValue(1);
	mousePressmode = 1;

	setFlags(ItemIsSelectable | ItemIsMovable);
	setAcceptHoverEvents(true);
}

NodeItem::NodeItem(double x, double y, QColor color, QString label, int weightSum, std::vector<std::string> coauthorList)
{
	this->x = x;
	this->y = y;
	this->color = color;
	this->label = label;
	this->weightSum = weightSum;
	this->coauthorList = coauthorList;
	mousePressmode = 1;
	setFlags(ItemIsSelectable | ItemIsMovable);
	setZValue(1);
}

NodeItem::NodeItem(double x, double y, QColor color, QString label, int weightSum)
{
	this->x = x;
	this->y = y;
	this->color = color;
	this->label = label;
	this->weightSum = weightSum;
	mousePressmode = 1;
	setFlags(ItemIsSelectable | ItemIsMovable);
	setZValue(1);
}

QRectF NodeItem::boundingRect() const
{
	return QRectF(0, 0, NODE_SIZE, NODE_SIZE);
}

QPainterPath NodeItem::shape() const
{
	QPainterPath path;
	path.addRect(0, 0, NODE_SIZE, NODE_SIZE);
	return path;
}

void NodeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);

	//set node pen style
	QPen oldPen = painter->pen();
	QPen pen = oldPen;
	pen.setWidth(0);
	pen.setColor(QColor(Qt::black));

	//label 출력
	QFont font("Gulim", 3);
	font.setStyleStrategy(QFont::ForceOutline);
	painter->setFont(font);
	painter->save();
	painter->drawText(0, 0, label);
	painter->restore();


	//node rectangle 출력
	QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
	if (option->state & QStyle::State_MouseOver) fillColor = fillColor.light(125);
	painter->setPen(pen);
	painter->setBrush(QBrush(fillColor));
	painter->drawRect(0, 0, NODE_SIZE, NODE_SIZE);
}

void NodeItem::setColor(QColor color)
{
	this->color = color;
}

int NodeItem::getWeightSum()
{
	return weightSum;
}

QString NodeItem::getLabel()
{
	return label;
}