// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "login.h"
#include "mmsystem.h"
//#include ".\skins\SkinPlusPlus.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
	//���������߳� 
static	UINT SendDataThread(LPVOID lpParam); 
	//���������߳� 
static	UINT ReceiveDataThread(LPVOID lpParam); 	
 
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
	m_strRecv = _T("");
	m_strSend = _T("");
	m_strAll = FALSE;
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_COMBO_USER, m_ctrUser);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrProgress);
	DDX_Control(pDX, IDC_BUTTON_END, m_ctrEnd);
	DDX_Control(pDX, IDC_CHECK_ME, m_ctrMe);
	DDX_Control(pDX, IDC_EDIT_SEND, m_ctrSend);
	DDX_Control(pDX, IDC_EDIT_RECV, m_ctrRecv);
	DDX_Control(pDX, IDC_LIST_USER, m_ctrList);
	DDX_Text(pDX, IDC_EDIT_RECV, m_strRecv);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Check(pDX, IDC_CHECK_ALL, m_strAll);
	DDX_CBString(pDX, IDC_COMBO_USER, m_strUser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnSend)
	ON_MESSAGE(WM_SHOWTASK,onShowTask)
	ON_MESSAGE(WM_SOCKET_READ,OnReceive)
	ON_LBN_SELCHANGE(IDC_LIST_USER, OnSelchangeListUser)
	ON_BN_CLICKED(IDMIN, OnMin)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TANS, OnButtonTans)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_RECV, OnButtonRecv)
	ON_BN_CLICKED(IDC_BUTTON_SHAKE, OnButtonShake)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//	InitializeSkin(_T("Minimized.ssk"));
	// Add "About..." menu item to system menu.
	CLogin dlg;
	if(dlg.DoModal()==IDOK)
	{
		cuser=dlg.m_strUser;
		u_long IpAddress;
		IpAddress=inet_addr(dlg.send_ip);//�ַ���IPת32λIP
		clientsock=socket(PF_INET,SOCK_STREAM,0);
		if(clientsock==INVALID_SOCKET)
		{
			AfxMessageBox("Create SOCKET Failed");
			WSACleanup(); 
			closesocket(clientsock);
			CClientDlg::OnCancel();
		}
		clientaddr.sin_family=AF_INET;
		clientaddr.sin_addr.s_addr=IpAddress;
		clientaddr.sin_port=htons(4009);//short �����ֽ���
		int nConnect=connect(clientsock,(sockaddr *)&clientaddr,sizeof(clientaddr));
		if(nConnect==-1)
		{
			MessageBox("���ӹ��̷�������\n��ȷ��IP������ȷ����",NULL,MB_OK);
			CDialog::OnCancel();
		}
		else
		{
			SendMsg(0,cuser,"");
		}
		WSAAsyncSelect(clientsock,m_hWnd,WM_SOCKET_READ,FD_READ|FD_CLOSE);
		this->SetWindowText(cuser);

	}
	else
	{
		CDialog::OnCancel();
	}

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here
	m_strUser=_T("���");
	m_ctrUser.AddString("���");
	m_ctrSend.SetFocus();
	UpdateData(false);
	OnMin();
	{
		HBITMAP m_hBitmap;
		CButton* PButton;
		PButton =(CButton *)GetDlgItem(IDC_BUTTON_TANS);
		m_hBitmap =::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1));
		PButton->SetBitmap(m_hBitmap);
		PButton =(CButton *)GetDlgItem(IDC_BUTTON_SHAKE);
		m_hBitmap =::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2));
		PButton->SetBitmap(m_hBitmap);

	}
	{
		ball.Create(this);  
		ball.AddTool(GetDlgItem(IDC_BUTTON_TANS),"�ļ�����"); 
		ball.AddTool(GetDlgItem(IDC_CHECK_ALL),"������Ϣֻ����ͶԷ�֪��");
		ball.AddTool(GetDlgItem(IDC_CHECK_ME),"ֻ���շ����Լ�����Ϣ");
		ball.AddTool(GetDlgItem(IDC_BUTTON_SHAKE),"���ʹ��ڶ���");
	}
	Ishake=1;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CClientDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_strSend=="")
	{
		MessageBox("��Ϣ����Ϊ��");
		return;
	}
	if(m_strUser==cuser)
	{
		MessageBox("���ܸ��Լ�����Ϣ");
		return;
	}
	if(m_strSend=="\/shake")
	{
		OnButtonShake();
	}
	else
	{
		CTime t=CTime::GetCurrentTime();
		CString strTime="(%m-%d %H:%M:%S)";
		strTime=t.Format(strTime);
		int type;
		if(m_strAll==1&&m_strUser!="���")
		{
			type=2;
			m_strRecv=m_strRecv+"\r\n"+strTime+"���"+m_strUser+"˵:"+m_strSend;
			m_strSend=strTime+cuser+"����˵:"+m_strSend;
		}
		else if(m_strUser=="���")
		{
			type=1;
			m_strRecv=m_strRecv+"\r\n"+strTime+"��Դ��˵:"+m_strSend;
			m_strSend=strTime+cuser+"�Դ��˵:"+m_strSend;
		}
		else if(m_strUser!="���")
		{
			type=1;
			m_strRecv=m_strRecv+"\r\n"+strTime+"���"+m_strUser+"˵:"+m_strSend;
			m_strSend=strTime+cuser+"��"+m_strUser+"˵:"+m_strSend;
		}
		if(m_strSend.GetLength()>400)
		{
			MessageBox("��������");
			return;
		}
		SendMsg(type,m_strUser,m_strSend);
		GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
		CEdit *p=new CEdit;
		p=(CEdit *)GetDlgItem(IDC_EDIT_RECV);
		p->LineScroll(p->GetLineCount(),0);
	}
	m_strSend=_T("");
	m_ctrSend.SetFocus();
	GetDlgItem(IDC_EDIT_SEND)->SetWindowText(m_strSend);
}
LRESULT CClientDlg::OnReceive(WPARAM wParam,LPARAM lParam)
{
	sendinfo info;
	int lEvent=WSAGETSELECTEVENT(lParam);
	switch(lEvent)
	{
	case FD_READ:
		{
			int BRead=recv(clientsock, (char*)&info,sizeof(info), 0);
			if(BRead==0)
			{	
				MessageBox("�����ж�");
				return 0;
			}
			if (BRead == SOCKET_ERROR)
			{
				MessageBox("���յ�һ��������Ϣ. ");
				return 0;
			}
			switch(info.type)
			{
			case 0://new people in
				{
					CString m;
					m=info.msg;
					if(m!="")
					{
						m_strRecv=m_strRecv+m;
						GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
					}
					m_ctrUser.AddString(info.name);
					m_ctrList.AddString(info.name);
				}
				break;
			case 1://to all or not private
				m_strMe=m_ctrMe.GetCheck();
				if(m_strMe==1&&cuser!=info.name)
					return 0;
				{
					CString shake;
					shake=info.msg;
					if(shake=="\\shake")
					{
						Shaking();
						return 0;
					}
				}
				m_strRecv=m_strRecv+"\r\n\r\n"+info.msg;
				GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
				break;
			case 2://to me only ˽��
				{
					CString shake;
					shake=info.msg;
					if(shake=="\\shake")
					{	
						Shaking();
						return 0;
					}
				}
				m_strRecv=m_strRecv+"\r\n\r\n"+info.msg;
				GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
				break;
			case 3:// people out
				m_strRecv=m_strRecv+"\r\n\r\n"+info.msg;
				UpdateData(false);
				m_ctrUser.DeleteString(m_ctrUser.FindString(1,info.name));
				m_ctrList.DeleteString(m_ctrList.FindString(1,info.name));
				break;
			case 4://�쳣�¼�
				{
				CString msg;
				msg=info.msg;
				MessageBox(msg);
				break;
				}
			case 5://�ļ���������
				{
					CString sender1;
					int k=0,i=0;
					char *p;
					for(int j=0;j<4;j++,k++)
					{
						i=0;
						p=&info.msg[k];
						while(info.msg[k]!='|')
						{
							k++;i++;
						}
						info.msg[k]='\0';
						if(j==0)
							f_ip=p;
						else if(j==1)
							sender1=p;
						else if(j==2)
							m_strFileName=p;
						else 
							m_dwFileSize=atoi(p);
					}
					if(IsTrans==1)
					{
						SendMsg(7,sender1,"�Է����ڴ����ļ������Ժ�!");
						return 0;
					}
					sender=sender1;
					CString filesize;
					filesize=KindChange(m_dwFileSize);
					filesize="("+filesize+")";
					m_strRecv=m_strRecv+"\r\n\r\n  "+sender+"������㷢���ļ�  "+m_strFileName+filesize;
					GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
					GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);
					m_ctrEnd.ShowWindow(SW_SHOW);
					m_ctrEnd.SetWindowText("�ܾ�");
					GetDlgItem(IDC_STATIC_RECV)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC_RECV)->SetWindowText("�����ļ���");
					GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(m_strFileName);
					GetDlgItem(IDC_STATIC_FILESIZE)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC_FILESIZE)->SetWindowText(filesize);
					GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC2)->SetWindowText("0%");
					GetDlgItem(IDC_BUTTON_RECV)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_BUTTON_TANS)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("�ٶ�: 0");
					if(this->IsWindowVisible()!=1)
					{
						::SetForegroundWindow(m_hWnd);
						::ShowWindow(m_hWnd,SW_SHOW);
					}
					IsTrans=1;
				}
				break;
			case 6://��ʼ���������߳� 
				m_strRecv=m_strRecv+"\r\n"+info.msg;
				GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
				AfxBeginThread(SendDataThread,this,THREAD_PRIORITY_NORMAL);
				break;
			case 7://�ļ���������ֹ�˴���,�Է��ܾ��ļ�
				IsEnd=1;	IsTrans=0;
				m_strRecv=m_strRecv+"\r\n"+info.msg;
				GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
				Hide();
				break;
			}
			break;
		}
	case FD_CLOSE:
		{
			m_strRecv=m_strRecv+"\r\n"+"�����������ѶϿ�!  �����µ�½";
			GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
			break;
		}
	}
	CEdit *p=new CEdit;
	p=(CEdit *)GetDlgItem(IDC_EDIT_RECV);
	p->LineScroll(p->GetLineCount(),0);
	PlaySound(MAKEINTRESOURCE(IDR_WAVE1),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);// 
/*  // 1����ð�����Դ��ģ������
	HMODULE hmod=AfxGetResourceHandle(); 
//����2��������Դ����Ϣ��
����HRSRC hSndResource=FindResource(hmod,MAKEINTRESOURCE(IDR_WAVE1),_T("WAVE"));
//����3. װ����Դ���ݲ�������
����HGLOBAL hGlobalMem=LoadResource(hmod,hSndResource);
LPCTSTR lpMemSound=(LPCSTR)LockResource(hGlobalMem);
��//��4�����������ļ���
����sndPlaySound(lpMemSound,SND_MEMORY);
//����5���ͷ���Դ�����
����FreeResource(hGlobalMem);*/
	HWND Hwnd;
	Hwnd=::GetForegroundWindow();
	if(this->IsWindowVisible()!=1)
		SetTimer(1,300,0);
	else if(Hwnd!=m_hWnd)
		this->FlashWindow(1);
	return 0;
}

void CClientDlg::OnSelchangeListUser() 
{
	// TODO: Add your control notification handler code here
	int i=m_ctrList.GetCurSel();
	m_ctrList.GetText(i,m_strUser);
//	GetDlgItem(IDC_COMBO_USER)->SetWindowText(m_strUser);
	m_ctrUser.SetCurSel(m_ctrUser.FindString(i,m_strUser));
//	m_ctrUser.SelectString(0,m_strUser);
	m_ctrSend.SetFocus();
}

void CClientDlg::OnMin() 
{
	// TODO: Add your control notification handler code here
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA); 
	m_nid.hWnd = this->m_hWnd; 
	m_nid.uID =IDR_MAINFRAME; 
	m_nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
	m_nid.uCallbackMessage = WM_SHOWTASK;

	//�Զ������Ϣ���� WM_SHOWTASK ͷ�����ж���ΪWM_USER+1
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	CString title;
	title="QC:"+cuser;
	strcpy(m_nid.szTip,title);//������������ʱ������ʾ������ 
	Shell_NotifyIcon(NIM_ADD,&m_nid);//�����������ͼ�� 
	this->ShowWindow(SW_HIDE);
	m_nid.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	KillTimer(1);
}
LRESULT CClientDlg::onShowTask(WPARAM wParam,LPARAM lParam)   
         //wParam���յ���ͼ���ID����lParam���յ���������Ϊ   
{  
	SetForegroundWindow();
	if(wParam!=IDR_MAINFRAME)   
		return   1;   
	switch(lParam)   
	{   
		case   WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�   
		{   
				LPPOINT   lpoint=new   tagPOINT;   
                 ::GetCursorPos(lpoint);//�õ����λ��   
                  CMenu   menu;   
                  menu.CreatePopupMenu();//����һ������ʽ�˵�   
                  //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ���   
                  //���أ��������������   
                  menu.AppendMenu(MF_STRING,WM_DESTROY,"�ر�");
                  //ȷ������ʽ�˵���λ��   
                  menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);   
                  //��Դ����   
                  HMENU   hmenu=menu.Detach();   
                  menu.DestroyMenu();   
                  delete   lpoint;   

		}   
          break;   
          case   WM_LBUTTONDBLCLK://˫������Ĵ���   
          {   
               this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶�  
			   KillTimer(1);		
			   m_nid.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
				Shell_NotifyIcon(NIM_MODIFY, &m_nid);

          }   
          break;   
    }   
          return   0;  
}
void CClientDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		{
			if(m_nid.hIcon == AfxGetApp()->LoadIcon(IDR_MAINFRAME))
				m_nid.hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
			else m_nid.hIcon =AfxGetApp()->LoadIcon(IDR_MAINFRAME);
			Shell_NotifyIcon(NIM_MODIFY, &m_nid);
		}
		break;
	case 2:
		{
			CString filesize;
			filesize=KindChange(ByteTotal-pb);
			pb=ByteTotal;
			filesize="�ٶ�:"+filesize+"/s";
			GetDlgItem(IDC_STATIC_SPEED)->SetWindowText(filesize);
		}
		break;
	case 3:
		{
			Ishake=1;
			KillTimer(3);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CClientDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	if(IsTrans==1)
		SendMsg(7,sender,"\r\n  �Է����˳�!");
	Shell_NotifyIcon(NIM_DELETE,&m_nid);//��������ɾ��ͼ�� 
}

BOOL CClientDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	switch(pMsg->message) 
	{ 
//		case WM_LBUTTONDOWN: 
//		case WM_LBUTTONUP: 
//		case WM_MOUSEMOVE: 
		ball.RelayEvent(pMsg); 
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

void CClientDlg::OnButtonTans() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_strUser==cuser)
	{
		MessageBox("���ܸ��Լ����ļ�");
		return;
	}
	if(m_strUser=="���")
	{
		MessageBox("���ܸ������˷��ļ�");
		return;
	}
   //���ļ��Ի��� 
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,  "�����ļ� (*.*)|*.*||"); 
	if(dlg.DoModal()==IDOK) 
	{ 
		m_strFilePath=dlg.GetPathName(); 
		m_strFileName=dlg.GetFileName();
		CFile file(m_strFilePath, CFile::modeRead);
		//��ȡ�ļ���С
		m_dwFileSize=file.GetLength();
		//�ر��ļ�
		file.Close();
		CString filesize;
		filesize=KindChange(m_dwFileSize);
		GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);
		m_ctrEnd.ShowWindow(SW_SHOW);
		m_ctrEnd.SetWindowText("ȡ��");
		GetDlgItem(IDC_STATIC_RECV)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_RECV)->SetWindowText("�����ļ���");
		GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(m_strFileName);
		GetDlgItem(IDC_STATIC_FILESIZE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FILESIZE)->SetWindowText(filesize);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC2)->SetWindowText("0%");
		GetDlgItem(IDC_BUTTON_TANS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SPEED)->SetWindowText("�ٶ�: 0");
		CString message;filesize.Format("%d",m_dwFileSize);
		message=cuser+"|"+m_strFileName+"|"+filesize+"|";
		SendMsg(5,m_strUser,message);
		Sendto=m_strUser;
		IsTrans=1;
	} 
}
	//���������߳� 
UINT SendDataThread(LPVOID lpParam)
{
	CClientDlg *pDlg=(CClientDlg *)lpParam; 
	CFile file;
	if(!file.Open(pDlg->m_strFileName,CFile::modeRead)) 
	{ 
		AfxMessageBox("���ļ�����"); 
		pDlg->GetDlgItem(IDC_BUTTON_TANS)->ShowWindow(SW_SHOW);
		pDlg->Hide();
		return 0; 
	} 	
	CSocket sockTemp;
	sockTemp.Create(2000);
	sockTemp.Listen(1);
	CSocket sockSend;
	sockTemp.Accept(sockSend);
	AfxMessageBox("�ɹ�");
	pDlg->IsEnd=0;
	pDlg->m_ctrProgress.SetRange(0,100);
	int BufSize=SENDDATE;
	int ByteNum=0,nLen=0,pos=0,pos1=0;
	pDlg->ByteTotal=0;pDlg->pb=0;
	CString str,str1;
	LPBYTE pBuf = new BYTE[BufSize]; 
	file.Seek(0, CFile::begin);
	pDlg->SendMsg(2,pDlg->Sendto,"  ��ʼ��������.......");
	pDlg->SetTimer(2,500,0);
	while(1)
	{
		if(pDlg->IsEnd==1)
			break;
		//һ�ζ�ȡBufSize��С���ļ�����
		nLen=file.Read(pBuf,BufSize);

		if(nLen==0)
			break;
		ByteNum=sockSend.Send(pBuf, nLen);
		if(ByteNum==SOCKET_ERROR)
		{ 
			AfxMessageBox("����ʧ�ܣ�"); 
			break;
		}
		pDlg->ByteTotal+=ByteNum;
		pos=int(pDlg->ByteTotal*100.0/pDlg->m_dwFileSize);
		if(pos1!=pos)
		{
			pos1=pos;
			pDlg->m_ctrProgress.SetPos(pos);
		}
		str.Format("%d",pos);
		if(str1!=str)
		{
			str1=str;
			str=str+"%";
			pDlg->GetDlgItem(IDC_STATIC2)->SetWindowText(str); 
		}
	}
	pDlg->KillTimer(2);
	sockSend.Close();
//	Sleep(200);
	file.Close();
	pDlg->IsEnd=0;
	pDlg->IsTrans=0;
	pDlg->Hide();
	pDlg->m_ctrProgress.SetPos(0);
	if(pDlg->ByteTotal==pDlg->m_dwFileSize)
		pDlg->SendMsg(2,pDlg->Sendto,"  �ļ��������!");
	return 0;
}
	//���������߳� 
UINT ReceiveDataThread(LPVOID lpParam)	
{
	CClientDlg *pDlg=(CClientDlg *)lpParam; 
	//�����ļ��Ի��� 
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"�����ļ� (*.*)|*.*||"); 
	strcpy(dlg.m_ofn.lpstrFile, pDlg->m_strFileName.GetBuffer(pDlg->m_strFileName.GetLength()));
	while(dlg.DoModal()!=IDOK) 
	{ 
		return 0;
	}     
	pDlg->GetDlgItem(IDC_BUTTON_END)->SetWindowText("��ֹ");
	CSocket sockRecv;
	if(sockRecv.Create(2001)==SOCKET_ERROR)
		sockRecv.Create();
	pDlg->GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(dlg.GetFileName());
	pDlg->GetDlgItem(IDC_BUTTON_RECV)->ShowWindow(SW_HIDE);//���հ�ť 
	pDlg->m_ctrProgress.SetRange(0,100);
	pDlg->SendMsg(6,pDlg->sender,"\r\n  �Է��ѽ������󣬽�������.......");
	while(sockRecv.Connect(pDlg->f_ip,2000)==0)
	{
		Sleep(50); 
	} 
	CFile file; 
	if(!file.Open(dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite))
	{ 
		AfxMessageBox("���ļ�����"); 
		return 0; 
	} 	
	pDlg->IsEnd=0;
	int BufSize=SENDDATE;
	int ByteNum=0,pos,pos1=0;
	pDlg->ByteTotal=pDlg->pb=0;
	CString str,str1;
	LPBYTE pBuf = new BYTE[BufSize]; 
	pDlg->SendMsg(2,pDlg->sender,"  ��ʼ��������.......");
	pDlg->SetTimer(2,500,0);
	while(1)
	{
		if(pDlg->IsEnd==1)
			break;
		//һ�ζ�ȡBufSize��С���ļ�����
		ByteNum=sockRecv.Receive(pBuf,BufSize);
		if(ByteNum==SOCKET_ERROR) 
		{ 
			AfxMessageBox("���մ���"); 
			break;
		}
		if(ByteNum==0)
			break;
		file.Write(pBuf,ByteNum);
		pDlg->ByteTotal+=ByteNum;
		pos=int(pDlg->ByteTotal*100.0/pDlg->m_dwFileSize);
		if(pos1!=pos)
		{
			pos1=pos;
			pDlg->m_ctrProgress.SetPos(pos);
		}
		str.Format("%d",pos);
		if(str1!=str)
		{
			str1=str;
			str=str+"%";
			pDlg->GetDlgItem(IDC_STATIC2)->SetWindowText(str); 
		}
	}
	pDlg->KillTimer(2);
	sockRecv.Close();	
	pDlg->IsEnd=0;
	pDlg->IsTrans=0;
	file.Close();
	pDlg->Hide();
	pDlg->m_ctrProgress.SetPos(0);
	if(pDlg->ByteTotal==pDlg->m_dwFileSize)
		pDlg->SendMsg(2,pDlg->sender,"  �ļ��������!");
	return 0;
}

void CClientDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	CString kind;
	IsEnd=1;	IsTrans=0;
	GetDlgItem(IDC_BUTTON_END)->GetWindowText(kind);
	if(kind=="�ܾ�")
	{
		m_strRecv=m_strRecv+"\r\n\r\n"+"  ���ܾ��˶Է��������ļ����䱻ȡ��";
		SendMsg(7,sender,"\r\n  �Է��ܾ�����������!");
		Hide();
	}
	else if(kind=="ȡ��")
	{
		m_strRecv=m_strRecv+"\r\n\r\n"+"  ��ȡ�����ļ�����";
		SendMsg(7,Sendto,"\r\n  �Է�ȡ�����ļ�����!");
		Hide();
	}
	else if(kind=="��ֹ")
	{
		m_strRecv=m_strRecv+"\r\n\r\n"+"  ����ֹ���ļ�����";
		SendMsg(7,sender,"\r\n  �Է���ֹ���ļ�����!");
	}
	GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
	CEdit *p=new CEdit;
	p=(CEdit *)GetDlgItem(IDC_EDIT_RECV);
	p->LineScroll(p->GetLineCount(),0);
}

void CClientDlg::OnButtonRecv() 
{
	// TODO: Add your control notification handler code here
	AfxBeginThread(ReceiveDataThread,this,THREAD_PRIORITY_NORMAL); 
}

void CClientDlg::Hide()
{
		GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
		m_ctrEnd.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RECV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FILESIZE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_TANS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_RECV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
}

void CClientDlg::SendMsg(int type, CString name, CString msg)
{
	sendinfo info;
	info.type=type;
	memcpy(info.name,name,name.GetLength()+1);
	memcpy(info.msg,msg,msg.GetLength()+1);
	int charsend=send(clientsock,(char*)&info,sizeof(info),0);
	if(charsend==SOCKET_ERROR)
	{
		MessageBox("���͹����з���һ������",NULL,MB_OK);
		return;
	}
}

CString CClientDlg::KindChange(DWORD dwFileSize)
{
	float size;
	CString filesize;
	if(dwFileSize<1024)
	{
		filesize.Format("%d�ֽ�",dwFileSize);
	}
	else if(dwFileSize<(1024*1024))
	{
		size=dwFileSize*1.0/1024;
		filesize.Format("%.1fKB",size);
	}
	else
	{
		size=dwFileSize*1.0/(1024*1024);
		filesize.Format("%.1fMB",size);
	}
	return filesize;
}

void CClientDlg::OnButtonShake() 
{
	// TODO: Add your control notification handler code here
	if(Ishake==0)
		return;
	UpdateData(true);
	int type;
	if(m_strUser!="���")
	{
		type=2;
		m_strSend=" "+cuser+"���㷢���˴��ڶ���";
	}
	else if(m_strUser=="���")
	{
		type=1;
		m_strSend="  "+cuser+"�Դ�ҷ����˴��ڶ���";
	}
	SendMsg(type,m_strUser,"\\shake");
	SendMsg(type,m_strUser,m_strSend);
	Shaking();
	m_strSend=_T("");
	m_ctrSend.SetFocus();
	GetDlgItem(IDC_EDIT_RECV)->SetWindowText(m_strRecv);
	CEdit *p=new CEdit;
	p=(CEdit *)GetDlgItem(IDC_EDIT_RECV);
	p->LineScroll(p->GetLineCount(),0);
	Ishake=0;
	SetTimer(3,10000,0);
}

void CClientDlg::Shaking()
{
	int ty=3;
		CRect   m_rect; 
		this->ShowWindow(SW_SHOW);
//		HWND Hwnd;
//		Hwnd=::GetForegroundWindow();
		::SetForegroundWindow(m_hWnd);
		::GetWindowRect(m_hWnd,&m_rect);  
		int recordy=m_rect.left;
		int recordx=m_rect.top;
		for(int i=0;i<3;i++)
		{
			m_rect.left=recordy;
			m_rect.top=recordx;
			m_rect.top = m_rect.top + ty;  
			m_rect.left = m_rect.left - ty;
			::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
			m_rect.top = m_rect.top -ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
			m_rect.top = m_rect.top -ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
			m_rect.left=m_rect.left+ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
			m_rect.left=m_rect.left+ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
			m_rect.top = m_rect.top + ty;  
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
			m_rect.top=m_rect.top+ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
			m_rect.top=m_rect.top+ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);Sleep(20);
			::SetWindowPos( m_hWnd,NULL,recordy,recordx,0,0,SWP_NOSIZE);Sleep(3);
		}
}

HBRUSH CClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HBRUSH 	hbr=CreateSolidBrush(RGB(250,250,250));
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
//DEL //	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//DEL  CTLCOLOR_BTN����ť�ؼ���
//DEL  CTLCOLOR_DLG���Ի���
//DEL  CTLCOLOR_EDIT���༭�ؼ���
//DEL  CTLCOLOR_LISTBOX���б��
//DEL  CTLCOLOR_MSGBOX����Ϣ��
//DEL  CTLCOLOR_SCROLLBAR����������
//DEL  CTLCOLOR_STATIC����̬��
//DEL /*	if(nCtlColor==CTLCOLOR_EDIT)
//DEL 	{	
//DEL //			pDC->SetBkMode(TRANSPARENT);
//DEL //			pDC->SetBkColor(RGB(255,0,255));
//DEL //			�˴������������ɫ
//DEL 			pDC->SetTextColor(RGB(0,0,180));
//DEL 	//		return m_hbr1;


