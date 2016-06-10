
// LiteSetupUICompressorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LiteSetupUICompressor.h"
#include "LiteSetupUICompressorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLiteSetupUICompressorDlg �Ի���



#include "../LiteSetup/LiteSetup.cpp"




using namespace Utils;




HRESULT CreateLink()
{
	HRESULT r = CoInitialize(NULL);


	IShellLink* psl;
	IPersistFile* ppf;
	r = CoCreateInstance(CLSID_ShellLink, NULL,
		CLSCTX_INPROC_SERVER, IID_IShellLinkA, (LPVOID *)&psl);

	// read short cut



	//create shortcut
	psl->SetPath(TEXT("c:\\windows\\notepad.exe"));
	psl->SetDescription(TEXT("snotepad"));
	psl->SetWorkingDirectory(TEXT("c:\\"));

	WORD key;
	key = (HOTKEYF_CONTROL << 8) | (HOTKEYF_ALT << 8) | 0x42;
	psl->SetHotkey(key);
	// Query IShellLink for the IPersistFile interface for saving the
	// shortcut in persistent storage.
	r = psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
	if (SUCCEEDED(r)) {
		WCHAR wsz[MAX_PATH];              // Ensure that the string is ANSI.
		MultiByteToWideChar(CP_ACP, 0, "c:\\windows\\explorer.lnk", -1,
			wsz, MAX_PATH);
		// Save the link by calling IPersistFile::Save.
		r = ppf->Save(wsz, TRUE);
		ppf->Release();
	}
	psl->Release();


	//	return r;



	CoUninitialize();
	return r;
}






class CSelectFolderDlg
{
public:
	//����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��
	static CString Show(HWND fhwnd)
	{
		TCHAR			szFolderPath[MAX_PATH] = { 0 };
		CString			strFolderPath = TEXT("");

		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("��ѡ��װ·���ļ��У�");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = NULL;
		sInfo.hwndOwner = fhwnd;

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


static CString path;





CLiteSetupUICompressorDlg::CLiteSetupUICompressorDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CLiteSetupUICompressorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLiteSetupUICompressorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _edit_root);
	DDX_Control(pDX, IDC_EDIT2, _edit_name);
	DDX_Control(pDX, IDC_STAT, _txt_path);
	DDX_Control(pDX, IDC_STATIC112, _txt_process);
	DDX_Control(pDX, IDC_PROGRESS1, _progress);
	DDX_Control(pDX, IDOK, _btn_start);
	DDX_Control(pDX, IDCANCEL, _btn_exit);
	DDX_Control(pDX, IDC_BUTTON1, _btn_select);
	DDX_Control(pDX, IDC_STATIC2314, _txt_total_size);
	DDX_Control(pDX, IDC_EDIT3, _edit_exe_name);
	DDX_Control(pDX, IDC_EDIT4, _edit_shortcut_name);
}

BEGIN_MESSAGE_MAP(CLiteSetupUICompressorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CLiteSetupUICompressorDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLiteSetupUICompressorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CLiteSetupUICompressorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLiteSetupUICompressorDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLiteSetupUICompressorDlg ��Ϣ�������

BOOL CLiteSetupUICompressorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������








	_progress.SetStep(1);








	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLiteSetupUICompressorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLiteSetupUICompressorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLiteSetupUICompressorDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CLiteSetupUICompressorDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


	CString x = CSelectFolderDlg::Show(this->GetSafeHwnd());

	CString nil;

	if (x == nil)
	{
		return;
	}

	path = x;


	_txt_path.SetWindowTextW(path);

}


bool CLiteSetupUICompressorDlg::CheckForCondition()
{
	CString nil;

	if (path == nil)
	{
		MessageBox(TEXT("��ѡ��Ŀ¼"), TEXT("��ʾ"), MB_OK);
		return false;
	}

	CString input;

	WCHAR  b[100];


	if (_edit_name.GetLine(0, (b), 100) == 0)
	{
		MessageBox(TEXT("�������������"), TEXT("��ʾ"), MB_OK);
		return false;

	}




	{

		WCHAR  b[100];


		if (_edit_exe_name.GetLine(0, (b), 100) == 0)
		{
			MessageBox(TEXT("��������Ҫ������ݷ�ʽ��exeȫ��"), TEXT("��ʾ"), MB_OK);
			return false;

		}
	}

	{

		WCHAR  b[100];


		if (_edit_shortcut_name.GetLine(0, (b), 100) == 0)
		{
			MessageBox(TEXT("�������ݷ�ʽ����"), TEXT("��ʾ"), MB_OK);
			return false;

		}
	}

	if (_edit_root.GetWindowTextLengthW() == 0)
	{
		MessageBox(TEXT("������Ĭ�ϳ����Ŀ¼(������·��)"), TEXT("��ʾ"), MB_OK);
		return false;
	}


	{
		CString x;


		if (MessageBox(TEXT("ȷ�ϴ��?"), TEXT("��ʾ"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			return false;
		}

	}
	return true;
}


// start to compress
void CLiteSetupUICompressorDlg::OnBnClickedOk()
{

	if (this->CheckForCondition() == false)return;


	{
		WCHAR  buffer[100] = { 0 };
		_edit_name.GetLine(0, (buffer), 100);
		const_for_exe::app_name = Utils::toAscii(buffer);
	}

	{
		WCHAR  buffer[100] = { 0 };
		_edit_root.GetLine(0, (buffer), 100);
		const_for_exe::dir_install = Utils::toAscii(buffer);
	}

	{
		WCHAR  buffer[100] = { 0 };
		_edit_exe_name.GetLine(0, (buffer), 100);
		const_for_exe::_shortcut_exe_name = Utils::toAscii(buffer);
	}

	{
		WCHAR  buffer[100] = { 0 };
		_edit_shortcut_name.GetLine(0, (buffer), 100);
		const_for_exe::_shortcut_name = Utils::toAscii(buffer);
	}


	_btn_start.EnableWindow(FALSE);
	_btn_exit.EnableWindow(FALSE);
	_btn_select.EnableWindow(FALSE);
	_edit_root.EnableWindow(FALSE);
	_edit_name.EnableWindow(FALSE);

	_edit_exe_name.EnableWindow(FALSE);
	_edit_shortcut_name.EnableWindow(FALSE);

	_txt_process.SetWindowTextW((toUnicode("���ڴ���: ɨ���ļ���...")));

	Compressor *s = new Compressor(toAscii(path.GetBuffer(0)));
	//Compressor *s = new Compressor("C:\Users\hk\Desktop\back");




	s->setCompressCallback([&](string name, int cur, int all)
	{

		_progress.SetRange(0, all);
		_progress.StepIt();


		{
			CString ss;
			ss.Insert(0, toUnicode("���ڴ���:"));
			char b[100];
			ss.Insert(ss.GetLength(), toUnicode(itoa(cur * 100 / all, b, 10)));
			ss.Insert(ss.GetLength(), toUnicode("% "));
			this->SetWindowTextW(ss);
		}
		{
			CString ss;
			ss.Insert(0, toUnicode("���ڴ���:"));
			char b[100];
			ss.Insert(ss.GetLength(), toUnicode(itoa(cur * 100 / all, b, 10)));
			ss.Insert(ss.GetLength(), toUnicode("% "));
			ss.Insert(ss.GetLength(), toUnicode(name.c_str()));

			_txt_process.SetWindowTextW((ss));
		}



	}, [&](CompressDoneCallBackData *_data)
	{
		CString txt;

		txt.Insert(txt.GetLength(), TEXT("�Ѵ������ݴ�С: "));
		string _cur = Utils::GetStorageSizeBrief(_data->_cur_file_size, "%.2f");
		txt.Insert(txt.GetLength(), Utils::toUnicode(_cur.c_str()));

		txt.Insert(txt.GetLength(), TEXT("       �������ݴ�С: "));

		_cur = Utils::GetStorageSizeBrief(_data->_cur_data_file_size, "%.2f");
		txt.Insert(txt.GetLength(), Utils::toUnicode(_cur.c_str()));


		txt.Insert(txt.GetLength(), TEXT("       ѹ����: "));


		unsigned long long  x = 100;

		x *= _data->_cur_data_file_size;
		x /= _data->_cur_file_size;

		string rit = Utils::ull_to_string(x);


		rit.append(" %");

		txt.Insert(txt.GetLength(), Utils::toUnicode(rit.c_str()));

		_txt_total_size.ShowWindow(SW_SHOW);
		_txt_total_size.SetWindowTextW(txt);


		if (_data->_is_done)
		{

			this->SetWindowTextW(toUnicode("LiteSetupUIConpressor"));
			_progress.SetPos(0);
			_txt_process.SetWindowTextW((toUnicode("������")));
			MessageBox(TEXT("������"), TEXT("��ʾ"), MB_OK);
			_btn_start.EnableWindow(TRUE);
			_btn_exit.EnableWindow(TRUE);
			_btn_select.EnableWindow(TRUE);
			_edit_root.EnableWindow(TRUE);
			_edit_name.EnableWindow(TRUE);
			_edit_exe_name.EnableWindow(TRUE);
			_edit_shortcut_name.EnableWindow(TRUE);

			//	ShellExecute(NULL, NULL, _T("explorer"), Utils::toUnicode("Output"), NULL, SW_SHOW);

		}

	});



	s->setErrorCallback([&](ErrorCode code, string msg)
	{

		if (code == ErrorCode::ERROR_COMPRESS)
		{
			this->SetWindowTextW(toUnicode("LiteSetupUIConpressor"));
			_progress.SetPos(0);
			_txt_process.SetWindowTextW((toUnicode("���ʧ��")));
			MessageBox(TEXT("���ʧ��"), TEXT("��ʾ"), MB_OK);
			_btn_start.EnableWindow(TRUE);
			_btn_exit.EnableWindow(TRUE);
			_btn_select.EnableWindow(TRUE);
			_edit_root.EnableWindow(TRUE);
			_edit_name.EnableWindow(TRUE);
		}
	});


	s->Compress();

}


void CLiteSetupUICompressorDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (MessageBox(TEXT("ȷ���˳�"), TEXT(""), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CDialogEx::OnCancel();
	}

}
