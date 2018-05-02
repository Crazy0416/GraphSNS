#include "SubscribeManageWidget.h"

/*
*/
SubscribeManageWidget::SubscribeManageWidget(CoauthorGraphItem* pCGI, QWidget *parent)
	: QDialog(parent)
{
	QGridLayout *topLayout = new QGridLayout(this);
	subscribeList = new QListWidget(this);
	addButton = new QPushButton("Add subscribe", this);
	deleteButton = new QPushButton("Delete subscribe", this);
	this->pCGI = pCGI;

	topLayout->addWidget(subscribeList, 100, 1000);
	topLayout->addWidget(addButton, 200, 100);
	topLayout->addWidget(deleteButton, 400, 100);

	setLayout(topLayout);
	setBaseSize(500, 1000);

	QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(onButtonClicked_addSubscribe()));
	QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(onButtonClicked_deleteSubscribe()));
}

SubscribeManageWidget::~SubscribeManageWidget()
{
}

void SubscribeManageWidget::onButtonClicked_addSubscribe() {
	QStringList coauthorList;

	for (int i = 0; i < pCGI->getNodeList().size(); i++) {
		coauthorList << pCGI->getNodeList()[i]->getLabel();
	}
	QString selectAuthor = QInputDialog::getItem(this, "choose author", "Choose author", coauthorList);
	addCoauthor(selectAuthor);
}

void SubscribeManageWidget::onButtonClicked_deleteSubscribe() {
	QListWidgetItem* selectedItem = subscribeList->takeItem(subscribeList->currentRow());
	delete selectedItem;
}