// Page1.cpp : 实现文件
//

#include "stdafx.h"
#include "LiteSetupUI.h"

#include "Page1.h"
#include "afxdialogex.h"
#include "afxpriv.h"
#include <tchar.h>
// Page1 对话框

IMPLEMENT_DYNAMIC(Page1, CPropertyPage)

Page1::Page1()
: CPropertyPage(Page1::IDD)
{


}

Page1::~Page1()
{
}

void Page1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Page1, CPropertyPage)
END_MESSAGE_MAP()


LPWSTR toUnicode(const char* _str)
{
	LPWSTR _ret;
	int _len = strlen(_str) * 2;

	_ret = new WCHAR[_len];

	MultiByteToWideChar(CP_ACP, 0, _str, -1, _ret, _len);

	return _ret;
}




char *  UnicodeToANSI(LPWSTR str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str,
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str,
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	string strText;
	strText = pElementText;
	//delete[] pElementText;
	return pElementText;
}



 static  CString str;

BOOL Page1::OnInitDialog()
{


	txt_info.Create(L"选择待压缩文件夹", WS_VISIBLE, CRect(10, 10, 600, 100), this);

	//	_input_path.Create(WS_VISIBLE, CRect(10, 10, 300, 100), this,0);


	 str =  CSelectFolderDlg::Show();

	txt_info.SetWindowTextW( str);

//	CString ss = "\"";


	//str.Insert(0,  toUnicode( "\""));
	//str.Insert(str.GetLength(), toUnicode("\""));


	p = new UnCompressor(UnicodeToANSI(str.GetBuffer(0)));


	p->setCompressCallback([&](string name)
	{

		name.insert(0, "\\");



		CString ss = toUnicode(name.c_str());
		ss.Insert(0, toUnicode(p->dir.c_str()));




		ss.Insert(0, toUnicode("正在提取  "));


		txt_info.SetWindowTextW(ss);

	});


//	p->UnCompress();












/*	p = new Compressor(( UnicodeToANSI(  str.GetBuffer(0) )));



	TRACE("76.................................");



	p->setCompressCallback(
		[=](string, int, int)
	{


	}
		
		,[=](string name)
	{
		auto s = (LPCTSTR)(p->dir.c_str());
	//	txt_info.SetWindowTextW(s);
	});

	p->Compress();


	*/
	return TRUE;

}



IMPLEMENT_DYNAMIC(Page2, CPropertyPage)

Page2::Page2()
: CPropertyPage(Page1::IDD)
{

}

Page2::~Page2()
{
}

void Page2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BOOL Page2::OnInitDialog()
{


	txt_info.Create(L"安装完成", WS_VISIBLE, CRect(10, 10, 600, 100), this);





	return TRUE;

}






BEGIN_MESSAGE_MAP(Page2, CPropertyPage)
END_MESSAGE_MAP()










// Page1 消息处理程序
