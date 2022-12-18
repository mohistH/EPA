#include "ATableModel.h"

/// --------------------------------------------------------------------------------
/// @brief: ATableModel::ATableModel
/// --------------------------------------------------------------------------------
ATableModel::ATableModel(QObject* parent /*= nullptr*/)
	: QAbstractTableModel(parent)
{
	;
}

/// --------------------------------------------------------------------------------
/// @brief: ATableModel::~ATableModel
/// --------------------------------------------------------------------------------
ATableModel::~ATableModel()
{

}


/// --------------------------------------------------------------------------------
/// @brief: ATableModel::headerData
/// --------------------------------------------------------------------------------
QVariant ATableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	switch (role)
	{
	case Qt::TextAlignmentRole:
		return QVariant{ Qt::AlignLeft | Qt::AlignVCenter };

	case Qt::DisplayRole:
	{
		if (Qt::Horizontal == orientation)
		{
			HashIntStr::const_iterator findIt = hash_column_header_name_.find(section);
			if (findIt != hash_column_header_name_.end())
			{
				return findIt.value();
			}
		}

		if (Qt::Vertical == orientation)
		{
			return section + 1;
		}
	}
	break;

	default:
		break;
	}

	return QVariant{};
}

/// --------------------------------------------------------------------------------
/// @brief: ATableModel::columnCount
/// --------------------------------------------------------------------------------
Q_INVOKABLE int ATableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return hash_column_header_name_.count();
}

/// --------------------------------------------------------------------------------
/// @brief: ATableModel::flags
/// --------------------------------------------------------------------------------
Q_INVOKABLE  Qt::ItemFlags ATableModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

/// --------------------------------------------------------------------------------
/// @brief: ATableModel::setColumnHeaderName
/// --------------------------------------------------------------------------------
int ATableModel::setColumnHeaderName(const int& which_column_id, const QString& header_name)
{
	hash_column_header_name_.insert(which_column_id, header_name);

	return 0;
}

/// --------------------------------------------------------------------------------
/// @brief: ATableModel::removeColumnHeaderName
/// --------------------------------------------------------------------------------
int ATableModel::removeColumnHeaderName(const int& which_column_id)
{
	HashIntStr::iterator del_it = hash_column_header_name_.find(which_column_id);
	if (del_it != hash_column_header_name_.end())
	{
		del_it = hash_column_header_name_.erase(del_it);

		return 0;
	}

	return 1;
}
