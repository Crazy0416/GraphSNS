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
* Author���� �����͵��� ListWidget���� ��Ÿ��
* Author�� QPushButton���� �����ϸ� �ش� author�� ������ �� ������ ũ�Ѹ� �ϰ� ��
*/
class ListInput :
	public QDialog
{
	Q_OBJECT

private:
	// Author���� ����Ʈ�� ������ ����
	QListWidget* listwidget;
	// �����ϰ� ���� Author�� ��ư���� Ŭ���ϸ� ũ�Ѹ� ����
	QPushButton* selectButton;

public:
	explicit ListInput(QWidget *parent = 0);
	~ListInput();

	/* ������Ƽ�� listwidget�� QString data�� �߰�
	*/
	void addItem(QString data) { listwidget->addItem(data); }

public slots:
	/* ��ư�� ������ ũ�Ѹ� ����
	*/
	void on_PushSelectButton_Clicked();
};

