#pragma once
#include "Typedef.h"


class IUIData
{
public:
	virtual ~IUIData() {}

	/// -------------------------------------------------------------------------------
	/// @brief:		读取UIdevenv数据
	/// @param: 	ListDevEnv * pout_list - 
	///  @ret:		int
	///				virtual 
	/// -------------------------------------------------------------------------------
	virtual int devenvModelData(ListDevEnv* pout_list) = 0;

	/// -------------------------------------------------------------------------------
	/// @brief:		获取exe dll Data
	/// @param: 	ListExeDllVersionInfo * pout_list - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	virtual int exeDllModelData(ListExeDllVersionInfo* pout_list) = 0;

	/// -------------------------------------------------------------------------------
	/// @brief:		获取安装包项目数据
	/// @param: 	ListInstallProjectVersion * pout_list - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	virtual int installProjectModelData(ListInstallProjectVersion* pout_list) = 0;

	/// -------------------------------------------------------------------------------
	/// @brief:		读取UIdevenv数据
	/// @param: 	ListDevEnv * pout_list - 
	///  @ret:		int
	///				virtual 
	/// -------------------------------------------------------------------------------
	virtual int setDevenvModelData(ListDevEnv* pout_list) = 0;

	/// -------------------------------------------------------------------------------
	/// @brief:		获取exe dll Data
	/// @param: 	ListExeDllVersionInfo * pout_list - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	virtual int setExeDllModelData(ListExeDllVersionInfo* pout_list) = 0;

	/// -------------------------------------------------------------------------------
	/// @brief:		获取安装包项目数据
	/// @param: 	ListInstallProjectVersion * pout_list - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	virtual int setInstallProjectModelData(ListInstallProjectVersion* pout_list) = 0;
};

