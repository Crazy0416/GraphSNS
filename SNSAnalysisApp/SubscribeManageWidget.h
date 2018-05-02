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
	CoauthorGraphItem* pCGI;					// CoauthorGraphItem 정보
	QListWidget* subscribeList;					// 구독한 사람들의 리스트
	QPushButton* addButton;						// 구독할 사람을 추가하는 Add 버튼
	QPushButton* deleteButton;					// 구독할 사람을 제거하는 Delete 버튼


public:
	explicit SubscribeManageWidget(CoauthorGraphItem* pCGI, QWidget *parent = 0);
	~SubscribeManageWidget();

	void addCoauthor(QString data) { subscribeList->addItem(data); }	// 프로퍼티인 listwidget에 QString data를 추가
	QListWidget* getSubscribeList() { return subscribeList; }


public slots:
	void onButtonClicked_addSubscribe();
	void onButtonClicked_deleteSubscribe();
};

