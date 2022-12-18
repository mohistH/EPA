#pragma once
#include "IExeVersion.h"

class CVersionInfo;

namespace oct_vi
{
	class ExeVersionImp : public IExeVersion
	{
	public:

		ExeVersionImp();
		virtual ~ExeVersionImp();


		virtual int Read(MapStrStr& map_file, MapVersionInfo& out_map_version) override;


		virtual int Write(MapVersionInfo* in_map_write) override;

	private:
		/// 获取文件版本
		void str2FileVesion(const CString& str, stFileVersion& file_ver);

		/// 获取产品版本
		void str2ProductVersion(const CString& str, stProductVersion& product_ver);

		/// 指定拆分
		void SplitCStr(CString source, CStringArray& dest, CString division);

		/// -------------------------------------------------------------------------------
		/// @brief:		将 new_version 中的信息写入 pfile_version
		/// @param: 	CVersionInfo * pfile_version -  写入哪个文件
		/// @param: 	stFileProductVersion & new_version - 对应的版本信息
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		int updateFileProductVersion(CVersionInfo*& pfile_version, stFileProductVersion& new_version);

	};
}
