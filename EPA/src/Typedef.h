#pragma once
#include <QString>
#include <QMap>
#include <QList>
#include <string.h>
#include <QMetaType>
#include <QObject>



enum FILE_TYPE
{
	/// exe dll
	FT_EXE_DLL = 1,
	/// vs 安装文件
	FT_VDPROJ = 2,
	/// VS开发工具
	FT_DEVENV = 3,
	/// 批处理文件
	FT_BAT = 5,
	/// 解决方案名称
	FT_SLN = 6,
};

using ListFileType = QList<FILE_TYPE>;


#pragma  pack(1)



struct stVersionCommon
{
public:
	stVersionCommon()
	{
		zero();
	}
	~stVersionCommon()
	{
		;
	}

	void zero()
	{
		major_ = 0;
		minor_ = 0;
		revision_ = 0;
		sln_name_ = QObject::tr("FileName");
		compiler_ = "";
	}

	/// 得到项目的所在路径
	QString toVdprojFileRoot() const
	{
		{
			if (0 == sln_name_.trimmed().length())
			{
				return QString{};
			}
		}

		/// 得到解决放方案路径
		QString&& sln_root = sln_name_.left(sln_name_.lastIndexOf('/'));

		return sln_root;
	}

public:
	QString compiler_{ "" };
	QString sln_name_{ "" };
	QString project_name_{};
	QString platform_name_{};
	QString configuration_name_{ };
	int major_ = 0;
	int minor_ = 0;
	int revision_ = 0;
};


/// ----------------------------------------------------------------------------------------
/// @brief: 版本信息
/// ----------------------------------------------------------------------------------------
struct stVdprojVersion_ : public stVersionCommon
{
public:
	stVdprojVersion_()
	{
		this->zero();
	}

	void zero()
	{
		stVersionCommon::zero();
		build_ = 0;
	}
	void set(int m, int s, int i, int b)
	{
		major_ = m;
		minor_ = s;
		revision_ = i;
		//build_ = b;
		build_ = b;/// QString("%1").arg(b, 6, QLatin1Char('0'));
	}

	QString toString()
	{
		QString retValue = QString("%1.%2.%3").arg(major_).arg(minor_).arg(revision_);
		return retValue;
	}

public:
	QString sln_name_;
	int major_ = 0;
	int minor_ = 0;
	int revision_ = 0;
	int build_ = 0;
};

using stVdprojVersion = stVdprojVersion_;
Q_DECLARE_METATYPE(stVdprojVersion);


/// ----------------------------------------------------------------------------------------
/// @brief: 安装项目属性
/// ----------------------------------------------------------------------------------------
struct stInstallProjectVersion_ : public stVersionCommon
{
public:
	stInstallProjectVersion_()
	{
		this->zero();
	}

	void zero()
	{
		//build_ = ;
		build_ = "2022";
		stVersionCommon::zero();
		ft_ = FT_EXE_DLL;
		bat_file_ = { "" };
	}


	void toProductVersion(const stVdprojVersion& vdprojc_version)
	{
		this->major_ = vdprojc_version.major_;
		minor_ = vdprojc_version.minor_;
		revision_ = vdprojc_version.revision_;
		build_ = QString("%1").arg(vdprojc_version.build_);
	}


	stVdprojVersion toVdprojversion()
	{
		stVdprojVersion ret_value;

		ret_value.major_ = major_;
		ret_value.minor_ = minor_;
		ret_value.revision_ = revision_;
		ret_value.build_ = build_.toInt();

		return ret_value;
	}

	/// sln-project-platform-configuration
	QString uniqueName() const
	{
		return "[VDPROJ] [" + sln_name_ + "-" + project_name_ + "-" + platform_name_ + "-" + configuration_name_ + "] ";
	}


public:
	QString build_{};
	///
	int ft_ = FT_VDPROJ;
	/// 脚本文件
	QString		bat_file_{};
};

/// 文件版本
using stInstallProjectVersion = stInstallProjectVersion_;


/// <文件名, 文件版本>
using MapInstallProjectVersion = QMap<QString, stInstallProjectVersion>;
using ListInstallProjectVersion = QList<stInstallProjectVersion>;
Q_DECLARE_METATYPE(MapInstallProjectVersion);
Q_DECLARE_METATYPE(ListInstallProjectVersion);


/// ----------------------------------------------------------------------------------------
/// @brief: exe和dll文件属性
/// ----------------------------------------------------------------------------------------
struct stExeDllVersionInfo_
{
public:
	stExeDllVersionInfo_()
	{
		zero_();
	}

	void zero_()
	{
		project_name_ = QObject::tr("ProjectName");
		file_name_ = QObject::tr("ExeDllFile");
		file_major_ = 0;
		file_minor_ = 0;
		file_revision_ = 0;
		file_build_ = 0;
		project_major_ = 0;
		project_minor_ = 0;
		project_revision_ = 0;
		project_build_ = { "2022" };
		batch_file_ = { "" };
	}


public:
	QString project_name_{ "ProjectName" };
	QString file_name_{ "ExeDllFile" };
	int file_major_ = 0;
	int file_minor_ = 0;
	int file_revision_ = 0;
	int file_build_ = 0;

	int project_major_ = 0;
	int project_minor_ = 0;
	int project_revision_ = 0;
	QString project_build_ = { "20222" };
	/// 批处理文件
	QString batch_file_{ "" };
};

/// exe dll的版本结构体
using stExeDllVersionInfo = stExeDllVersionInfo_;


/// <项目名称, exe dll的版本结构体>
using MapExeDllVersionInfo = QMap<QString, stExeDllVersionInfo>;
using ListExeDllVersionInfo = QList<stExeDllVersionInfo>;
Q_DECLARE_METATYPE(MapExeDllVersionInfo);
Q_DECLARE_METATYPE(ListExeDllVersionInfo);

/// ----------------------------------------------------------------------------------------
/// @brief: VS版本
/// ----------------------------------------------------------------------------------------
struct stDevEnvInfo_
{
public:
	stDevEnvInfo_()
	{
		zero();
	}
	void zero()
	{
		name_ = QObject::tr("vs version");
		devenv_ = QObject::tr("path to devenv.exe");
	}

public:
	///名称
	QString		name_{};
	/// 对应的路径
	QString		devenv_{};
};

using stDevEnvInfo = stDevEnvInfo_;


using MapDevEnvInfo = QMap<QString, stDevEnvInfo>;
using ListDevEnv = QList<stDevEnvInfo>;

Q_DECLARE_METATYPE(MapDevEnvInfo);
Q_DECLARE_METATYPE(ListDevEnv);


using MapStrStr = QMap<QString, QString>;
using MapIntStr = QMap<int, QString>;
using MapIntInt = QMap<int, int>;
#pragma pack()

/// 数据库文件相对路径
const QString s_file_record_name = "/fr/fr.sl3";

/// VSwhere路径
const QString s_vswhere_name = "/vh/query.exe";
/// 查询vs信息
const QString s_query_vs_cmd = " -legacy -prerelease -format json";
const QString s_query_vs_cmd_arr[]
{
	"-legacy",
	"-prerelease",
	"-format json"
};

/// 数据库建库语句- devenv
const QString s_create_sql_table_devenv = "CREATE TABLE DevEnv ("\
"name   TEXT PRIMARY KEY "\
"UNIQUE "\
"NOT NULL, "\
"devenv TEXT "\
");"\
;

/// exe dll
const QString s_create_sql_table_exe_dll = "CREATE TABLE ExeDll ("\
"    project_name     TEXT    NOT NULL, "\
"    file_name        TEXT    PRIMARY KEY "\
"                             UNIQUE "\
"                             NOT NULL, "\
"    file_major       INTEGER, "\
"    file_minor       INTEGER, "\
"    file_revision    INTEGER, "\
"    file_build       INTEGER NOT NULL, "\
"    project_major    INTEGER NOT NULL, "\
"    project_minor    INTEGER NOT NULL, "\
"    project_revision INTEGER NOT NULL, "\
"    project_build    TEXT    NOT NULL, "\
"    bat_file         TEXT "\
");";

/// 安装包项目
const QString s_create_sql_table_vdproj = "CREATE TABLE VdProj ( "\
"    compiler_name TEXT    NOT NULL, "\
"    file_name     TEXT    PRIMARY KEY "\
"                          UNIQUE "\
"                          NOT NULL, "\
"    project_name  TEXT, "\
"    platform_name TEXT, "\
"    configuration TEXT, "\
"    major         INTEGER NOT NULL, "\
"    minor         INTEGER NOT NULL, "\
"    revision      INTEGER NOT NULL, "\
"    build         TEXT    NOT NULL, "\
"    file_type     INTEGER NOT NULL, "\
"    bat_file      TEXT "\
");";
