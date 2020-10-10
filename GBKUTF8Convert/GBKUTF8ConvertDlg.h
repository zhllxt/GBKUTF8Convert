
// GBKUTF8ConvertDlg.h: 头文件
//

#pragma once

#include <string>

// CGBKUTF8ConvertDlg 对话框
class CGBKUTF8ConvertDlg : public CDialog
{
// 构造
public:
	CGBKUTF8ConvertDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GBKUTF8CONVERT_DIALOG };
	CCheckListBox m_List;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();

// 实现
protected:
	HICON m_hIcon;

// 生成的消息映射函数
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonGbkToUtf8();
	afx_msg void OnBnClickedButtonUtf8ToGbk();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
