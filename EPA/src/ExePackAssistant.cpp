#include "ExePackAssistant.h"
#include "ui_ExePackAssistant.h"
#include <QFileDialog>
#include <QDebug>




ExePackAssistant::ExePackAssistant(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::ExePackAssistantClass())
{
	ui->setupUi(this);
	connectSigSlot();
	last_create_file_path_ = QApplication::applicationDirPath();
	last_open_file_root_ = QApplication::applicationDirPath();
	//loadLanguage();
}

ExePackAssistant::~ExePackAssistant()
{
	delete ui;
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::setBtnEnabledOpen
/// --------------------------------------------------------------------------------
void ExePackAssistant::setBtnEnabledOpen(const bool& is_enabled)
{
	ui->push_btn_open->setEnabled(is_enabled);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::setBtnEnabledClose
/// --------------------------------------------------------------------------------
void ExePackAssistant::setBtnEnabledClose(const bool& is_enabled)
{
	ui->push_btn_close->setEnabled(is_enabled);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::setBtnEnabledCreate
/// --------------------------------------------------------------------------------
void ExePackAssistant::setBtnEnabledCreate(const bool& is_enabled)
{
	ui->push_btn_create->setEnabled(is_enabled);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::devenvModelData
/// --------------------------------------------------------------------------------
int ExePackAssistant::devenvModelData(ListDevEnv* pout_list)
{
	//throw std::logic_error("The method or operation is not implemented.");
	return ui->file_pacj_mgr_widget->devenvModelData(pout_list);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::exeDllModelData
/// --------------------------------------------------------------------------------
int ExePackAssistant::exeDllModelData(ListExeDllVersionInfo* pout_list)
{
	return ui->file_pacj_mgr_widget->exeDllModelData(pout_list);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::installProjectModelData
/// --------------------------------------------------------------------------------
int ExePackAssistant::installProjectModelData(ListInstallProjectVersion* pout_list)
{
	return ui->file_pacj_mgr_widget->installProjectModelData(pout_list);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::setDevenvModelData
/// --------------------------------------------------------------------------------
int ExePackAssistant::setDevenvModelData(ListDevEnv* pout_list)
{
	return ui->file_pacj_mgr_widget->setDevenvModelData(pout_list);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::setExeDllModelData
/// --------------------------------------------------------------------------------
int ExePackAssistant::setExeDllModelData(ListExeDllVersionInfo* pout_list)
{
	return ui->file_pacj_mgr_widget->setExeDllModelData(pout_list);
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackAssistant::setInstallProjectModelData
/// --------------------------------------------------------------------------------
int ExePackAssistant::setInstallProjectModelData(ListInstallProjectVersion* pout_list)
{
	return ui->file_pacj_mgr_widget->setInstallProjectModelData(pout_list);
}

/// --------------------------------------------------------------------------------
/// @brief: ??????file_mgr??????
/// --------------------------------------------------------------------------------
FilePackMgrWidget* ExePackAssistant::mgrWidget()
{
	return ui->file_pacj_mgr_widget;
}

/// --------------------------------------------------------------------------------
/// @brief: 	?????? ?????????
/// --------------------------------------------------------------------------------
void ExePackAssistant::connectSigSlot()
{
	/// ????????????
	connect(ui->push_btn_create, &QPushButton::pressed, this, &ExePackAssistant::slotBtnCreatePressed);
	connect(ui->push_btn_open, &QPushButton::pressed, this, &ExePackAssistant::slotBtnOpenPressed);
	connect(ui->push_btn_close, &QPushButton::pressed, this, &ExePackAssistant::sigClose);
}

/// --------------------------------------------------------------------------------
/// @brief: ????????????
/// --------------------------------------------------------------------------------
void ExePackAssistant::loadLanguage()
{
	ui->retranslateUi(this);
	ui->file_pacj_mgr_widget->loadLanguage();
}

/// --------------------------------------------------------------------------------
/// @brief: ????????????
/// --------------------------------------------------------------------------------
void ExePackAssistant::slotBtnCreatePressed()
{
	/// ????????????
	const QString& new_file = QFileDialog::getSaveFileName(this,
		tr("please input the file name"),
		last_create_file_path_,
		"sl3(*.sl3)"
	);

	if (0 == new_file.length())
	{
		/// ?????????????????????????????????
		return;
	}

	emit sigCreateFile(new_file);

	last_create_file_path_ = new_file.left(new_file.lastIndexOf('/'));

}

/// --------------------------------------------------------------------------------
/// @brief: ??????????????????
/// --------------------------------------------------------------------------------
void ExePackAssistant::slotBtnOpenPressed()
{
	/// ????????????
	const QString& new_file = QFileDialog::getOpenFileName(this,
		tr("please select the file of .sl3"),
		last_open_file_root_,
		"sl3(*.sl3)"
	);

	if (0 == new_file.length())
	{
		/// ?????????????????????????????????
		return;
	}

	emit sigOpenFile(new_file);

	last_open_file_root_ = new_file.left(new_file.lastIndexOf('/'));
}
