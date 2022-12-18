#include "DevEnvModel.h"


DevEnvModel::DevEnvModel(QObject* parent)
	: ATableModel(parent)
{
	this->init();
}

DevEnvModel::~DevEnvModel()
{}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::rowCount
/// --------------------------------------------------------------------------------
int DevEnvModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return list_file_info_.count();
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::data
/// --------------------------------------------------------------------------------
QVariant DevEnvModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	const int row_index = index.row();
	const int column_index = index.column();

	switch (role)
	{

	case Qt::TextAlignmentRole:
		return QVariant{ Qt::AlignLeft | Qt::AlignVCenter };
		break;

	case Qt::DisplayRole:
	case Qt::EditRole:
	{
		HashColumnDispatch::const_iterator findIt = hash_column_dispatch_.find(column_index);
		if (findIt != hash_column_dispatch_.end())
		{
			if (findIt.value().column_data_func_)
			{
				const stDevEnvInfo fi = list_file_info_.at(row_index);
				return (this->*findIt.value().column_data_func_)(fi);
			}
		}
	}
	break;

	default:
		break;
	}

	return QVariant{};
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::setData
/// --------------------------------------------------------------------------------
bool DevEnvModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	if (Qt::EditRole == role)
	{
		const int row_index = index.row();
		const int column_index = index.column();

		HashColumnDispatch::const_iterator findIt = hash_column_dispatch_.find(column_index);
		if (findIt != hash_column_dispatch_.end())
		{
			if (findIt.value().set_column_data_func_)
			{
				stDevEnvInfo fi = list_file_info_.at(row_index);
				(this->*findIt.value().set_column_data_func_)(&fi, value);

				list_file_info_[row_index] = fi;
			}
		}
	}

	return true;
}

/// --------------------------------------------------------------------------------
/// @brief: DevEnvModel::flags
/// --------------------------------------------------------------------------------
Qt::ItemFlags DevEnvModel::flags(const QModelIndex& index) const
{
	/// | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::setModelData
/// --------------------------------------------------------------------------------
void DevEnvModel::setModelData(const ListDevEnv& newData)
{
	list_file_info_ = newData;
	beginResetModel();
	endResetModel();
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::clearModelData
/// --------------------------------------------------------------------------------
void DevEnvModel::clearModelData()
{
	{
		ListDevEnv list_tmp;
		list_file_info_.swap(list_tmp);
	}
	beginResetModel();
	endResetModel();

}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::modelData
/// --------------------------------------------------------------------------------
ListDevEnv DevEnvModel::modelData()
{
	return list_file_info_;
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::init
/// --------------------------------------------------------------------------------
void DevEnvModel::init()
{
	{
		HashColumnDispatch tmp;
		hash_column_dispatch_.swap(tmp);
	}

	auto insert_dispatch_func = [&](const int& column_index, ColumnDataFunc data_func, SetColumnDataFunc set_func, const QString& column_name)
	{
		stColumnDataFunc scdf;
		scdf.column_data_func_ = data_func;
		scdf.set_column_data_func_ = set_func;
		hash_column_dispatch_.insert(column_index, scdf);

		setColumnHeaderName(column_index, column_name);
	};


	insert_dispatch_func(0, &DevEnvModel::columnData00, &DevEnvModel::setColumnData00, tr("VS Name(*)"));
	insert_dispatch_func(1, &DevEnvModel::columnData01, &DevEnvModel::setColumnData01, tr("devenv.exe path(*)"));
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::columnData00
/// --------------------------------------------------------------------------------
QVariant DevEnvModel::columnData00(const stDevEnvInfo& file_info) const
{
	return QVariant{ file_info.name_ };
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::columnData01
/// --------------------------------------------------------------------------------
QVariant DevEnvModel::columnData01(const stDevEnvInfo& file_info) const
{
	return QVariant{ file_info.devenv_ };
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::setColumnData00
/// --------------------------------------------------------------------------------
void DevEnvModel::setColumnData00(stDevEnvInfo* pfi, const QVariant& column_data)
{
	pfi->name_ = column_data.toString();
}

/// --------------------------------------------------------------------------------
/// @brief: VSModel::setColumnData01
/// --------------------------------------------------------------------------------
void DevEnvModel::setColumnData01(stDevEnvInfo* pfi, const QVariant& column_data)
{
	pfi->devenv_ = column_data.toString();
}

