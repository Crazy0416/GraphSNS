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

}
//=====================================================================================================
void subwindow::initscene()
{
	scene = new QGraphicsScene(this);
}

void subwindow::setGraph(CoauthorGraphItem* pCGI)
{
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
	QGraphicsItem* graph_item = pPGI;
	graph_item->setPos(0, 0);
	scene->addItem(graph_item);
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
			string temp2 = to_string(pCGI->getNodeList()[i]->getWeightSum());
			string temp3 = "";
			for (int j = 0; j < pCGI->getNodeList()[i]->getcoauthorList().size(); j++){
				temp3 += pCGI->getNodeList()[i]->getcoauthorList()[j] + ",";
			}
			string temp = pCGI->getNodeList()[i]->getLabel().toStdString().append("\nedge count : ") + temp2;
			temp.append("\ncoauthor : ").append(temp3);
			//////////////////////////////////////////////////////////////////////////////////
			QMessageBox::warning(this, "Exists!!", temp.c_str(), QMessageBox::Cancel, QMessageBox::Cancel);
			exist = true;
		}
	}
	if(!exist)
		QMessageBox::warning(this, "Couldn't find Author!!", "Couldn't find Author!!", QMessageBox::Cancel, QMessageBox::Cancel);
}
