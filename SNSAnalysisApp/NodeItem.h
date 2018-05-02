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
	int x;						// x좌표
	int y;						// y좌표
	QColor color;				// 색칠될 색상
	QString label;				// author 이름
	int weightSum;				// 노드에 연결된 엣지의 합
	std::vector<std::string> coauthorList;// 노드와 연결된 coauthor들의 이름'
	int	mousePressmode;					//  마우스 프레스 모드 설정
	std::vector<bibliographyData> bibliographyList;	// 논문 데이터 리스트

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
