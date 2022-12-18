#include "FileRecord.h"


FileRecord::FileRecord()
{
	;
}

FileRecord::~FileRecord()
{
	;
}

/// --------------------------------------------------------------------------------
/// @brief: FileRecord::denevn
/// --------------------------------------------------------------------------------
MapDevEnvInfo* FileRecord::devenv()
{
	return &map_vs_devenv_;
}

/// --------------------------------------------------------------------------------
/// @brief: FileRecord::exeDllVersion
/// --------------------------------------------------------------------------------
MapExeDllVersionInfo* FileRecord::exeDllVersion()
{
	return &map_exe_dll_;
}

/// --------------------------------------------------------------------------------
/// @brief: FileRecord::vdprojVersion
/// --------------------------------------------------------------------------------
MapInstallProjectVersion* FileRecord::vdprojVersion()
{
	return &map_vdproj_version_;
}

/// --------------------------------------------------------------------------------
/// @brief: FileRecord::setDBFile
/// --------------------------------------------------------------------------------
void FileRecord::setDBFile(const QString& db_file)
{
	db_file_ = db_file;
}

/// --------------------------------------------------------------------------------
/// @brief: FileRecord::dbFile
/// --------------------------------------------------------------------------------
QString FileRecord::dbFile() const
{
	return db_file_;
}

/// --------------------------------------------------------------------------------
/// @brief: 删除内存中的所有数据
/// --------------------------------------------------------------------------------
void FileRecord::clearData()
{
	/// 文件清空
	{
		MapInstallProjectVersion tmp_map;
		map_vdproj_version_.swap(tmp_map);
	}

	//{
	//	MapDevEnvInfo tmp_map;
	//	map_vs_devenv_.swap(tmp_map);
	//}

	{
		MapExeDllVersionInfo tmp_map;
		map_exe_dll_.swap(tmp_map);
	}
}

/// --------------------------------------------------------------------------------
/// @brief: 对比compiler name, 是否在当前操作系统中， 不在则设置为空
/// --------------------------------------------------------------------------------
void FileRecord::processInstallPrjectData()
{
	for (auto& item : map_vdproj_version_)
	{
		/// 如果不在更新的列表中，则设置为空
		MapDevEnvInfo::iterator find_it = map_vs_devenv_.find(item.compiler_);
		if (find_it == map_vs_devenv_.end())
		{
			item.compiler_ = "";
		}
	}
}
