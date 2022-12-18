#ifndef FILE_RECORD_UTILS_H_
#define FILE_RECORD_UTILS_H_


#include <QObject>
#include <QMap>
#include "Typedef.h"

class FileRecord;

class FileRecordUtil : public QObject
{
	Q_OBJECT

public:
	explicit FileRecordUtil(QObject* parent = nullptr);
	virtual ~FileRecordUtil();

	/// -------------------------------------------------------------------------------
	/// @brief:		读取数据库中的内容
	/// @param: 	FileRecord * pout_fr - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	int Read(FileRecord* pout_fr, const QString& while_file);

	/// -------------------------------------------------------------------------------
	/// @brief:		写入数据库内容
	/// @param: 	const FileRecord * pin_fr - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	int write(FileRecord* pin_fr, const QString& while_file, const QStringList& create_sql);

private:
	using MapDBName = QMap<FILE_TYPE, QString>;

	MapDBName		map_db_name_;

};



#endif /// FILE_RECORD_UTILS_H_