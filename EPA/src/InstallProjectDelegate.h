#pragma once
#include <QHash>

#include <QItemDelegate>
#include "Typedef.h"
class QTableView;



class InstallProjectModel;

class InstallProjectDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	explicit InstallProjectDelegate(QObject* parent = nullptr);
	virtual  ~InstallProjectDelegate();

	/// --------------------------------------------------------------------------------
	/// @brief: createEditor
	/// --------------------------------------------------------------------------------
	virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setModelData
	/// --------------------------------------------------------------------------------
	virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setEditorData
	/// --------------------------------------------------------------------------------
	virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;

	/// -------------------------------------------------------------------------------
	/// @brief:		设置data数据
	/// @param: 	const ExePackModel * pmodel - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setModel(InstallProjectModel* pmodel, QTableView* ptv);

private:
	/// vs版本选择
	QWidget* createColumnWidget00(QWidget* parent) const;
	/// 解决方案文件
	QWidget* createColumnWidget01(QWidget* parent) const;
	/// 项目文件
	QWidget* createColumnWidget02(QWidget* parent) const;
	/// 平台文件
	QWidget* createColumnWidget03(QWidget* parent) const;
	/// 配置
	QWidget* createColumnWidget04(QWidget* parent) const;
	/// 主版本号
	QWidget* createColumnWidget05(QWidget* parent) const;
	/// 小版本
	QWidget* createColumnWidget06(QWidget* parent) const;
	/// 内部版本
	QWidget* createColumnWidget07(QWidget* parent) const;
	/// 小版本
	QWidget* createColumnWidget08(QWidget* parent) const;
	/// 批处理
	QWidget* createColumnWidget09(QWidget* parent) const;


	void setColumnModelData00(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData01(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData02(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData03(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData04(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData05(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData06(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData07(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData08(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	void setColumnModelData09(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;


	void setColumnEditorData00(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData01(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData02(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData03(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData04(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData05(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData06(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData07(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData08(QWidget* editor, const QModelIndex& index) const;
	void setColumnEditorData09(QWidget* editor, const QModelIndex& index) const;


private:
	/// -------------------------------------------------------------------------------
	/// @brief:		创建下拉框
	/// @param: 	QWidget * parent - 
	/// @param: 	const QStringList & list_value - 
	///  @ret:		QT_NAMESPACE::QWidget*
	///				
	/// -------------------------------------------------------------------------------
	QWidget* createColumnCombobox(QWidget* parent, const QStringList& list_value) const;

	/// -------------------------------------------------------------------------------
	/// @brief:		创建spinbox
	/// @param: 	QWidget * parent - 
	///  @ret:		QT_NAMESPACE::QWidget*
	///				
	/// -------------------------------------------------------------------------------
	QWidget* createColumnSpinBox(QWidget* parent, const int max_value) const;


private:
	using CreateColumnWidgetFunc = QWidget * (InstallProjectDelegate::*)(QWidget* parent) const;
	using SetColumnModelDataFunc = void (InstallProjectDelegate::*)(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	using SetColumnEditDataFunc = void (InstallProjectDelegate::*)(QWidget* editor, const QModelIndex& index) const;

	struct  stColumnWidgetFunc_
	{
		CreateColumnWidgetFunc create_func_ = nullptr;
		SetColumnModelDataFunc	set_model_data_func_ = nullptr;
		SetColumnEditDataFunc set_editor_data_func_ = nullptr;
	};

	using stColumnWidgetFunc = stColumnWidgetFunc_;
	using HashColumnWidgetFunc = QHash<int, stColumnWidgetFunc>;

	HashColumnWidgetFunc	hash_column_widget_func_;

	/// 用于获取界面的数据
	InstallProjectModel* pmodel_data_ = nullptr;

	/// QTableView
	QTableView* ptv_ = nullptr;
};
