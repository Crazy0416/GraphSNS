#pragma once
#include <qdialog.h>
#include <qlistwidget.h>
#include<qcoreapplication.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include <GraphItem.h>

class SubscribeManageWidget
	: public QDialog
{
	Q_OBJECT
private:
	CoauthorGraphItem* pCGI;					// CoauthorGraphItem ����
	QListWidget* subscribeList;					// ������ ������� ����Ʈ
	QPushButton* addButton;						// ������ ����� �߰��ϴ� Add ��ư
	QPushButton* deleteButton;					// ������ ����� �����ϴ� Delete ��ư


public:
	explicit SubscribeManageWidget(CoauthorGraphItem* pCGI, QWidget *parent = 0);
	~SubscribeManageWidget();

	void addCoauthor(QString data) { subscribeList->addItem(data); }	// ������Ƽ�� listwidget�� QString data�� �߰�
	QListWidget* getSubscribeList() { return subscribeList; }


public slots:
	void onButtonClicked_addSubscribe();
	void onButtonClicked_deleteSubscribe();
};

