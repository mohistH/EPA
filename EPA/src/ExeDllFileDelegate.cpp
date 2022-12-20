#include "ExeDllFileDelegate.h"
#include <QPushButton>
#include "VersionHelper.h"
#include <QComboBox>
#include <QSpinBox>
#include <QApplication>
#include "FileFilterHelper.h"
#include <QFileDialog>


ExeDllFileDelegate::ExeDllFileDelegate(QObject* parent)
	: AItemDelegate(parent)
{
	/// 绑定列
	removeColumn(AItemDelegate::CI_00);

	removeColumn(AItemDelegate::CI_09);
}

ExeDllFileDelegate::~ExeDllFileDelegate()
{}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::createColumnWidget00
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget00(QWidget* parent) const
{
	return nullptr;
}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::createColumnWidget01
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget01(QWidget* parent) const
{
	QPushButton* pbtn = new(std::nothrow) QPushButton(parent);

	return pbtn;
}

/// --------------------------------------------------------------------------------
/// @brief: 主版本号
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget02(QWidget* parent) const
{
	return createColumnCombobox(parent, VersionHelper::majorVersion());
}

/// --------------------------------------------------------------------------------
/// @brief: 小版本号
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget03(QWidget* parent) const
{
	return createColumnSpinBox(parent, 99);
}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::createColumnWidget04
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget04(QWidget* parent) const
{
	return createColumnSpinBox(parent, 255);
}

/// --------------------------------------------------------------------------------
/// @brief: 文件修订版本号
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget05(QWidget* parent) const
{
	return createColumnSpinBox(parent, 100 * 10000);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget06(QWidget* parent) const
{
	return createColumnCombobox(parent, VersionHelper::majorVersion());
}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::createColumnWidget07
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget07(QWidget* parent) const
{
	return createColumnSpinBox(parent, 99);
}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::createColumnWidget08
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget08(QWidget* parent) const
{
	return createColumnSpinBox(parent, 255);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品修订版本号
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget09(QWidget* parent) const
{
	return nullptr;
}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::createColumnWidget10
/// --------------------------------------------------------------------------------
QWidget* ExeDllFileDelegate::createColumnWidget10(QWidget* parent) const
{
	QPushButton* pbtn = new(std::nothrow) QPushButton(parent);

	return pbtn;
}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::setColumnModelData00
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData00(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	;
}

/// --------------------------------------------------------------------------------
/// @brief: 文件名
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData01(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QPushButton* pbtn = (QPushButton*)editor;
	const QString&& btn_text = pbtn->text();
	model->setData(index, btn_text);
}

/// --------------------------------------------------------------------------------
/// @brief: 文件版本-主
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData02(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* pcb = (QComboBox*)editor;
	/// 获取选择
	const QVariant&& value = pcb->itemData(pcb->currentIndex());
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: 文件版本-小
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData03(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* pcb = (QSpinBox*)editor;
	/// 获取选择
	const QVariant&& value = pcb->value();
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: 文件版本-内部
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData04(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* pcb = (QSpinBox*)editor;
	/// 获取选择
	const QVariant&& value = pcb->value();
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: 文件版本-修订
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData05(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* psb = (QSpinBox*)editor;
	const int&& sb_value = psb->value();
	model->setData(index, sb_value);

}

/// --------------------------------------------------------------------------------
/// @brief: 产品版本-主
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData06(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* pcb = (QComboBox*)editor;
	/// 获取选择
	const QVariant&& value = pcb->itemData(pcb->currentIndex());
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品版本-小
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData07(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* pcb = (QSpinBox*)editor;
	/// 获取选择
	const QVariant&& value = pcb->value();
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品版本-内
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData08(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* pcb = (QSpinBox*)editor;
	/// 获取选择
	const QVariant&& value = pcb->value();
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品版本-修
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData09(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	;
}

/// --------------------------------------------------------------------------------
/// @brief: 批处理文件
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnModelData10(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QPushButton* psb = (QPushButton*)editor;
	const QString&& sb_value = psb->text();
	model->setData(index, sb_value);
}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::setColumnEditorData00
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData00(QWidget* editor, const QModelIndex& index) const
{
	;
}

/// --------------------------------------------------------------------------------
/// @brief: ExeDllFileDelegate::setColumnEditorData01
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData01(QWidget* editor, const QModelIndex& index) const
{

	const QString&& btn_text = index.model()->data(index).toString();
	QPushButton* pbtn = (QPushButton*)editor;
	QString btn_new_name;
	{
		static QString last_dir = QApplication::applicationDirPath();

		const QString&& str_filter = FileFilterHelper::ins().fileSuffix(FT_EXE_DLL);

		const QString&& select_file = QFileDialog::getOpenFileName(
			editor,
			tr("please select the exe or dll file"),
			last_dir,
			str_filter
		);

		if (0 < select_file.trimmed().length())
		{
			btn_new_name = select_file;
			last_dir = select_file.left(select_file.lastIndexOf("/"));
			emit sigParseExeDll(index.row(), btn_new_name);
		}
		else
		{
			btn_new_name = btn_text;
		}
	}

	pbtn->setText(btn_new_name);
}

/// --------------------------------------------------------------------------------
/// @brief: 主版本
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData02(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QComboBox* psb = (QComboBox*)editor;
	const int data_index = psb->findData(ver_value);
	psb->setCurrentIndex(data_index);

}

/// --------------------------------------------------------------------------------
/// @brief: 小
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData03(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QSpinBox* psb = (QSpinBox*)editor;
	psb->setValue(ver_value);
}

/// --------------------------------------------------------------------------------
/// @brief: 内部
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData04(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QSpinBox* psb = (QSpinBox*)editor;
	psb->setValue(ver_value);
}

/// --------------------------------------------------------------------------------
/// @brief: 文件-修订
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData05(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QSpinBox* psb = (QSpinBox*)editor;
	psb->setValue(ver_value);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品-主
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData06(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QComboBox* psb = (QComboBox*)editor;
	const int data_index = psb->findData(ver_value);
	psb->setCurrentIndex(data_index);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品-小
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData07(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QSpinBox* psb = (QSpinBox*)editor;
	psb->setValue(ver_value);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品=内部
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData08(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QSpinBox* psb = (QSpinBox*)editor;
	psb->setValue(ver_value);
}

/// --------------------------------------------------------------------------------
/// @brief: 产品-修订
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData09(QWidget* editor, const QModelIndex& index) const
{
	;
}

/// --------------------------------------------------------------------------------
/// @brief: 批处理文件
/// --------------------------------------------------------------------------------
void ExeDllFileDelegate::setColumnEditorData10(QWidget* editor, const QModelIndex& index) const
{
	const QString&& btn_text = index.model()->data(index).toString();
	QPushButton* pbtn = (QPushButton*)editor;
	QString btn_new_name;
	{
		static QString last_dir = QApplication::applicationDirPath();

		const QString&& str_filter = FileFilterHelper::ins().fileSuffix(FT_BAT);

		const QString&& select_file = QFileDialog::getOpenFileName(
			editor,
			tr("please select the batch file"),
			last_dir,
			str_filter
		);

		if (0 < select_file.trimmed().length())
		{
			last_dir = select_file.left(last_dir.lastIndexOf('/'));
		}

		btn_new_name = select_file;
	}

	pbtn->setText(btn_new_name);
}
