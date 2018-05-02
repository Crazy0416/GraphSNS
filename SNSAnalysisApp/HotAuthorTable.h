#pragma once
#include <qtablewidget.h>
#include <QtWidgets/QWidget>
#include <QGraphicsScene>

using namespace std;

class HotAuthorTable
	: public QTableWidget
{
	Q_OBJECT

public:
	HotAuthorTable();
	~HotAuthorTable();

	public slots:
	void mycellChanged(int a, int b);
};

