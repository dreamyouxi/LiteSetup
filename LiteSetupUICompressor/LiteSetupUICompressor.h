
// LiteSetupUICompressor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLiteSetupUICompressorApp: 
// �йش����ʵ�֣������ LiteSetupUICompressor.cpp
//

class CLiteSetupUICompressorApp : public CWinApp
{
public:
	CLiteSetupUICompressorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLiteSetupUICompressorApp theApp;