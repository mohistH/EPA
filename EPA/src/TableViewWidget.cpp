/*
 * @Author: mohistH 281208257@qq.com
 * @Date: 2022-12-16 15:58:09
 * @LastEditors: mohistH 281208257@qq.com
 * @LastEditTime: 2022-12-18 14:34:07
 * @FilePath: \CMAKE-EPA\EPA\src\TableViewWidget.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "TableViewWidget.h"
#include "ui_TableViewWidget.h"

TableViewWidget::TableViewWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::TableViewWidgetClass())
{
	ui->setupUi(this);
	connect(ui->push_btn_insert, &QPushButton::released, this, &TableViewWidget::sigInsert);
	connect(ui->push_btn_remove, &QPushButton::released, this, &TableViewWidget::sigRemove);
	connect(ui->push_btn_carry, &QPushButton::released, this, &TableViewWidget::sigCarry);

	this->setEnabled(false);
}

TableViewWidget::~TableViewWidget()
{
	delete ui;
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::tableView
/// --------------------------------------------------------------------------------
QTableView* TableViewWidget::tableView()
{
	return ui->table_view;
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setModel
/// --------------------------------------------------------------------------------
void TableViewWidget::setModel(QAbstractTableModel* pmodel)
{
	ui->table_view->setModel(pmodel);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setItemDelegate
/// --------------------------------------------------------------------------------
void TableViewWidget::setItemDelegate(QAbstractItemDelegate* pdelegate)
{
	ui->table_view->setItemDelegate(pdelegate);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnEnabledInert
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnEnabledInert(const bool is_enabled)
{
	ui->push_btn_insert->setEnabled(is_enabled);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnEnabledRemove
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnEnabledRemove(const bool is_enabled)
{
	ui->push_btn_remove->setEnabled(is_enabled);
	setBtnEnabledCarry(is_enabled);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnEnabledCarry
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnEnabledCarry(const bool is_enabled)
{
	ui->push_btn_carry->setEnabled(is_enabled);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setWidgetsEnabledDefault
/// --------------------------------------------------------------------------------
void TableViewWidget::setEnabeldWithData()
{
	setEnabled(true);
	setBtnEnabledInert(true);

	/// 获取行数
	const int&& row_count = ui->table_view->model()->rowCount();
	/// @brief  没有行， 则禁用删除和执行按钮
	const bool is_enabled = (0 == row_count ? false : true);
	setBtnEnabledCarry(is_enabled);
	setBtnEnabledRemove(is_enabled);

}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnVisibleInsert
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnVisibleInsert(const bool is_visible)
{
	ui->push_btn_insert->setVisible(is_visible);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnVisibleRemove
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnVisibleRemove(const bool is_visible)
{
	ui->push_btn_remove->setVisible(is_visible);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnVisibleCarry
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnVisibleCarry(const bool is_visible)
{
	ui->push_btn_carry->setVisible(is_visible);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnNameInsert
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnNameCarry(const QString& text)
{
	ui->push_btn_carry->setText(text);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnNameInsert
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnNameInsert(const QString& text)
{
	ui->push_btn_insert->setText(text);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::setBtnNameRemove
/// --------------------------------------------------------------------------------
void TableViewWidget::setBtnNameRemove(const QString& text)
{
	ui->push_btn_remove->setText(text);
}

/// --------------------------------------------------------------------------------
/// @brief: TableViewWidget::loadLanguage
/// --------------------------------------------------------------------------------
void TableViewWidget::loadLanguage()
{
	ui->retranslateUi(this);
}

