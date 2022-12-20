#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <QHash>
#include <QMap>
#include <unordered_map>
#include "Typedef.h"
#include "TableModelDelegateShare.h"
#include "TableViewWidget.h"
#include "FileVersionUpdater.h"
#include <QScopedPointer>
#include "DevEnvModel.h"
#include "DevEnvDelegate.h"
#include "FileVersionUpdater.h"
#include "ExeDllFileModel.h"
#include "ExeDllFileDelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FilePackMgrWidgetClass; };
QT_END_NAMESPACE

#include <QTableView>
#include <QTranslator>

#include <InstallProjectModel.h>
#include <InstallProjectDelegate.h>
#include <FileRecord.h>
#include <Typedef.h>
#include <EPAInterface.h>

class FilePackMgrWidget : public QWidget, public TableModelDelegateShare, public FileRecord, public IUIData/// , LanguageHelper
{
	Q_OBJECT

signals:
	/// exe dll的应用按钮点击
	void sigExeDllApplyPressed(const ListExeDllVersionInfo& which_map);

	/// vs版本选择-应用按钮点击
	void sigDevEnvApplyPressed();

	/// 安装项目--应用按钮点击
	void sigVdprojApplyPressed(const ListInstallProjectVersion& list_data);

	/// hint, 参数不为空
	void sigShowHint(const QString& text);

	/// 解析exedll
	void sigParseExeDll(const int row_index, const QString& select_file) const;

public:
	explicit FilePackMgrWidget(QWidget* parent = nullptr);
	~FilePackMgrWidget();


	/// -------------------------------------------------------------------------------
	/// @brief:		读取UIdevenv数据
	/// @param: 	ListDevEnv * pout_list - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	int devenvModelData(ListDevEnv* pout_list) override;

	/// -------------------------------------------------------------------------------
	/// @brief:		获取exe dll Data
	/// @param: 	ListExeDllVersionInfo * pout_list - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	int exeDllModelData(ListExeDllVersionInfo* pout_list)override;

	/// -------------------------------------------------------------------------------
	/// @brief:		获取安装包项目数据
	/// @param: 	ListInstallProjectVersion * pout_list - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	int installProjectModelData(ListInstallProjectVersion* pout_list)override;


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
	/// @brief:		设置默认值状态
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setUIEnableDefault();

	/// @brief  清空UI数据
	void clearUIData();

	/// -------------------------------------------------------------------------------
	/// @brief:		加载翻译
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void loadLanguage();

	void init();

public slots:
	/// -------------------------------------------------------------------------------
	/// @brief:		显示结果
	/// @param: 	const QString & text - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void slotOutputResult(const QString& text);

private slots:
	void slotPushBtnInsertExeDllPressed();
	void slotPushBtnRemoveExeDllPressed();
	void slotPushBtnCarryExeDllPressed();

	void slotPushBtnInsertVdprojPressed();
	void slotPushBtnRemoveVdprojPressed();
	void slotPushBtnApplyVdprojPressed();

	void slotPushBtnInsertVSVersionPressed();
	void slotPushBtnRemoveVSVersionPressed();
	void slotPushBtnApplyVSVersionPressed();

private:
	/// -------------------------------------------------------------------------------
	/// @brief:		获取选择的行
	/// @param: 	MapIntInt * pout_map - 
	/// @param: 	QTableView * ptv - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	int tableViewSelectedRows(MapIntInt* pout_map, QTableView* ptv);

private:
	template<typename TFuncInsert, typename TFuncRemove, typename TFuncCarry>
	void connectSigSlot(TableViewWidget* ptvw, TFuncInsert func_insert, TFuncRemove func_remove, TFuncCarry func_carry)
	{
		connect(ptvw, &TableViewWidget::sigInsert, this, func_insert);
		connect(ptvw, &TableViewWidget::sigRemove, this, func_remove);
		connect(ptvw, &TableViewWidget::sigCarry, this, func_carry);
	}





private:
	Ui::FilePackMgrWidgetClass* ui = nullptr;

	template<typename TModel, typename TDelegate>
	struct stUnitModelDelegate_
	{
		stUnitModelDelegate_()
		{
			;
		}

		stUnitModelDelegate_(const stUnitModelDelegate_& instance)
		{
			ptvw_ = instance.ptvw_;
			model_.reset(instance.model_.data());
			delegate_.reset(instance.delegate_.data());

			//recv_carry_ = instance.recv_carry_;
			//recv_remove_ = instance.recv_remove_;
			//recv_insert_ = instance.recv_insert_;
		}

		stUnitModelDelegate_& operator = (const stUnitModelDelegate_& instance)
		{
			ptvw_ = instance.ptvw_;
			model_.reset(instance.model_.data());
			delegate_.reset(instance.delegate_.data());
			//recv_carry_ = instance.recv_carry_;
			//recv_remove_ = instance.recv_remove_;
			//recv_insert_ = instance.recv_insert_;

			return *this;
		}

		~stUnitModelDelegate_()
		{

		}

		void setValue(TableViewWidget*& ptv)
		{
			ptvw_ = ptv;
		}

	public:
		QScopedPointer<TModel>  model_;
		QScopedPointer<TDelegate> delegate_;
		TableViewWidget* ptvw_ = nullptr;

		//void** recv_insert_ = nullptr;
		//void** recv_remove_ = nullptr;
		//void** recv_carry_ = nullptr;
	};

	using stInstallProjectMD = stUnitModelDelegate_<InstallProjectModel, InstallProjectDelegate>;
	using stVSModelDelegate = stUnitModelDelegate_<DevEnvModel, DevEnvDelegate>;

	using stExeDllMD = stUnitModelDelegate_<ExeDllFileModel, ExeDllFileDelegate>;

private:

	/// vs版本
	stVSModelDelegate	devenv_md_;

	/// EXE DLL
	stExeDllMD		exe_dll_md_;

	/// 打包项目
	stInstallProjectMD	vdproj_md_;

	QTranslator		language_trans_;
};
