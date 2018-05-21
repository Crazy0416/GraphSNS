#pragma once

#include <QtWidgets/QWidget>
#include <QGraphicsScene>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/graph_traits.hpp>

#include "ui_subwindow.h"
#include "GraphItem.h"
#include "qmainwindow.h"

using namespace boost;
using namespace std;

class subwindow : public QMainWindow
{
	Q_OBJECT

public:
	subwindow(QWidget *parent = 0);
	~subwindow();

	void setGraph(CoauthorGraphItem* pCGI);
	void setGraph(PaperGraphItem* pPGI);
	void setCGraph(CoauthorGraphItem* pCGI) { this->pCGI = pCGI; };		// pCGI가 외부에서 할당되고 나서 불러야한다.

private:
	void initscene();

	Ui::subwindow ui;
	QGraphicsScene *scene;
	CoauthorGraphItem* pCGI;
	PaperGraphItem* pPGI;

	public slots:
	void Main_findAuthor();
};