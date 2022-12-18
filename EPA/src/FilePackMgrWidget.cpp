#include <QHeaderView>
#include <QAbstractItemView>
#include <QDebug>
//#include <QMessageBox>
#include <QProcess>
#include <QPushButton>

#include <FilePackMgrWidget.h>
#include <ui_FilePackMgrWidget.h>
#include <FileRecordUtil.h>
#include <DevEnvHelper.h>
#include <ParseSlnHelper.h>
#include <Typedef.h>
#include <VersionHelper.h>



FilePackMgrWidget::FilePackMgrWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FilePackMgrWidgetClass())
{
	ui->setupUi(this);
	//init();

	connect(this, &FilePackMgrWidget::sigShowHint, this, &FilePackMgrWidget::slotOutputResult);
	ui->splitter->setStretchFactor(0, 5);
	ui->splitter->setStretchFactor(1, 7);

	//loadLanguage();
}

FilePackMgrWidget::~FilePackMgrWidget()
{
	delete ui;
}

/// --------------------------------------------------------------------------------
/// @brief: VS 版本
/// --------------------------------------------------------------------------------
int FilePackMgrWidget::devenvModelData(ListDevEnv* pout_list)
{
	if (nullptr == pout_list)
	{
		return 1;
	}

	*pout_list = devenv_md_.model_.data()->modelData();

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: 获取exe dll Data
/// --------------------------------------------------------------------------------
int FilePackMgrWidget::exeDllModelData(ListExeDllVersionInfo* pout_list)
{
	if (nullptr == pout_list)
	{
		return 1;
	}

	*pout_list = exe_dll_md_.model_.data()->modelData();

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: 安装包
/// --------------------------------------------------------------------------------
int FilePackMgrWidget::installProjectModelData(ListInstallProjectVersion* pout_list)
{
	if (nullptr == pout_list)
	{
		return 1;
	}

	*pout_list = vdproj_md_.model_.data()->modelData();

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: FilePackMgrWidget::setDevenvModelData
/// --------------------------------------------------------------------------------
int FilePackMgrWidget::setDevenvModelData(ListDevEnv* pout_list)
{
	if (nullptr == pout_list)
	{
		return 1;
	}

	devenv_md_.model_.data()->setModelData(*pout_list);

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: FilePackMgrWidget::setExeDllModelData
/// --------------------------------------------------------------------------------
int FilePackMgrWidget::setExeDllModelData(ListExeDllVersionInfo* pout_list)
{
	//throw std::logic_error("The method or operation is not implemented.");
	if (nullptr == pout_list)
	{
		return 1;
	}

	/// 读取exe dll信息
	exe_dll_md_.model_.data()->setModelData(*pout_list);

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: FilePackMgrWidget::setInstallProjectModelData
/// --------------------------------------------------------------------------------
int FilePackMgrWidget::setInstallProjectModelData(ListInstallProjectVersion* pout_list)
{
	if (nullptr == pout_list)
	{
		return 1;
	}

	vdproj_md_.model_.data()->setModelData(*pout_list);

	return 0;

}

/// --------------------------------------------------------------------------------
/// @brief: 
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::setUIEnableDefault()
{
	setEnabled(true);
	devenv_md_.ptvw_->setEnabeldWithData();
	exe_dll_md_.ptvw_->setEnabeldWithData();
	vdproj_md_.ptvw_->setEnabeldWithData();
	ui->widget_ret_output->setUIEnabled();
}


/// --------------------------------------------------------------------------------
/// @brief: FilePackMgrWidget::init
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::init()
{
	/// VS版本
	{
		devenv_md_.ptvw_ = ui->widget_vs_version;
		QTableView* ptv = devenv_md_.ptvw_->tableView();
		BaseInit<DevEnvModel, DevEnvDelegate>(ptv, devenv_md_.model_, devenv_md_.delegate_);
		connectSigSlot(devenv_md_.ptvw_, &FilePackMgrWidget::slotPushBtnInsertVSVersionPressed, &FilePackMgrWidget::slotPushBtnRemoveVSVersionPressed, &FilePackMgrWidget::slotPushBtnApplyVSVersionPressed);

		ptv->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

		devenv_md_.ptvw_->setBtnVisibleCarry(true);
		devenv_md_.ptvw_->setBtnVisibleInsert(false);
		devenv_md_.ptvw_->setBtnVisibleRemove(false);
	}

	/// exe dll
	{
		exe_dll_md_.ptvw_ = ui->widget_exe_dll;
		QTableView* ptv = exe_dll_md_.ptvw_->tableView();
		BaseInit<ExeDllFileModel, ExeDllFileDelegate>(ptv, exe_dll_md_.model_, exe_dll_md_.delegate_);
		connectSigSlot(exe_dll_md_.ptvw_, &FilePackMgrWidget::slotPushBtnInsertExeDllPressed, &FilePackMgrWidget::slotPushBtnRemoveExeDllPressed, &FilePackMgrWidget::slotPushBtnCarryExeDllPressed);

		ptv->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	}

	/// 安装包项目
	{
		vdproj_md_.ptvw_ = ui->widget_vdproj;
		QTableView* ptv = vdproj_md_.ptvw_->tableView();

		BaseInit<InstallProjectModel, InstallProjectDelegate>(ptv, vdproj_md_.model_, vdproj_md_.delegate_);
		//vdproj_md_.model_.reset(new(std::nothrow) InstallProjectModel(ptv));
		//ptv->setModel(vdproj_md_.model_.data());

		//vdproj_md_.delegate_.reset(new(std::nothrow) InstallProjectDelegate(ptv));
		//ptv->setItemDelegate(vdproj_md_.delegate_.data());

		vdproj_md_.delegate_.data()->setModel(vdproj_md_.model_.data(), ptv);

		connectSigSlot(ui->widget_vdproj, &FilePackMgrWidget::slotPushBtnInsertVdprojPressed, &FilePackMgrWidget::slotPushBtnRemoveVdprojPressed, &FilePackMgrWidget::slotPushBtnApplyVdprojPressed);
		//connect(ui->widget_vdproj->tableView(), &QTableView::clicked, this, &FilePackMgrWidget::slotInstallProjectTableViewPressed);

	}

	setEnabled(false);
}

/// --------------------------------------------------------------------------------
/// @brief: FilePackMgrWidget::loadLanguage
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::loadLanguage()
{
	ui->retranslateUi(this);

	//devenv_md_.ptvw_->loadLanguage();
	exe_dll_md_.ptvw_->loadLanguage();
	vdproj_md_.ptvw_->loadLanguage();
	ui->widget_ret_output->loadLanguage();

	vdproj_md_.ptvw_->setBtnNameCarry(tr("carry"));
	exe_dll_md_.ptvw_->setBtnNameCarry(tr("carry"));
	devenv_md_.ptvw_->setBtnNameCarry(tr("refresh"));
}

/// --------------------------------------------------------------------------------
/// @brief:  exe dll 插入
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnInsertExeDllPressed()
{
	stExeDllMD& md = exe_dll_md_;
	this->TableViewRowInsert<ExeDllFileModel, stExeDllVersionInfo>(md.ptvw_->tableView(), md.model_.data());

	/// 插入
	emit sigShowHint(VersionHelper::dateTime() + tr(" [EXE_DLL]") + " " + tr(" succeed to insert"));

	/// 因为时插入，所以，设置删除按钮可用
	md.ptvw_->setBtnEnabledRemove(true);
}

/// --------------------------------------------------------------------------------
/// @brief: exe- 删除
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnRemoveExeDllPressed()
{
	stExeDllMD& md = exe_dll_md_;

	/// 解析执行
	QTableView* ptv = md.ptvw_->tableView();
	ExeDllFileModel* pmodel = md.model_.data();

	/// 1. 获取当前选择了哪些行
	/// 获取的是行是按照界面选择的顺序执行的
	/// 
	/// 使用map是为了避免重复的行
	MapIntInt map_selected_rows;
	if (0 != tableViewSelectedRows(&map_selected_rows, ptv))
	{
		/// 插入
		emit sigShowHint(VersionHelper::dateTime() + tr(" [EXE_DLL]") + " " + tr(" please select rows to delete"));
		return;
	}

	/// 2. 获取界面对应的model数据
	ListExeDllVersionInfo& list_ui_data = pmodel->modelData();
	if (0 == list_ui_data.count())
	{
		return;
	}

	/// 解析删除
	this->TableViewRowRemove<ExeDllFileModel, ListExeDllVersionInfo>(md.model_.data(), &list_ui_data, map_selected_rows.values());



	emit sigShowHint(VersionHelper::dateTime() + tr(" [EXE_DLL]") + " " + tr(" succeed to remove row(s)"));

	const bool&& isEnaed = (0 == md.model_->rowCount() ? false : true);
	md.ptvw_->setBtnEnabledRemove(isEnaed);
}

/// --------------------------------------------------------------------------------
/// @brief: exe--应用
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnCarryExeDllPressed()
{
	ListExeDllVersionInfo select_list;
	{
		/// 构造文件修改器代码
		stExeDllMD& md = exe_dll_md_;

		QTableView* ptv = md.ptvw_->tableView();
		ExeDllFileModel* pmodel = md.model_.data();

		/// 1. 获取点击的行
		/// 使用map是为了避免重复的行
		MapIntInt map_selected_rows;
		if (0 != tableViewSelectedRows(&map_selected_rows, ptv))
		{
			/// 插入
			emit sigShowHint(VersionHelper::dateTime() + tr(" [EXE_DLL]") + " " + tr(" please select row(s) to carry"));
			return;
		}

		/// 2. 获取UI选择了那些行的数据
		/// 遍历得到对应行
		auto&& list_select_row_index = map_selected_rows.values();
		/// 升序
		qSort(list_select_row_index.begin(), list_select_row_index.end());

		for (const auto& select_row_index : list_select_row_index)
		{
			select_list.append(pmodel->modelData().at(select_row_index));
		}
	}

	emit sigExeDllApplyPressed(select_list);

}

/// --------------------------------------------------------------------------------
/// @brief: 安装包项目-插入
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnInsertVdprojPressed()
{
	stInstallProjectMD& md = vdproj_md_;

	/// 解析插入
	this->TableViewRowInsert<InstallProjectModel, stInstallProjectVersion>(md.ptvw_->tableView(), md.model_.data());

	/// 插入
	emit sigShowHint(VersionHelper::dateTime() + tr(" [VDPROJ]") + " " + tr(" succeed to insert"));

	/// 因为时插入，所以，设置删除按钮可用
	md.ptvw_->setBtnEnabledRemove(true);
}

/// --------------------------------------------------------------------------------
/// @brief: 安装包项目-删除
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnRemoveVdprojPressed()
{
	stInstallProjectMD& md = vdproj_md_;
	/// 解析执行
	QTableView* ptv = md.ptvw_->tableView();
	InstallProjectModel* pmodel = md.model_.data();

	/// 1. 获取当前选择了哪些行
	/// 获取的是行是按照界面选择的顺序执行的
	/// 
	/// 使用map是为了避免重复的行
	MapIntInt map_selected_rows;
	if (0 != tableViewSelectedRows(&map_selected_rows, ptv))
	{
		/// 插入
		emit sigShowHint(VersionHelper::dateTime() + tr(" [VDPROJ]") + " " + tr(" please select rows to delete"));
		return;
	}

	/// 2. 获取界面对应的model数据
	ListInstallProjectVersion& list_ui_data = pmodel->modelData();
	if (0 == list_ui_data.count())
	{
		return;
	}

	/// 解析删除
	this->TableViewRowRemove<InstallProjectModel, ListInstallProjectVersion>(md.model_.data(), &list_ui_data, map_selected_rows.values());

	/// 插入
	emit sigShowHint(VersionHelper::dateTime() + tr(" [VDPROJ]") + " " + tr(" succeed to remove row(s)"));

	const bool&& isEnaed = (0 == md.model_->rowCount() ? false : true);
	md.ptvw_->setBtnEnabledRemove(isEnaed);
}

/// --------------------------------------------------------------------------------
/// @brief: 安装包项目--执行
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnApplyVdprojPressed()
{
	QPushButton* pbtn = (QPushButton*)(sender());

	/// 3. 得到当前选择的行，
	ListInstallProjectVersion select_list;
	{
		stInstallProjectMD& md = vdproj_md_;

		QPushButton* pbtn = (QPushButton*)(sender());
		pbtn->setEnabled(false);


		/// 解析执行
		/// ---------------------------------------------------------------------------------------
		QTableView* ptv = md.ptvw_->tableView();
		InstallProjectModel* pmodel = md.model_.data();

		/// 1. 获取当前选择了哪些行
		/// 获取的是行是按照界面选择的顺序执行的

		/// 使用map是为了避免重复的行
		MapIntInt map_selected_rows;
		if (0 != tableViewSelectedRows(&map_selected_rows, ptv))
		{
			/// 
			emit sigShowHint(VersionHelper::dateTime() + tr(" [VDPROJ]") + " " + tr(" please select row(s) to carry"));
			pbtn->setEnabled(true);
			return;
		}

		/// 2. 获取界面对应的model数据
		const ListInstallProjectVersion& list_ui_data = pmodel->modelData();
		if (0 == list_ui_data.count())
		{
			pbtn->setEnabled(true);
			return;
		}

		/// 遍历得到对应行
		auto&& list_select_row_index = map_selected_rows.values();
		/// 升序
		qSort(list_select_row_index.begin(), list_select_row_index.end());

		for (const auto& select_row_index : list_select_row_index)
		{
			select_list.append(list_ui_data.at(select_row_index));
		}
	}

	/// 5. 抛出信号
	emit sigVdprojApplyPressed(select_list);

	pbtn->setEnabled(true);
}

/// --------------------------------------------------------------------------------
/// @brief: VS版本-插入
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnInsertVSVersionPressed()
{
	stVSModelDelegate& md = devenv_md_;
	this->TableViewRowInsert<DevEnvModel, stDevEnvInfo>(md.ptvw_->tableView(), md.model_.data());
	/// 因为时插入，所以，设置删除按钮可用
	md.ptvw_->setBtnEnabledRemove(true);
}

/// --------------------------------------------------------------------------------
/// @brief: VS版本-删除
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnRemoveVSVersionPressed()
{
	stVSModelDelegate& md = devenv_md_;
	this->TableViewRowRemove<DevEnvModel>(md.ptvw_->tableView(), md.model_.data());
	const bool&& isEnaed = (0 == md.model_->rowCount() ? false : true);
	md.ptvw_->setBtnEnabledRemove(isEnaed);
}

/// --------------------------------------------------------------------------------
/// @brief: vs 版本选择 -- 应用
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotPushBtnApplyVSVersionPressed()
{
	emit sigDevEnvApplyPressed();
}


/// --------------------------------------------------------------------------------
/// @brief: FilePackMgrWidget::tableViewSelectedRows
/// --------------------------------------------------------------------------------
int FilePackMgrWidget::tableViewSelectedRows(MapIntInt* pout_map, QTableView* ptv)
{
	if ((nullptr == pout_map) || (nullptr == ptv))
	{
		return 1;
	}

	const QModelIndexList& selected_row = ptv->selectionModel()->selectedRows();
	for (auto& item : selected_row)
	{
		/// 行号
		const int&& row_index = item.row();
		qDebug() << "row=" << row_index;

		pout_map->insert(row_index, row_index);
	}

	/// 如果没有选择行 而点击界面的执行按钮， 则提示
	if (0 == pout_map->count())
	{
		/// todo - 提示用户
		return 2;
	}

	for (auto& item : *pout_map)
	{
		qDebug() << "map_row=" << item;
	}

	return 0;
}


void FilePackMgrWidget::clearUIData()
{
	using namespace oct_epa;
	/// 读取 安装包项目的配置文件
	{
		ListInstallProjectVersion list_data;
		setInstallProjectModelData(&list_data);
	}

	/////读取vs 信息
	//{
	//	ListDevEnv list_data;
	//	setDevenvModelData(&list_data);
	//}
	/// 读取exe dll信息
	{
		ListExeDllVersionInfo list_data;
		setExeDllModelData(&list_data);
	}

	setEnabled(false);
}

/// --------------------------------------------------------------------------------
/// @brief: 显示结果
/// --------------------------------------------------------------------------------
void FilePackMgrWidget::slotOutputResult(const QString& text)
{
	ui->widget_ret_output->append(text);
}

