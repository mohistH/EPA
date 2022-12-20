#pragma once

#include <QObject>
#include <QScopedPointer>
#include <FileRecord.h>
#include <FileVersionUpdater.h>
#include <LanguageHelper.h>

class ExePackAssistant;
namespace oct_epa
{
	/// ----------------------------------------------------------------------------------------
	/// @brief: controller
	/// ----------------------------------------------------------------------------------------
	class EPAController : public QObject, public LanguageHelper
	{
		Q_OBJECT

	signals:
		/// 显示提示
		void sigShowHint(const QString& text);
	public:
		EPAController(QObject* parent = nullptr);
		~EPAController();

		QWidget* mainWidget();

		void loadLanguage();

	private slots:
		/// -------------------------------------------------------------------------------
		/// @brief:		vs版本选择
		/// @param: 	const ListDevEnv & list_data - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void slotDevEnvApplyPressed();


		/// -------------------------------------------------------------------------------
		/// @brief:		exe-dll-应用按钮
		/// @param: 	const MapExeDllVersionInfo & which_map - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void slotExeDllApplyPressed(const ListExeDllVersionInfo& which_map);

		/// -------------------------------------------------------------------------------
		/// @brief:		安装包项目点击应用
		/// @param: 	const ListInstallProjectVersion & list_data - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void slotVdprojApplyPressed(const ListInstallProjectVersion& list_data);


		/// -------------------------------------------------------------------------------
		/// @brief:		创建文件
		/// @param: 	const QString & & new_file - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void slotCreateSl3File(const QString& new_file);

		/// -------------------------------------------------------------------------------
		/// @brief:		打开文件内容
		/// @param: 	const QString & sl3_file - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void slotOpenSl3File(const QString& sl3_file);

		/// -------------------------------------------------------------------------------
		/// @brief:		关闭文件
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void slotCloseSl3File();

		/// -------------------------------------------------------------------------------
		/// @brief:		解析文件版本号
		/// @param: 	const int row_index - 
		/// @param: 	const QString & select_file - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void slotParseExeDll(const int row_index, const QString& select_file);

	private:
		int init();
		void uninit();
		int readDB(const QString& config_file);
		void writeDB(const QString& config_file);
		void parseSlnFileInfo();
		void connectSigSlot();
		void initUI();
		void setUIWidgetEnabled();
		void readDevEnv();
		void parseExeDllVersion();


	private:
		/// -------------------------------------------------------------------------------
		/// @brief:		将内存中的数据写入UI
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void setMemDataToUI();

		/// -------------------------------------------------------------------------------
		/// @brief:		获取UI数据
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void setUIDataToMem();

		/// -------------------------------------------------------------------------------
		/// @brief:		读取界面exe和dll表格中的信息
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void readUIDataExeDllToMem();

		/// -------------------------------------------------------------------------------
		/// @brief:		读取 exedll中的数据
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void readUIDataDevEnvToMem();

		/// -------------------------------------------------------------------------------
		/// @brief:		读取界面中安装包项目的数据
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void readUIDataVdprojToMem();

		/// -------------------------------------------------------------------------------
		/// @brief:		更新安装包项目文件对应的版本号
		/// @param: 	const ListInstallProjectVersion & list_data - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		void updateVdprojFileVersion(const stInstallProjectVersion& ipv, QString& out_error);

		/// -------------------------------------------------------------------------------
		/// @brief:		生成项目输出
		/// @param: 	const ListInstallProjectVersion & list_data - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		QString generateVdproject(const stInstallProjectVersion& ipv);

		/// -------------------------------------------------------------------------------
		/// @brief:		更新exe dll的文件信息
		/// @param: 	const ListExeDllVersionInfo & list_data - 
		///  @ret:		void
		///				
		/// -------------------------------------------------------------------------------
		QString updateExeDllFileVersion(const stExeDllVersionInfo& exe_dll_info);


		/// -------------------------------------------------------------------------------
		/// @brief:		获取对应的devenv命令
		/// @param: 	const stInstallProjectVersion & which_vdproj - 
		///  @ret:		QT_NAMESPACE::QString
		///				
		/// -------------------------------------------------------------------------------
		QString devenvCmdToCompilerSln(const stInstallProjectVersion& which_vdproj);

		/// -------------------------------------------------------------------------------
		/// @brief:		执行命令
		/// @param: 	const QString & str_cmd - 
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		int runBatchCmd(const QString& str_cmd, const QString& bat_root, QString& out_error);

		/// -------------------------------------------------------------------------------
		/// @brief:		运行batch 文件
		/// @param: 	const QString & batch_file - 
		///  @ret:		int
		///				
		/// -------------------------------------------------------------------------------
		int runBatchFile(const QString& batch_file, const QString& bat_root);

	private:
		void setHint(const QString& text);

	private:
		/// 主程序UI
		//ExePackAssistant	main_widget_;
		QScopedPointer<ExePackAssistant> main_widget_;

		/// 更新文件版本对象
		QScopedPointer<FileVersionUpdater> pversion_updater_;

		/// 配置文件中的记录
		FileRecord	file_record_;


	};
}
