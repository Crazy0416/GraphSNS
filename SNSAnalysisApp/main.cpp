#include "dblpanalysisapp.h"
#include <QtWidgets/QApplication>

using namespace std;

/*
* File name : main.cpp
* author : ���ȣ, ������
* purpose : main function
* Usage file : x
*/
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	dblpAnalysisApp w;
	w.show();

	return a.exec();
}
