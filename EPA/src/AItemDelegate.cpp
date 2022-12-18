#include "AItemDelegate.h"
#include <QSpinBox>
#include <QComboBox>

AItemDelegate::AItemDelegate(QObject* parent)
	: QItemDelegate(parent)
{
	HashColumnWidgetFunc& hash_cwf = hash_column_widget_func_;

	stColumnWidgetFunc cwf;
	auto insert_func = [&hash_cwf, &cwf](const int& column_index, CreateColumnWidgetFunc create_func, SetColumnModelDataFunc set_func, SetColumnEditDataFunc set_editor_func)
	{
		cwf.create_func_ = create_func;
		cwf.set_model_data_func_ = set_func;
		cwf.set_editor_data_func_ = set_editor_func;

		hash_cwf.insert(column_index, cwf);
	};

	insert_func(CI_00, &AItemDelegate::createColumnWidget00, &AItemDelegate::setColumnModelData00, &AItemDelegate::setColumnEditorData00);
	insert_func(CI_01, &AItemDelegate::createColumnWidget01, &AItemDelegate::setColumnModelData01, &AItemDelegate::setColumnEditorData01);
	insert_func(CI_02, &AItemDelegate::createColumnWidget02, &AItemDelegate::setColumnModelData02, &AItemDelegate::setColumnEditorData02);
	insert_func(CI_03, &AItemDelegate::createColumnWidget03, &AItemDelegate::setColumnModelData03, &AItemDelegate::setColumnEditorData03);
	insert_func(CI_04, &AItemDelegate::createColumnWidget04, &AItemDelegate::setColumnModelData04, &AItemDelegate::setColumnEditorData04);
	insert_func(CI_05, &AItemDelegate::createColumnWidget05, &AItemDelegate::setColumnModelData05, &AItemDelegate::setColumnEditorData05);
	insert_func(CI_06, &AItemDelegate::createColumnWidget06, &AItemDelegate::setColumnModelData06, &AItemDelegate::setColumnEditorData06);
	insert_func(CI_07, &AItemDelegate::createColumnWidget07, &AItemDelegate::setColumnModelData07, &AItemDelegate::setColumnEditorData07);
	insert_func(CI_08, &AItemDelegate::createColumnWidget08, &AItemDelegate::setColumnModelData08, &AItemDelegate::setColumnEditorData08);
	insert_func(CI_09, &AItemDelegate::createColumnWidget09, &AItemDelegate::setColumnModelData09, &AItemDelegate::setColumnEditorData09);
	insert_func(CI_10, &AItemDelegate::createColumnWidget10, &AItemDelegate::setColumnModelData10, &AItemDelegate::setColumnEditorData10);
}

AItemDelegate::~AItemDelegate()
{}


/// --------------------------------------------------------------------------------
/// @brief: ExePackDelegate::createEditor
/// --------------------------------------------------------------------------------
QWidget* AItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//throw std::logic_error("The method or operation is not implemented.");
	{
		const int&& column_index = index.column();
		HashColumnWidgetFunc::const_iterator find_it = hash_column_widget_func_.find(column_index);

		if (find_it != hash_column_widget_func_.end())
		{
			CreateColumnWidgetFunc create_func = find_it.value().create_func_;
			if (create_func)
			{
				return (this->*create_func)(parent);
			}
		}
	}

	return QItemDelegate::createEditor(parent, option, index);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackDelegate::setModelData
/// --------------------------------------------------------------------------------
void AItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	//throw std::logic_error("The method or operation is not implemented.");
	{
		HashColumnWidgetFunc::const_iterator findIt = hash_column_widget_func_.find(index.column());
		if (findIt != hash_column_widget_func_.end())
		{
			SetColumnModelDataFunc set_func = findIt.value().set_model_data_func_;
			if (set_func)
			{
				(this->*set_func)(editor, model, index);
				return;
			}
		}
	}

	QItemDelegate::setModelData(editor, model, index);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackDelegate::setEditorData
/// --------------------------------------------------------------------------------
void AItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	//throw std::logic_error("The method or operation is not implemented.");
	{
		HashColumnWidgetFunc::const_iterator findIt = hash_column_widget_func_.find(index.column());
		if (findIt != hash_column_widget_func_.end())
		{
			SetColumnEditDataFunc set_func = findIt.value().set_editor_data_func_;
			if (set_func)
			{
				(this->*set_func)(editor, index);
				return;
			}
		}
	}

	QItemDelegate::setEditorData(editor, index);
}

/// --------------------------------------------------------------------------------
/// @brief: AItemDelegate::setColumnHash
/// --------------------------------------------------------------------------------
void AItemDelegate::setColumnHash(const int& column_index, CreateColumnWidgetFunc create_func, SetColumnModelDataFunc set_func, SetColumnEditDataFunc set_editor_func)
{
	HashColumnWidgetFunc& hash_cwf = hash_column_widget_func_;

	stColumnWidgetFunc cwf;
	cwf.create_func_ = create_func;
	cwf.set_model_data_func_ = set_func;
	cwf.set_editor_data_func_ = set_editor_func;
	hash_cwf.insert(column_index, cwf);
}

/// --------------------------------------------------------------------------------
/// @brief: AItemDelegate::removeColumn
/// --------------------------------------------------------------------------------
void AItemDelegate::removeColumn(const int& column_index)
{
	hash_column_widget_func_.remove(column_index);
}

/// --------------------------------------------------------------------------------
/// @brief: AItemDelegate::createColumnCombobox
/// --------------------------------------------------------------------------------
QWidget* AItemDelegate::createColumnCombobox(QWidget* parent, const QStringList& list_value) const
{
	QComboBox* pcb = new(std::nothrow)  QComboBox(parent);
	if (pcb)
	{
		for (const auto& str : list_value)
		{
			pcb->addItem(str, str);
		}
	}

	return pcb;
}

/// --------------------------------------------------------------------------------
/// @brief: AItemDelegate::createColumnSpinBox
/// --------------------------------------------------------------------------------
QWidget* AItemDelegate::createColumnSpinBox(QWidget* parent, const int max_value) const
{
	QSpinBox* psb = new(std::nothrow) QSpinBox(parent);
	if (psb)
	{
		psb->setRange(0, max_value);
	}

	return psb;
}
