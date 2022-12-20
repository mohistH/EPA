#ifndef VS_WHERE_HELPER_H_
#define VS_WHERE_HELPER_H_
#include <Typedef.h>
#include<QHash>

namespace oct_epa
{
	class VSWhereHelper
	{
	public:
		VSWhereHelper();
		virtual ~VSWhereHelper();

	public:
		/// -------------------------------------------------------------------------------
		/// @brief:		获取本机devenv数据
		/// @param: 	MapDevEnvInfo * pout_dei - 
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		int read(MapDevEnvInfo* pout_dei);


	private:
		/// -------------------------------------------------------------------------------
		/// @brief:		readVS2017Upper
		/// @param: 	MapDevEnvInfo * pout_dei - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		int readVS2017Upper(MapDevEnvInfo* pout_dei);

		int readVS2015Lower(MapDevEnvInfo* pout_dei);
	private:

		struct stVS2010_VS2015_
		{
		public:

			stVS2010_VS2015_()
			{
				zero();
			}

			void zero()
			{
				reg_key_.clear();
				vs_name_.clear();
			}

			void set(const QString& key, const QString& name)
			{
				this->reg_key_ = key;
				this->vs_name_ = name;
			}

		public:
			/// 对应的
			QString reg_key_{};
			QString vs_name_{};
		};

		using stVS2010_VS2015 = stVS2010_VS2015_;

		/// <Vs name, st>
		using HashVS2010_VS2015 = QHash<QString, stVS2010_VS2015>;

		HashVS2010_VS2015 hash_vs2010_vs2015_;


	};
}

#endif //!VS_WHERE_HELPER_H_