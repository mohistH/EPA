#pragma once

#include "IExeVersion.h"
#include "Typedef.h"

namespace oct_epa
{

	class VDProjFileAssistant;

	/// ----------------------------------------------------------------------------------------
	/// @brief: 用于统一修改文件版本信息
	/// ----------------------------------------------------------------------------------------
	class FileVersionUpdater
	{
	public:
		FileVersionUpdater();
		virtual ~FileVersionUpdater();

		/// 读取参数中的文件版本信息
		int Read(MapInstallProjectVersion& map_file);
		/// 读取exedll中的版本号
		int Read(MapExeDllVersionInfo& map_file);

		/// 写入参数中的版本信息
		int Write(MapExeDllVersionInfo* map_write);

		/// 写入参数中的文件版本
		int Write(MapInstallProjectVersion* map_write);

		/// 读取错误信息
		QString ErrorStr();

	private:
		void uninit();

		void SetErrorStr(const QString& str);

		int ReadExeVersion(stInstallProjectVersion& out_value);

		int ReadVdprojVersion(stInstallProjectVersion& out_value);



	private:

		/// vdproj
		VDProjFileAssistant* pvdprojc_ = nullptr;
		/// exe dll文件
		oct_vi::IExeVersion* pexe_version_ = nullptr;

		QString error_str_;

	};

}
