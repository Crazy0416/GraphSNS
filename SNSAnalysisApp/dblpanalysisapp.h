#pragma once

#include <QtWidgets/QMainWindow>
#include <qinputdialog.h>
#include <qgridlayout.h>
#include <qlistwidget.h>
#include <vector>

#include "ui_dblpanalysisapp.h"
#include "GraphItem.h"
#include "subwindow.h"
#include "listInput.h"
#include "SubscribeManageWidget.h"
#include "HotAuthorTable.h"

/*
* File name : dblpanalysisapp.cpp
* author : 김민호, 정석현
* purpose : 앱 총괄 관리, 구동을 맡음
* Usage file : dblpanalysisapp.h
*/

class dblpAnalysisApp : public QMainWindow
{
	Q_OBJECT

public:
	dblpAnalysisApp(QWidget *parent = 0);
	~dblpAnalysisApp();

	void load_CGI();
	void load_PGI();

private:
	Ui::dblpAnalysisAppClass ui;
	QGraphicsScene *scene;

	const char* COAUTHORSHIP_FILENAME = "dblp-coauthor.txt";
	const char* PAPER_FILENAME = "dblp-paper.txt";

	CoauthorGraphItem* pCGI;
	PaperGraphItem* pPGI;

	QListWidget* subscribeListWidget;		// 구독자 리스트 위젯
	vector<QString> subscribeArray;			// 구독자 이름 배열
	SubscribeManageWidget* subscribemanage;	

	public slots :
	// 스트리밍 데이터 처리
	void updateCGI(); 
	void updatePGI();

	// 기본 기능
	void Main_Visualization();
	void Main_TopK();
	void Main_TopKfromA();
	void Main_Chain();
	void Main_AuthorCrawling();

	// 추가 기능
	void Main_VisualizationInCircle();
	void Main_Filtering();
	void showHotAuthorRank();
	void manageSubscribe();
	void Print_percent();
};
