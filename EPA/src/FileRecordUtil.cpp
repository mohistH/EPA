#include "FileRecordUtil.h"
#include "FileRecord.h"
#include "Typedef.h"
#include "SqliteExt.h"
#include "SqliteExt.inl"
#include <QApplication>
#include <QFileInfo>


FileRecordUtil::FileRecordUtil(QObject* parent)
	: QObject(parent)
{
	map_db_name_.insert(FT_EXE_DLL, "ExeDll");
	map_db_name_.insert(FT_VDPROJ, "VdProj");
	map_db_name_.insert(FT_DEVENV, "DevEnv");
}

FileRecordUtil::~FileRecordUtil()
{
	;
}

/// --------------------------------------------------------------------------------
/// @brief: FileRecordUtil::Read
/// --------------------------------------------------------------------------------
int FileRecordUtil::Read(FileRecord* pout_fr, const QString& while_file)
{
	if (nullptr == pout_fr)
	{
		return -1;
	}

	using namespace oct_sqlite;
	SqliteExt se;

	stSQLInit init_param;
	init_param.str_sqlite_file_ = while_file;/// QApplication::applicationDirPath() + s_file_record_name;
	init_param.en_es_ = oct_sqlite::ES_MANUAL_SUBMIT;
	init_param.en_tt_ = oct_sqlite::TT_TABLES;

	int ret = se.initialize(init_param);
	if (0 != ret)
	{
		return ret;
	}

	/// 读取数据
	{
		/// 读取devenv.exe中的数据
		// se.table_data_qmap_<QString, stDevEnvInfo>(map_db_name_.value(FT_DEVENV), pout_fr->devenv(), 0);

		/// 读取exe dll 中的数据
		se.table_data_qmap_<QString, stExeDllVersionInfo>(map_db_name_.value(FT_EXE_DLL), pout_fr->exeDllVersion(), 0);

		/// 读取打包项目中的文件
		se.table_data_qmap_<QString, stInstallProjectVersion>(map_db_name_.value(FT_VDPROJ), pout_fr->vdprojVersion(), 0);

	}

	se.unInit();

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: FileRecordUtil::write
/// --------------------------------------------------------------------------------
int FileRecordUtil::write(FileRecord* pin_fr, const QString& while_file, const QStringList& create_sql)
{
	if (nullptr == pin_fr)
	{
		return -1;
	}


	using namespace oct_sqlite;
	SqliteExt se;

	/// 如果文件不存在，则创建
	{
		QFileInfo fi(pin_fr->dbFile());
		if (false == fi.exists())
		{
			/// 创建文件
			se.newDBFile(while_file, create_sql);
		}
	}

	{
		stSQLInit init_param;
		init_param.str_sqlite_file_ = while_file;// QApplication::applicationDirPath() + s_file_record_name;
		init_param.en_es_ = oct_sqlite::ES_MANUAL_SUBMIT;
		init_param.en_tt_ = oct_sqlite::TT_TABLES;

		int ret = se.initialize(init_param);
		if (0 != ret)
		{
			return ret;
		}
	}

	// {
	// 	/// 写devenv.exe中的数据
	// 	int rett = se.writeTableData<QString, stDevEnvInfo>(map_db_name_.value(FT_DEVENV), pin_fr->devenv(), 0);
	// 	int xx = 0;
	// }

	{
		/// 读取exe dll 中的数据
		se.writeTableData<QString, stExeDllVersionInfo>(map_db_name_.value(FT_EXE_DLL), pin_fr->exeDllVersion(), 0);
	}

	/// 读取安装项目文件
	{
		se.writeTableData<QString, stInstallProjectVersion>(map_db_name_.value(FT_VDPROJ), pin_fr->vdprojVersion(), 0);
	}
	se.unInit();

	return 0;
}

