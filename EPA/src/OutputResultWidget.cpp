
#include "OutputResultWidget.h"
#include "ui_OutputResultWidget.h"
#include <QScrollBar>

OutputResultWidget::OutputResultWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::OutputResultWidgetClass())
{
	ui->setupUi(this);

	//setNewRowText("111");
	//setNewRowText("222");
	//setNewRowText("333");
		/// 设置进制换行
	ui->text_edit_result->setWordWrapMode(QTextOption::NoWrap);
	ui->text_edit_result->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ui->text_edit_result->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ui->text_edit_result->document()->setMaximumBlockCount(MAX_ROW_COUNT_200);
}

OutputResultWidget::~OutputResultWidget()
{
	delete ui;
}

/// --------------------------------------------------------------------------------
/// @brief: 添加一行新的数据
/// --------------------------------------------------------------------------------
void OutputResultWidget::append(const QString& text)
{
	/// 如果当前已经大于200行，则清空，重新插入
	if (MAX_ROW_COUNT_200 <= ui->text_edit_result->document()->lineCount())
	{
		ui->text_edit_result->clear();
	}

	ui->text_edit_result->append(text);

	QScrollBar* pscrollbar = ui->text_edit_result->verticalScrollBar();
	if (pscrollbar)
	{
		pscrollbar->setValue(pscrollbar->maximum());
	}
}

/// -------------------------------------------------------------------------------
/// @brief:		设置UI控件默认状态
///  @ret:		void
///				
/// -------------------------------------------------------------------------------
void OutputResultWidget::setUIEnabled()
{
	setEnabled(true);
	//ui->text_edit_result->setReadOnly(true);
	//ui->text_edit_result->setEnabled(false);
}

/// --------------------------------------------------------------------------------
/// @brief: OutputResultWidget::loadLanguage
/// --------------------------------------------------------------------------------
void OutputResultWidget::loadLanguage()
{
	ui->retranslateUi(this);
}
