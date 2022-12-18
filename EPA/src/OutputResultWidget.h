
#pragma once

#include <QWidget>
#include <QTranslator>


QT_BEGIN_NAMESPACE
namespace Ui { class OutputResultWidgetClass; };
QT_END_NAMESPACE


/// ----------------------------------------------------------------------------------------
/// @brief: 结果显示窗口
/// ----------------------------------------------------------------------------------------
class OutputResultWidget : public QWidget
{
	enum
	{
		MAX_ROW_COUNT_200 = 200,
	};
	Q_OBJECT

public:
	explicit OutputResultWidget(QWidget* parent = nullptr);
	virtual ~OutputResultWidget();

	/// -------------------------------------------------------------------------------
	/// @brief:		设置要显示的内容， 无需手动添加行换，内部自动处理
	/// @param: 	const QString & text - 
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void append(const QString& text);

	/// -------------------------------------------------------------------------------
	/// @brief:		设置UI控件默认状态
	///  @ret:		void
	///				
	/// -------------------------------------------------------------------------------
	void setUIEnabled();


	void loadLanguage();

private:
	Ui::OutputResultWidgetClass* ui = nullptr;
};
