#include "pch.h"
#include "ExeVersionImp.h"
#include "CVersionUpdater.h"
#include "CVersionInfo.h"
#include <minwindef.h>


namespace oct_vi
{

	ExeVersionImp::ExeVersionImp()
	{

	}

	ExeVersionImp::~ExeVersionImp()
	{

	}

	int ExeVersionImp::Read(MapStrStr& map_file, MapVersionInfo& out_map_version)
	{
		//throw std::logic_error("The method or operation is not implemented.");
		if (0 == map_file.size())
		{
			return 1;
		}

		CVersionUpdater* pvu = new(std::nothrow) CVersionUpdater;
		if (nullptr == pvu)
		{
			return 2;
		}

		int ret_value = 0;

		stFileProductVersion fpv;

		/// 遍历文件
		for (MapStrStr::iterator fileIt = map_file.begin(); fileIt != map_file.end(); ++fileIt)
		{
			if (FALSE == PathFileExistsA(fileIt->second.file_.c_str()))
			{
				fileIt->second.str_error_ = std::string("file isnt existed");
				continue;
			}

			if (false == pvu->Open(fileIt->second.file_, fileIt->second.str_error_))
			{
				ret_value = 2;
				continue;
			}

			/// 解析成功，获取文件的版本信息
			fpv.zero();

			const CVersionInfo* version_info = pvu->VersionInfo;
			if (1 > version_info->StringFileInfo->Children.size())
			{
				char arr[10] = { 0 };
				sprintf(arr, "%d", GetLastError());
				fileIt->second.str_error_ = std::string("failed to get version infomation, last error_id=") + std::string(arr, 10);
				continue;
			}

			auto item = version_info->StringFileInfo->Children[0];

			/// 获取文件版本信息
			str2FileVesion(*(item->GetFileVersion()), fpv.file_);

			/// 获取产品版本信息
			str2ProductVersion(*(item->GetProductVersion()), fpv.product_);

			/// 插入
			out_map_version.emplace(fileIt->first, fpv);
		}

		delete pvu;

		return 0;
	}

	/// ----------------------------------------------------------------------------------------
	/// @brief: 写入文件
	/// ----------------------------------------------------------------------------------------
	int ExeVersionImp::Write(MapVersionInfo* in_map_write)
	{
		if (nullptr == in_map_write)
		{
			return 5;
		}

		//throw std::logic_error("The method or operation is not implemented.");
		if (0 == in_map_write->size())
		{
			return 1;
		}

		CVersionUpdater* pvu = new(std::nothrow) CVersionUpdater;
		if (nullptr == pvu)
		{
			return 2;
		}

		int failure_count = 0;

		/// ---------------------------------------------------------------------------------------
		/// 遍历map， 将文件中的信息写入文件

		std::string file_error;

		for (MapVersionInfo::iterator fileIt = in_map_write->begin(); fileIt != in_map_write->end(); ++fileIt)
		{
			if (FALSE == PathFileExistsA(fileIt->first.c_str()))
			{
				++failure_count;
				continue;
			}

			if (false == pvu->Open(fileIt->first.c_str(), file_error))
			{
				++failure_count;
				continue;
			}

			/// 将文件版本和产品版本信息写入文件
			if (0 != updateFileProductVersion(pvu->VersionInfo, fileIt->second))
			{
				++failure_count;
				continue;
			}

			/// 更新文件 失败
			if (false == pvu->Update())
			{
				++failure_count;
			}
		}

		delete pvu;

		if (0 != failure_count)
		{
			return 3;
		}

		return 0;
	}

	void ExeVersionImp::str2FileVesion(const CString& str, stFileVersion& file_ver)
	{
		/// 1. 文件版本示例“  1.0.0.1
		CStringArray out_arr;
		SplitCStr(str, out_arr, L".");

		/// 2. 赋值
		file_ver.major_ = _wtoi(out_arr[0]);

		file_ver.minor_ = _wtoi(out_arr[1]);

		file_ver.revision_ = _wtoi(out_arr[2]);

		file_ver.build_ = _wtoi(out_arr[3]);

	}

	void ExeVersionImp::str2ProductVersion(const CString& str, stProductVersion& product_ver)
	{
		/// 1. 文件版本示例“  1.0.0.11ABC
		CStringArray out_arr;
		SplitCStr(str, out_arr, L".");

		/// 2. 赋值
		product_ver.major_ = _wtoi(out_arr[0]);

		product_ver.minor_ = _wtoi(out_arr[1]);

		product_ver.revision_ = _wtoi(out_arr[2]);
		USES_CONVERSION;
		product_ver.build_ = std::string(W2A(out_arr[3]));
	}

	/// --------------------------------------------------------------------------------
	/// @brief: ExeVersionImp::SplitCStr
	/// --------------------------------------------------------------------------------
	void ExeVersionImp::SplitCStr(CString source, CStringArray& dest, CString division)
	{
		dest.RemoveAll();//自带清空属性
		CString temp = source;
		int tag = 0;
		while (1)
		{
			tag = temp.Find(division);
			if (tag >= 0)
			{
				dest.Add(temp.Left(tag));
				temp = temp.Right(temp.GetLength() - tag - 1);
			}
			else { break; }
		}
		dest.Add(temp);

	}

	/// --------------------------------------------------------------------------------
	/// @brief: ExeVersionImp::updateFileProductVersion
	/// --------------------------------------------------------------------------------
	int ExeVersionImp::updateFileProductVersion(CVersionInfo*& pfile_version, stFileProductVersion& new_version)
	{
		if (nullptr == pfile_version)
		{
			return 1;
		}

		if (1 > pfile_version->StringFileInfo->Children.size())
		{
			return 2;
		}

		auto item = pfile_version->StringFileInfo->Children[0];

		//inline void SetComments(LPCTSTR value_) { this->Set(Comments, value_); }
		//inline void SetCompanyName(LPCTSTR value_) { this->Set(CompanyName, value_); }
		//inline void SetFileDescription(LPCTSTR value_) { this->Set(FileDescription, value_); }
		//inline void SetFileVersion(LPCTSTR value_) { this->Set(FileVersion, value_); }
		//inline void SetInternalName(LPCTSTR value_) { this->Set(InternalName, value_); }
		//inline void SetLegalCopyright(LPCTSTR value_) { this->Set(LegalCopyright, value_); }
		//inline void SetLegalTrademarks(LPCTSTR value_) { this->Set(LegalTrademarks, value_); }
		//inline void SetOriginalFilename(LPCTSTR value_) { this->Set(OriginalFilename, value_); }
		//inline void SetPrivateBuild(LPCTSTR value_) { this->Set(PrivateBuild, value_); }
		//inline void SetProductName(LPCTSTR value_) { this->Set(ProductName, value_); }
		//inline void SetProductVersion(LPCTSTR value_) { this->Set(ProductVersion, value_); }
		//inline void SetSpecialBuild(LPCTSTR value_) { this->Set(SpecialBuild, value_); }

		/// 1. 设置文件版本信息: 1.2.3.5
		{
			const stFileVersion& file_ver = new_version.file_;
			CString str_file_version;
			str_file_version.Format(L"%d.%d.%d.%d", file_ver.major_, file_ver.minor_, file_ver.revision_, file_ver.build_);
			item->SetFileVersion(str_file_version);
		}

		/// 2. 设置产品版本信息: 1.2.3.5ABCD
		{
			const stProductVersion& product_ver = new_version.product_;
			CString str_build(product_ver.build_.c_str());

			CString str_file_version;
			str_file_version.Format(L"%d.%d.%d.%s", product_ver.major_, product_ver.minor_, product_ver.revision_, str_build);
			item->SetProductVersion(str_file_version);
		}

		pfile_version->FixedFileInfo.dwFileVersionMS = MAKELONG(new_version.file_.minor_, new_version.file_.major_);
		pfile_version->FixedFileInfo.dwFileVersionLS = MAKELONG(new_version.file_.build_, new_version.file_.revision_);

		return 0;
	}

}