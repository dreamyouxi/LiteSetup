
// LiteSetupUICompressorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LiteSetupUICompressor.h"
#include "LiteSetupUICompressorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLiteSetupUICompressorDlg 对话框



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


// CLiteSetupUICompressorDlg 消息处理程序

BOOL CLiteSetupUICompressorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码








	_progress.SetStep(1);








	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLiteSetupUICompressorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLiteSetupUICompressorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLiteSetupUICompressorDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CLiteSetupUICompressorDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码


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
		MessageBox(TEXT("请选择目录"), TEXT("提示"), MB_OK);
		return false;
	}

	CString input;

	WCHAR  b[100];


	if (_edit_name.GetLine(0, (b), 100) == 0)
	{
		MessageBox(TEXT("请输入程序名称"), TEXT("提示"), MB_OK);
		return false;

	}




	{

		WCHAR  b[100];


		if (_edit_exe_name.GetLine(0, (b), 100) == 0)
		{
			MessageBox(TEXT("请输入需要创建快捷方式的exe全名"), TEXT("提示"), MB_OK);
			return false;

		}
	}

	{

		WCHAR  b[100];


		if (_edit_shortcut_name.GetLine(0, (b), 100) == 0)
		{
			MessageBox(TEXT("请输入快捷方式名称"), TEXT("提示"), MB_OK);
			return false;

		}
	}

	if (_edit_root.GetWindowTextLengthW() == 0)
	{
		MessageBox(TEXT("请输入默认程序根目录(不包含路径)"), TEXT("提示"), MB_OK);
		return false;
	}


	{
		CString x;


		if (MessageBox(TEXT("确认打包?"), TEXT("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
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

	_txt_process.SetWindowTextW((toUnicode("正在处理: 扫描文件中...")));

	Compressor *s = new Compressor(toAscii(path.GetBuffer(0)));
	//Compressor *s = new Compressor("C:\Users\hk\Desktop\back");




	s->setCompressCallback([&](string name, int cur, int all)
	{

		_progress.SetRange(0, all);
		_progress.StepIt();


		{
			CString ss;
			ss.Insert(0, toUnicode("正在处理:"));
			char b[100];
			ss.Insert(ss.GetLength(), toUnicode(itoa(cur * 100 / all, b, 10)));
			ss.Insert(ss.GetLength(), toUnicode("% "));
			this->SetWindowTextW(ss);
		}
		{
			CString ss;
			ss.Insert(0, toUnicode("正在处理:"));
			char b[100];
			ss.Insert(ss.GetLength(), toUnicode(itoa(cur * 100 / all, b, 10)));
			ss.Insert(ss.GetLength(), toUnicode("% "));
			ss.Insert(ss.GetLength(), toUnicode(name.c_str()));

			_txt_process.SetWindowTextW((ss));
		}



	}, [&](CompressDoneCallBackData *_data)
	{
		CString txt;

		txt.Insert(txt.GetLength(), TEXT("已处理数据大小: "));
		string _cur = Utils::GetStorageSizeBrief(_data->_cur_file_size, "%.2f");
		txt.Insert(txt.GetLength(), Utils::toUnicode(_cur.c_str()));

		txt.Insert(txt.GetLength(), TEXT("       生成数据大小: "));

		_cur = Utils::GetStorageSizeBrief(_data->_cur_data_file_size, "%.2f");
		txt.Insert(txt.GetLength(), Utils::toUnicode(_cur.c_str()));


		txt.Insert(txt.GetLength(), TEXT("       压缩率: "));


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
			_txt_process.SetWindowTextW((toUnicode("打包完成")));
			MessageBox(TEXT("打包完成"), TEXT("提示"), MB_OK);
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
			_txt_process.SetWindowTextW((toUnicode("打包失败")));
			MessageBox(TEXT("打包失败"), TEXT("提示"), MB_OK);
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
	// TODO:  在此添加控件通知处理程序代码

	if (MessageBox(TEXT("确认退出"), TEXT(""), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CDialogEx::OnCancel();
	}

}
