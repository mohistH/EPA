#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QFileDialog>
#include <QApplication>
#include <QTableView>

#include "InstallProjectDelegate.h"
#include "versionHelper.h"
#include "DevEnvHelper.h"
#include "FileFilterHelper.h"
#include "ParseSlnHelper.h"
#include "InstallProjectModel.h"



InstallProjectDelegate::InstallProjectDelegate(QObject* parent)
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

	insert_func(0, &InstallProjectDelegate::createColumnWidget00, &InstallProjectDelegate::setColumnModelData00, &InstallProjectDelegate::setColumnEditorData00);
	insert_func(1, &InstallProjectDelegate::createColumnWidget01, &InstallProjectDelegate::setColumnModelData01, &InstallProjectDelegate::setColumnEditorData01);
	insert_func(2, &InstallProjectDelegate::createColumnWidget02, &InstallProjectDelegate::setColumnModelData02, &InstallProjectDelegate::setColumnEditorData02);
	insert_func(3, &InstallProjectDelegate::createColumnWidget03, &InstallProjectDelegate::setColumnModelData03, &InstallProjectDelegate::setColumnEditorData03);
	insert_func(4, &InstallProjectDelegate::createColumnWidget04, &InstallProjectDelegate::setColumnModelData04, &InstallProjectDelegate::setColumnEditorData04);
	insert_func(5, &InstallProjectDelegate::createColumnWidget05, &InstallProjectDelegate::setColumnModelData05, &InstallProjectDelegate::setColumnEditorData05);
	insert_func(6, &InstallProjectDelegate::createColumnWidget06, &InstallProjectDelegate::setColumnModelData06, &InstallProjectDelegate::setColumnEditorData06);
	insert_func(7, &InstallProjectDelegate::createColumnWidget07, &InstallProjectDelegate::setColumnModelData07, &InstallProjectDelegate::setColumnEditorData07);
	//insert_func(8, &VDprojDelegate::createColumnWidget08, &VDprojDelegate::setColumnModelData08, &VDprojDelegate::setColumnEditorData08);
	insert_func(8, &InstallProjectDelegate::createColumnWidget09, &InstallProjectDelegate::setColumnModelData09, &InstallProjectDelegate::setColumnEditorData09);
}

InstallProjectDelegate::~InstallProjectDelegate()
{}

/// --------------------------------------------------------------------------------
/// @brief: ExePackDelegate::createEditor
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
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
void InstallProjectDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
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
/// @brief: ??????data??????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setModel(InstallProjectModel* pmodel, QTableView* ptv)
{
	pmodel_data_ = pmodel;
	ptv_ = ptv;
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackDelegate::setEditorData
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
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
/// @brief: vs????????????
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnWidget00(QWidget* parent) const
{
	QComboBox* pcb = new(std::nothrow)  QComboBox(parent);
	if (pcb)
	{
		DevEnvHelper& deh = DevEnvHelper::ins();
		QStringList vs_name_list = deh.vsName();
		MapStrStr& map_devenv = deh.devenv();
		/// ??????
		qSort(vs_name_list.begin(), vs_name_list.end());

		for (const QString& compiler_name : vs_name_list)
		{
			pcb->addItem(compiler_name, compiler_name);// map_devenv.value(compiler_name));
		}

		if (0 < pcb->count())
		{
			pcb->setCurrentIndex(-1);
		}
	}

	return pcb;
}

/// --------------------------------------------------------------------------------
/// @brief: ????????????
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnWidget01(QWidget* parent) const
{
	/// ?????????????????????????????????????????????????????????
	QPushButton* pbtn = new(std::nothrow)  QPushButton(parent);
	if (pbtn)
	{
		pbtn->setText(tr("select file"));
	}

	return pbtn;
}

/// --------------------------------------------------------------------------------
/// @brief: ????????????
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnWidget02(QWidget* parent) const
{
	QComboBox* pcb = new(std::nothrow) QComboBox(parent);
	if (pcb && pmodel_data_ && ptv_)
	{
		const ListInstallProjectVersion& list_data = pmodel_data_->modelData();
		const int current_row = ptv_->currentIndex().row();
		if (0 <= current_row)
		{
			/// ???????????????sln??????
			const QString& sln_file = list_data.at(current_row).sln_name_;

			/// <????????????????????????????????????>
			HashStrStr hash_proj;
			ParseSlnHelper::ins().vdproj(sln_file, &hash_proj);

			/// ?????????????????????, ??????????????????
			for (HashStrStr::iterator findIt = hash_proj.begin(); findIt != hash_proj.end(); ++findIt)
			{
				pcb->addItem(findIt.key(), findIt.key());
			}
		}
	}
	return pcb;
}

/// --------------------------------------------------------------------------------
/// @brief: ??????
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnWidget03(QWidget* parent) const
{
	QComboBox* pcb = new(std::nothrow) QComboBox(parent);
	if (pcb && pmodel_data_ && ptv_)
	{
		const ListInstallProjectVersion& list_data = pmodel_data_->modelData();
		const int current_row = ptv_->currentIndex().row();
		if (0 <= current_row)
		{
			/// ???????????????sln??????
			const QString& sln_file = list_data.at(current_row).sln_name_;

			/// <????????????????????????????????????>
			HashStrStr hash_proj;
			/// ?????????????????????????????????????????????????????????????????????????????????
			ParseSlnHelper::ins().platform(sln_file, &hash_proj);
			/// ?????????????????????, ??????????????????
			for (HashStrStr::iterator findIt = hash_proj.begin(); findIt != hash_proj.end(); ++findIt)
			{
				pcb->addItem(findIt.key(), findIt.key());
			}
		}
	}
	return pcb;
}

/// --------------------------------------------------------------------------------
/// @brief: ???????????????
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnWidget04(QWidget* parent) const
{
	QComboBox* pcb = new(std::nothrow) QComboBox(parent);
	if (pcb && pmodel_data_ && ptv_)
	{
		const ListInstallProjectVersion& list_data = pmodel_data_->modelData();
		const int&& current_row = ptv_->currentIndex().row();
		if (0 <= current_row)
		{
			/// ???????????????sln??????
			const QString& sln_file = list_data.at(current_row).sln_name_;

			/// <????????????????????????????????????>
			HashStrStr hash_proj;
			/// ?????????????????????????????????????????????????????????????????????????????????
			ParseSlnHelper::ins().configuration(sln_file, &hash_proj);
			/// ?????????????????????, ??????????????????
			for (HashStrStr::iterator findIt = hash_proj.begin(); findIt != hash_proj.end(); ++findIt)
			{
				pcb->addItem(findIt.key(), findIt.key());
			}
		}
	}
	return pcb;
}

/// --------------------------------------------------------------------------------
/// @brief: ?????????
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnWidget05(QWidget* parent) const
{
	return createColumnCombobox(parent, VersionHelper::majorVersion());
}

/// --------------------------------------------------------------------------------
/// @brief: ?????????
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnWidget06(QWidget* parent) const
{
	return createColumnSpinBox(parent, 99);
}

/// ????????????
QWidget* InstallProjectDelegate::createColumnWidget07(QWidget* parent) const
{
	return createColumnSpinBox(parent, 255);
}

/// ????????????
QWidget* InstallProjectDelegate::createColumnWidget08(QWidget* parent) const
{
	return createColumnSpinBox(parent, 100 * 10000);
}
/// ???????????????
QWidget* InstallProjectDelegate::createColumnWidget09(QWidget* parent) const
{
	QPushButton* pbtn = new(std::nothrow)  QPushButton(parent);
	if (pbtn)
	{
		pbtn->setText(tr("select a bat file"));
	}

	return pbtn;
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackDelegate::createColumnCombobox
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnCombobox(QWidget* parent, const QStringList& list_value) const
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
/// @brief: ExePackDelegate::createColumnSpinBox
/// --------------------------------------------------------------------------------
QWidget* InstallProjectDelegate::createColumnSpinBox(QWidget* parent, const int max_value) const
{
	QSpinBox* psb = new(std::nothrow) QSpinBox(parent);
	if (psb)
	{
		psb->setRange(0, max_value);
	}

	return psb;
}

/// --------------------------------------------------------------------------------
/// @brief: vs??????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnModelData00(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* pcb = (QComboBox*)editor;

	/// ????????????
	const QVariant&& value = pcb->itemData(pcb->currentIndex());
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: ??????????????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnModelData01(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QPushButton* pbtn = (QPushButton*)editor;
	const QString&& btn_text = pbtn->text();
	model->setData(index, btn_text);
}

/// --------------------------------------------------------------------------------
/// @brief: ????????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnModelData02(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* pcb = (QComboBox*)editor;
	/// ????????????
	const QVariant&& value = pcb->itemData(pcb->currentIndex());
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: ??????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnModelData03(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* pcb = (QComboBox*)editor;

	/// ????????????
	const QVariant&& value = pcb->itemData(pcb->currentIndex());
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief: ???????????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnModelData04(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* pcb = (QComboBox*)editor;

	/// ????????????
	const QVariant&& value = pcb->itemData(pcb->currentIndex());
	model->setData(index, value);
}

/// --------------------------------------------------------------------------------
/// @brief:?????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnModelData05(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* psb = (QComboBox*)editor;
	model->setData(index, psb->currentText().toInt());
}

/// --------------------------------------------------------------------------------
/// @brief: ?????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnModelData06(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* psb = (QSpinBox*)editor;
	model->setData(index, psb->text());
}


/// ????????????
void InstallProjectDelegate::setColumnModelData07(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* psb = (QSpinBox*)editor;
	model->setData(index, psb->text());
}

/// ????????????
void InstallProjectDelegate::setColumnModelData08(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* psb = (QSpinBox*)editor;
	model->setData(index, psb->text());
}

/// ???????????????
void InstallProjectDelegate::setColumnModelData09(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QPushButton* pbtn = (QPushButton*)editor;
	model->setData(index, pbtn->text());
}














/// --------------------------------------------------------------------------------
/// @brief: vs??????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnEditorData00(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QComboBox* psb = (QComboBox*)editor;
	psb->setCurrentIndex(psb->findData(ver_value));
}

/// --------------------------------------------------------------------------------
/// @brief: ??????????????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnEditorData01(QWidget* editor, const QModelIndex& index) const
{

	const QString&& btn_text = index.model()->data(index).toString();
	QPushButton* pbtn = (QPushButton*)editor;
	QString btn_new_name;
	{
		static QString last_dir = QApplication::applicationDirPath();

		const QString&& str_filter = FileFilterHelper::ins().fileSuffix(FT_SLN);

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

			/// ????????????
			ParseSlnHelper::ins().Read(btn_new_name);
		}
		else
		{
			btn_new_name = btn_text;
		}
	}

	pbtn->setText(btn_new_name);

}

/// --------------------------------------------------------------------------------
/// @brief: ????????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnEditorData02(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QComboBox* psb = (QComboBox*)editor;
	psb->setCurrentIndex(psb->findData(ver_value));
}

/// --------------------------------------------------------------------------------
/// @brief: ??????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnEditorData03(QWidget* editor, const QModelIndex& index) const
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
/// ???????????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnEditorData04(QWidget* editor, const QModelIndex& index) const
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
/// @brief: ?????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnEditorData05(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QComboBox* psb = (QComboBox*)editor;
	psb->setCurrentIndex(psb->findData(ver_value));
}

/// --------------------------------------------------------------------------------
/// @brief: ?????????
/// --------------------------------------------------------------------------------
void InstallProjectDelegate::setColumnEditorData06(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QSpinBox* psb = (QSpinBox*)editor;
	psb->setValue(ver_value);
}
/// ????????????
void InstallProjectDelegate::setColumnEditorData07(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QSpinBox* psb = (QSpinBox*)editor;
	psb->setValue(ver_value);
}

/// ????????????
void InstallProjectDelegate::setColumnEditorData08(QWidget* editor, const QModelIndex& index) const
{
	if (nullptr == editor)
	{
		return;
	}

	const int&& ver_value = index.model()->data(index).toInt();
	QSpinBox* psb = (QSpinBox*)editor;
	psb->setValue(ver_value);
}

/// ???????????????
void InstallProjectDelegate::setColumnEditorData09(QWidget* editor, const QModelIndex& index) const
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

