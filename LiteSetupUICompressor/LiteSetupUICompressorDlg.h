

/*
this is compressor   for litesetup

by dreamyouxi.com
*/




// LiteSetupUICompressorDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "../LiteSetup/LiteSetup.h"


// CLiteSetupUICompressorDlg 对话框
class CLiteSetupUICompressorDlg : public CDialogEx
{
	// 构造
public:
	CLiteSetupUICompressorDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_LITESETUPUICOMPRESSOR_DIALOG };

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
	afx_msg void OnEnChangeEdit1();
	CEdit _edit_root;
	CEdit _edit_name;
	CStatic _txt_path;
	CStatic _txt_process;
	CProgressCtrl _progress;
	CButton _btn_start;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	CButton _btn_exit;
	CButton _btn_select;
	afx_msg void OnBnClickedCancel();
	CStatic _txt_total_size;


private:
	bool CheckForCondition();

public:
	CEdit _edit_exe_name;
	CEdit _edit_shortcut_name;
};
