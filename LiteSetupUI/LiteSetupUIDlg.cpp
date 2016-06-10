
// LiteSetupUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LiteSetupUI.h"
#include "LiteSetupUIDlg.h"
#include "afxdialogex.h"


//   #include "Page1.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../LiteSetup/LiteSetup.cpp"

// CLiteSetupUIDlg 对话框

using namespace Utils;
void func()
{
	Compressor ll("F:\\中文目录");
	ll.Compress();



}












CLiteSetupUIDlg::CLiteSetupUIDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CLiteSetupUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLiteSetupUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TXT_Status, _txt_status);
	DDX_Control(pDX, IDC_BTN_NEXT, _btn_next);
	DDX_Control(pDX, IDC_BTN_CANCEL, _btn_cancel);
	DDX_Control(pDX, IDC_LIST1, _list_file);
	DDX_Control(pDX, IDC_SELECT, _btn_select);
	DDX_Control(pDX, IDC_PROGRESS1, _progress);
	DDX_Control(pDX, IDC_STATIC1111, _txt_total_size);
	DDX_Control(pDX, IDC_CHECK3, _check_shortcut);
	DDX_Control(pDX, IDC_CHECK1, _check_open);
	DDX_Control(pDX, IDC_CHECK2, _check_run);
}

BEGIN_MESSAGE_MAP(CLiteSetupUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()



	ON_BN_CLICKED(IDC_BTN_NEXT, &CLiteSetupUIDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CLiteSetupUIDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_SELECT, &CLiteSetupUIDlg::OnBnClickedSelect)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CLiteSetupUIDlg::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()











CString path1;




// CLiteSetupUIDlg 消息处理程序



void  CLiteSetupUIDlg::CheckFilesExist()
{

	if (Utils::isFileExist(UNCOMPRESS_LOG_FILE) == false)
	{
		MessageBox(TEXT("文件丢失"), TEXT("错误"), MB_OK);
		exit(0);
	}


	if (Utils::isFileExist(DATA_FILE_NAME) == false)
	{
		MessageBox(TEXT("文件丢失"), TEXT("错误"), MB_OK);
		exit(0);
	}

}


BOOL CLiteSetupUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	this->CheckFilesExist();

	_check_shortcut.SetCheck(1);
	_check_run.SetCheck(1);



	DirectoryMgr::ParseConst_for_exe();

	TCHAR   buf[MAX_PATH];

	SHGetSpecialFolderPath(NULL, buf, CSIDL_PROGRAM_FILES, FALSE);

	//path = buf;
	path1 = buf;
	path1 += const_for_exe::dir_install.c_str();



	this->setSetupIndex(2);





	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLiteSetupUIDlg::OnPaint()
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
HCURSOR CLiteSetupUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CLiteSetupUIDlg::OnBnClickedBtnNext()
{

	// 
	if (_index == 1)
	{
		this->setSetupIndex(2);
		return;
	}



	// last ui index is 2 
	if (_index == 2)
	{



		CString xx = path1;
		xx.Insert(0, toUnicode("确认要安装在:   "));


		if (IDNO == MessageBox(xx, TEXT("确认安装"), MB_YESNO)){
			return;

		}

		Utils::CreateDir(Utils::toAscii(path1.GetBuffer(0)));


		this->setSetupIndex(3);

		this->Load_3();
		_progress.ShowWindow(SW_SHOW);

		// TODO:  在此添加控件通知处理程序代码
		if (_is_done_uncompress == false)
		{

			_btn_next.EnableWindow(FALSE);
			_btn_cancel.EnableWindow(FALSE);


			uncomp->UnCompress();
		}
		else
		{
			//PostMessage(WM_CLOSE);
		}
		return;
	}



	if (_index == 3)
	{

		this->setSetupIndex(4);
		return;
	}

	if (_index == 4)
	{


		if (_check_run.GetCheck())
		{
			CString app = path1;
			app += Utils::toUnicode("\\");

			app += Utils::toUnicode(const_for_exe::_shortcut_exe_name.c_str());

			CString workpath = path1;
			workpath += Utils::toUnicode("\\");

			Utils::log(Utils::toAscii(workpath.GetBuffer(0)));

			ShellExecuteW(NULL, NULL, app, NULL, workpath, SW_SHOW);

			//ShellExecute(NULL, NULL, _T("explorer"), app, NULL, SW_SHOW);
		}


		if (_check_shortcut.GetCheck())
		{
			CreateShortcut();

		}
		if (_check_open.GetCheck())
		{

			ShellExecute(NULL, NULL, _T("explorer"), path1, NULL, SW_SHOW);
		}



		this->SendMessage(WM_CLOSE);

	}
}


void CLiteSetupUIDlg::OnBnClickedBtnCancel()
{

	if (MessageBox(TEXT("确认退出"), TEXT(""), MB_YESNO | MB_ICONQUESTION) == IDYES){
		this->SendMessage(WM_CLOSE);
	}

}
void  CLiteSetupUIDlg::GetTotalFileSizeBrief(CString *txt)
{

	/*


	*txt = TEXT("至少需要 ");

	unsigned long long _file_size = 0;
	unsigned long long des = 1;

	{
	for (int i = const_for_exe::_total_file_size_string.size() - 1;
	i >= 0; i--, des *= 10)
	{
	_file_size += des*(const_for_exe::_total_file_size_string[i] - '0');
	}
	}
	const_for_exe::_total_file_size = _file_size;




	int len = 0;
	unsigned long long _file_size_tmp = _file_size;
	int _s_len = const_for_exe::_total_file_size_string.size();

	while (_file_size_tmp /= 1024)
	{
	len++;
	}


	static  char * _size_array[5] = { " Bytes", " KB", " MB", " GB", " TB" };



	_file_size_tmp = const_for_exe::_total_file_size;



	double _file_size_double = 0;


	char buf[200];


	if (len == 0)
	{
	// b

	sprintf(buf, "%d", _file_size_tmp);

	}
	else if (len == 1)
	{
	//kb
	_file_size_double = _file_size_tmp / 1024.0;
	sprintf(buf, "%.1f", _file_size_double);

	}
	else if (len == 2)
	{
	//mb
	_file_size_double = _file_size_tmp / 1024.0 / 1024.0;

	sprintf(buf, "%.1f", _file_size_double);
	}
	else if (len == 3)
	{
	//gb
	//	_size_num = _file_size_tmp / 1024.0 / 1024.0 / 1024.0;
	_file_size_tmp /= 1024;
	_file_size_double = _file_size_tmp / 1024.0 / 1024.0;
	sprintf(buf, "%.1f", _file_size_double);

	}
	else if (len == 4)
	{
	//tb
	_file_size_tmp /= 1024;
	_file_size_tmp /= 1024;
	_file_size_double = _file_size_tmp / 1024.0 / 1024.0;
	sprintf(buf, "%.1f", _file_size_double);
	}


	txt->Insert(txt->GetLength(), Utils::toUnicode(buf));

	// kb mb gb ...
	txt->Insert(txt->GetLength(), Utils::toUnicode(_size_array[len]));

	txt->Insert(txt->GetLength(), Utils::toUnicode(" 的空闲磁盘空间"));

	Utils::log("%d", const_for_exe::_total_file_size);

	*/


	*txt = TEXT("至少需要 ");

	string brief = Utils::GetStorageSizeBrief(const_for_exe::_total_file_size);

	// kb mb gb ...
	txt->Insert(txt->GetLength(), Utils::toUnicode(brief.c_str()));

	txt->Insert(txt->GetLength(), Utils::toUnicode(" 的空闲磁盘空间"));


}



void  CLiteSetupUIDlg::closeAllUI()
{
	_btn_next.ShowWindow(SW_HIDE);
	//_btn_cancel.ShowWindow(SW_HIDE);

	_list_file.ShowWindow(SW_HIDE);
	_btn_select.ShowWindow(SW_HIDE);
	_progress.ShowWindow(SW_HIDE);

	_txt_total_size.ShowWindow(SW_HIDE);
	_check_shortcut.ShowWindow(SW_HIDE);
	_check_open.ShowWindow(SW_HIDE);
	_check_run.ShowWindow(SW_HIDE);


}


void CLiteSetupUIDlg::setSetupIndex(int _index)
{

	this->closeAllUI();


	this->_index = _index;

	CString app_name = toUnicode(const_for_exe::app_name.c_str());


	if (_index == 2)
	{
		// 
		this->SetWindowTextW(TEXT("安装向导--") + app_name);
		_txt_status.SetWindowTextW(path1);
		_txt_total_size.ShowWindow(SW_SHOW);

		this->SetWindowTextW(TEXT("安装向导-") + app_name);

		{
			CString txt;
			this->GetTotalFileSizeBrief(&txt);
			_txt_total_size.SetWindowTextW(txt);

		}

		_btn_next.SetWindowTextW(TEXT("安装"));
		_btn_next.ShowWindow(SW_SHOW);
		_txt_status.ShowWindow(SW_SHOW);
		_btn_select.ShowWindow(SW_SHOW);
		return;
	}

	if (_index == 3)
	{
		// 
		this->SetWindowTextW(TEXT("正在扫描文件信息"));
		_txt_status.SetWindowTextW(TEXT("正在扫描文件信息，请等待。"));
		//this->Load_2();
		_btn_next.SetWindowTextW(TEXT("下一步"));
		_btn_next.ShowWindow(SW_SHOW);
		_list_file.ShowWindow(SW_SHOW);

		return;
	}


	if (_index == 4)
	{
		// 

		this->SetWindowTextW(TEXT("安装完成-") + app_name);
		_btn_next.SetWindowTextW(TEXT("完成安装"));
		_btn_next.ShowWindow(SW_SHOW);
		_btn_cancel.ShowWindow(SW_HIDE);

		_check_shortcut.ShowWindow(SW_SHOW);
		_check_open.ShowWindow(SW_SHOW);
		_check_run.ShowWindow(SW_SHOW);

		CString name = Utils::toUnicode("运行");
		name += Utils::toUnicode(const_for_exe::app_name.c_str());
		_check_run.SetWindowTextW(name);


		return;
	}




}





void CLiteSetupUIDlg::OnBnClickedSelect()
{
	// TODO:  在此添加控件通知处理程序代码

	CString x = CSelectFolderDlg::Show(this->GetSafeHwnd());

	CString nil;

	if (x == nil)
	{
		return;
	}

	path1 = x;
	path1 += const_for_exe::dir_install.c_str();



	_txt_status.SetWindowTextW(path1);


}


void CLiteSetupUIDlg::Load_3()
{

	CString app_name = toUnicode(const_for_exe::app_name.c_str());


	//	_txt_status.SetWindowTextW(TEXT("正在提取: "));
	CFont* font = new CFont;

	font->CreatePointFont(100, TEXT("正楷"));

	_txt_status.SetFont(font);




	//	_txt_status.GetFont()->;




	//	txt_info.Create(L"选择待压缩文件夹", WS_VISIBLE, CRect(10, 10, 600, 100), this);

	//	_input_path.Create(WS_VISIBLE, CRect(10, 10, 300, 100), this,0);



	//_txt_status.SetWindowTextW(path1);

	//	CString ss = "\"";


	//str.Insert(0,  toUnicode( "\""));
	//str.Insert(str.GetLength(), toUnicode("\""));


	uncomp = new UnCompressor(toAscii(path1.GetBuffer(0)));

	//_progress.SetRange(0, 100);


	//	_progress.SetPos(50);

	_progress.SetStep(1);

	uncomp->setCompressCallback([&](string name, int _cur, int total, unsigned long long _cur_total_size)
	{

		name.insert(0, "\\");

		_progress.SetRange32(0, total);
		_progress.StepIt();


		//	Sleep(100);

		char b[100];



		{
			CString s = toUnicode("正在安装-");

			s.Insert(s.GetLength(), toUnicode(itoa(_cur * 100 / total, b, 10)));

			s.Insert(s.GetLength(), toUnicode("%"));
			this->SetWindowTextW(s);

		}




		{
			CString ss;
			ss.Insert(0, toUnicode("正在提取: "));
			//ss.Insert(ss.GetLength(), toUnicode(uncomp->dir.c_str()));
			ss.Insert(ss.GetLength(), toUnicode(name.c_str()));
			_txt_status.SetWindowTextW(ss);
		}
		{

			CString ss;
			ss.Insert(0, toUnicode("输出文件: "));
			ss.Insert(ss.GetLength(), toUnicode(uncomp->dir.c_str()));
			ss.Insert(ss.GetLength(), toUnicode(name.c_str()));
			_list_file.InsertString(_list_file.GetCount(), ss);
			_list_file.SetCurSel(_list_file.GetCount() - 1);
		}


		{
			CString txt = TEXT("进度: ");
			string _cur = Utils::GetStorageSizeBrief(_cur_total_size, "%.2f");

			txt.Insert(txt.GetLength(), Utils::toUnicode(_cur.c_str()));

			txt.Insert(txt.GetLength(), Utils::toUnicode(" / "));

			string _max = Utils::GetStorageSizeBrief(const_for_exe::_total_file_size, "%.2f");

			txt.Insert(txt.GetLength(), Utils::toUnicode(_max.c_str()));



			//txt.Insert(txt.GetLength(), Utils::toUnicode(" 的空闲磁盘空间"));

			_txt_total_size.ShowWindow(SW_SHOW);
			_txt_total_size.SetWindowTextW(txt);




		}
	});


	uncomp->setUnCompressDoneCallback([&]()
	{
		_btn_next.EnableWindow(TRUE);
		_is_done_uncompress = true;

		_txt_status.SetWindowTextW(TEXT("安装完成"));

		this->SetWindowTextW(TEXT("安装完成"));

		_txt_total_size.ShowWindow(SW_HIDE);
		_list_file.InsertString(_list_file.GetCount(), toUnicode("安装完成"));


		_list_file.SetCurSel(_list_file.GetCount() - 1);


	});


}


void CLiteSetupUIDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

void CLiteSetupUIDlg::CreateShortcut()
{
	fstream f;
	f.open("shortcut.vbs", ios::out);

	string cmd;
	f << "Set WshShell = WScript.CreateObject(\"WScript.Shell\")" << endl;
	f << "strDesktop = WshShell.SpecialFolders(\"Desktop\")" << endl;

	cmd = "set oShellLink = WshShell.CreateShortcut(strDesktop & \"\\";
	cmd += const_for_exe::_shortcut_name;
	cmd += ".lnk\")";
	f << cmd << endl;



	cmd = "oShellLink.TargetPath = \"";
	cmd += Utils::toAscii(path1.GetBuffer(0));
	cmd += "\\";
	cmd += const_for_exe::_shortcut_exe_name;
	cmd += "\"";

	f << cmd << endl;


	f << "oShellLink.WindowStyle = 1" << endl;


	cmd = "oShellLink.IconLocation = \"";
	cmd += Utils::toAscii(path1.GetBuffer(0));
	cmd += "\\";
	cmd += const_for_exe::_shortcut_exe_name;
	cmd += ",0";
	cmd += "\"";

	f << cmd << endl;



	cmd = "oShellLink.WorkingDirectory =  \"";
	cmd += Utils::toAscii(path1.GetBuffer(0));
	cmd += "\"";

	f << cmd << endl;


	f << "oShellLink.Save" << endl;


	f.flush();
	f.close();

	//system("shortcut.vbs");

	Utils::system_hide_cmd("cmd /c shortcut.vbs");



	Utils::DeleteFileForName("shortcut.vbs");


}
