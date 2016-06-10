

/*
this is uncompressor   for litesetup

by dreamyouxi.com
*/







// LiteSetupUIDlg.h : 头文件
//

#pragma once


#include "../LiteSetup/LiteSetup.h"
#include "afxwin.h"
#include "afxcmn.h"



// CLiteSetupUIDlg 对话框
class CLiteSetupUIDlg : public CDialogEx
{
	// 构造
public:
	CLiteSetupUIDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_LITESETUPUI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:


	CStatic _txt_status;
	UnCompressor *uncomp;
	Compressor *comp;


	afx_msg void OnBnClickedBtnNext();
	CButton _btn_next;



private:
	bool _is_done_uncompress = false;

	/**
	 * @brief  change ui index
	 */
	void setSetupIndex(int _index);
	int _index;

	void  GetTotalFileSizeBrief(CString *str);


	void closeAllUI();
public:
	afx_msg void OnBnClickedBtnCancel();
	CButton _btn_cancel;
	CListBox _list_file;




	afx_msg void OnBnClickedSelect();


	void Load_3();
	CButton _btn_select;
	CProgressCtrl _progress;
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic _txt_total_size;


private:

	void CheckFilesExist();
	void CreateShortcut();

public:
	CButton _check_shortcut;
	CButton _check_open;
	CButton _check_run;
};







class CSelectFolderDlg
{
public:
	//创建一个选择文件夹的对话框，返回所选路径
	static CString Show(HWND fhwnd)
	{
		TCHAR			szFolderPath[MAX_PATH] = { 0 };
		CString			strFolderPath = TEXT("");

		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("请选择安装路径文件夹：");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = NULL;
		sInfo.hwndOwner = fhwnd;

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
