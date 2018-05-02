#include "listInput.h"
#include <qinputdialog.h>
/*
* GridLayout���� ����
* Author���� ����Ʈ�� listwidget�� 
* ���õ� author�� ���� ũ�Ѹ��� �����ϰ� ����� QPushButton���� ����
*/
ListInput::ListInput(QWidget *parent)
	: QDialog(parent)
{
	// GridLayout ���� �� listwidget�� ��ư ����
	QGridLayout *topLayout = new QGridLayout(this);
	listwidget = new QListWidget(this);
	selectButton = new QPushButton("select",this);

	topLayout->addWidget(listwidget, 100, 1000);
	topLayout->addWidget(selectButton, 1200, 100);

	setLayout(topLayout);
	setBaseSize(500, 500);

	//connect
	QObject::connect(selectButton, SIGNAL(clicked()), this, SLOT(on_PushSelectButton_Clicked()));
}

/* �Ҹ���*/
ListInput::~ListInput()
{
	delete listwidget;
	delete selectButton;
}

/*
* ��ư�� ������ �� ����Ǵ� ����
* ��ư�� ���� ��� ù ������ ���� �� -> 2���� ���̽��� ����
* �� 1. ������ author�� �� �ϳ� �����Ͽ� �ٷ� �� author�� �� ���� �����ִ� URL �޴� ��� http://dblp.uni-trier.de/pers~ �� �� URL ����
* -> ���� �ٷ� ũ�Ѹ�
* �� 2. ������ author�� ����� �̸��� ���� �����Ͽ� ����� author���� �����ִ� URL �޴� ��� http://dblp.uni-trier.de/search/author?q=~ �� �� URL ����
* -> on_PushSelectButton_Clicked ���� �ȿ��� ListInput ���� ����
* -> ����� �̸� ũ�Ѹ��Ͽ� QInputDialog::InputItem �� �߰�
* -> author�� ������ ������ ListInput �����帧�� ����
* -> ������ text�� ��ġ�� �����ϴ� Element�� href attribute�� URL�� �Ľ��Ͽ� �ٽ� Navigate
* -> ������ author�� �� ���� �ٷ� ũ�Ѹ�
*/
void ListInput::on_PushSelectButton_Clicked()
{
	/*
	
	// strData : �ֱ� ������ text ��ȯ
	QString strData = listwidget->currentItem()->text();
	// list : strData�� '.' �����ڷ� ���� QStringList
	QStringList list = strData.split('.');

	if (strData != NULL)		// ��ĭ�� �ƴ� author�� ������ ���
	{
		QString strDataName;
		for (int i = 1; i < list.size(); i++)		// �̸��� �ε��� ���� ex) Alon Y. Levy -> 4.Alon Y. Levy
		{
			strDataName.append(list[i]);
			if(i != list.size() - 1)
				strDataName.append(".");
		}
		qDebug() << strDataName;

		/////////////// <author ũ�Ѹ�>
		WebDriver firefox = Start(Firefox());

		string dblpURL = "http://dblp.uni-trier.de/search/author?author=";

		dblpURL += strDataName.toStdString();

		firefox.Navigate(dblpURL);
		vector<Element> menu;
		////////////// </author ũ�Ѹ�>

		if ((firefox.GetUrl().find("dblp.uni-trier.de/search") != -1))		// 1. �Է��� author�� ���� ����� �����ϴ� ���
		{
			menu = firefox.FindElements(ById("completesearch-authors"));
			// all_author�� href�� ������ Element�� �� �������������� �ʿ����� �ʴ� �����͵� ũ�Ѹ��� -> index�� ����� �ذ�
			vector<Element> all_author = menu[0].FindElements(ByCss("a[href]"));		
			
			int cnt = 0;		// all_author count - 1
			int listIndex = 1;	// span�� ��, ��  index
			// a[href]�� ������ Element �迭 �� author�� ������ element�� �ε����� ����
			// author�� ������ �ʴ� Element�� ��ġ���� -1�� ���� ����
			int *authorlistIndex = new int[all_author.size()];	
			for (int i = 0; i < all_author.size(); i++) authorlistIndex[i] = -1;		// �ʱ�ȭ
			
			QStringList authorList;			// QInputDialog�� ���� ������ author���� QString List
			for (Element i : all_author)
			{
				if (i.FindElements(ByTag("span")).size() != 0)		// i Element�� author �̸� ������ �ִٸ� ���ǹ� ����
				{
					QString tmp =  QString::number(listIndex) + "." + QString::fromStdString(i.GetText());
					qDebug() << tmp;
					authorList << tmp;
					authorlistIndex[cnt] = listIndex;		// authorlistIndex[cnt]�� span�� index �ִ´�.
					listIndex++;
				}
				cnt++;
			}
			// �˻��� author�� ����� �̸��� ���� author ���� â
			QString secontAuthor = QInputDialog::getItem(this, "choose author", "Choose author", authorList);
			QStringList secondAuthorSplit = secontAuthor.split(".");
			// secondAuthorIndex : ���� author�� ��� InputDialog���� ������ author�� �ε��� ��
			int secondAuthorIndex = secondAuthorSplit[0].toInt();	

			qDebug() << "similar author index : " << QString::number(secondAuthorIndex);

			for (int i = 0; i < all_author.size(); i++)
			{
				if (authorlistIndex[i] == secondAuthorIndex)		// ���� author�� �ε��� == �� ���� author�� ���� Element�� �ε����� ���ٸ� �� URL�� ũ�Ѹ�
				{
					string url = all_author[i].GetAttribute("href");
					vector<Element> menu2 = firefox.Navigate(url).FindElements(ByClass("entry"));
					
					for (Element i : menu2)
					{

						QString tmp = QString::fromStdString(i.FindElement(ByClass("title")).GetText());		// �� ���� ũ�Ѹ�
						qDebug() << tmp << endl;
					}
				}
			}
		}
		else																// 2. �Է��� ����� �� �� ��� �����ϴ� ���
		{
			menu = firefox.FindElements(ByClass("entry"));
			
			for (Element i : menu)
			{
				
				QString tmp = QString::fromStdString(i.FindElement(ByClass("title")).GetText());	// �� ���� ũ�Ѹ�
				qDebug() << tmp << endl;
			}
		}
	}
	else							// author�� �������� ���ߴٸ�
	{
		qDebug() << "error!!!";
	}
	
	this->close();								// �� ���� ����
	*/
}