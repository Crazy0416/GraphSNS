#include "HotAuthorTable.h"



HotAuthorTable::HotAuthorTable()
{
	int nrow, ncolumn;
	QStringList list;

	nrow = 10;
	ncolumn = 3;

	//set size of QTableWidget
	setRowCount(nrow);
	setColumnCount(ncolumn);

	//add header to QTableWidgetItem
	list << "bibliography Count" << "Author" << "bibliography Keys";
	setHorizontalHeaderLabels(list);
	setEditTriggers(QAbstractItemView::AllEditTriggers);
	//set sample item
	QTableWidgetItem *newitem = new QTableWidgetItem("Fill Item");
	newitem->setFlags(newitem->flags() ^ Qt::ItemIsEditable);
	setItem(0, 0, newitem);

	//Create Signal and slot for QTableWidget
	connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(mycellChanged(int, int)));
}


HotAuthorTable::~HotAuthorTable()
{
}

void HotAuthorTable::mycellChanged(int a, int b) {

}