#include "ATableView.h"
#include <QHeaderView>
#include <QAbstractItemView>
#include <QToolTip>
#include <QCursor>
//#include <QDebug>

ATableView::ATableView(QWidget* parent)
	: QTableView(parent)
{
	this->verticalHeader()->setVisible(false);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->horizontalHeader()->setStretchLastSection(true);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->horizontalHeader()->setSelectionMode(QAbstractItemView::SingleSelection);
	//this->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);


	setMouseTracking(true);
	connect(this, &QTableView::entered, this, &ATableView::slotShowToolTip);
}

ATableView::~ATableView()
{}

void ATableView::slotShowToolTip(const QModelIndex& index)
{
	/// 1. 获取内容
	const QString&& str_text = this->model()->data(index).toString();
	QToolTip::showText(QCursor::pos(), str_text);
	//qDebug() << "-----text=" << str_text;
}
