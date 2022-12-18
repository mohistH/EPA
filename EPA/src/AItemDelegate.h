#pragma once

#include <QItemDelegate>

class AItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	enum COLUMN_ID
	{
		CI_00 = 0,
		CI_01 = 1,
		CI_02 = 2,
		CI_03 = 3,
		CI_04 = 4,
		CI_05 = 5,
		CI_06 = 6,
		CI_07 = 7,
		CI_08 = 8,
		CI_09 = 9,
		CI_10 = 10,
	};

public:
	using CreateColumnWidgetFunc = QWidget * (AItemDelegate::*)(QWidget* parent) const;
	using SetColumnModelDataFunc = void (AItemDelegate::*)(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	using SetColumnEditDataFunc = void (AItemDelegate::*)(QWidget* editor, const QModelIndex& index) const;

public:
	explicit AItemDelegate(QObject* parent = nullptr);
	virtual ~AItemDelegate();
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
	/// @brief:		绑定列
	/// @param: 	const int & column_index - 
	/// @param: 	CreateColumnWidgetFunc create_func - 
	/// @param: 	SetColumnModelDataFunc set_func - 
	/// @param: 	SetColumnEditDataFunc set_editor_func - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setColumnHash(const int& column_index, CreateColumnWidgetFunc create_func, SetColumnModelDataFunc set_func, SetColumnEditDataFunc set_editor_func);

	void removeColumn(const int& column_index);

public:
	virtual QWidget* createColumnWidget00(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget01(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget02(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget03(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget04(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget05(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget06(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget07(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget08(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget09(QWidget* parent) const = 0;
	virtual QWidget* createColumnWidget10(QWidget* parent) const = 0;


	virtual void setColumnModelData00(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData01(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData02(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData03(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData04(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData05(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData06(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData07(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData08(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData09(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
	virtual void setColumnModelData10(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;


	virtual void setColumnEditorData00(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData01(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData02(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData03(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData04(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData05(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData06(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData07(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData08(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData09(QWidget* editor, const QModelIndex& index) const = 0;
	virtual void setColumnEditorData10(QWidget* editor, const QModelIndex& index) const = 0;

public:
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

public:


	struct  stColumnWidgetFunc_
	{
		CreateColumnWidgetFunc create_func_ = nullptr;
		SetColumnModelDataFunc	set_model_data_func_ = nullptr;
		SetColumnEditDataFunc set_editor_data_func_ = nullptr;
	};

	using stColumnWidgetFunc = stColumnWidgetFunc_;
	using HashColumnWidgetFunc = QHash<int, stColumnWidgetFunc>;

	HashColumnWidgetFunc	hash_column_widget_func_;

};
