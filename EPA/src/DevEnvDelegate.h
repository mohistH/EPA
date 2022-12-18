#pragma once
#include <QItemDelegate>
#include <QHash>


class DevEnvDelegate : public QItemDelegate
{

	Q_OBJECT
public:
	explicit DevEnvDelegate(QObject* parent = nullptr);
	virtual ~DevEnvDelegate();


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

private:
	/// VS版本
	QWidget* createColumnWidget01(QWidget* parent) const;


	void setColumnModelData01(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

	void setColumnEditorData01(QWidget* editor, const QModelIndex& index) const;

private:
	using CreateColumnWidgetFunc = QWidget * (DevEnvDelegate::*)(QWidget* parent) const;
	using SetColumnModelDataFunc = void (DevEnvDelegate::*)(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	using SetColumnEditDataFunc = void (DevEnvDelegate::*)(QWidget* editor, const QModelIndex& index) const;

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

