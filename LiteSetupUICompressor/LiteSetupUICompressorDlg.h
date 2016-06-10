

/*
this is compressor   for litesetup

by dreamyouxi.com
*/




// LiteSetupUICompressorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "../LiteSetup/LiteSetup.h"


// CLiteSetupUICompressorDlg �Ի���
class CLiteSetupUICompressorDlg : public CDialogEx
{
	// ����
public:
	CLiteSetupUICompressorDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_LITESETUPUICOMPRESSOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
