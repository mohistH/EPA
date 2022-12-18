#include "ParseSlnHelper.h"
#include <QFile>


ParseSlnHelper::ParseSlnHelper()
{
}

ParseSlnHelper::~ParseSlnHelper()
{
}

ParseSlnHelper& ParseSlnHelper::ins()
{
	static ParseSlnHelper instance;
	return instance;
}

int ParseSlnHelper::Read(const QString& which_sln, stSlnFileInfo* pout_info)
{

	if (nullptr == pout_info)
	{
		return 2;
	}

	/// 1. 先从hash中找
	{
		HashSlnInfo::iterator findIt = hash_sln_info_.find(which_sln);
		if (findIt != hash_sln_info_.end())
		{
			*pout_info = findIt.value();
			return 0;
		}
	}

	/// 2. 解析文件
	return readSln(which_sln, pout_info);
}

/// --------------------------------------------------------------------------------
/// @brief: ParseSlnHelper::Read
/// --------------------------------------------------------------------------------
int ParseSlnHelper::Read(const QString& which_sln)
{
	/// 1. 先从hash中找
	{
		HashSlnInfo::iterator findIt = hash_sln_info_.find(which_sln);
		if (findIt != hash_sln_info_.end())
		{
			return 0;
		}
	}

	/// 2. 解析文件
	readSln(which_sln);

	return 0;
}

HashSlnInfo& ParseSlnHelper::hashSlnInfo()
{
	return hash_sln_info_;
}

int ParseSlnHelper::vdproj(const QString& which_sln, HashStrStr* pout_value)
{
	stSlnFileInfo* psln_file = nullptr;
	if (0 != querySlnInfo(which_sln, psln_file))
	{
		return 1;
	}

	if (nullptr == pout_value)
	{
		return 2;
	}


	*pout_value = psln_file->hash_vdproj_;

	return 0;
}

int ParseSlnHelper::vcxproj(const QString& which_sln, HashStrStr* pout_value)
{
	stSlnFileInfo* psln_file = nullptr;
	if (0 != querySlnInfo(which_sln, psln_file))
	{
		return 1;
	}

	if (nullptr == pout_value)
	{
		return 2;
	}


	*pout_value = psln_file->hash_vcxproj_;

	return 0;
}

int ParseSlnHelper::platform(const QString& which_sln, HashStrStr* pout_value)
{
	stSlnFileInfo* psln_file = nullptr;
	if (0 != querySlnInfo(which_sln, psln_file))
	{
		return 1;
	}

	if (nullptr == pout_value)
	{
		return 2;
	}


	*pout_value = psln_file->hash_platform_;

	return 0;
}

int ParseSlnHelper::configuration(const QString& which_sln, HashStrStr* pout_value)
{
	stSlnFileInfo* psln_file = nullptr;
	if (0 != querySlnInfo(which_sln, psln_file))
	{
		return 1;
	}

	if (nullptr == pout_value)
	{
		return 2;
	}


	*pout_value = psln_file->hash_configuration_;

	return 0;
}

int ParseSlnHelper::remove(const QString& which_sln)
{
	HashSlnInfo::iterator find_del_it = hash_sln_info_.find(which_sln);
	if (find_del_it == hash_sln_info_.end())
	{
		return 1;
	}

	find_del_it = hash_sln_info_.erase(find_del_it);

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: 删除所有数据
/// --------------------------------------------------------------------------------
void ParseSlnHelper::clearData()
{
	HashSlnInfo tmp_hash;
	hash_sln_info_.swap(tmp_hash);
}

/// 读取文件中的信息
int ParseSlnHelper::readSln(const QString& which_sln, stSlnFileInfo* pout_info)
{
	sln_file_key_parse_.zero();
	{
		QFile sln_file(which_sln);

		if (false == sln_file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			return 1;
		}

		/// 加入hash
		stSlnFileInfo& new_item = *pout_info;
		/// 解析文件内容
		while (false == sln_file.atEnd())
		{
			///读取一行
			const QString str_single_line = sln_file.readLine();

			parseProjectInSln(str_single_line, &new_item);

			parsePlatformConfigureInSln(str_single_line, &new_item, &sln_file);
		}

		sln_file.close();

		/// 解析得到sln文件所在路径
		new_item.sln_root_ = which_sln.left(which_sln.lastIndexOf('-') - 1);

		/// 解析结束，加入hash
		HashSlnInfo& hash_sln = hash_sln_info_;
		hash_sln.insert(which_sln, new_item);
	}

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: ParseSlnHelper::readSln
/// --------------------------------------------------------------------------------
int ParseSlnHelper::readSln(const QString& which_sln)
{
	sln_file_key_parse_.zero();
	{
		QFile sln_file(which_sln);

		if (false == sln_file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			return 1;
		}

		/// 加入hash
		stSlnFileInfo new_item;
		/// 解析文件内容
		while (false == sln_file.atEnd())
		{
			///读取一行
			const QString str_single_line = sln_file.readLine();

			parseProjectInSln(str_single_line, &new_item);

			parsePlatformConfigureInSln(str_single_line, &new_item, &sln_file);
		}

		sln_file.close();

		/// 解析得到sln文件所在路径
		new_item.sln_root_ = which_sln.left(which_sln.lastIndexOf('-') - 1);

		/// 解析结束，加入hash
		HashSlnInfo& hash_sln = hash_sln_info_;
		hash_sln.insert(which_sln, new_item);
	}

	return 0;
}

/// 解析工程文件
int ParseSlnHelper::parseProjectInSln(const QString& single_line, stSlnFileInfo* pspi)
{
	if (nullptr == pspi)
	{
		return 2;
	}

	/// Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "MFCLibrary1", "MFCLibrary1\MFCLibrary1.vcxproj", "{94B58FB7-100A-4CB2-8983-4F4438E6AB63}"
	/// 如果文件中 不是 指定开头的数据， 则pass
	stSlnKeyInfo& skf = sln_file_key_parse_;
	if (0 > single_line.indexOf(skf.project_key_begin_))
	{
		return 1;
	}

	/// 1. 直接字符串截断获取  "MFCLibrary1", "MFCLibrary1\MFCLibrary1.vcxproj" 
	QStringList list_project_info;
	{
		/// 这都是固定的长度的，无需关心内容， 因为内容长度是固定的
		const int&& project_pre_fix_len = QString("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = ").length();
		const int&& project_suffix_len = QString(", \"{94B58FB7-100A-4CB2-8983-4F4438E6AB63}").length();

		///  "MFCLibrary1", "MFCLibrary1\MFCLibrary1.vcxproj" 
		const int&& single_line_length = single_line.length();
		const QString str_project_info = single_line.mid(project_pre_fix_len, single_line_length - project_pre_fix_len - project_suffix_len - 1);
		list_project_info = str_project_info.split(",");

		if (2 < list_project_info.count())
		{
			list_project_info.removeAt(2);
		}
	}

	if (2 <= list_project_info.count())
	{
		HashStrStr& hash_vdproj = pspi->hash_vdproj_;
		HashStrStr& hash_vcxproj = pspi->hash_vcxproj_;

		/// 2. 项目名称
		QString str_project_name;
		{
			/// "MFCLibrary1"
			str_project_name = list_project_info.at(0).trimmed();
			/// 去除首尾的 冒号, -1 是 因为最后的的冒号
			str_project_name = str_project_name.mid(1, str_project_name.length() - 2);
		}

		/// 项目路径
		QString str_project_path;
		{
			str_project_path = list_project_info.at(1).trimmed();
			str_project_path = str_project_path.mid(1, str_project_path.length() - 2);
			str_project_path = str_project_path.replace('\\', '/');
		}

		/// 如果是安装包项目文件
		if (0 < str_project_path.lastIndexOf(skf.vdproj_))
		{
			/// 加入
			hash_vdproj.insert(str_project_name, str_project_path);
		}
		/// 常规项目工程文件
		else if (0 < str_project_path.lastIndexOf(skf.vcxproj_))
		{
			/// 加入
			hash_vcxproj.insert(str_project_name, str_project_path);
		}
	}

	return 0;
}

/// 解析平台和配置管理器
int ParseSlnHelper::parsePlatformConfigureInSln(const QString& single_line, stSlnFileInfo* pspi, QFile* pfile)
{
	if ((nullptr == pspi) || (nullptr == pfile))
	{
		return 1;
	}

	//GlobalSection(SolutionConfigurationPlatforms) = preSolution
	//	Debug | WIN32 = Debug | WIN32
	//	Debug | x64 = Debug | x64
	//	Debug | x86 = Debug | x86
	//	Release | WIN32 = Release | WIN32
	//	Release | x64 = Release | x64
	//	Release | x86 = Release | x86
	//	TA_DEBUG | WIN32 = TA_DEBUG | WIN32
	//	TA_DEBUG | x64 = TA_DEBUG | x64
	//	TA_DEBUG | x86 = TA_DEBUG | x86
	//	TA_RELEASE | WIN32 = TA_RELEASE | WIN32
	//	TA_RELEASE | x64 = TA_RELEASE | x64
	//	TA_RELEASE | x86 = TA_RELEASE | x86
	//EndGlobalSection

	stSlnKeyInfo& parse_key = sln_file_key_parse_;


	/// 已经解析结束了
	if (true == parse_key.isParsingSlnConfigEnd())
	{
		return 3;
	}


	/// 如果是开始: d当前行是： GlobalSection(SolutionConfigurationPlatforms) = preSolution。 需要解析的是下一行
	if (0 > single_line.indexOf(parse_key.sln_configure_begin_))
	{
		return 2;
	}

	HashStrStr& hash_platform = pspi->hash_platform_;
	HashStrStr& hash_config = pspi->hash_configuration_;

	/// 读取【 GlobalSection(SolutionConfigurationPlatforms) = preSolution】 的下一行
	QString sln_config_line;
	while (false == pfile->atEnd())
	{
		/// 读取一行数据: Debug | WIN32 = Debug | WIN32
		sln_config_line = pfile->readLine();

		/// 如果是结束行
		if (0 <= sln_config_line.indexOf(parse_key.sln_configure_end_))
		{
			break;
		}

		sln_config_line = sln_config_line.trimmed();

		QString str_config_name;
		QString str_platform_name;
		{
			/// Debug | WIN32; Debug | WIN32
			const QStringList&& list_line_config_platform = sln_config_line.split('=');
			/// 因为两个是一样的，这里仅仅解析一个： Debug | WIN32
			const QString&& str_config_sln = list_line_config_platform.at(0).trimmed();

			/// 
			const QStringList&& list_cp = str_config_sln.split('|');
			if (2 <= list_cp.count())
			{
				str_config_name = list_cp.at(0);
				str_platform_name = list_cp.at(1);
			}
		}

		if (0 < str_platform_name.length())
		{
			/// 解析结束， 将解析的结果加入map
			hash_platform.insert(str_platform_name, str_platform_name);
		}

		if (0 < str_config_name.length())
		{
			/// 解决方案配置
			hash_config.insert(str_config_name, str_config_name);
		}
	}

	/// 标志解析已经结束
	parse_key.setSlnConfigParseEnd(true);


	return 0;
}

int ParseSlnHelper::querySlnInfo(const QString& which_sln, stSlnFileInfo*& pout_value)
{
	HashSlnInfo::iterator sln_iterater = hash_sln_info_.find(which_sln);
	if (sln_iterater == hash_sln_info_.end())
	{
		return 1;
	}

	pout_value = &(sln_iterater.value());

	return 0;
}
