#ifndef FILE_RECORD_H_
#define FILE_RECORD_H_
#include <QObject>
#include "typedef.h"


class FileRecord
{
public:
	explicit FileRecord();
	virtual  ~FileRecord();

	/// -------------------------------------------------------------------------------
	/// @brief:		获取vs各版本
	///  @ret:		MapDevEnvInfo*
	///				
	/// -------------------------------------------------------------------------------
	MapDevEnvInfo* devenv();

	/// -------------------------------------------------------------------------------
	/// @brief:		获取exe dll表格中的数据
	///  @ret:		MapExeDllVersionInfo*
	///				
	/// -------------------------------------------------------------------------------
	MapExeDllVersionInfo* exeDllVersion();

	/// -------------------------------------------------------------------------------
	/// @brief:		安装包项目数据
	///  @ret:		MapInstallProjectVersion*
	///				
	/// -------------------------------------------------------------------------------
	MapInstallProjectVersion* vdprojVersion();

	/// -------------------------------------------------------------------------------
	/// @brief:		保存配置文件
	/// @param: 	const QString & db_file - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setDBFile(const QString& db_file);

	/// -------------------------------------------------------------------------------
	/// @brief:		哪个配置文件
	///  @ret:		QT_NAMESPACE::QString
	///				
	/// -------------------------------------------------------------------------------
	QString dbFile() const;

	/// -------------------------------------------------------------------------------
	/// @brief:		删除内存中的所有数据
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void clearData();

	/// -------------------------------------------------------------------------------
	/// @brief:		对比compiler name, 是否在当前操作系统中， 不在则设置为空
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void processInstallPrjectData();

private:
	/// 安装包项目
	MapInstallProjectVersion	map_vdproj_version_;

	/// devenv.exe信息
	MapDevEnvInfo		map_vs_devenv_;

	/// exe dll表
	MapExeDllVersionInfo	map_exe_dll_;

	/// 哪个配置文件
	QString db_file_;
};



#endif /// !FILE_RECORD_H_