#pragma once
#include <QStringList>
#include <QHash>

class VersionHelper
{
public:
	using HashStrStr = QHash<QString, QString>;

public:
	/// -------------------------------------------------------------------------------
	/// @brief:		主版本
	///  @ret:		QT_NAMESPACE::QStringList
	///				
	/// -------------------------------------------------------------------------------
	static QStringList majorVersion();

	/// -------------------------------------------------------------------------------
	/// @brief:		QString 转 std::string
	/// @param: 	const QString & str - 
	///  @ret:		std::string
	///				
	/// -------------------------------------------------------------------------------
	static std::string qstr2str(const QString& str);

	static QString str2qstr(const std::string& str);

	/// -------------------------------------------------------------------------------
	/// @brief:		获取当前日期时间： [2022-12-01 01:02:03] 
	///  @ret:		QString
	///				
	/// -------------------------------------------------------------------------------
	static QString dateTime();
};

