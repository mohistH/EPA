#include <VSWhereHelper.h>
#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDebug>

namespace oct_epa
{

	/// --------------------------------------------------------------------------------
	/// @brief: VSWhereHelper::VSWhereHelper
	/// --------------------------------------------------------------------------------
	VSWhereHelper::VSWhereHelper()
	{

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

}