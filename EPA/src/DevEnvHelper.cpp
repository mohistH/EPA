#include "DevEnvHelper.h"


DevEnvHelper::DevEnvHelper()
{
	;
}

DevEnvHelper::~DevEnvHelper()
{
	;
}


/// --------------------------------------------------------------------------------
/// @brief: DevEnvHelper::ins
/// --------------------------------------------------------------------------------
DevEnvHelper& DevEnvHelper::ins()
{
	static DevEnvHelper instance;
	return instance;
}

/// --------------------------------------------------------------------------------
/// @brief: DevEnvHelper::set
/// --------------------------------------------------------------------------------
void DevEnvHelper::set(const ListDevEnv& new_list)
{
	{
		MapStrStr tmp_map;
		map_vs_name_.swap(tmp_map);
	}

	for (const auto& item : new_list)
	{
		map_vs_name_.insert(item.name_, item.devenv_);
	}
}

/// --------------------------------------------------------------------------------
/// @brief: DevEnvHelper::vsName
/// --------------------------------------------------------------------------------
QStringList DevEnvHelper::vsName()
{
	QList<QString>list_ret = map_vs_name_.keys();

	QStringList list_str;
	for (auto& item : list_ret)
	{
		list_str << item;
	}

	return list_str;
}

/// --------------------------------------------------------------------------------
/// @brief: DevEnvHelper::devenv
/// --------------------------------------------------------------------------------
QString DevEnvHelper::devenv(const QString& vs_name)
{
	MapStrStr::iterator findIt = map_vs_name_.find(vs_name);
	if (findIt != map_vs_name_.end())
	{
		return findIt.value();
	}

	return QString("");
}

MapStrStr& DevEnvHelper::devenv()
{
	return map_vs_name_;
}
