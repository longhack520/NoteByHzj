#pragma once


// CAdd2Dlg 对话框

class CAdd2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdd2Dlg)

public:
	CAdd2Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdd2Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
