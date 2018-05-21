#include "dblpanalysisapp.h"
#include <qstatusbar.h>
#include <qaction.h>
#include "qmainwindow.h"

dblpAnalysisApp::dblpAnalysisApp(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// 세마포어 설정
	cgiSem = new QSemaphore(1);
	pgiSem = new QSemaphore(1);

	// 배경 화면 설정
	ui.setupUi(this);
	QPixmap bkgnd("bkgnd.png");
	bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
	this->setFixedSize(600, 400);

	// 파일 로드
	load_CGI();
	load_PGI();

	subscribemanage = new SubscribeManageWidget(pCGI);
	// 파일 업데이트
	QTimer *pTimer = new QTimer(this);
	QObject::connect(pTimer, SIGNAL(timeout()), SLOT(updateCGI()));
	//QObject::connect(pTimer, SIGNAL(timeout()), SLOT(updatePGI()));
	pTimer->start(10000); // 1sec == 1000mils;

	// 메뉴 바 설정
	QMenu *pAppMenu;
	QStatusBar *pStatusBar;

	// 기본 기능 메뉴 설정
	QAction *pSlotVisualize = new QAction(tr("Visualization"), this);
	QObject::connect(pSlotVisualize, SIGNAL(triggered()), this, SLOT(Main_Visualization()));

	QAction *pSlotTopK = new QAction(tr("Top-k Coloring"), this);
	QObject::connect(pSlotTopK, SIGNAL(triggered()), this, SLOT(Main_TopK()));

	QAction *pSlotTopKfromA = new QAction(tr("Top-K from A Coloring"), this);
	QObject::connect(pSlotTopKfromA, SIGNAL(triggered()), this, SLOT(Main_TopKfromA()));

	QAction *pSlotChain = new QAction(tr("Chain from A to B"), this);
	QObject::connect(pSlotChain, SIGNAL(triggered()), this, SLOT(Main_Chain()));

	QAction *pSlotAuthorCrawling = new QAction(tr("Author Crawling"), this);
	QObject::connect(pSlotAuthorCrawling, SIGNAL(triggered()), this, SLOT(Main_AuthorCrawling()));

	pAppMenu = menuBar()->addMenu(tr("Basic Menu"));
	pAppMenu->addAction(pSlotVisualize);
	pAppMenu->addAction(pSlotTopK);
	pAppMenu->addAction(pSlotTopKfromA);
	pAppMenu->addAction(pSlotChain);
	pAppMenu->addAction(pSlotAuthorCrawling);
	
	// 추가 기능 메뉴 설정
	QAction *pSlotVisualizeInCircle = new QAction(tr("Visualization In Circle Layer"), this);
	QObject::connect(pSlotVisualizeInCircle, SIGNAL(triggered()), this, SLOT(Main_VisualizationInCircle()));

	QAction *pSlotFiltering = new QAction(tr("Filtering"), this);
	QObject::connect(pSlotFiltering, SIGNAL(triggered()), this, SLOT(Main_Filtering()));

	QAction *pSlotshowSubscribe = new QAction(tr("Hot Author in year"), this);
	QObject::connect(pSlotshowSubscribe, SIGNAL(triggered()), this, SLOT(showHotAuthorRank()));

	QAction *pSlotmanageSubscribe = new QAction(tr("Subscribe Manage"), this);
	QObject::connect(pSlotmanageSubscribe, SIGNAL(triggered()), this, SLOT(manageSubscribe()));

	QAction *pSlotprintPercent = new QAction(tr("Print Percent"), this);
	QObject::connect(pSlotprintPercent, SIGNAL(triggered()), this, SLOT(Print_percent()));

	pAppMenu = menuBar()->addMenu(tr("Additional Menu"));
	pAppMenu->addAction(pSlotshowSubscribe);
	pAppMenu->addAction(pSlotmanageSubscribe);
	pAppMenu->addAction(pSlotVisualizeInCircle);
	pAppMenu->addAction(pSlotFiltering);
	pAppMenu->addAction(pSlotprintPercent);
}

dblpAnalysisApp::~dblpAnalysisApp()
{

}

//=====================================================================================================
void dblpAnalysisApp::load_CGI()
{
	cgiSem->acquire(1);
	try {
		ifstream fsIn(COAUTHORSHIP_FILENAME);
		pCGI = new CoauthorGraphItem(fsIn);

		fsIn.close();
	}
	catch (const std::exception& e) {
		qDebug() << "Coauthor File Reading Error: " << e.what();
	}
	cgiSem->release(1);
}
void dblpAnalysisApp::load_PGI()
{
	pgiSem->acquire(1);
	try {
		ifstream fsIn(PAPER_FILENAME);
		pPGI = new PaperGraphItem(fsIn);

		fsIn.close();
	}
	catch(const std::exception& e) {
		qDebug() << "Paper File Reading Error: " << e.what();
	}
	pgiSem->release(1);
}
//=====================================================================================================
void dblpAnalysisApp::updateCGI()
{
	cgiSem->acquire(1);
	QStringList* updateCoauthorList;
	try {
		ifstream fsIn(COAUTHORSHIP_FILENAME);
		updateCoauthorList = pCGI->updateGraphReturnAuthorList(fsIn);
		
		if (updateCoauthorList == nullptr) {
			cgiSem->release(1);
			return;
		}
		
		for (int i = 0; i < updateCoauthorList->size(); i++) {
			QString coauthorName = updateCoauthorList->at(i);
			for (int j = 0; j < subscribemanage->getSubscribeList()->count(); j++) {
				QListWidgetItem* data = subscribemanage->getSubscribeList()->item(j);
				if (data->text().compare(coauthorName) == 0) {
					QMessageBox::warning(this, "Update data", "Update " + coauthorName, QMessageBox::Cancel, QMessageBox::Cancel);
				}
			}
		}
		delete updateCoauthorList;
		fsIn.close();
	}
	catch (const std::exception& e) {
		qDebug() << "Coauthor File Reading Error: " << e.what();
	}
	cgiSem->release(1);
}
void dblpAnalysisApp::updatePGI()
{
	pgiSem->acquire(1);
	try {
		ifstream fsIn(PAPER_FILENAME);
		pPGI->updateGraph(fsIn);

		fsIn.close();
	}
	catch (const std::exception& e) {
		qDebug() << "Paper File Reading Error: " << e.what();
	}
	pgiSem->release(1);
}
//=====================================================================================================
void dblpAnalysisApp::Main_Visualization()
{
	subwindow* sw = new subwindow;
	sw->setGraph(pCGI);
	sw->setCGraph(pCGI);
	sw->show();
}
void dblpAnalysisApp::Main_TopK()
{
	CoauthorGraphItem* pTempCGI = new CoauthorGraphItem(*pCGI);

	bool ok;
	QString K = QInputDialog::getText(NULL, "K Size", "K Size", QLineEdit::Normal, "", &ok);
	if (ok && !K.isEmpty())
		;// K 숫자 입력인지 예외 처리

	pTempCGI->TopK(K.toInt());

	subwindow* sw = new subwindow;
	sw->setGraph(pTempCGI);
	sw->show();
}
void dblpAnalysisApp::Main_TopKfromA()
{
	CoauthorGraphItem* pTempCGI = new CoauthorGraphItem(*pCGI);
	QStringList coauthorList;
	for (int i = 0; i < pCGI->getNodeList().size(); i++) {
		coauthorList << pCGI->getNodeList()[i]->getLabel();
	}

	bool ok;
	QString K = QInputDialog::getText(NULL, "K Size", "K Size", QLineEdit::Normal, "", &ok);
	if (ok && !K.isEmpty())
		;// K 숫자 입력인지 예외 처리

	//QString author = QInputDialog::getText(NULL, "Author Name", "Author Name", QLineEdit::Normal, "", &ok);
	QString author = QInputDialog::getItem(this, "choose author", "Choose author", coauthorList);

	pTempCGI->TopKfromA(K.toInt(),author);

	subwindow* sw = new subwindow;
	sw->setGraph(pTempCGI);
	sw->show();
}
void dblpAnalysisApp::Main_Chain()
{
	CoauthorGraphItem* pTempCGI = new CoauthorGraphItem(*pCGI);
	QStringList coauthorList;
	for (int i = 0; i < pCGI->getNodeList().size(); i++) {
		coauthorList << pCGI->getNodeList()[i]->getLabel();
	}

	bool ok;
	QString author1 = QInputDialog::getItem(this, "choose author1", "Choose author1", coauthorList);
	QString author2 = QInputDialog::getItem(this, "choose author2", "Choose author2", coauthorList);
	// author name 없는 경우 예외 처리

	pTempCGI->bfs(author1, author2);

	subwindow* sw = new subwindow;
	sw->setGraph(pTempCGI);
	sw->show();
}

void dblpAnalysisApp::Main_AuthorCrawling()
{
	// 리스트 위젯을 넣을 창 만듦
	ListInput* list = new ListInput();

	// 리스트 창에 author 데이터 넣음
	graph_traits<Graph>::vertex_iterator vi, ve;
	auto Cgraph_vertex_name = get(vertex_name, *pCGI->getGraph());
	int cnt = 1;
	for (tie(vi, ve) = vertices(*pCGI->getGraph()); vi != ve; ++vi) {
		//qDebug() << Cgraph_vertex_name[*vi].c_str();
		//qDebug() << Cgraph_vertex_index[*vi];
		QString tmp = QString::number(cnt);
		tmp.append(".");
		tmp.append(Cgraph_vertex_name[*vi].c_str());
		list->addItem(tmp);
		cnt++;
	}

	list->show();
}
//=====================================================================================================
void dblpAnalysisApp::Main_VisualizationInCircle()
{
	CoauthorGraphItem* pTempCGI = new CoauthorGraphItem(*pCGI);

	pTempCGI->VisualizationInCircle();

	subwindow* sw = new subwindow;
	sw->setGraph(pTempCGI);
	sw->show();
}

void dblpAnalysisApp::Main_Filtering()
{
	qDebug() << "Hello";
	PaperGraphItem* pTempPGI = new PaperGraphItem(pPGI->getGraph());

	bool ok;
	QString year = QInputDialog::getText(NULL, "Year", "Year", QLineEdit::Normal, "", &ok);
	QString conf = QInputDialog::getText(NULL, "Conference", "Conference", QLineEdit::Normal, "", &ok);

	pTempPGI->Filtering(year, conf);

	subwindow* sw = new subwindow;
	sw->setGraph(pTempPGI);
	sw->show();
}

void dblpAnalysisApp::showHotAuthorRank()
{
	HotAuthorTable *t = new HotAuthorTable();
	t->show();
}

void dblpAnalysisApp::manageSubscribe()
{
	subscribemanage->show();
}

void dblpAnalysisApp::Print_percent()
{
	CoauthorGraphItem* pTempCGI = new CoauthorGraphItem(*pCGI);

	QStringList coauthorList;
	for (int i = 0; i < pCGI->getNodeList().size(); i++)
	{
		coauthorList << pCGI->getNodeList()[i]->getLabel();
	}
	QString author1 = QInputDialog::getItem(this, "choose author1", "Choose author1", coauthorList);
	// author name -> idx로 변환
	//pTempCGI->paint_percent(); // <- 파라미터 넣기 그리고 비율 계산

	pTempCGI->paint_percent(author1);


	subwindow* sw = new subwindow;
	sw->setGraph(pTempCGI);
	sw->show();

}
//=====================================================================================================