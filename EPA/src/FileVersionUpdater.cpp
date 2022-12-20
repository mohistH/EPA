#include "FileVersionUpdater.h"
#include "VersionHelper.h"
#include "VDProjFileAssistant.h"


namespace oct_epa
{

	FileVersionUpdater::FileVersionUpdater()
	{
		pexe_version_ = oct_vi::new_ev();
		pvdprojc_ = new(std::nothrow) VDProjFileAssistant;
	}

	FileVersionUpdater::~FileVersionUpdater()
	{
		uninit();
	}

	int FileVersionUpdater::Read(MapInstallProjectVersion& map_file)
	{
		if (nullptr == pexe_version_ || nullptr == pvdprojc_)
		{
			SetErrorStr("failed to creat obj");
			return 1;
		}

		if (0 == map_file.count())
		{
			SetErrorStr("the map_file's count is zero");
			return 2;
		}


		int failure_count = 0;
		/// 遍历文件。 获取文件版本
		for (MapInstallProjectVersion::iterator fileIt = map_file.begin(); fileIt != map_file.end(); ++fileIt)
		{
			if (FT_EXE_DLL == fileIt->ft_)
			{
				ReadExeVersion(fileIt.value());
			}
			else if (FT_VDPROJ == fileIt->ft_)
			{
				ReadVdprojVersion(fileIt.value());
			}
			else
			{
				++failure_count;
			}
		}

		if (0 == failure_count)
		{

			SetErrorStr("the file type of map_file is false");
			return 3;
		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 读取exedll中的版本号
	/// --------------------------------------------------------------------------------
	int FileVersionUpdater::Read(MapExeDllVersionInfo& map_file)
	{
		if (0 == map_file.count())
		{
			return 2;
		}

		oct_vi::MapStrStr map_src_file;
		oct_vi::MapVersionInfo map_out_version;
		{

			for (MapExeDllVersionInfo::iterator find_it = map_file.begin(); find_it != map_file.end(); ++find_it)
			{
				oct_vi::stFileState fs;
				fs.file_ = VersionHelper::qstr2str(find_it->file_name_);

				map_src_file.emplace(fs.file_, fs);
			}
		}

		/// 读取
		if (nullptr == pexe_version_)
		{
			return 1;
		}

		/// 读取数据后， 再重新赋值
		const int ret_value = pexe_version_->Read(map_src_file, map_out_version);
		{
			for (MapExeDllVersionInfo::iterator file_it = map_file.begin(); file_it != map_file.end(); ++file_it)
			{
				const std::string& std_file = VersionHelper::qstr2str(file_it->file_name_);
				oct_vi::MapVersionInfo::iterator exe_dll_it = map_out_version.find(std_file);

				if (exe_dll_it == map_out_version.end())
				{
					continue;
				}
				/// 赋值
				file_it->file_major_ = exe_dll_it->second.file_.major_;
				file_it->file_minor_ = exe_dll_it->second.file_.minor_;
				file_it->file_revision_ = exe_dll_it->second.file_.revision_;
				file_it->file_build_ = exe_dll_it->second.file_.build_;

				/// 
				file_it->project_major_ = exe_dll_it->second.product_.major_;
				file_it->project_minor_ = exe_dll_it->second.product_.minor_;
				file_it->project_revision_ = exe_dll_it->second.product_.revision_;
				file_it->project_build_ = VersionHelper::str2qstr(exe_dll_it->second.product_.build_);
			}
		}

		return ret_value;
	}

	QString FileVersionUpdater::ErrorStr()
	{
		return error_str_;
	}

	void FileVersionUpdater::uninit()
	{
		try
		{
			if (pexe_version_)
			{
				pexe_version_ = oct_vi::del_ev(pexe_version_);
			}

			if (pvdprojc_)
			{
				delete pvdprojc_;
				pvdprojc_ = nullptr;
			}
		}
		catch (...)
		{
			;
		}
	}

	void FileVersionUpdater::SetErrorStr(const QString& str)
	{
		error_str_ = str;
	}

	int FileVersionUpdater::ReadExeVersion(stInstallProjectVersion& out_value)
	{
		/// 调用exe 读取对象获取版本信息
		oct_vi::MapStrStr map_file;
		oct_vi::MapVersionInfo map_version;

		{
			oct_vi::stFileState fs;
			fs.file_ = VersionHelper::qstr2str(out_value.sln_name_);
			map_file.emplace(fs.file_, fs);
		}

		pexe_version_->Read(map_file, map_version);

		return 0;
	}

	int FileVersionUpdater::ReadVdprojVersion(stInstallProjectVersion& out_value)
	{
		//pvdprojc_->init();
		const stVdprojVersion version = pvdprojc_->version(out_value.sln_name_);

		out_value.toProductVersion(version);

		return 0;
	}

	int FileVersionUpdater::Write(MapExeDllVersionInfo* map_write)
	{
		oct_vi::MapVersionInfo map_exe_dll_write;
		{
			oct_vi::stFileProductVersion st_fpv;
			/// using MapVersionInfo = std::map<std::string, stFileProductVersion>;
			for (auto& item : (*map_write))
			{
				/// 设置文件版本
				st_fpv.file_.major_ = item.file_major_;
				st_fpv.file_.minor_ = item.file_minor_;
				st_fpv.file_.revision_ = item.file_revision_;
				st_fpv.file_.build_ = item.file_build_;

				/// 设置产品版本
				st_fpv.product_.major_ = item.project_major_;
				st_fpv.product_.minor_ = item.project_minor_;
				st_fpv.product_.revision_ = item.project_revision_;
				st_fpv.product_.build_ = VersionHelper::qstr2str(item.project_build_);

				const std::string& std_name = VersionHelper::qstr2str(item.file_name_);
				map_exe_dll_write.emplace(std_name, st_fpv);

				st_fpv.zero();
			}
		}

		int ret = pexe_version_->Write(&map_exe_dll_write);
		if (0 != ret)
		{
			return 1;
		}

		return 0;
	}

	int FileVersionUpdater::Write(MapInstallProjectVersion* map_write)
	{
		if (nullptr == map_write)
		{
			return -1;
		}

		/// map_write: <项目文件， 文件属性>

		stVdprojVersion vdproj;
		for (MapInstallProjectVersion::iterator findIt = map_write->begin(); findIt != map_write->end(); ++findIt)
		{
			vdproj.sln_name_ = findIt.key();
			vdproj.major_ = findIt->major_;
			vdproj.minor_ = findIt->minor_;
			vdproj.revision_ = findIt->revision_;
			vdproj.build_ = findIt->build_.toInt();

			pvdprojc_->setVersion(vdproj);

			vdproj.zero();
		}

		return 0;
	}

}