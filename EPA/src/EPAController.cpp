#include <QMessageBox>
#include <QProcess>
#include <QApplication>
#include <QFile>

#include <EPAController.h>
#include <FileRecordUtil.h>
#include <ParseSlnHelper.h>
#include <FilePackMgrWidget.h>
#include <DevEnvHelper.h>
#include <SqliteExt.h>
#include <SqliteExt.inl>
#include <VSWhereHelper.h>
#include <VersionHelper.h>
#include <ExePackAssistant.h>

namespace oct_epa
{
	EPAController::EPAController(QObject* parent)
		: QObject(parent)
	{
		main_widget_.reset(new(std::nothrow) ExePackAssistant);
		main_widget_.data()->mgrWidget()->init();

		/// 绑定信号槽
		connectSigSlot();

		/// @brief  解析devenv
		readDevEnv();

		/// 创建文件更新对象
		pversion_updater_.reset(new(std::nothrow) FileVersionUpdater);



		/// 初始化
		// init();
		{
			const QString&& str_file = QApplication::applicationDirPath() + s_file_record_name;
			// qDebug() << "str-file=" << str_file; 
			slotOpenSl3File(str_file);
		}



		initUI();
		loadLanguage();
	}

	EPAController::~EPAController()
	{
		uninit();
	}

	/// --------------------------------------------------------------------------------
	/// @brief: vs版本选择--应用
	/// --------------------------------------------------------------------------------
	void EPAController::slotDevEnvApplyPressed()
	{
		readDevEnv();

		/// 获取Vdproj的list, 将 compiler列清空
		{
			MapDevEnvInfo* pmap_dev = file_record_.devenv();

			ListInstallProjectVersion list_data;
			main_widget_.data()->installProjectModelData(&list_data);
			for (auto& item : list_data)
			{
				/// 如果不在更新的列表中，则设置为空
				MapDevEnvInfo::iterator find_it = pmap_dev->find(item.compiler_);
				if (find_it == pmap_dev->end())
				{
					item.compiler_ = "";
				}
			}

			/// 重新设置UI数据
			main_widget_.data()->setInstallProjectModelData(&list_data);
		}

		setHint(tr("succeed to refresh"));
	}

	/// --------------------------------------------------------------------------------
	/// @brief: exe-dll-应用按钮
	/// --------------------------------------------------------------------------------
	void EPAController::slotExeDllApplyPressed(const ListExeDllVersionInfo& list_data)
	{
		for (const auto& item : list_data)
		{
			const QString& log_prefix = tr("[EXE_DLL] [") + item.project_name_ + "] ";
			{
				/// 1. 修改文件版本号
				const QString str_ret = updateExeDllFileVersion(item);
				setHint(log_prefix + str_ret);
			}

			{
				{
					QFile bf(item.batch_file_);
					if (false == bf.exists())
					{
						setHint(log_prefix + tr("failed to carry batch file, its not exist"));
						continue;
					}
				}

				/// 2. 执行批处理文件
				const QString&& batch_file_root = item.batch_file_.left(item.batch_file_.lastIndexOf('/'));
				runBatchFile(item.batch_file_, batch_file_root);

				setHint(log_prefix + tr("ended to run batch file"));
			}
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 安装包项目点击应用
	/// --------------------------------------------------------------------------------
	void EPAController::slotVdprojApplyPressed(const ListInstallProjectVersion& list_data)
	{
		for (const auto& item : list_data)
		{
			const QString&& log_prefix = item.uniqueName();
			{
				QString file_version_error;
				/// 3 修改文件的版本号
				updateVdprojFileVersion(item, file_version_error);

				if (0 == file_version_error.length())
				{
					file_version_error = tr("succeed to update vdproj version");
				}

				setHint(log_prefix + file_version_error);
			}

			{
				/// 5. 编译解决方案
				QString&& ret_error = generateVdproject(item);
				if (0 == ret_error.length())
				{
					ret_error = tr("succeed to generate sln");
				}

				setHint(log_prefix + ret_error);
			}

			{
				if (0 == item.bat_file_.length())
				{
					setHint(log_prefix + tr("failed to carry batch file , its empty"));
				}
				else
				{
					{
						QFile bf(item.bat_file_);
						if (false == bf.exists())
						{
							setHint(log_prefix + tr("failed to carry batch file, its not exist"));
							continue;
						}
					}

					/// 6. 处理处理脚本
					const QString&& batch_file_root = item.bat_file_.left(item.bat_file_.lastIndexOf('/'));
					runBatchFile(item.bat_file_, batch_file_root);

					setHint(log_prefix + tr("ended to run batch file"));
				}
			}
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 创建文件
	/// --------------------------------------------------------------------------------
	void EPAController::slotCreateSl3File(const QString& new_file)
	{
		/// 建库
		using namespace  oct_sqlite;
		SqliteExt se;

		/// 2. 准备sql语句
		QStringList list_sql;
		list_sql << s_create_sql_table_devenv << s_create_sql_table_exe_dll << s_create_sql_table_vdproj;

		const int& new_file_ret = se.newDBFile(new_file, list_sql);
		if (0 != new_file_ret)
		{
			/// 提示用户
			;
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 打开文件内容
	/// --------------------------------------------------------------------------------
	void EPAController::slotOpenSl3File(const QString& sl3_file)
	{
		file_record_.setDBFile(sl3_file);
		/// 重新读取文件
		init();

		main_widget_.data()->setBtnEnabledClose(true);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 关闭文件
	/// --------------------------------------------------------------------------------
	void EPAController::slotCloseSl3File()
	{
		/// 将UI数据写文件
		uninit();

		/// 1. 将UI数据清空
		FileRecord& fr = file_record_;

		setHint(tr("succeed to close file [%1]").arg(fr.dbFile()));

		/// 2. 内存清空
		fr.clearData();

		/// 配置文件名称清空
		fr.setDBFile("");

		/// 3. 解决方案清空
		ParseSlnHelper::ins().clearData();

		main_widget_.data()->setBtnEnabledClose(false);

		/// 6 设置UI默认状态
		setUIWidgetEnabled();

		/// 标题栏重置为默认值
		const QString&& title_prefix = tr("EPA");
		main_widget_.data()->setWindowTitle(title_prefix);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 解析文件版本号
	/// --------------------------------------------------------------------------------
	void EPAController::slotParseExeDll(const int row_index, const QString& select_file)
	{
		FileVersionUpdater* pfvu = pversion_updater_.data();
		if ((nullptr == pfvu) || (0 > row_index))
		{
			setHint(tr("[EXE_DLL]") + tr(" internal error or row index is negative"));
			return;
		}

		/// 读取文件exe内容
		MapExeDllVersionInfo map_out_file;
		{
			stExeDllVersionInfo evi;
			evi.file_name_ = select_file;
			map_out_file.insert(evi.project_name_, evi);

			int ret = pfvu->Read(map_out_file);
			if (0 != ret)
			{
				setHint(tr("[EXE_DLL]") + tr("please select a file"));
				return;
			}
		}
		/// 修改list_model中对应的数据，再赋值
		ListExeDllVersionInfo list_exe_dll_data;
		main_widget_.data()->exeDllModelData(&list_exe_dll_data);

		/// 赋值

		{
			/// 读取结果
			stExeDllVersionInfo read_item;
			{
				MapExeDllVersionInfo::iterator find_it = map_out_file.begin();
				read_item = find_it.value();
			}
			stExeDllVersionInfo target_item = list_exe_dll_data.at(row_index);

			/// 仅修改界面的版本号
			read_item.project_name_ = target_item.project_name_;
			read_item.batch_file_ = target_item.batch_file_;
			target_item = read_item;

			list_exe_dll_data[row_index] = target_item;
		}

		/// 显示数据
		main_widget_.data()->setExeDllModelData(&list_exe_dll_data);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::init
	/// --------------------------------------------------------------------------------
	int EPAController::init()
	{
		QString str_main_titile;
		int ret_value = 0;

		/// 加载配置文件
		if (0 != readDB(file_record_.dbFile()))
		{
			ret_value = 1;
			setHint(tr("failed to load the file [%1], maybe its incorrect").arg(file_record_.dbFile()));
		}
		else
		{
			/// 解析exe和dll的版本信息
			parseExeDllVersion();

			/// 解析配置文件中，安装包表中的compiler 是否在当前操作系统中devnenv中
			file_record_.processInstallPrjectData();

			/// 解析文件内容
			parseSlnFileInfo();

			/// 下面执行UI数据加载
			setMemDataToUI();

			/// 设置主窗口标题显示
			str_main_titile = "===" + file_record_.dbFile();

			/// 加载成功，设置UI状态
			main_widget_.data()->mgrWidget()->setUIEnableDefault();
			setHint(tr("succeed to load the record file [%1]").arg(file_record_.dbFile()));
		}

		/// 显示窗口标题栏
		const QString&& title_prefix = tr("EPA");
		main_widget_.data()->setWindowTitle(title_prefix + str_main_titile);

		return ret_value;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::uninit
	/// --------------------------------------------------------------------------------
	void EPAController::uninit()
	{
		/// 获取UI数据
		setUIDataToMem();

		/// 写入文件
		writeDB(file_record_.dbFile());
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::loadDB
	/// --------------------------------------------------------------------------------
	int EPAController::readDB(const QString& config_file)
	{
		FileRecordUtil fru;
		int ret = fru.Read(&file_record_, config_file);
		if (0 != ret)
		{
			/// 加载失败，清空db文件
			file_record_.setDBFile("");

			return 1;
		}

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::writeDB
	/// --------------------------------------------------------------------------------
	void EPAController::writeDB(const QString& config_file)
	{
		/// 2. 准备sql语句  << s_create_sql_table_devenv
		QStringList list_sql;
		list_sql << s_create_sql_table_exe_dll << s_create_sql_table_vdproj;
		FileRecordUtil fru;
		int ret = fru.write(&file_record_, config_file, list_sql);
		if (0 != ret)
		{
			;
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::parseSlnFileInfo
	/// --------------------------------------------------------------------------------
	void EPAController::parseSlnFileInfo()
	{
		/// vdproj 解决方案
		MapInstallProjectVersion* pwhich_file = file_record_.vdprojVersion();

		/// 解析 sln文件内容
		for (MapInstallProjectVersion::iterator slnIt = pwhich_file->begin(); slnIt != pwhich_file->end(); ++slnIt)
		{
			ParseSlnHelper::ins().Read(slnIt->sln_name_);
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 信号槽关联
	/// --------------------------------------------------------------------------------
	void EPAController::connectSigSlot()
	{
		FilePackMgrWidget* pmgr_widget = main_widget_.data()->mgrWidget();

		/// VS版本管理应用
		connect(pmgr_widget, &FilePackMgrWidget::sigDevEnvApplyPressed, this, &EPAController::slotDevEnvApplyPressed);
		connect(pmgr_widget, &FilePackMgrWidget::sigExeDllApplyPressed, this, &EPAController::slotExeDllApplyPressed);
		connect(pmgr_widget, &FilePackMgrWidget::sigVdprojApplyPressed, this, &EPAController::slotVdprojApplyPressed);
		connect(pmgr_widget, &FilePackMgrWidget::sigParseExeDll, this, &EPAController::slotParseExeDll);

		connect(main_widget_.data(), &ExePackAssistant::sigCreateFile, this, &EPAController::slotCreateSl3File);
		connect(main_widget_.data(), &ExePackAssistant::sigOpenFile, this, &EPAController::slotOpenSl3File);
		connect(main_widget_.data(), &ExePackAssistant::sigClose, this, &EPAController::slotCloseSl3File);

		connect(this, &EPAController::sigShowHint, pmgr_widget, &FilePackMgrWidget::slotOutputResult);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::initUI
	/// --------------------------------------------------------------------------------
	void EPAController::initUI()
	{
		bool btn_close_enabled = false;
		{
			QFile sl3_file(file_record_.dbFile());
			btn_close_enabled = sl3_file.exists();
		}

		main_widget_.data()->setBtnEnabledClose(btn_close_enabled);


	}

	/// --------------------------------------------------------------------------------
	/// @brief: 设置主程序默认控件的状态
	/// --------------------------------------------------------------------------------
	void EPAController::setUIWidgetEnabled()
	{
		FilePackMgrWidget* fpmw = main_widget_.data()->mgrWidget();

		/// 默认值， 没有打开文件时，界面的控件设置为禁用
		fpmw->clearUIData();
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::readDevEnv
	/// --------------------------------------------------------------------------------
	void EPAController::readDevEnv()
	{
		/// 重新读取
		VSWhereHelper vswh;
		FileRecord& fr = file_record_;
		const int ret = vswh.read(fr.devenv());
		if (0 != ret)
		{
			const QString& vs_where_name = QApplication::applicationDirPath() + s_vswhere_name;
			if (2 == ret)
			{
				setHint(tr("error, the file [%1] was lost, please check").arg(vs_where_name));
			}
			else
			{
				setHint(tr("failed to query VS information, maybe the [%1] was false, please dont change it").arg(vs_where_name));
			}
		}

		DevEnvHelper::ins().set(fr.devenv()->values());
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 初始化后，解析exe和dll的版本号
	/// --------------------------------------------------------------------------------
	void EPAController::parseExeDllVersion()
	{
		MapExeDllVersionInfo* map_exe_dll = file_record_.exeDllVersion();
		if (0 == pversion_updater_.data()->Read(*map_exe_dll))
		{
			/// 读取成功后，
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::loadLanguage
	/// --------------------------------------------------------------------------------
	void EPAController::loadLanguage()
	{
		//const QString& str_qm = ":/lang/lang/zh_CN.qm";
		//this->load(str_qm);

		main_widget_.data()->loadLanguage();
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 将内存中的数据写入UI
	/// --------------------------------------------------------------------------------
	void EPAController::setMemDataToUI()
	{
		FileRecord& fr = file_record_;

		/// 读取 安装包项目的配置文件
		{
			auto&& list_fpi = fr.vdprojVersion()->values();
			main_widget_.data()->setInstallProjectModelData(&list_fpi);
		}

		///读取vs 信息
		{
			auto&& list_data = fr.devenv()->values();
			main_widget_.data()->setDevenvModelData(&list_data);
		}
		/// 读取exe dll信息
		{
			auto&& list_data = fr.exeDllVersion()->values();
			main_widget_.data()->setExeDllModelData(&list_data);
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 获取UI数据
	/// --------------------------------------------------------------------------------
	void EPAController::setUIDataToMem()
	{
		readUIDataDevEnvToMem();
		readUIDataExeDllToMem();
		readUIDataVdprojToMem();
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 读取界面exe和dll表格中的信息
	/// --------------------------------------------------------------------------------
	void EPAController::readUIDataExeDllToMem()
	{
		FileRecord& fr = file_record_;

		MapExeDllVersionInfo* pmap_exe_dll = fr.exeDllVersion();
		{
			MapExeDllVersionInfo tmp_map;
			pmap_exe_dll->swap(tmp_map);
		}
		/// 获取UI数据
		ListExeDllVersionInfo list_data;
		main_widget_.data()->exeDllModelData(&list_data);

		/// 重新赋值
		for (stExeDllVersionInfo& item : list_data)
		{
			pmap_exe_dll->insert(item.project_name_, item);
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 读取 devenv 中的数据
	/// --------------------------------------------------------------------------------
	void EPAController::readUIDataDevEnvToMem()
	{
		FileRecord& fr = file_record_;
		MapDevEnvInfo* pmap_dev = fr.devenv();
		{
			MapDevEnvInfo tmp_map;
			pmap_dev->swap(tmp_map);
		}

		/// 获取UI数据
		ListDevEnv list_data;
		main_widget_.data()->devenvModelData(&list_data);

		/// 重新赋值
		for (stDevEnvInfo& item : list_data)
		{
			pmap_dev->insert(item.name_, item);
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 读取界面中安装包项目的数据
	/// --------------------------------------------------------------------------------
	void EPAController::readUIDataVdprojToMem()
	{
		FileRecord& fr = file_record_;
		MapInstallProjectVersion* map_vdproj = fr.vdprojVersion();
		/// 清空
		{
			MapInstallProjectVersion tmp_map;
			map_vdproj->swap(tmp_map);
		}

		/// 重新赋值
		ListInstallProjectVersion list_data;
		main_widget_.data()->installProjectModelData(&list_data);

		for (stInstallProjectVersion& item : list_data)
		{
			map_vdproj->insert(item.sln_name_, item);
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 更新安装包项目文件对应的版本号
	/// --------------------------------------------------------------------------------
	void EPAController::updateVdprojFileVersion(const stInstallProjectVersion& ipv, QString& out_error)
	{
		FileVersionUpdater* pvu = pversion_updater_.data();

		/// 构建
		MapInstallProjectVersion map_file_write;
		{
			ParseSlnHelper& psh = ParseSlnHelper::ins();

			const stInstallProjectVersion& st_fpv = ipv;

			/// 得到解决方案路径
			const QString& sln_root = st_fpv.toVdprojFileRoot();

			if (0 == sln_root.length())
			{
				out_error = tr("failed to carry, sln name is empty, please check");
				return;
			}

			/// 项目的完整路径
			QString str_vdproj_root;
			{
				HashStrStr hash_vdproj;

				/// 根据解决方案 文件 获取其对应的 项目配置
				if (0 != psh.vdproj(st_fpv.sln_name_, &hash_vdproj))
				{
					return;
				}
				const QString& str_vdproj_file = st_fpv.project_name_;

				/// 根据项目名称获取其对应的完整名称
				str_vdproj_root = hash_vdproj.value(str_vdproj_file);

				if (0 == str_vdproj_root.length())
				{
					return;
				}
			}

			/// 结合项目拼接,构成项目 vdproj 完整路径
			const QString vdproj_file = sln_root + "/" + str_vdproj_root;

			/// 加入map: <项目文件，项目文件属性>
			map_file_write.insert(vdproj_file, st_fpv);
		}

		/// 没有创建更新文件对象, 返回
		if (!pversion_updater_)
		{
			out_error = tr("internal error");
		}
		else
		{
			const int&& ret_val = pversion_updater_->Write(&map_file_write);
			if (0 != ret_val)
			{
				/// 说明某些文件存在更新失败，提示用户
				//QMessageBox::information(nullptr, tr("warning"), tr("some files to update file version failed"));
				out_error = tr("failed to update file version, please check vdproj properties file with visual studio");
			}
		}
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 	生成项目输出
	/// --------------------------------------------------------------------------------
	QString EPAController::generateVdproject(const stInstallProjectVersion& ipv)
	{
		/// 获取命令，
		const QString&& devenv_cmd = devenvCmdToCompilerSln(ipv);
		if (0 == devenv_cmd.length())
		{
			return tr("failed to generate sln, please check the configuration items, maybe its empty");
		}

		const QString&& batch_file_root = ipv.sln_name_.left(ipv.sln_name_.lastIndexOf('/'));

		/// 执行命令, 
		QString err_result;
		runBatchCmd(devenv_cmd, batch_file_root, err_result);

		return err_result;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 运行batch 文件
	/// --------------------------------------------------------------------------------
	int EPAController::runBatchFile(const QString& batch_file, const QString& bat_root)
	{
		if (0 == batch_file.length())
		{
			return 1;
		}

		QProcess process;
		process.setWorkingDirectory(bat_root);
		process.start(batch_file);
		process.waitForStarted();
		process.waitForFinished();

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::setHint
	/// --------------------------------------------------------------------------------
	void EPAController::setHint(const QString& text)
	{
		emit sigShowHint(VersionHelper::dateTime() + " " + text);
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 更新exe dll的文件信息
	/// --------------------------------------------------------------------------------
	QString EPAController::updateExeDllFileVersion(const stExeDllVersionInfo& exe_dll_info)
	{
		QString str_ret = tr("succeeded to update file version");

		/// 1. 构造参数
		MapExeDllVersionInfo map_write;
		map_write.insert(exe_dll_info.project_name_, exe_dll_info);

		/// 2. 写文件
		if (0 != map_write.count())
		{
			if (!pversion_updater_)
			{
				return tr("failed to update filer version, its an internal error");
			}

			FileVersionUpdater* pfvu = pversion_updater_.data();
			if (0 != pfvu->Write(&map_write))
			{
				str_ret = tr("failed to update file version , maybe its not exist");
			}
		}

		return str_ret;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 根据解决方案获取对应的编译指令
	/// --------------------------------------------------------------------------------
	QString EPAController::devenvCmdToCompilerSln(const stInstallProjectVersion& which_sln)
	{
		QString str_ret;

		DevEnvHelper& dev_env_helper = DevEnvHelper::ins();
		ParseSlnHelper& sln_helper = ParseSlnHelper::ins();

		/// 获取VS版本: 需要获取对应的vs路径
		const QString&& dev_env_full_name = dev_env_helper.devenv(which_sln.compiler_);
		if (0 == dev_env_full_name.length())
		{
			return str_ret;
		}
		/// 获取解决方案名称
		if (0 == which_sln.sln_name_.trimmed().length())
		{
			return str_ret;
		}

		/// 获取项目名称
		QString project_name;
		{
			HashStrStr project_hash;
			int ret = sln_helper.vdproj(which_sln.sln_name_, &project_hash);
			if (0 != ret)
			{
				return str_ret;
			}

			project_name = project_hash.value(which_sln.project_name_);

			if (0 == project_name.length())
			{
				return str_ret;
			}
		}

		/// 获取平台
		const QString& platform_name = which_sln.platform_name_;

		/// 获取配置管理器
		const QString config_name = which_sln.configuration_name_;

		str_ret = dev_env_full_name + " " + which_sln.sln_name_ + " " + "/build " + "\"" + config_name + "|" + platform_name + "\" " + project_name;

		return str_ret;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: 执行命令
	/// --------------------------------------------------------------------------------
	int EPAController::runBatchCmd(const QString& batch_cmd, const QString& bat_root, QString& out_error)
	{
		if (0 == batch_cmd.length())
		{
			out_error = tr("batch cmd is invalid, please cheeck configuration items");
			//emit sigShowHint(tr("batch cmd is invalid, please cheeck"));
			//QMessageBox::information(nullptr, tr("title_warning"), tr("batch cmd is invalid, please cheeck"));
			return 1;
		}

#ifdef Q_OS_WIN
		{
			QProcess process(this);
			process.setProgram("cmd.exe");
			QStringList cmd_param_list;
			cmd_param_list << batch_cmd;
			/// 启动
			process.setArguments(cmd_param_list);
			process.setWorkingDirectory(bat_root);

			const int&& cmd_ret = process.execute(batch_cmd);
			//process.start();
			/// If the process cannot be started , -2 is returned. 
			/// If the process crashes, -1 is returned. 
			/// Otherwise, the process' exit code is returned.

			if (-2 == cmd_ret)
			{
				out_error = tr("the process cannot be started");
			}
			else if (-1 == cmd_ret)
			{
				out_error = tr("the process crashes");
			}
			else
			{
				;
			}
		}
#endif /// Q_OS_WIN
		//

		return 0;
	}

	/// --------------------------------------------------------------------------------
	/// @brief: EPAController::mainWidget
	/// --------------------------------------------------------------------------------
	QWidget* EPAController::mainWidget()
	{
		return main_widget_.data();
	}
}
