#ifndef FILE_FILTER_HELPER_H_
#define FILE_FILTER_HELPER_H_
#include "Typedef.h"


/// ----------------------------------------------------------------------------------------
/// @brief: 统计文件后缀
/// ----------------------------------------------------------------------------------------
class FileFilterHelper
{
public:
	FileFilterHelper();
	virtual ~FileFilterHelper();

	static FileFilterHelper& ins();

	/// -------------------------------------------------------------------------------
	/// @brief:		获取对应文件类型的文件后缀
	/// @param: 	const FILE_TYPE ft - 哪个文件
	///  @ret:		QT_NAMESPACE::QString
	///				
	/// -------------------------------------------------------------------------------
	QString fileSuffix(const FILE_TYPE ft);

private:
	MapIntStr	map_file_suffix_;
};



#endif ///!FILE_FILTER_HELPER_H_