
#ifndef _IAEPRODEBUG_H_
#define _IAEPRODEBUG_H_

#include "resource.h"
#include "IaeProException.h"
/*
#ifdef _DEBUG
#define IAE_ASSERT(x) if (!x) ASSERT(x);
#else
#define IAE_ASSERT(x) IaeProAssert(x, "An unspecified error has occurred!");
#endif
*/
void IaeProAssert(bool bStatement, UINT uID)
{
	if (!bStatement) {
		CString str;
		str.LoadString(IDS_FFTDIMERROR);

		throw CIaeProException(str);
	}
}

#endif