
// MiniCDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MiniC.h"
#include "MiniCDlg.h"
#include "afxdialogex.h"
#include "analyse.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMiniCDlg 对话框



CMiniCDlg::CMiniCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINIC_DIALOG, pParent)
	, m_path(_T(""))
	, m_input(_T(""))
	, m_output(_T(""))
	, flag(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CODE, code);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_CODE, m_input);
	DDX_Text(pDX, IDC_TREE, m_output);
	DDX_Text(pDX, IDC_DISSEMANTIC, m_semantic);
	DDX_Text(pDX, IDC_MC, m_mc);
	DDX_Control(pDX, IDC_LIST, m_result);
}

BEGIN_MESSAGE_MAP(CMiniCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CMiniCDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_SAVE, &CMiniCDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SYNTAX, &CMiniCDlg::OnBnClickedSyntax)
	ON_BN_CLICKED(IDC_SHOW, &CMiniCDlg::OnBnClickedShow)
	ON_BN_CLICKED(IDC_RESULT, &CMiniCDlg::OnBnClickedResult)
	ON_BN_CLICKED(IDC_BTNSEMANTIC, &CMiniCDlg::OnBnClickedBtnsemantic)
	ON_BN_CLICKED(IDC_BTNMC, &CMiniCDlg::OnBnClickedBtnmc)
END_MESSAGE_MAP()


// CMiniCDlg 消息处理程序

BOOL CMiniCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_path = _T("");
	m_input = _T("");
	m_output = _T("");

	m_result.ModifyStyle(0, LVS_REPORT);
	m_result.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_result.InsertColumn(0, _T("行号"));
	m_result.InsertColumn(1, _T("记号"));
	m_result.InsertColumn(2, _T("字符"));
	m_result.SetColumnWidth(0, 50);
	for (int i = 1; i < 3; i++)
		m_result.SetColumnWidth(i, 150);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMiniCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMiniCDlg::OnPaint()
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
HCURSOR CMiniCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

string sts = "./sts.out";
string str = "./syntaxtree.out";
string temp = "./tmp.out";
string sem = "./semantic.out";
string mc = "./.tm";

void CMiniCDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
		m_path = path;
		UpdateData(false);
		ifstream file(path);

		if (!file) return;

		char buffer[1024];
		string str = "";
		while (!file.eof())
		{
			file.getline(buffer, 1024);
			str += buffer;
			str += "\r\n";
		}
		m_input = str.c_str();
		file.close();
		UpdateData(false);
		//MessageBox(m_input);
	}
	else
	{
		//MessageBox(L"文件选择窗体创建失败");
		return;
	}
}


void CMiniCDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text, path, fileName = L"temp.txt";
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrTitle = L"我的文件保存对话框";
	dlg.m_ofn.lpstrFilter = L"Text Files(*.cpp)\0*.cpp\0All Files(*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrDefExt = L"txt";
	if (IDOK == dlg.DoModal())
	{
		path = dlg.GetPathName();
		ofstream file(path);
		UpdateData(true);
		code.GetWindowTextW(m_input);
		for (int i = 0; i < m_input.GetLength(); i++)
			if (m_input[i] != '\r')
				file.put(m_input[i]);
		file.close();
	}
}

void CMiniCDlg::OnBnClickedSyntax()
{
	// TODO: 在此添加控件通知处理程序代码
	ofstream file(temp);
	flag = 1; //按了生成语法树按钮
	code.GetWindowTextW(m_input);
	for (int i = 0; i < m_input.GetLength(); i++)
		if (m_input[i] != '\r')
			file.put(m_input[i]);
	file.close();
	//analyse(temp, str, sts,sem);  //输入路径，输出文件名
	MessageBox(L"生成成功，点击查看即可查看语法树");
}


void CMiniCDlg::OnBnClickedShow()
{
	// TODO: 在此添加控件通知处理程序代码

	m_output = "";
	ifstream file(str.c_str());
	//ifstream file(file_path.c_str());
	if (!file) return;
	char buffer[1024];
	string tree = "";
	while (!file.eof())
	{
		file.getline(buffer, 1024);
		tree += buffer;
		tree += "\r\n";
	}
	m_output = tree.c_str();

	file.close();
	UpdateData(false);
}

void CMiniCDlg::OnBnClickedResult()
{
	// TODO: 在此添加控件通知处理程序代码
	//FILE *file;
	//file = fopen("sts.out","w");
	//fclose(file);
	m_result.DeleteAllItems();
	if (TRUE)
	{ 
		ofstream ofile(temp);
		code.GetWindowTextW(m_input);
		for (int i = 0; i < m_input.GetLength(); i++)
			if (m_input[i] != '\r')
				ofile.put(m_input[i]);
		ofile.close();
		analyse(temp, str, sts,sem); 
	}

	ifstream ifile(sts.c_str());
	if (!ifile) 
		return;
	char buf[1000];
	int i = 0; 

	while (!ifile.eof())
	{
		string str1 = ""; 
		string str2 = ""; 
		string str3 = "";
		ifile.getline(buf, 1024);
		string t = "";
		t += buf;
		int p1 = t.find_first_of(':');
		if (p1 == -1) 
			continue;
		int p2 = t.find_first_of('&');
		if (p2 == -1) 
			continue;
		int p3 = p2 - p1;
		str1 += t.substr(0, p1);
		str2 += t.substr(p1 + 1, p3 - 1);
		str3 += t.substr(p2 + 1);
		if (str3 == "") 
			continue;
		CString temp1(str3.c_str());
		CString temp2(str1.c_str());
		CString temp3(str2.c_str());
		CString format;
		i = m_result.GetItemCount();
		format.Format(_T("%d"), i);
		m_result.InsertItem(i, format);
		m_result.SetItemText(i, 0, temp1);
		m_result.SetItemText(i, 1, temp2);
		m_result.SetItemText(i, 2, temp3);
	}

	ifile.close();
	UpdateData(false);
}

void CMiniCDlg::OnBnClickedBtnsemantic()
{
	// TODO: 在此添加控件通知处理程序代码
	m_semantic = "";
	ifstream file(sem.c_str());
	if (!file) return;
	char buffer[1024];
	string dissemantic = "";
	while (!file.eof())
	{
		file.getline(buffer, 1024);
		dissemantic += buffer;
		dissemantic += "\r\n";
	}
	m_semantic = dissemantic.c_str();

	file.close();
	UpdateData(false);
}


void CMiniCDlg::OnBnClickedBtnmc()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mc = "";
	ifstream file(mc.c_str());
	if (!file) return;
	char buffer[1024];
	string dismc = "";
	while (!file.eof())
	{
		file.getline(buffer, 1024);
		dismc += buffer;
		dismc += "\r\n";
	}
	m_mc = dismc.c_str();

	file.close();
	UpdateData(false);
}
