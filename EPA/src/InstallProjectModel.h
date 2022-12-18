#pragma once

#include <QHash>
#include "ATableModel.h"
#include "Typedef.h"


class InstallProjectModel : public ATableModel
{
	Q_OBJECT
public:
	explicit InstallProjectModel(QObject* parent = nullptr);
	virtual ~InstallProjectModel();

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	/// -------------------------------------------------------------------------------
	/// @brief:		设置数据
	/// @param: 	const ListFileInfo & newData - 新的model数据
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setModelData(const ListInstallProjectVersion& newData);

	/// -------------------------------------------------------------------------------
	/// @brief:		清空model数据
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void clearModelData();

	/// -------------------------------------------------------------------------------
	/// @brief:		获取model数据
	///  @ret:		ListFileInfo
	///				
	/// -------------------------------------------------------------------------------
	ListInstallProjectVersion modelData();

private:
	/// -------------------------------------------------------------------------------
	/// @brief:		内部初始化
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void init();


	/// -------------------------------------------------------------------------------
	/// @brief:		 vs 版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData00(const stInstallProjectVersion& file_info) const;


	/// -------------------------------------------------------------------------------
	/// @brief:		解决方案名称
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData01(const stInstallProjectVersion& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		项目名称
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData02(const stInstallProjectVersion& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		平台名称
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData03(const stInstallProjectVersion& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		配置管理器
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData04(const stInstallProjectVersion& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		主版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData05(const stInstallProjectVersion& file_info) const;


	/// -------------------------------------------------------------------------------
	/// @brief:		小版本
	/// @param: 	const stFileProductVersion & file_info - 
	///  @ret:		QT_NAMESPACE::QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData06(const stInstallProjectVersion& file_info) const;



	/// -------------------------------------------------------------------------------
	/// @brief:		内部版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData07(const stInstallProjectVersion& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		修订版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData08(const stInstallProjectVersion& file_info) const;


	/// -------------------------------------------------------------------------------
	/// @brief:		批处理文件
	/// @param: 	const stFileProductVersion & file_info - 
	///  @ret:		QT_NAMESPACE::QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData09(const stInstallProjectVersion& file_info) const;




	/// -------------------------------------------------------------------------------
	/// @brief:		vs选择
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData00(stInstallProjectVersion* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		解决方案
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData01(stInstallProjectVersion* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		项目文件
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData02(stInstallProjectVersion* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		平台
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData03(stInstallProjectVersion* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		配置管理器
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData04(stInstallProjectVersion* pfi, const QVariant& column_data);

	/// -------------------------------------------------------------------------------
	/// @brief:		主版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData05(stInstallProjectVersion* pfi, const QVariant& column_data);

	/// -------------------------------------------------------------------------------
	/// @brief:		小版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData06(stInstallProjectVersion* pfi, const QVariant& column_data);



	/// -------------------------------------------------------------------------------
	/// @brief:		内部版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData07(stInstallProjectVersion* pfi, const QVariant& column_data);

	/// -------------------------------------------------------------------------------
	/// @brief:		小版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData08(stInstallProjectVersion* pfi, const QVariant& column_data);

	/// -------------------------------------------------------------------------------
	/// @brief:		批处理文件
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData09(stInstallProjectVersion* pfi, const QVariant& column_data);

private:

	/// 函数指针
	using ColumnDataFunc = QVariant(InstallProjectModel::*)(const stInstallProjectVersion& file_info) const;
	using SetColumnDataFunc = void (InstallProjectModel::*)(stInstallProjectVersion* pfi, const QVariant& column_data);

	struct stColumnDataFunc_
	{
		ColumnDataFunc		column_data_func_ = nullptr;
		SetColumnDataFunc	set_column_data_func_ = nullptr;
	};

	using stColumnDataFunc = stColumnDataFunc_;

	/// <列号，对应列的处理函数>
	using HashColumnDispatch = QHash<int, stColumnDataFunc>;



private:

	/// 数据
	ListInstallProjectVersion			list_file_info_;

	/// 列处理函数
	HashColumnDispatch		hash_column_dispatch_;
};

