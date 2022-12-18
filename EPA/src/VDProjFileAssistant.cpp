#include "VDProjFileAssistant.h"

namespace oct_epa
{

	VDProjFileAssistant::~VDProjFileAssistant()
	{
		;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: VDProjFileAssistant::version
	/// --------------------------------------------------------------------------------
	stVdprojVersion VDProjFileAssistant::version(const QString& vdproj_name)
	{
		version_.sln_name_ = vdproj_name;

		QString target_value = readFile();
		getVersionFromFile(target_value);

		return version_;
	}


	/// --------------------------------------------------------------------------------
	/// @brief: VDProjFileAssistant::setVersion
	/// --------------------------------------------------------------------------------
	int VDProjFileAssistant::setVersion(const stVdprojVersion& vi)
	{
		if (0 == vi.sln_name_.length())
		{
			return 1;
		}

		version_ = vi;
		/// 获取文件内容
		readFile();
		/// 将信息写入文件
		updateFile();

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: VDProjFileAssistant::unInit
	/// --------------------------------------------------------------------------------
	int VDProjFileAssistant::updateFile()
	{
		/// 0. 修改版本信息
		{
			if (0 < product_version_row_index_ && 0 < product_version_key_pos_)
			{
				QString& target_value = file_content_[product_version_row_index_];

				///  "ProductVersion" = "8:
				target_value = target_value.left(find_key_.length() + product_version_key_pos_);
				/// 加上版本
				target_value += version_.toString() + "\"\n";
			}
			else
			{
				return 2;
			}
		}

		/// 1. 将内容写入文件
		//vdproj_file_.setFileName(file_name_);
		QFile  vdproj_file_(version_.sln_name_);


		if (false == vdproj_file_.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		{
			return 3;
		}

		for (int index = 0; index < file_content_.count(); ++index)
		{
			QString& target_value = file_content_[index];
			vdproj_file_.write(target_value.toLocal8Bit());
		}

		vdproj_file_.close();


		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: VDProjFileAssistant::readFile
	/// --------------------------------------------------------------------------------
	QString VDProjFileAssistant::readFile()
	{
		//vdproj_file_.setFileName(file_name_);
		QFile  vdproj_file_(version_.sln_name_);
		if (false == vdproj_file_.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			return "";
		}

		file_content_.clear();

		/// 读取文件内容
		while (false == vdproj_file_.atEnd())
		{
			file_content_ << vdproj_file_.readLine();
		}
		vdproj_file_.close();

		///如果没找到想要的信息，则返回错误
		const QString& find_key = find_key_;

		QString target_value;
		for (int index = 0; index < file_content_.count(); ++index)
		{
			QString& line_text = file_content_[index];
			const int find_pos = line_text.indexOf(find_key);
			if (0 < find_pos)
			{
				product_version_key_pos_ = find_pos;
				product_version_row_index_ = index;

				const int&& ver_len = line_text.length() - find_pos - find_key.length();
				if (0 < ver_len)
				{
					/// 得到版本： 1.0.0\n 
					target_value = line_text.right(ver_len);
					break;
				}

			}
		}

		//if (0 == target_value.length())
		//{
		//	return 2;
		//}

		return target_value;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: VDProjFileAssistant::getVersionFromFile
	/// --------------------------------------------------------------------------------
	int VDProjFileAssistant::getVersionFromFile(QString& target_value)
	{
		if (0 == target_value.length())
		{
			return 1;
		}

		/// 去除\n
		const QString&& new_line_flag{ "\\n" };
		target_value = target_value.left(target_value.length() - new_line_flag.length());
		/// 此时，ver_value = 1.0.0, 转为对应的版本信息
		QStringList tmp_list = target_value.split('.');
		const int&& tmp_list_count = tmp_list.count();
		if (0 < tmp_list_count)
		{
			version_.major_ = tmp_list.at(0).toInt();
		}

		if (1 < tmp_list_count)
		{
			version_.minor_ = tmp_list.at(1).toInt();
		}

		if (2 < tmp_list_count)
		{
			version_.revision_ = tmp_list.at(2).toInt();
		}

		return 0;
	}

}