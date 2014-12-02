// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B6281B0A_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
#define AFX_STDAFX_H__B6281B0A_11DB_11D6_887A_00304F10C7F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxole.h>

#include <VisWin.h>

#if VIS_OS_IS_NT

	#include <VisCore.h>

	// We default to handling these data types
	typedef CVisRGBABytePixel		CDefaultPixel;
	typedef CVisImage<CDefaultPixel>	CDefaultImage;
	typedef CVisSequence<CDefaultPixel>	CDefaultSequence;

	// These are the old type names.  They are provided here to support
	// existing applications but should not be used in new code.
	// typedef CVisRGBABytePixel		CCameraPixel;
	// typedef CVisImage<CCameraPixel>		CCameraImage;

#else // VIS_OS_IS_NT

	// If we're building on Window 95 or Windows 98, we want to avoid putting
	// too many templated classes into the precompiled header files, so we
	// don't include the other Vision SDK header files here.  In that case,
	// we need to use #defines instead of typedefs for the default types.
	#define CDefaultPixel CVisRGBABytePixel
	#define CDefaultImage CVisImage<CDefaultPixel>
	#define CDefaultSequence CVisSequence<CDefaultPixel>

#endif // VIS_OS_IS_NT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_STDAFX_H__B6281B0A_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
