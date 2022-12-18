#pragma once
#include <QFile>
#include "Typedef.h"


namespace oct_epa
{
	/// ----------------------------------------------------------------------------------------
/// @brief:  vdproj 文件替换版本号
/// ----------------------------------------------------------------------------------------
	class VDProjFileAssistant
	{
	public:
		VDProjFileAssistant()
		{
			;
		}

		virtual ~VDProjFileAssistant();

		stVdprojVersion version(const QString& vdproj_name);

		int setVersion(const stVdprojVersion& vi);

	private:
		int updateFile();

		/// -------------------------------------------------------------------------------
		/// @brief:		获取目标字符串
		///  @ret:		QT_NAMESPACE::QString
		///				
		/// -------------------------------------------------------------------------------
		QString readFile();

		int getVersionFromFile(QString& target_value);


	private:
		stVdprojVersion version_;
		QStringList	file_content_;
		/// 版本所在行
		int product_version_row_index_ = -1;
		int product_version_key_pos_ = -1;
		QString find_key_{ "\"ProductVersion\" = \"8:" };
	};


}