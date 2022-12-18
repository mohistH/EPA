#include "DevEnvDelegate.h"
#include < QPushButton>
#include <QApplication>
#include <QFileDialog>
#include "FileFilterHelper.h"



DevEnvDelegate::DevEnvDelegate(QObject* parent)
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

	insert_func(1, &DevEnvDelegate::createColumnWidget01, &DevEnvDelegate::setColumnModelData01, &DevEnvDelegate::setColumnEditorData01);

}

DevEnvDelegate::~DevEnvDelegate()
{}

/// --------------------------------------------------------------------------------
/// @brief: VSDelegate::createEditor
/// --------------------------------------------------------------------------------
QWidget* DevEnvDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
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
/// @brief: VSDelegate::setModelData
/// --------------------------------------------------------------------------------
void DevEnvDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
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
/// @brief: VSDelegate::setEditorData
/// --------------------------------------------------------------------------------
void DevEnvDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
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
/// @brief: VSDelegate::createColumnWidget01
/// --------------------------------------------------------------------------------
QWidget* DevEnvDelegate::createColumnWidget01(QWidget* parent) const
{
	/// 创建按钮，用于点击选择，打开弹出对话框
	QPushButton* pbtn = new(std::nothrow)  QPushButton(parent);
	return pbtn;
}

/// --------------------------------------------------------------------------------
/// @brief: VSDelegate::setColumnModelData01
/// --------------------------------------------------------------------------------
void DevEnvDelegate::setColumnModelData01(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QPushButton* pbtn = (QPushButton*)editor;
	const QString&& btn_text = pbtn->text();
	model->setData(index, btn_text);
}

/// --------------------------------------------------------------------------------
/// @brief: VSDelegate::setColumnEditorData01
/// --------------------------------------------------------------------------------
void DevEnvDelegate::setColumnEditorData01(QWidget* editor, const QModelIndex& index) const
{
	const QString&& btn_text = index.model()->data(index).toString();
	QPushButton* pbtn = (QPushButton*)editor;

	const QString&& str_filter = FileFilterHelper::ins().fileSuffix(FT_DEVENV);

	static QString str_last_path = QApplication::applicationDirPath();
	const QString&& open_file_name = QFileDialog::getOpenFileName(editor,
		tr("Please select the devenv.exe path"), str_last_path,
		str_filter);

	QString btn_text_new;
	if (0 == open_file_name.length())
	{
		btn_text_new = btn_text;
	}
	else
	{
		btn_text_new = open_file_name;
		str_last_path = btn_text_new.left(btn_text_new.lastIndexOf('/'));
	}

	pbtn->setText(btn_text_new);

}