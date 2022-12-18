#pragma once

#include <QTableView>

class ATableView : public QTableView
{
	Q_OBJECT

public:
	explicit ATableView(QWidget* parent = nullptr);
	virtual ~ATableView();

private slots:
	void slotShowToolTip(const QModelIndex& index);
private:

};
