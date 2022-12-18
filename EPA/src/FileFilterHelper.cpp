
#include "FileFilterHelper.h"


FileFilterHelper::FileFilterHelper()
{
	MapIntStr& map_fs = map_file_suffix_;
	map_fs.insert(FT_DEVENV, "devenv.exe (devenv.exe)");
	map_fs.insert(FT_EXE_DLL, "exe (*.exe *.dll)");
	map_fs.insert(FT_VDPROJ, "vdproj (*.vdproj)");
	map_fs.insert(FT_BAT, "bat(*.bat)");
	map_fs.insert(FT_SLN, "Solution(*.sln)");
}

FileFilterHelper::~FileFilterHelper()
{}

/// --------------------------------------------------------------------------------
/// @brief: FileFilterHelper::ins
/// --------------------------------------------------------------------------------
FileFilterHelper& FileFilterHelper::ins()
{
	static FileFilterHelper instance;
	return instance;
}

/// --------------------------------------------------------------------------------
/// @brief: FileFilterHelper::fileSuffix
/// --------------------------------------------------------------------------------
QString FileFilterHelper::fileSuffix(const FILE_TYPE ft)
{
	MapIntStr::iterator findIt = map_file_suffix_.find(ft);
	if (findIt != map_file_suffix_.end())
	{
		return findIt.value();
	}

	return "*.*";
}

