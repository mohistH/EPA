#pragma once
#include "AItemDelegate.h"

class QTableView;


class ExeDllFileDelegate : public AItemDelegate
{
	Q_OBJECT

public:
	explicit ExeDllFileDelegate(QObject* parent = nullptr);
	virtual  ~ExeDllFileDelegate();


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget00
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget00(QWidget* parent) const override;

	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget01
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget01(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget02
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget02(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget03
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget03(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget04
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget04(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget05
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget05(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget06
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget06(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget07
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget07(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget08
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget08(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget09
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget09(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: createColumnWidget10
	/// --------------------------------------------------------------------------------
	virtual QWidget* createColumnWidget10(QWidget* parent) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData00
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData00(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData01
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData01(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData02
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData02(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData03
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData03(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData04
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData04(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData05
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData05(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData06
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData06(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData07
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData07(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData08
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData08(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData09
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData09(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnModelData10
	/// --------------------------------------------------------------------------------
	virtual void setColumnModelData10(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData00
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData00(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData01
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData01(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData02
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData02(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData03
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData03(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData04
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData04(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData05
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData05(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData06
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData06(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData07
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData07(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData08
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData08(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData09
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData09(QWidget* editor, const QModelIndex& index) const override;


	/// --------------------------------------------------------------------------------
	/// @brief: setColumnEditorData10
	/// --------------------------------------------------------------------------------
	virtual void setColumnEditorData10(QWidget* editor, const QModelIndex& index) const override;

};

