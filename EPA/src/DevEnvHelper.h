#pragma once
#include "Typedef.h"

class DevEnvHelper
{
public:
	DevEnvHelper();
	virtual ~DevEnvHelper();

	static DevEnvHelper& ins();

	void set(const ListDevEnv& new_list);

	QStringList vsName();

	QString devenv(const QString& vs_name);

	MapStrStr& devenv();

private:
	MapStrStr map_vs_name_;
};
