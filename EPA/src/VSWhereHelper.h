#ifndef VS_WHERE_HELPER_H_
#define VS_WHERE_HELPER_H_
#include <Typedef.h>

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
	};
}

#endif //!VS_WHERE_HELPER_H_