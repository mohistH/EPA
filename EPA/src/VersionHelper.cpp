#include "VersionHelper.h"
#include <QDateTime>
#include <QDate>
#include <QTime>


/// --------------------------------------------------------------------------------
/// @brief: VersionHelper::majorVersion
/// --------------------------------------------------------------------------------
QStringList VersionHelper::majorVersion()
{
	QStringList ret_list;
	ret_list << "0" << "1" << "2" << "3" << "4";

	return ret_list;
}

std::string VersionHelper::qstr2str(const QString& str)
{
	QByteArray cba = str.toLocal8Bit();
	return std::string(cba);
}

QString VersionHelper::str2qstr(const std::string& str)
{
	return "";
}

/// --------------------------------------------------------------------------------
/// @brief: 获取当前日期时间： [2022-12-01 01:02:03] 
/// --------------------------------------------------------------------------------
QString VersionHelper::dateTime()
{
	QString str_ret;
	{
		QDateTime& dt = QDateTime::currentDateTime();

		QDate& date = dt.date();
		QTime& time = dt.time();
		str_ret = QString("[%1-%2-%3 %4:%5:%6]")
			.arg(date.year(), 4, 10, QLatin1Char('0'))
			.arg(date.month(), 2, 10, QLatin1Char('0'))
			.arg(date.day(), 2, 10, QLatin1Char('0'))

			.arg(time.hour(), 2, 10, QLatin1Char('0'))
			.arg(time.minute(), 2, 10, QLatin1Char('0'))
			.arg(time.second(), 2, 10, QLatin1Char('0'));
	}

	return str_ret;
}

