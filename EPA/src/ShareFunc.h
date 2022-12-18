#pragma once
#include <QString>
#include <string>

class ShareFunc
{
public:

	static std::string QStr2Str(const QString& qstr)
	{
		QByteArray cba = qstr.toLocal8Bit();
		return std::string(cba);
	}

	static QString Str2Qstr(const std::string& str)
	{
		return QString::fromLocal8Bit(QByteArray::fromRawData(str.c_str(), str.length()));
	}

};