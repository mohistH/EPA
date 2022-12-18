#pragma once
#include <QHash>
#include "ATableModel.h"
#include "Typedef.h"



class DevEnvModel : public ATableModel
{
	Q_OBJECT

public:
	explicit DevEnvModel(QObject* parent = nullptr);
	virtual ~DevEnvModel();

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

	/// -------------------------------------------------------------------------------
	/// @brief:		设置数据
	/// @param: 	const ListVSVersion & newData - 新的model数据
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setModelData(const ListDevEnv& newData);

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
	ListDevEnv modelData();


private:
	/// -------------------------------------------------------------------------------
	/// @brief:		内部初始化
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void init();


	/// -------------------------------------------------------------------------------
	/// @brief:		用于model中调用，设置第几列显示的数据内容 - 第一列时文件全名
	/// @param: 	const stVSVersion & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData00(const stDevEnvInfo& file_info) const;


	/// -------------------------------------------------------------------------------
	/// @brief:		显示主版本号
	/// @param: 	const stVSVersion & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData01(const stDevEnvInfo& file_info) const;


	/// -------------------------------------------------------------------------------
	/// @brief:		文件名
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData00(stDevEnvInfo* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		主版本号
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData01(stDevEnvInfo* pfi, const QVariant& column_data);



private:

	/// 函数指针
	using ColumnDataFunc = QVariant(DevEnvModel::*)(const stDevEnvInfo& file_info) const;
	using SetColumnDataFunc = void (DevEnvModel::*)(stDevEnvInfo* pfi, const QVariant& column_data);

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
	ListDevEnv			list_file_info_;

	/// 列处理函数
	HashColumnDispatch		hash_column_dispatch_;
};

