#include "ExeDllFileModel.h"


ExeDllFileModel::ExeDllFileModel(QObject* parent)
	: ATableModel(parent)
{
	this->init();
}

ExeDllFileModel::~ExeDllFileModel()
{

}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::rowCount
/// --------------------------------------------------------------------------------
int ExeDllFileModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return list_file_info_.count();
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::data
/// --------------------------------------------------------------------------------
QVariant ExeDllFileModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
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
				const stExeDllVersionInfo fi = list_file_info_.at(row_index);
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
/// @brief: ExePackModel::setData
/// --------------------------------------------------------------------------------
bool ExeDllFileModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
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
				stExeDllVersionInfo fi = list_file_info_.at(row_index);
				(this->*findIt.value().set_column_data_func_)(&fi, value);

				list_file_info_[row_index] = fi;
			}
		}
	}

	return true;
}


/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::setModelData
/// --------------------------------------------------------------------------------
void ExeDllFileModel::setModelData(const ListExeDllVersionInfo& newData)
{
	list_file_info_ = newData;
	beginResetModel();
	endResetModel();
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::clearModelData
/// --------------------------------------------------------------------------------
void ExeDllFileModel::clearModelData()
{
	{
		ListExeDllVersionInfo tmp_list;
		list_file_info_.swap(tmp_list);
	}

	beginResetModel();
	endResetModel();
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::modelData
/// --------------------------------------------------------------------------------
ListExeDllVersionInfo ExeDllFileModel::modelData()
{
	return list_file_info_;
}


/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::init
/// --------------------------------------------------------------------------------
void ExeDllFileModel::init()
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


	insert_dispatch_func(0, &ExeDllFileModel::columnData00, &ExeDllFileModel::setColumnData00, tr("Project Name"));
	insert_dispatch_func(1, &ExeDllFileModel::columnData01, &ExeDllFileModel::setColumnData01, tr("File Name"));
	insert_dispatch_func(2, &ExeDllFileModel::columnData02, &ExeDllFileModel::setColumnData02, tr("File Major Version"));
	insert_dispatch_func(3, &ExeDllFileModel::columnData03, &ExeDllFileModel::setColumnData03, tr("File Minor Version"));
	insert_dispatch_func(4, &ExeDllFileModel::columnData04, &ExeDllFileModel::setColumnData04, tr("File Revision Version"));
	insert_dispatch_func(5, &ExeDllFileModel::columnData05, &ExeDllFileModel::setColumnData05, tr("File Build Version"));
	insert_dispatch_func(6, &ExeDllFileModel::columnData06, &ExeDllFileModel::setColumnData06, tr("Product Major Version"));
	insert_dispatch_func(7, &ExeDllFileModel::columnData07, &ExeDllFileModel::setColumnData07, tr("Product Minor Version"));
	insert_dispatch_func(8, &ExeDllFileModel::columnData08, &ExeDllFileModel::setColumnData08, tr("Product Revision Version"));
	insert_dispatch_func(9, &ExeDllFileModel::columnData09, &ExeDllFileModel::setColumnData09, tr("Product Build Version"));
	insert_dispatch_func(10, &ExeDllFileModel::columnData10, &ExeDllFileModel::setColumnData10, tr("Batch File"));
}

/// --------------------------------------------------------------------------------
/// @brief: 项目名称
/// --------------------------------------------------------------------------------
QVariant ExeDllFileModel::columnData00(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.project_name_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 文件
/// --------------------------------------------------------------------------------
QVariant ExeDllFileModel::columnData01(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.file_name_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 主版本号
/// --------------------------------------------------------------------------------
QVariant ExeDllFileModel::columnData02(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.file_major_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 小版本
/// --------------------------------------------------------------------------------
QVariant ExeDllFileModel::columnData03(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.file_minor_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 内部版本号
/// --------------------------------------------------------------------------------
QVariant ExeDllFileModel::columnData04(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.file_revision_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 修订版本号
/// --------------------------------------------------------------------------------
QVariant ExeDllFileModel::columnData05(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.file_build_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 主版本
/// --------------------------------------------------------------------------------
QVariant ExeDllFileModel::columnData06(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.project_major_ };
}

/// 小版本
QVariant ExeDllFileModel::columnData07(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.project_minor_ };
}

/// 内部版本
QVariant ExeDllFileModel::columnData08(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.project_revision_ };
}

/// 修订版本
QVariant ExeDllFileModel::columnData09(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.project_build_ };
}

/// 批处理文件
QVariant ExeDllFileModel::columnData10(const stExeDllVersionInfo& file_info) const
{
	return QVariant{ file_info.batch_file_ };
}

/// --------------------------------------------------------------------------------
/// @brief: vs版本
/// --------------------------------------------------------------------------------
void ExeDllFileModel::setColumnData00(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->project_name_ = column_data.toString();
}

/// --------------------------------------------------------------------------------
/// @brief: 解决方案名称
/// --------------------------------------------------------------------------------
void ExeDllFileModel::setColumnData01(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->file_name_ = column_data.toString();
}

/// --------------------------------------------------------------------------------
/// @brief: 项目文件
/// --------------------------------------------------------------------------------
void ExeDllFileModel::setColumnData02(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->file_major_ = column_data.toInt();
}

/// --------------------------------------------------------------------------------
/// @brief: 
/// --------------------------------------------------------------------------------
void ExeDllFileModel::setColumnData03(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->file_minor_ = column_data.toInt();
}

/// --------------------------------------------------------------------------------
/// @brief: 
/// --------------------------------------------------------------------------------
void ExeDllFileModel::setColumnData04(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->file_revision_ = column_data.toInt();
}

/// --------------------------------------------------------------------------------
/// @brief: 
/// --------------------------------------------------------------------------------
void ExeDllFileModel::setColumnData05(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->file_build_ = column_data.toInt();
}

/// --------------------------------------------------------------------------------
/// @brief: 
/// --------------------------------------------------------------------------------
void ExeDllFileModel::setColumnData06(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->project_major_ = column_data.toInt();
}

/// 
void ExeDllFileModel::setColumnData07(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->project_minor_ = column_data.toInt();
}

/// 
void ExeDllFileModel::setColumnData08(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->project_revision_ = column_data.toInt();
}

/// 
void ExeDllFileModel::setColumnData09(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->project_build_ = column_data.toString();
}

/// 批处理文件
void ExeDllFileModel::setColumnData10(stExeDllVersionInfo* pfi, const QVariant& column_data)
{
	pfi->batch_file_ = column_data.toString();
}

