
#ifndef _IAEPROEXCEPTION_H_
#define _IAEPROEXCEPTION_H_

class CIaeProException
{
	public:
		CIaeProException();
		CIaeProException(CString e) { m_szError = e; }

		CString GetError() { return m_szError; }

	protected:
		CString	m_szError;
};

#endif
