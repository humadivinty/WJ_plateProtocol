
// TestTool_WJSWDLL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestTool_WJSWDLLApp: 
// �йش����ʵ�֣������ TestTool_WJSWDLL.cpp
//

class CTestTool_WJSWDLLApp : public CWinApp
{
public:
	CTestTool_WJSWDLLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestTool_WJSWDLLApp theApp;