#pragma once
#include <QAbstractTableModel>
#include <QHash>
class ATableModel :
	public QAbstractTableModel
{
public:
	/// <列索引，列头名称>
	using HashIntStr = QHash<int, QString>;

public:
	explicit ATableModel(QObject* parent = nullptr);
	virtual ~ATableModel();

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	Q_INVOKABLE virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	Q_INVOKABLE virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

	/// -------------------------------------------------------------------------------
	/// @brief:		设置列药显示的名称
	/// @param: 	const int & which_column_id - 
	/// @param: 	const QString & header_name - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	int setColumnHeaderName(const int& which_column_id, const QString& header_name);

	/// -------------------------------------------------------------------------------
	/// @brief:		删除某列名称
	/// @param: 	const int & which_column_id - 
	///  @ret:		int
	///				
	/// -------------------------------------------------------------------------------
	int removeColumnHeaderName(const int& which_column_id);

private:
	/// 列表头名称
	HashIntStr		hash_column_header_name_;

};

