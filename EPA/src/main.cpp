#include <QtWidgets/QApplication>
#include <EPAController.h>
#include <QWidget>
#include <QTranslator>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QTranslator		language_trans_;
	const QString& str_qm = ":/lang/lang/zh_CN.qm";
	language_trans_.load(str_qm);

	a.installTranslator(&language_trans_);
	/// 
	oct_epa::EPAController epac;
	epac.setTranslator(&language_trans_);

	/// 主程序UI
	QWidget* pmain_ui = epac.mainWidget();
	if (pmain_ui)
	{
		pmain_ui->show();
	}
	else
	{
		return 0;
	}

	return a.exec();
}
