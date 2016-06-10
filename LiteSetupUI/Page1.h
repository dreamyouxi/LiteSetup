#pragma once


// Page1 对话框


#include "../LiteSetup/LiteSetup.h"
#include <string>
#include <vector>
#include <string.h>

using namespace  std;

/*
文件：SelectFolderDlg.h
说明：提供一个选择文件夹的对话框
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
	//创建一个选择文件夹的对话框，返回所选路径
	static CString Show()
	{
		TCHAR			szFolderPath[MAX_PATH] = { 0 };
		CString			strFolderPath = TEXT("");

		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("请选择待压缩文件夹：");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = NULL;

		// 显示文件夹选择对话框
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名
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

	// 对话框数据
	enum { IDD = IDD_LITESETUPUI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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

	// 对话框数据
	enum { IDD = IDD_LITESETUPUI_DIALOG };

protected:

	CStatic txt_info;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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