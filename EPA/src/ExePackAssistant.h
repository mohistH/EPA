#pragma once

#include <QtWidgets/QWidget>
#include "EPAInterface.h"
#include "Typedef.h"


QT_BEGIN_NAMESPACE
namespace Ui { class ExePackAssistantClass; };
QT_END_NAMESPACE


class FilePackMgrWidget;

class ExePackAssistant : public QWidget, public IUIData
{
	Q_OBJECT

signals:
	/// 创建文件信号， 参数-新建文件全路径
	void sigCreateFile(const QString& new_file);
	/// 打开文件
	void sigOpenFile(const QString& which_file);
	/// 关闭
	void sigClose();

public:
	explicit ExePackAssistant(QWidget* parent = nullptr);
	~ExePackAssistant();


	/// -------------------------------------------------------------------------------
	/// @brief:		设置打开按钮
	/// @param: 	const bool & is_enabled - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setBtnEnabledOpen(const bool& is_enabled);

	/// -------------------------------------------------------------------------------
	/// @brief:		设置关闭按钮
	/// @param: 	const bool & is_enabled - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setBtnEnabledClose(const bool& is_enabled);

	/// -------------------------------------------------------------------------------
	/// @brief:		设置创建按钮
	/// @param: 	const bool & is_enabled - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setBtnEnabledCreate(const bool& is_enabled);


	/// --------------------------------------------------------------------------------
	/// @brief: devenvModelData
	/// --------------------------------------------------------------------------------
	virtual int devenvModelData(ListDevEnv* pout_list) override;


	/// --------------------------------------------------------------------------------
	/// @brief: exeDllModelData
	/// --------------------------------------------------------------------------------
	virtual int exeDllModelData(ListExeDllVersionInfo* pout_list) override;


	/// --------------------------------------------------------------------------------
	/// @brief: installProjectModelData
	/// --------------------------------------------------------------------------------
	virtual int installProjectModelData(ListInstallProjectVersion* pout_list) override;


	/// --------------------------------------------------------------------------------
	/// @brief: setDevenvModelData
	/// --------------------------------------------------------------------------------
	virtual int setDevenvModelData(ListDevEnv* pout_list) override;


	/// --------------------------------------------------------------------------------
	/// @brief: setExeDllModelData
	/// --------------------------------------------------------------------------------
	virtual int setExeDllModelData(ListExeDllVersionInfo* pout_list) override;


	/// --------------------------------------------------------------------------------
	/// @brief: setInstallProjectModelData
	/// --------------------------------------------------------------------------------
	virtual int setInstallProjectModelData(ListInstallProjectVersion* pout_list) override;

	/// -------------------------------------------------------------------------------
	/// @brief:		读取file_mgr窗口
	///  @ret:		FilePackMgrWidget*
	///				
	/// -------------------------------------------------------------------------------
	FilePackMgrWidget* mgrWidget();


	/// -------------------------------------------------------------------------------
	/// @brief:		加载翻译
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void loadLanguage();

private:
	/// -------------------------------------------------------------------------------
	/// @brief:		链接 信号槽
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void connectSigSlot();



private slots:
	/// -------------------------------------------------------------------------------
	/// @brief:		创建按钮
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void slotBtnCreatePressed();

	/// -------------------------------------------------------------------------------
	/// @brief:		打开按钮点击
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void slotBtnOpenPressed();


private:
	Ui::ExePackAssistantClass* ui = nullptr;

	/// 上一次创建文件路径
	QString last_create_file_path_ = "";

	/// 上一次打开文件路径
	QString last_open_file_root_ = "";



};
