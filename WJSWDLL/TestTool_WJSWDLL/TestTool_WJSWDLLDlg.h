
// TestTool_WJSWDLLDlg.h : 头文件
//

#pragma once


// CTestTool_WJSWDLLDlg 对话框
class CTestTool_WJSWDLLDlg : public CDialogEx
{
// 构造
public:
	CTestTool_WJSWDLLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTTOOL_WJSWDLL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
