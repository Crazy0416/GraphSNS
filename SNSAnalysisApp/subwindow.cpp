#include "subwindow.h"
#include "GraphicsView.h"

#include <QTimer>

subwindow::subwindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initscene();
	QWidget *widget = new QWidget();
	View* view = new View("temp view");
	view->view()->setScene(scene);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(view);
	widget->setLayout(layout);
	setCentralWidget(widget);
	setWindowTitle(tr("sub window"));
	resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
	this->setAttribute(Qt::WA_DeleteOnClose);	// 화면끄자마자 메모리에서 해제...

	pCGI = nullptr;
	pPGI = nullptr;

	// 메뉴 바 설정
	QMenu *pAppMenu;
	QStatusBar *pStatusBar;

	// 기본 기능 메뉴 설정
	QAction *pSlotfindAuthor = new QAction(tr("findAuthor"), this);
	QObject::connect(pSlotfindAuthor, SIGNAL(triggered()), this, SLOT(Main_findAuthor()));

	pAppMenu = menuBar()->addMenu(tr("Additional Menu"));
	pAppMenu->addAction(pSlotfindAuthor);
	
}

subwindow::~subwindow()
{
	delete scene;			// TODO: 여기서 nodeList와 edgeList 삭제되는지 확인 좀;
	if(pCGI != nullptr)
		delete pCGI;
	if(pPGI != nullptr)
		delete pPGI;
	qDebug() << "subwindow delete" << endl;
}
//=====================================================================================================
void subwindow::initscene()
{
	scene = new QGraphicsScene(this);
}

void subwindow::setGraph(CoauthorGraphItem* pCGI)
{
	this->pCGI = pCGI;

	QGraphicsItem* graph_item = pCGI;
	graph_item->setPos(0, 0);
	//scene->addItem(pCGI);
	for (auto index : pCGI->getNodeList())
	{
		index->setPos(index->getX(), index->getY());
		scene->addItem(index);
	}
	for (auto index : pCGI->getEdgeList())
	{
		//index->setPos(0, 0);
		scene->addItem(index);
	}
}

void subwindow::setGraph(PaperGraphItem* pPGI)
{
	this->pPGI = pPGI;

	QGraphicsItem* graph_item = pPGI;
	graph_item->setPos(0, 0);
	//scene->addItem(graph_item);
	for (auto index : pPGI->getNodeList())
	{
		index->setPos(index->getX(), index->getY());
		scene->addItem(index);
	}
	for (auto index : pPGI->getEdgeList())
	{
		//index->setPos(0, 0);
		scene->addItem(index);
	}
}

void subwindow::Main_findAuthor() {
	bool ok;
	bool exist = false;
	QString AuthorName = QInputDialog::getText(NULL, "Find Author", "Write Author name", QLineEdit::Normal, "", &ok);
	if (AuthorName.isEmpty())
	{
		QMessageBox::warning(this, "Empty Name!!", "Empty Name!!", QMessageBox::Cancel, QMessageBox::Cancel);
		return;
	}
	for (int i = 0; i < pCGI->getNodeList().size(); i++) {
		if (pCGI->getNodeList()[i]->getLabel().contains(AuthorName))
		{
			/* temp :
			* (Authorname)
			* edge count : (node weightsum)
			* coauthor : (coauthorList)
			*/
			pCGI->getNodeList()[i]->setColor(Qt::blue);
			pCGI->getNodeList()[i]->setFontSize(9);
			pCGI->getNodeList()[i]->setFontWeight(QFont::Bold);
			pCGI->getNodeList()[i]->setNodeSize(12);
			exist = true;
		}
		else {
			pCGI->getNodeList()[i]->setColor(Qt::green);
		}
	}
	if(!exist)
		QMessageBox::warning(this, "Couldn't find Author!!", "Couldn't find Author!!", QMessageBox::Cancel, QMessageBox::Cancel);
}
