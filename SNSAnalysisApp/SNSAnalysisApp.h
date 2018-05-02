#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SNSAnalysisApp.h"

class SNSAnalysisApp : public QMainWindow
{
	Q_OBJECT

public:
	SNSAnalysisApp(QWidget *parent = Q_NULLPTR);

private:
	Ui::SNSAnalysisAppClass ui;
};
