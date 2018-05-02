#include "listInput.h"
#include <qinputdialog.h>
/*
* GridLayout으로 구성
* Author들의 리스트인 listwidget과 
* 선택된 author를 통해 크롤링을 시작하게 만드는 QPushButton으로 구성
*/
ListInput::ListInput(QWidget *parent)
	: QDialog(parent)
{
	// GridLayout 생성 후 listwidget과 버튼 삽입
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

/* 소멸자*/
ListInput::~ListInput()
{
	delete listwidget;
	delete selectButton;
}

/*
* 버튼이 눌렸을 때 실행되는 슬롯
* 버튼이 눌릴 경우 첫 위젯이 실행 됨 -> 2가지 케이스가 존재
* ◇ 1. 선택한 author가 단 하나 존재하여 바로 그 author의 논문 제목 보여주는 URL 받는 경우 http://dblp.uni-trier.de/pers~ 로 된 URL 받음
* -> 제목 바로 크롤링
* ◇ 2. 선택한 author가 비슷한 이름이 여럿 존재하여 비슷한 author들을 보여주는 URL 받는 경우 http://dblp.uni-trier.de/search/author?q=~ 로 된 URL 받음
* -> on_PushSelectButton_Clicked 슬롯 안에서 ListInput 위젯 생성
* -> 비슷한 이름 크롤링하여 QInputDialog::InputItem 에 추가
* -> author를 선택할 때까지 ListInput 실행흐름이 멈춤
* -> 선택한 text의 위치에 존재하는 Element의 href attribute의 URL을 파싱하여 다시 Navigate
* -> 선택한 author의 논문 제목 바로 크롤링
*/
void ListInput::on_PushSelectButton_Clicked()
{
	/*
	
	// strData : 최근 아이템 text 반환
	QString strData = listwidget->currentItem()->text();
	// list : strData를 '.' 구분자로 나눈 QStringList
	QStringList list = strData.split('.');

	if (strData != NULL)		// 빈칸이 아닌 author를 선택한 경우
	{
		QString strDataName;
		for (int i = 1; i < list.size(); i++)		// 이름에 인덱스 삽입 ex) Alon Y. Levy -> 4.Alon Y. Levy
		{
			strDataName.append(list[i]);
			if(i != list.size() - 1)
				strDataName.append(".");
		}
		qDebug() << strDataName;

		/////////////// <author 크롤링>
		WebDriver firefox = Start(Firefox());

		string dblpURL = "http://dblp.uni-trier.de/search/author?author=";

		dblpURL += strDataName.toStdString();

		firefox.Navigate(dblpURL);
		vector<Element> menu;
		////////////// </author 크롤링>

		if ((firefox.GetUrl().find("dblp.uni-trier.de/search") != -1))		// 1. 입력한 author가 여러 사람이 존재하는 경우
		{
			menu = firefox.FindElements(ById("completesearch-authors"));
			// all_author는 href를 가지는 Element를 다 가져옴으로인해 필요하지 않는 데이터도 크롤링함 -> index를 사용해 해결
			vector<Element> all_author = menu[0].FindElements(ByCss("a[href]"));		
			
			int cnt = 0;		// all_author count - 1
			int listIndex = 1;	// span이 들어간, 즉  index
			// a[href]를 가지는 Element 배열 중 author를 가지는 element의 인덱스를 저장
			// author를 가지지 않는 Element의 위치에는 -1의 값을 가짐
			int *authorlistIndex = new int[all_author.size()];	
			for (int i = 0; i < all_author.size(); i++) authorlistIndex[i] = -1;		// 초기화
			
			QStringList authorList;			// QInputDialog에 넣을 유사한 author들의 QString List
			for (Element i : all_author)
			{
				if (i.FindElements(ByTag("span")).size() != 0)		// i Element가 author 이름 가지고 있다면 조건문 실행
				{
					QString tmp =  QString::number(listIndex) + "." + QString::fromStdString(i.GetText());
					qDebug() << tmp;
					authorList << tmp;
					authorlistIndex[cnt] = listIndex;		// authorlistIndex[cnt]에 span의 index 넣는다.
					listIndex++;
				}
				cnt++;
			}
			// 검색한 author와 비슷한 이름을 가진 author 고르는 창
			QString secontAuthor = QInputDialog::getItem(this, "choose author", "Choose author", authorList);
			QStringList secondAuthorSplit = secontAuthor.split(".");
			// secondAuthorIndex : 유사 author를 띄운 InputDialog에서 선택한 author의 인덱스 값
			int secondAuthorIndex = secondAuthorSplit[0].toInt();	

			qDebug() << "similar author index : " << QString::number(secondAuthorIndex);

			for (int i = 0; i < all_author.size(); i++)
			{
				if (authorlistIndex[i] == secondAuthorIndex)		// 유사 author의 인덱스 == 그 유사 author를 가진 Element의 인덱스가 같다면 그 URL로 크롤링
				{
					string url = all_author[i].GetAttribute("href");
					vector<Element> menu2 = firefox.Navigate(url).FindElements(ByClass("entry"));
					
					for (Element i : menu2)
					{

						QString tmp = QString::fromStdString(i.FindElement(ByClass("title")).GetText());		// 논문 주제 크롤링
						qDebug() << tmp << endl;
					}
				}
			}
		}
		else																// 2. 입력한 사람이 단 한 사람 존재하는 경우
		{
			menu = firefox.FindElements(ByClass("entry"));
			
			for (Element i : menu)
			{
				
				QString tmp = QString::fromStdString(i.FindElement(ByClass("title")).GetText());	// 논문 주제 크롤링
				qDebug() << tmp << endl;
			}
		}
	}
	else							// author를 선택하지 못했다면
	{
		qDebug() << "error!!!";
	}
	
	this->close();								// 이 위젯 종료
	*/
}