#pragma once
#include <QHash>

class QFile;


using HashStrStr = QHash<QString, QString>;

struct stSlnFileInfo
{
	/// 解决方案sln文件所在路径
	QString sln_root_{};

	/// 一个解决方案中有多个项目文件, <项目名称，项目的相对路径>
	HashStrStr	hash_vdproj_;
	/// 一个解决方案中有多个项目文件, <项目名称，项目的相对路径>
	HashStrStr	hash_vcxproj_;
	/// 对应的平台解决方案
	HashStrStr hash_platform_;
	/// 对应的配置管理器
	HashStrStr hash_configuration_;
};

/// <解决方案文件，文件中的信息>
using HashSlnInfo = QHash<QString, stSlnFileInfo>;

/// sln文件解析
class ParseSlnHelper
{
public:
	ParseSlnHelper();
	virtual ~ParseSlnHelper();

	/// instance
	static ParseSlnHelper& ins();

	/// 解析文件
	int Read(const QString& which_sln, stSlnFileInfo* pout_info);
	int Read(const QString& which_sln);

	/// 获取所有sln文件信息
	HashSlnInfo& hashSlnInfo();

	/// 获取vd proj文件
	int vdproj(const QString& which_sln, HashStrStr* pout_value);

	/// 获取vcxprocj
	int vcxproj(const QString& which_sln, HashStrStr* pout_value);

	/// 获取平台
	int platform(const QString& which_sln, HashStrStr* pout_value);

	/// 获取配置
	int configuration(const QString& which_sln, HashStrStr* pout_value);

	/// sln
	int remove(const QString& which_sln);

	/// 删除所有数据
	void clearData();

private:
	/// 读取文件中的信息
	int readSln(const QString& which_sln, stSlnFileInfo* pout_info);
	int readSln(const QString& which_sln);


	/// 解析sln中的项目
	int parseProjectInSln(const QString& single_line, stSlnFileInfo* pspi);

	/// 解析平台和配置管理器
	int parsePlatformConfigureInSln(const QString& single_line, stSlnFileInfo* psi, QFile* pfile);

	int querySlnInfo(const QString& which_sln, stSlnFileInfo*& pout_value);

private:
	/// 保存所有的解决方案文件信息 
	HashSlnInfo	 hash_sln_info_;

	struct stSlnKeyInfo_
	{
	public:
		/// 项目开头的表示
		QString project_key_begin_{ "Project(\"" };
		/// 解决方案开始和结束的
		QString sln_configure_begin_{ "	GlobalSection(SolutionConfigurationPlatforms) = preSolution" };
		/// 解决方案结束
		QString sln_configure_end_{ "	EndGlobalSection" };

		const QString vcxproj_{ "vcxproj" };
		const QString vdproj_{ "vdproj" };

	private:
		/// 解析解平台配置和配置管理器是否开始 
		bool sln_config_is_end = false;

	public:
		void zero()
		{
			sln_config_is_end = false;
		}

		bool isParsingSlnConfigEnd()
		{
			return sln_config_is_end;
		}

		void setSlnConfigParseEnd(const bool is_end)
		{
			sln_config_is_end = is_end;
		}
	};

	using stSlnKeyInfo = stSlnKeyInfo_;

	stSlnKeyInfo	sln_file_key_parse_;

};
