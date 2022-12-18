#pragma once

#include "Typedef.h"
#include "ATableModel.h"



/// ----------------------------------------------------------------------------------------
/// @brief: exe dll 文件model
/// ----------------------------------------------------------------------------------------
class ExeDllFileModel : public ATableModel
{

	Q_OBJECT
public:
	explicit ExeDllFileModel(QObject* parent = nullptr);
	virtual ~ExeDllFileModel();

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	/// -------------------------------------------------------------------------------
	/// @brief:		设置数据
	/// @param: 	const ListFileInfo & newData - 新的model数据
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setModelData(const ListExeDllVersionInfo& newData);

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
	ListExeDllVersionInfo modelData();


private:
	/// -------------------------------------------------------------------------------
	/// @brief:		内部初始化
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void init();


	/// -------------------------------------------------------------------------------
	/// @brief:		 项目名称
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData00(const stExeDllVersionInfo& file_info) const;


	/// -------------------------------------------------------------------------------
	/// @brief:		exe文件
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData01(const stExeDllVersionInfo& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		主版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData02(const stExeDllVersionInfo& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		小版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData03(const stExeDllVersionInfo& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		内部版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData04(const stExeDllVersionInfo& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		修订版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData05(const stExeDllVersionInfo& file_info) const;


	/// -------------------------------------------------------------------------------
/// @brief:		主版本
/// @param: 	const stFileInfo & file_info - 
///  @ret:		QVariant
///				
/// -------------------------------------------------------------------------------
	QVariant columnData06(const stExeDllVersionInfo& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		小版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData07(const stExeDllVersionInfo& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		内部版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData08(const stExeDllVersionInfo& file_info) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		修订版本
	/// @param: 	const stFileInfo & file_info - 
	///  @ret:		QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData09(const stExeDllVersionInfo& file_info) const;


	/// -------------------------------------------------------------------------------
	/// @brief:		
	/// @param: 	const stFileProductVersion & file_info - 
	///  @ret:		QT_NAMESPACE::QVariant
	///				
	/// -------------------------------------------------------------------------------
	QVariant columnData10(const stExeDllVersionInfo& file_info) const;





	/// -------------------------------------------------------------------------------
	/// @brief:		 项目名称
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData00(stExeDllVersionInfo* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		exe文件
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData01(stExeDllVersionInfo* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		主版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData02(stExeDllVersionInfo* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		小版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData03(stExeDllVersionInfo* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		内部版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData04(stExeDllVersionInfo* pfi, const QVariant& column_data);

	/// -------------------------------------------------------------------------------
	/// @brief:		修订版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData05(stExeDllVersionInfo* pfi, const QVariant& column_data);

	/// -------------------------------------------------------------------------------
	/// @brief:		主版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData06(stExeDllVersionInfo* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		小版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData07(stExeDllVersionInfo* pfi, const QVariant& column_data);


	/// -------------------------------------------------------------------------------
	/// @brief:		内部版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData08(stExeDllVersionInfo* pfi, const QVariant& column_data);

	/// -------------------------------------------------------------------------------
	/// @brief:		修订版本
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData09(stExeDllVersionInfo* pfi, const QVariant& column_data);

	/// -------------------------------------------------------------------------------
	/// @brief:		批处理文件
	/// @param: 	stFileInfo * pfi - 
	/// @param: 	const QVariant & column_data - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnData10(stExeDllVersionInfo* pfi, const QVariant& column_data);


private:

	/// 函数指针
	using ColumnDataFunc = QVariant(ExeDllFileModel::*)(const stExeDllVersionInfo& file_info) const;
	using SetColumnDataFunc = void (ExeDllFileModel::*)(stExeDllVersionInfo* pfi, const QVariant& column_data);

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
	ListExeDllVersionInfo			list_file_info_;

	/// 列处理函数
	HashColumnDispatch			hash_column_dispatch_;

};

