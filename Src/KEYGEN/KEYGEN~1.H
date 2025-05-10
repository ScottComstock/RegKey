// KeyGenDlg.h : header file
//

#if !defined(AFX_KEYGENDLG_H__51BE5B06_5E2B_11D2_A1AF_904E4BE4013B__INCLUDED_)
#define AFX_KEYGENDLG_H__51BE5B06_5E2B_11D2_A1AF_904E4BE4013B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "keygenc.h"

/////////////////////////////////////////////////////////////////////////////
// CKeyGenDlg dialog

class CKeyGenDlg : public CDialog
{
// Construction
public:
	CKeyGenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyGenDlg)
	enum { IDD = IDD_KEYGEN_DIALOG };
  CButton m_generateButton;
  CButton m_deleteButton;
  CButton m_infoButton;
  CListBox    m_appList;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  int     m_type;
  KGINFO  *m_pKGInfo;
  void    UpdateButtons();
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKeyGenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnClickedButtonGenerate();
  afx_msg void OnClickedButtonAdd();
  afx_msg void OnClickedButtonInfo();
  afx_msg void OnClickedButtonDelete();
  afx_msg void OnSelchangeListApps();
  afx_msg void OnDblclkListApps();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGenerateDialog dialog

class CGenerateDialog : public CDialog
{
// Construction
public:
    CGenerateDialog(CWnd* pParent = NULL);  // standard constructor
    CGenerateDialog(KGINFO *pKGInfo, unsigned short int uAppNum, CWnd* pParent = NULL);

// Dialog Data
    //{{AFX_DATA(CGenerateDialog)
    enum { IDD = IDD_DIALOG_GENERATE };
    CStatic m_passwordStatic;
    CStatic m_filenameStatic;
    CStatic m_appName;
    CButton m_OKButton;
    CString m_password;
    CString m_filename;
    CString m_registrationString;
    CString m_seed;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void CheckOK();
    virtual BOOL displayKey(char *pszMessage, char *pszKey, BOOL bUserEntered);
    
    KGINFO *m_pKGInfo;
    unsigned short int m_uAppNum;
    BOOL m_fileBased;

    // Generated message map functions
    //{{AFX_MSG(CGenerateDialog)
    afx_msg void OnChangeEditString();
    virtual BOOL OnInitDialog();
    afx_msg void OnChangeEditSeed();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CNewAppDialog dialog

class CNewAppDialog : public CDialog
{
// Construction
public:
    CNewAppDialog(CWnd* pParent = NULL);    // standard constructor
    CNewAppDialog(KGINFO *pKGInfo, CWnd* pParent = NULL);
    
    BOOL m_added;

// Dialog Data
    //{{AFX_DATA(CNewAppDialog)
    enum { IDD = IDD_DIALOG_NEW_APP };
    CButton m_OKButton;
    CString m_appName;
    CString m_password;
    BOOL    m_needsPassword;
    CString m_genCode;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void AfxDataInit(void);
    KGINFO  *m_pKGInfo;
    virtual void CheckOK();

    // Generated message map functions
    //{{AFX_MSG(CNewAppDialog)
    virtual BOOL OnInitDialog();
    afx_msg void OnChangeEditName();
    afx_msg void OnChangeEditPassword();
    virtual void OnOK();
    afx_msg void OnChangeEditGeneration();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CPasswordDialog dialog

class CPasswordDialog : public CDialog
{
// Construction
public:
    CPasswordDialog(CWnd* pParent = NULL);  // standard constructor
    
    BOOL m_choseOK;

// Dialog Data
    //{{AFX_DATA(CPasswordDialog)
    enum { IDD = IDD_DIALOG_PASSWORD };
    CButton m_buttonOK;
    CString m_password;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CPasswordDialog)
    virtual void OnOK();
    afx_msg void OnChangeEditPassword();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CAppInfoDialog dialog

class CAppInfoDialog : public CDialog
{
// Construction
public:
    CAppInfoDialog(CWnd* pParent = NULL);   // standard constructor
    CAppInfoDialog(unsigned short int uAppNum, CString password, KGINFO *pKGInfo, CWnd* pParent = NULL);

// Dialog Data
    //{{AFX_DATA(CAppInfoDialog)
    enum { IDD = IDD_DIALOG_APPINFO };
    CButton m_OKButton;
    CString m_password;
    CString m_appName;
    CString m_generationCode;
    //}}AFX_DATA
    
    BOOL m_nameChanged;

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void checkOK();
    
    unsigned short int m_uAppNum;
    CString m_originalPassword;
    CString m_originalName;
    KGINFO *m_pKGInfo;
    KGAPPINFO m_KGAppInfo;
    

    // Generated message map functions
    //{{AFX_MSG(CAppInfoDialog)
    virtual void OnOK();
    afx_msg void OnChangeEditGeneration();
    afx_msg void OnChangeEditName();
    afx_msg void OnChangeEditPassword();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CRegInfoDialog dialog

class CRegInfoDialog : public CDialog
{
// Construction
public:
    CRegInfoDialog(CWnd* pParent = NULL);   // standard constructor

// Interface
    void SetKey(const char *pszKey, const char *pszString);

// Dialog Data
    //{{AFX_DATA(CRegInfoDialog)
    enum { IDD = IDD_DIALOG_KEYINFO };
    CStatic m_regKey;
    //}}AFX_DATA

// Implementation
protected:
    CString sRegKey;
    CString sRegString;
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CRegInfoDialog)
    afx_msg void OnClickedButtonCopy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYGENDLG_H__51BE5B06_5E2B_11D2_A1AF_904E4BE4013B__INCLUDED_)
