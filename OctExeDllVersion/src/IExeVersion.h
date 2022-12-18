#ifndef IEXE_VERSION_H_
#define IEXE_VERSION_H_


#include <string>
#include <unordered_map>
#include <map>


#ifndef EXE_VERSION_LIB 
#define EXE_VERSION_LIB __declspec(dllexport)
#else
#define EXE_VERSION_LIB __declspec(dllimport)
#endif /// !EXE_VERSION_LIB

namespace oct_vi
{


	struct stVersionBase
	{
	public:
		stVersionBase()
		{

		}

		void zero()
		{
			major_ = 0;
			minor_ = 0;
			revision_ = 0;
		}

		int		major_ = 0;
		int		minor_ = 0;
		int		revision_ = 0;
	};

	/// 产品版本
	struct stProductVersion_ : stVersionBase
	{
	public:
		stProductVersion_()
		{
			this->zero();
		}

		void zero()
		{
			stVersionBase::zero();
			build_ = std::string("221203");
		}

	public:
		std::string build_{};
	};

	using stProductVersion = stProductVersion_;


	/// 文件版本
	struct stFileVersion_ : stVersionBase
	{
	public:

		stFileVersion_()
		{
			this->zero();
		}

		void zero()
		{
			build_ = 0;
			stVersionBase::zero();
		}

	public:
		int build_ = 0;
	};

	using stFileVersion = stFileVersion_;


	/// 文件版本信息
	struct stFileProductVersion_
	{
		stFileProductVersion_()
		{
			this->zero();
		}

		/// 文件版本
		stFileVersion file_;
		/// 产品版本
		stProductVersion product_;

		void zero()
		{
			file_.zero();
			product_.zero();
		}
	};

	using stFileProductVersion = stFileProductVersion_;


	/// <文件，文件信息>
	using MapVersionInfo = std::map<std::string, stFileProductVersion>;



	struct stFileState_
	{
		std::string file_{};
		std::string str_error_{};
	};

	using stFileState = stFileState_;

	using MapStrStr = std::map < std::string, stFileState >;



#ifdef __cplusplus
	extern "C" {
#endif 

		/// 接口类
		class IExeVersion
		{
		public:
			virtual ~IExeVersion() {}

			virtual int Read(MapStrStr& map_file, MapVersionInfo& out_map_version) = 0;

			virtual int Write(MapVersionInfo* in_map_write) = 0;
		};



		EXE_VERSION_LIB IExeVersion* new_ev();

		EXE_VERSION_LIB IExeVersion* del_ev(IExeVersion* pobj);

#ifdef __cplusplus
	}
#endif 
}


#endif // IEXE_VERSION_H_