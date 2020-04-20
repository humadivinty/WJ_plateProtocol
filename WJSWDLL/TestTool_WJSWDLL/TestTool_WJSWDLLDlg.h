
// TestTool_WJSWDLLDlg.h : ͷ�ļ�
//

#pragma once


// CTestTool_WJSWDLLDlg �Ի���
class CTestTool_WJSWDLLDlg : public CDialogEx
{
// ����
public:
	CTestTool_WJSWDLLDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTTOOL_WJSWDLL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnUpdatePlateData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonWvsInitialize();

    bool GetItemText(int ItemID, char* buffer, size_t bufSize);
    void ShowMsg(CEdit *pEdit, CString strMsg);
    void ShowMessage(CString strMsg);
    afx_msg void OnBnClickedButtonWvsClosehv();
    afx_msg void OnBnClickedButtonWvsSettime();
    afx_msg void OnBnClickedButtonWvsGetbigimage();

public:
    unsigned char* m_pImgBuffer;    
    char m_chCurrentDir[256];
    int m_iCameraMode; 
    bool m_bAutoInvok;

    afx_msg void OnBnClickedButtonWvsGetfarbigimage();
    afx_msg void OnBnClickedButtonWvsGetsmallimage();
    afx_msg void OnBnClickedButtonWvsGetplateno();
    afx_msg void OnBnClickedButtonWvsForcesendlanehv();
    afx_msg void OnBnClickedButtonWvsStartrealplay();
    afx_msg void OnBnClickedButtonWvsStoprealplay();
    afx_msg void OnBnClickedButtonWvsStartrecord();
    afx_msg void OnBnClickedButtonWvsStoprecord();
    afx_msg void OnBnClickedButtonWvsGethvisconnected();

    bool Tool_SaveFile(const char* fileName, void* pbData, size_t dataSize);
    afx_msg void OnBnClickedButtonWvsGetrecord();
    afx_msg void OnBnClickedRadioOpen();
    afx_msg void OnBnClickedRadioClose();
};
