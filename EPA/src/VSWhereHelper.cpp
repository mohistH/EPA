#include <VSWhereHelper.h>
#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDebug>
#include <QSettings>
#include <QFile>

namespace oct_epa
{

	/// --------------------------------------------------------------------------------
	/// @brief: VSWhereHelper::VSWhereHelper
	/// --------------------------------------------------------------------------------
	VSWhereHelper::VSWhereHelper()
	{
		HashVS2010_VS2015& hash_vs = hash_vs2010_vs2015_;

		stVS2010_VS2015 item_hash;
		auto insert_hash = [&hash_vs, &item_hash](const QString& key, const QString& name)
		{
			item_hash.zero();
			item_hash.set(key, name);

			hash_vs.insert(item_hash.vs_name_, item_hash);
		};

		const QString& key_suffix = "/InstallDir";
		insert_hash("10.0" + key_suffix, "Visual Studio 2010");
		insert_hash("11.0" + key_suffix, "Visual Studio 2012");
		insert_hash("12.0" + key_suffix, "Visual Studio 2013");
		insert_hash("14.0" + key_suffix, "Visual Studio 2015");
	}

	/// --------------------------------------------------------------------------------
	/// @brief: VSWhereHelper::~VSWhereHelper
	/// --------------------------------------------------------------------------------
	VSWhereHelper::~VSWhereHelper()
	{

	}

	/// --------------------------------------------------------------------------------
	/// @brief: 
	/// --------------------------------------------------------------------------------
	int VSWhereHelper::read(MapDevEnvInfo* pout_dei)
	{
		/// 参数检查
		if (nullptr == pout_dei)
		{
			return 1;
		}

		readVS2017Upper(pout_dei);
		readVS2015Lower(pout_dei);

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: VSWhereHelper::readVS2017Upper
	/// --------------------------------------------------------------------------------
	int VSWhereHelper::readVS2017Upper(MapDevEnvInfo* pout_dei)
	{

		/// 用于保存结果
		QByteArray ba_vs;
		{
			/// 2. 如果vswhere 不存在，则pass
			const QString& vs_where = QApplication::applicationDirPath() + s_vswhere_name;
			{
				QFile file_vw(vs_where);
				if (false == file_vw.exists())
				{
					return 2;
				}
			}

			/// 3. 读取信息
			QProcess process;
			process.setWorkingDirectory(QApplication::applicationDirPath() + "/vh");

			// //---
			// const QString& batch_file = QApplication::applicationDirPath() + "/vh/2.bat";
			// process.start(batch_file);
			// //--

			//QStringList tmp_query_cmd;
			//tmp_query_cmd << " -legacy " << " -prerelease " << "  -format json > 2.json ";

			//process.setProgram(vs_where);
			const QString& query_cmd_tmp = QApplication::applicationDirPath() + s_vswhere_name + s_query_vs_cmd;
			// const QString& query_cmd_tmp = ":/vswhere/vswhere/vswhere.exe" + s_query_vs_cmd;
			qDebug() << query_cmd_tmp;
			process.start(query_cmd_tmp);

			process.waitForStarted();
			process.waitForFinished();
			process.waitForReadyRead();
			ba_vs = process.readAllStandardOutput();

			const int start_pos = ba_vs.indexOf('[');
			if (0 > start_pos)
			{
				return 8;
			}
			ba_vs = ba_vs.mid(start_pos - 1, ba_vs.length() - start_pos).trimmed();
			QString str_tmp(ba_vs);
			ba_vs = str_tmp.toUtf8();
			if (0 == ba_vs.length())
			{
				return 7;
			}
		}

		QJsonParseError  jpe;
		QJsonDocument jd = QJsonDocument::fromJson(ba_vs, &jpe);

		/// 解析错误
		if (QJsonParseError::NoError != jpe.error)
		{
			//qDebug() << jpe.errorString();
			return 6;
		}

		QJsonArray& json_arr = jd.array();

		/// 解析devenv的地址
		stDevEnvInfo dev_info;
		const int& arr_size = json_arr.count();
		for (int index = 0; index < arr_size; ++index)
		{
			QJsonValue& jv = json_arr.at(index);
			/// 得到devenv.exe的地址
			dev_info.devenv_ = jv["productPath"].toString();
			if (0 == dev_info.devenv_.length())
			{
				continue;
			}

			/// 名称 displayName
			dev_info.name_ = jv["displayName"].toString();
			if (0 == dev_info.name_.length())
			{
				dev_info.name_ = "DevEnvName" + QString("%1").arg(index + 1);
			}

			/// 加入map
			pout_dei->insert(dev_info.name_, dev_info);

			dev_info.zero();
		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: VSWhereHelper::readVS2015Lower
	/// --------------------------------------------------------------------------------
	int VSWhereHelper::readVS2015Lower(MapDevEnvInfo* pout_dei)
	{
		//const QString&& reg_key_prefix = "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio";
		const QString&& reg_key_prefix = "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\";
		QSettings reg_edit(reg_key_prefix, QSettings::Registry64Format);// NativeFormat);/// QSettings::Registry64Format);
		stDevEnvInfo dev_info;


		//QStringList key_list = reg_edit.allKeys();
		QString str_tmp_2 = reg_edit.value("ApplicationID").toString();
		if (0 != str_tmp_2.length())
		{
			int x = 0;
		}

		/// 遍历注册表for (const autos item hash : hash vs2010 2015 
		///读取键值，如果为空，则继续读取: 得到的是 path/common7/IDE目录OStringo dev_env_namereg_edit.value(item_hash.reg_key_).tostring()if (adev_env_name .length( ))
		for (const auto& item_hash : hash_vs2010_vs2015_)
		{
			/// 读取简直
			QString dev_env_name = reg_edit.value(item_hash.reg_key_).toString();
			if (0 == dev_env_name.trimmed().length())
			{
				continue;
			}

			dev_env_name += "/devenv.exe";

			/// 文件不存在。
			{
				QFile dev_env_file(dev_env_name);
				if (false == dev_env_file.exists())
				{
					continue;
				}
			}

			dev_info.devenv_ = dev_env_name;
			dev_info.name_ = item_hash.vs_name_;
			pout_dei->insert(dev_info.name_, dev_info);

			dev_info.zero();

		}

		return 0;
	}

}