#include "pch.h"
#include "IExeVersion.h"
#include "ExeVersionImp.h"


namespace oct_vi
{

	IExeVersion* new_ev()
	{
		IExeVersion* pret = new (std::nothrow)ExeVersionImp;
		return pret;
	}

	IExeVersion* del_ev(IExeVersion* pobj)
	{
		if (pobj)
		{
			delete pobj;
			pobj = nullptr;
		}

		return pobj;
	}

}
