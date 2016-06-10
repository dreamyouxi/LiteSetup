#pragma once


// Page1 �Ի���


#include "../LiteSetup/LiteSetup.h"
#include <string>
#include <vector>
#include <string.h>

using namespace  std;

/*
�ļ���SelectFolderDlg.h
˵�����ṩһ��ѡ���ļ��еĶԻ���
*/

#ifndef SELECT_FOLDER_DLG_H
#define SELECT_FOLDER_DLG_H


#ifndef BIF_NEWDIALOGSTYLE
#define  BIF_NEWDIALOGSTYLE  0x0040
#endif


#endif









class CSelectFolderDlg
{
public:
	//����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��
	static CString Show()
	{
		TCHAR			szFolderPath[MAX_PATH] = { 0 };
		CString			strFolderPath = TEXT("");

		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("��ѡ���ѹ���ļ��У�");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = NULL;

		// ��ʾ�ļ���ѡ��Ի���
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// ȡ���ļ�����
			if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				strFolderPath = szFolderPath;
			}
		}
		if (lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}

		return strFolderPath;

	}

};



class Page1 : public CPropertyPage
{
	DECLARE_DYNAMIC(Page1)

public:
	Page1();
	virtual ~Page1();

	// �Ի�������
	enum { IDD = IDD_LITESETUPUI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog()override;
	DECLARE_MESSAGE_MAP()


	UnCompressor *p;

	CStatic txt_info;
	CEdit _input_path;

	LRESULT  OnWizardNext()override
	{

		p->UnCompress();
		return 0;

	}
};


class Page2 : public CPropertyPage
{
	DECLARE_DYNAMIC(Page2)

public:
	Page2();
	virtual ~Page2();

	// �Ի�������
	enum { IDD = IDD_LITESETUPUI_DIALOG };

protected:

	CStatic txt_info;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog()override;
	DECLARE_MESSAGE_MAP()
};


class  PropertSheet :public CPropertySheet
{
public:

	PropertSheet()
	{
		this->SetWizardMode();
		this->AddPage(&p1);

		this->AddPage(&p2);
	}
	Page1 p1;
	Page2 p2;






};