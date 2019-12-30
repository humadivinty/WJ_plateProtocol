
// TestTool_WJSWDLLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestTool_WJSWDLL.h"
#include "TestTool_WJSWDLLDlg.h"
#include "afxdialogex.h"
#include <direct.h>
#include <string>

#include "../WJSWDLL/WJSWDLL.h"
#ifdef DEBUG
#pragma comment(lib, "../debug/WJSWDLL.lib" )
#else
#pragma comment(lib, "../release/WJSWDLL.lib" )
#endif // DEBUG

#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int g_MsgPLATENO = ::RegisterWindowMessage("PLATENO");

#define MAX_IMG_BUF_SIZE (10*1024*1024)
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestTool_WJSWDLLDlg �Ի���



CTestTool_WJSWDLLDlg::CTestTool_WJSWDLLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestTool_WJSWDLLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestTool_WJSWDLLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestTool_WJSWDLLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_WVS_Initialize, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsInitialize)
    ON_REGISTERED_MESSAGE(g_MsgPLATENO, OnUpdatePlateData)
    ON_BN_CLICKED(IDC_BUTTON_WVS_CloseHv, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsClosehv)
    ON_BN_CLICKED(IDC_BUTTON_WVS_Settime, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsSettime)
    ON_BN_CLICKED(IDC_BUTTON_WVS_GetBigImage, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGetbigimage)
    ON_BN_CLICKED(IDC_BUTTON_WVS_GetFarBigImage, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGetfarbigimage)
    ON_BN_CLICKED(IDC_BUTTON_WVS_GetSmallImage, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGetsmallimage)
    ON_BN_CLICKED(IDC_BUTTON_WVS_GetPlateNo, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGetplateno)
    ON_BN_CLICKED(IDC_BUTTON_WVS_ForceSendLaneHv, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsForcesendlanehv)
    ON_BN_CLICKED(IDC_BUTTON_WVS_StartRealPlay, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsStartrealplay)
    ON_BN_CLICKED(IDC_BUTTON_WVS_StopRealPlay, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsStoprealplay)
    ON_BN_CLICKED(IDC_BUTTON_WVS_Startrecord, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsStartrecord)
    ON_BN_CLICKED(IDC_BUTTON_WVS_Stoprecord, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsStoprecord)
    ON_BN_CLICKED(IDC_BUTTON_WVS_GetHvIsConnected, &CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGethvisconnected)
END_MESSAGE_MAP()


// CTestTool_WJSWDLLDlg ��Ϣ�������

BOOL CTestTool_WJSWDLLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    memset(m_chCurrentDir, '\0', sizeof(m_chCurrentDir));
    _getcwd(m_chCurrentDir, sizeof(m_chCurrentDir));
    
    CString str;
    str.Format("msg = %d", g_MsgPLATENO);
    ShowMessage(str);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestTool_WJSWDLLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestTool_WJSWDLLDlg::OnPaint()
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
HCURSOR CTestTool_WJSWDLLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CTestTool_WJSWDLLDlg::OnUpdatePlateData(WPARAM wParam, LPARAM lParam)
{
    int iLaneID = (int)wParam;
    CString strLog;
    strLog.Format("receive msg from iLaneID = %d", iLaneID);
    ShowMessage(strLog);

    return 0;
}

void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsInitialize()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int iRet = WVS_Initialize();
    
    char chLog[256] = {0};
    sprintf_s(chLog, sizeof(chLog), "WVS_Initialize = %d", iRet);
    ShowMessage(chLog);
}

bool CTestTool_WJSWDLLDlg::GetItemText(int ItemID, char* buffer, size_t bufSize)
{
    CString strTemp;
    GetDlgItem(ItemID)->GetWindowText(strTemp);
    if (strTemp.GetLength() < bufSize)
    {
        //sprintf(buffer, "%s", strTemp.GetBuffer());
        sprintf_s(buffer, bufSize, "%s", strTemp.GetBuffer());
        strTemp.ReleaseBuffer();
        return true;
    }
    return false;
}

void CTestTool_WJSWDLLDlg::ShowMsg(CEdit *pEdit, CString strMsg)
{
    if (pEdit == NULL)
    {
        return;
    }

    CTime  time = CTime::GetCurrentTime();
    CString strTmp;
    pEdit->GetWindowText(strTmp);
    if (strTmp.IsEmpty() || strTmp.GetLength() > 4096)
    {
        strTmp = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
        strTmp += strMsg;
        pEdit->SetWindowText(strTmp);
        return;
    }

    strTmp += _T("\r\n");
    strTmp += time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
    strTmp += strMsg;
    pEdit->SetWindowText(strTmp);

    if (pEdit != NULL)
    {
        pEdit->LineScroll(pEdit->GetLineCount() - 1);
    }
}

void CTestTool_WJSWDLLDlg::ShowMessage(CString strMsg)
{
    CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MSG);
    ShowMsg(pEdit, strMsg);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsClosehv()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    int iRet = WVS_CloseHv();

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_CloseHv = %d", iRet);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsSettime()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = {0};
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    int iRet = WVS_Settime(iLaneID);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_Settime(%d) = %d",iLaneID,  iRet);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGetbigimage()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    int iIdentNo = 0;
    int iImageBufSize = MAX_IMG_BUF_SIZE;
    
    if (m_pImgBuffer == NULL)
    {
        m_pImgBuffer = new unsigned char[MAX_IMG_BUF_SIZE];
        memset(m_pImgBuffer, 0, MAX_IMG_BUF_SIZE);
    }
    else
    {
        memset(m_pImgBuffer, 0, MAX_IMG_BUF_SIZE);
    }

    int iRet = WVS_GetBigImage(iLaneID, iIdentNo, (char*)m_pImgBuffer, iImageBufSize, &iImageBufSize);

    if (0 == iRet
        && iImageBufSize > 0)
    {
        char chFileName[256] = {0};
        sprintf_s(chFileName, sizeof(chFileName), "%s\\result\\%d-%lu_big.jpg", m_chCurrentDir, iLaneID, GetTickCount());
        Tool_SaveFile(chFileName, m_pImgBuffer, iImageBufSize);
    }

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_GetBigImage(%d) = %d, image length = %d", iLaneID, iRet, iImageBufSize);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGetfarbigimage()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    int iIdentNo = 0;
    int iImageBufSize = MAX_IMG_BUF_SIZE;

    if (m_pImgBuffer == NULL)
    {
        m_pImgBuffer = new unsigned char[MAX_IMG_BUF_SIZE];
        memset(m_pImgBuffer, 0, MAX_IMG_BUF_SIZE);
    }
    else
    {
        memset(m_pImgBuffer, 0, MAX_IMG_BUF_SIZE);
    }

    int iRet = WVS_GetFarBigImage(iLaneID, iIdentNo, (char*)m_pImgBuffer, iImageBufSize, &iImageBufSize);

    if (0 == iRet
        && iImageBufSize > 0)
    {
        char chFileName[256] = { 0 };
        sprintf_s(chFileName, sizeof(chFileName), "%s\\result\\%d-%lu_far_big.jpg", m_chCurrentDir, iLaneID, GetTickCount());
        Tool_SaveFile(chFileName, m_pImgBuffer, iImageBufSize);
    }

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_GetFarBigImage(%d) = %d, image length = %d", iLaneID, iRet, iImageBufSize);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGetsmallimage()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    int iIdentNo = 0;
    int iImageBufSize = MAX_IMG_BUF_SIZE;

    if (m_pImgBuffer == NULL)
    {
        m_pImgBuffer = new unsigned char[MAX_IMG_BUF_SIZE];
        memset(m_pImgBuffer, 0, MAX_IMG_BUF_SIZE);
    }
    else
    {
        memset(m_pImgBuffer, 0, MAX_IMG_BUF_SIZE);
    }

    int iRet = WVS_GetSmallImage(iLaneID, iIdentNo, (char*)m_pImgBuffer, iImageBufSize, &iImageBufSize);

    if (0 == iRet
        && iImageBufSize > 0)
    {
        char chFileName[256] = { 0 };
        sprintf_s(chFileName, sizeof(chFileName), "%s\\result\\%d-%lu_small.jpg", m_chCurrentDir, iLaneID, GetTickCount());
        Tool_SaveFile(chFileName, m_pImgBuffer, iImageBufSize);
    }

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_GetSmallImage(%d) = %d, image length = %d", iLaneID, iRet, iImageBufSize);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGetplateno()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);
    int iIdentNo = 0;

    char chPlateNumber[64] = {0};
    int iRet = WVS_GetPlateNo(iLaneID, iIdentNo, chPlateNumber, sizeof(chPlateNumber));

    if (0 == iRet)
    {
        char chFileName[256] = { 0 };
        sprintf_s(chFileName, sizeof(chFileName), "%s\\result\\%d-%lu_plate.txt", m_chCurrentDir, iLaneID, GetTickCount());
        Tool_SaveFile(chFileName, chPlateNumber, sizeof(chPlateNumber));
    }

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "chPlateNumber(%d) = %d, PlateNumber = %s", iLaneID, iRet, chPlateNumber);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsForcesendlanehv()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);
    int iImageOutType = 0;

    int iRet = WVS_ForceSendLaneHv(iLaneID, iImageOutType);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_ForceSendLaneHv(%d) = %d, ", iLaneID, iRet);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsStartrealplay()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    int iRet = WVS_StartRealPlay(iLaneID, GetDlgItem(IDC_STATIC_Vdieo)->GetSafeHwnd());

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_StartRealPlay(%d) = %d, ", iLaneID, iRet);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsStoprealplay()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    int iRet = WVS_StopRealPlay(iLaneID);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_StopRealPlay(%d) = %d, ", iLaneID, iRet);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsStartrecord()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    char chVideoName[256] = {0};
    sprintf_s(chVideoName, sizeof(chVideoName), "%s\\%d-%lu.mp4",m_chCurrentDir, iLaneID, GetTickCount());

    int iRet = WVS_Startrecord(iLaneID, chVideoName);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_Startrecord(%d, %s) = %d, ", iLaneID, chVideoName, iRet);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsStoprecord()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    int iRet = WVS_Stoprecord(iLaneID);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_Stoprecord(%d) = %d, ", iLaneID,  iRet);
    ShowMessage(chLog);
}


void CTestTool_WJSWDLLDlg::OnBnClickedButtonWvsGethvisconnected()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    char chValue[256] = { 0 };
    GetItemText(IDC_EDIT_LANEID, chValue, sizeof(chValue));
    int iLaneID = atoi(chValue);

    int iRet = WVS_GetHvIsConnected(iLaneID);

    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "WVS_GetHvIsConnected(%d) = %d, ", iLaneID, iRet);
    ShowMessage(chLog);
}

bool CTestTool_WJSWDLLDlg::Tool_SaveFile(const char* szPath, void* pbData, size_t dataSize)
{
    if (NULL == szPath
        || NULL == pbData
        || dataSize <= 0)
    {
        return false;
    }

    char chLogBuff[MAX_PATH] = { 0 };
    bool bRet = false;

    if (NULL != strstr(szPath, "\\") || NULL != strstr(szPath, "/"))
    {
        std::string tempFile(szPath);
        size_t iPosition = std::string::npos;
        if (NULL != strstr(szPath, "\\"))
        {
            iPosition = tempFile.rfind("\\");
        }
        else
        {
            iPosition = tempFile.rfind("/");
        }
        std::string tempDir = tempFile.substr(0, iPosition + 1);
        if (!MakeSureDirectoryPathExists(tempDir.c_str()))
        {
            memset(chLogBuff, '\0', sizeof(chLogBuff));
            //sprintf_s(chLogBuff, "%s save failed", chImgPath);
            sprintf_s(chLogBuff, sizeof(chLogBuff), "%s save failed, create path failed.", szPath);
        }
    }

    size_t iWritedSpecialSize = 0;
    FILE* fp = NULL;
    //fp = fopen(chImgPath, "wb+");
    errno_t errCode;
    _set_errno(0);
    errCode = fopen_s(&fp, szPath, "wb+");
    if (fp)
    {
        //iWritedSpecialSize = fwrite(pImgData, dwImgSize , 1, fp);
        iWritedSpecialSize = fwrite(pbData, sizeof(BYTE), dataSize, fp);
        fflush(fp);
        fclose(fp);
        fp = NULL;
        bRet = true;
    }
    else
    {
        memset(chLogBuff, '\0', sizeof(chLogBuff));
        //sprintf_s(chLogBuff, "%s save failed", chImgPath);
        sprintf_s(chLogBuff, sizeof(chLogBuff), "%s open failed, error code = %d", szPath, errCode);
    }
    return bRet;
}
