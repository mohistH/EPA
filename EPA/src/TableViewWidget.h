#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <QTranslator>

class QTableView;
class QAbstractTableModel;
class QAbstractItemDelegate;



QT_BEGIN_NAMESPACE
namespace Ui { class TableViewWidgetClass; };
QT_END_NAMESPACE

class TableViewWidget : public QWidget
{
	Q_OBJECT

signals:
	void sigInsert();
	void sigRemove();
	void sigCarry();

public:
	explicit TableViewWidget(QWidget* parent = nullptr);
	virtual ~TableViewWidget();

	/// -------------------------------------------------------------------------------
	/// @brief:		
	///  @ret:		QT_NAMESPACE::QTableView*
	///				
	/// -------------------------------------------------------------------------------
	QTableView* tableView();

	void setModel(QAbstractTableModel* pmodel);
	void setItemDelegate(QAbstractItemDelegate* pdelegate);

	void setBtnEnabledInert(const bool is_enabled);
	void setBtnEnabledRemove(const bool is_enabled);
	void setBtnEnabledCarry(const bool is_enabled);

	void setEnabeldWithData();

	void setBtnVisibleInsert(const bool is_visible);
	void setBtnVisibleRemove(const bool is_visible);
	void setBtnVisibleCarry(const bool is_visible);

	void setBtnNameCarry(const QString& text);
	void setBtnNameInsert(const QString& text);
	void setBtnNameRemove(const QString& text);

	void loadLanguage();

private:
	Ui::TableViewWidgetClass* ui = nullptr;

	QTranslator		language_trans_;
};
