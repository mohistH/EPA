#include "InstallProjectModel.h"


InstallProjectModel::InstallProjectModel(QObject* parent)
	: ATableModel(parent)
{
	this->init();
}

InstallProjectModel::~InstallProjectModel()
{}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::rowCount
/// --------------------------------------------------------------------------------
int InstallProjectModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return list_file_info_.count();
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::data
/// --------------------------------------------------------------------------------
QVariant InstallProjectModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
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
				const stInstallProjectVersion fi = list_file_info_.at(row_index);
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
bool InstallProjectModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
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
				stInstallProjectVersion fi = list_file_info_.at(row_index);
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
void InstallProjectModel::setModelData(const ListInstallProjectVersion& newData)
{
	list_file_info_ = newData;
	beginResetModel();
	endResetModel();
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::clearModelData
/// --------------------------------------------------------------------------------
void InstallProjectModel::clearModelData()
{
	{
		ListInstallProjectVersion tmp_list;
		list_file_info_.swap(tmp_list);
	}

	beginResetModel();
	endResetModel();
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::modelData
/// --------------------------------------------------------------------------------
ListInstallProjectVersion InstallProjectModel::modelData()
{
	return list_file_info_;
}

/// --------------------------------------------------------------------------------
/// @brief: ExePackModel::init
/// --------------------------------------------------------------------------------
void InstallProjectModel::init()
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


	insert_dispatch_func(0, &InstallProjectModel::columnData00, &InstallProjectModel::setColumnData00, tr("DevEnv"));
	insert_dispatch_func(1, &InstallProjectModel::columnData01, &InstallProjectModel::setColumnData01, tr("Solution File"));
	insert_dispatch_func(2, &InstallProjectModel::columnData02, &InstallProjectModel::setColumnData02, tr("Project Name"));
	insert_dispatch_func(3, &InstallProjectModel::columnData03, &InstallProjectModel::setColumnData03, tr("Platfrom"));
	insert_dispatch_func(4, &InstallProjectModel::columnData04, &InstallProjectModel::setColumnData04, tr("Configuration"));
	insert_dispatch_func(5, &InstallProjectModel::columnData05, &InstallProjectModel::setColumnData05, tr("Major Version"));
	insert_dispatch_func(6, &InstallProjectModel::columnData06, &InstallProjectModel::setColumnData06, tr("Minor Version"));
	insert_dispatch_func(7, &InstallProjectModel::columnData07, &InstallProjectModel::setColumnData07, tr("Revision Version"));
	//insert_dispatch_func(8, &InstallProjectModel::columnData08, &InstallProjectModel::setColumnData08, tr("Build Version"));
	insert_dispatch_func(8, &InstallProjectModel::columnData09, &InstallProjectModel::setColumnData09, tr("Batch File"));
}

/// --------------------------------------------------------------------------------
/// @brief: VS版本列
/// --------------------------------------------------------------------------------
QVariant InstallProjectModel::columnData00(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.compiler_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 解决方案文件列
/// --------------------------------------------------------------------------------
QVariant InstallProjectModel::columnData01(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.sln_name_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 项目文件列
/// --------------------------------------------------------------------------------
QVariant InstallProjectModel::columnData02(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.project_name_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 平台文件列
/// --------------------------------------------------------------------------------
QVariant InstallProjectModel::columnData03(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.platform_name_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 配置管理器
/// --------------------------------------------------------------------------------
QVariant InstallProjectModel::columnData04(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.configuration_name_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 主版本
/// --------------------------------------------------------------------------------
QVariant InstallProjectModel::columnData05(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.major_ };
}

/// --------------------------------------------------------------------------------
/// @brief: 小版本
/// --------------------------------------------------------------------------------
QVariant InstallProjectModel::columnData06(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.minor_ };
}

/// 内部版本
QVariant InstallProjectModel::columnData07(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.revision_ };
}

/// 修订版本
QVariant InstallProjectModel::columnData08(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.build_ };
}

/// 批处理文件
QVariant InstallProjectModel::columnData09(const stInstallProjectVersion& file_info) const
{
	return QVariant{ file_info.bat_file_ };
}

/// --------------------------------------------------------------------------------
/// @brief: vs版本
/// --------------------------------------------------------------------------------
void InstallProjectModel::setColumnData00(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->compiler_ = column_data.toString();
}

/// --------------------------------------------------------------------------------
/// @brief: 解决方案名称
/// --------------------------------------------------------------------------------
void InstallProjectModel::setColumnData01(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->sln_name_ = column_data.toString();
}

/// --------------------------------------------------------------------------------
/// @brief: 项目文件
/// --------------------------------------------------------------------------------
void InstallProjectModel::setColumnData02(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->project_name_ = column_data.toString();
}

/// --------------------------------------------------------------------------------
/// @brief: 平台管理器
/// --------------------------------------------------------------------------------
void InstallProjectModel::setColumnData03(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->platform_name_ = column_data.toString();
}

/// --------------------------------------------------------------------------------
/// @brief: 配置管理器
/// --------------------------------------------------------------------------------
void InstallProjectModel::setColumnData04(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->configuration_name_ = column_data.toString();
}

/// --------------------------------------------------------------------------------
/// @brief: 主版本
/// --------------------------------------------------------------------------------
void InstallProjectModel::setColumnData05(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->major_ = column_data.toInt();
}

/// --------------------------------------------------------------------------------
/// @brief: 小版本
/// --------------------------------------------------------------------------------
void InstallProjectModel::setColumnData06(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->minor_ = column_data.toInt();
}

/// 内部版本
void InstallProjectModel::setColumnData07(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->revision_ = column_data.toInt();
}

/// 修订版本
void InstallProjectModel::setColumnData08(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->build_ = column_data.toString();
}

/// 批处理文件
void InstallProjectModel::setColumnData09(stInstallProjectVersion* pfi, const QVariant& column_data)
{
	pfi->bat_file_ = column_data.toString();
}
