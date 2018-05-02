#pragma once
#include <QtWidgets/QMainWindow>
#include "qdialog.h"
#include <iostream>
#include <qdebug.h>

#include<qcoreapplication.h>
#include <qthread.h>

#include <qlistwidget.h>
#include <QGridLayout>
#include <qpushbutton.h>
using namespace std;

/*
* Author들의 데이터들을 ListWidget으로 나타냄
* Author를 QPushButton으로 선택하면 해당 author가 참여한 논문 주제를 크롤링 하게 됌
*/
class ListInput :
	public QDialog
{
	Q_OBJECT

private:
	// Author들의 리스트의 정보를 가짐
	QListWidget* listwidget;
	// 선택하고 싶은 Author를 버튼으로 클릭하면 크롤링 시작
	QPushButton* selectButton;

public:
	explicit ListInput(QWidget *parent = 0);
	~ListInput();

	/* 프로퍼티인 listwidget에 QString data를 추가
	*/
	void addItem(QString data) { listwidget->addItem(data); }

public slots:
	/* 버튼을 누르면 크롤링 시작
	*/
	void on_PushSelectButton_Clicked();
};

