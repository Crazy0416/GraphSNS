#pragma once

#include <QGraphicsItem>
#include <QColor>
#include <vector>
#include <string>

const int NODE_SIZE = 4;

struct bibliographyData {
	int year;
	QString key;
};

class NodeItem
	: public QGraphicsItem
{
private:
	int x;						// x��ǥ
	int y;						// y��ǥ
	QColor color;				// ��ĥ�� ����
	QString label;				// author �̸�
	int weightSum;				// ��忡 ����� ������ ��
	std::vector<std::string> coauthorList;// ���� ����� coauthor���� �̸�'
	int	mousePressmode;					//  ���콺 ������ ��� ����
	std::vector<bibliographyData> bibliographyList;	// �� ������ ����Ʈ

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public:
	NodeItem(double x, double y, QColor color, QString label);
	NodeItem(double x, double y, QColor color, QString label, int weightSum);
	NodeItem(double x, double y, QColor color, QString label, int weightSum, std::vector<std::string> coauthorList);
	NodeItem(NodeItem& src);

	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	void setColor(QColor color);
	void setcoauthorList(std::vector<std::string> data) { coauthorList.swap(data); }
	std::vector<bibliographyData>* getbibliographyList() { return &bibliographyList; }
	int getWeightSum();
	QString getLabel();
	std::vector<std::string> getcoauthorList() { return coauthorList; }
	int getX() { return x; }
	int getY() { return y; }

	void coauthorPush_back(std::string coauthor) { coauthorList.push_back(coauthor); }
	void bibliographyPush_back(bibliographyData data) { bibliographyList.push_back(data); }

	bool operator>(const NodeItem& ni) const { return weightSum > ni.weightSum; }
	bool operator<(const NodeItem& ni) const { return weightSum < ni.weightSum; }
};
