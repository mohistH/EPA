#ifndef LANGUAGE_HELPER_H_
#define LANGUAGE_HELPER_H_
#include <QTranslator>


class LanguageHelper
{
public:
	void setTranslator(QTranslator* ptl)
	{
		translator_ = ptl;
	}

	void load(const QString& str_qm)
	{
		if (translator_)
		{
			translator_->load(str_qm);
		}
	}

private:
	QTranslator* translator_ = nullptr;

};
#endif // LANGUAGE_HELPER_H_