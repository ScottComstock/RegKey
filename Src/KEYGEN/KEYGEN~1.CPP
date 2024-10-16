// KeyGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyGen.h"
#include "KeyGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyGenDlg dialog

CKeyGenDlg::CKeyGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyGenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyGenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pKGInfo = new KGINFO;

	kgInit("", m_pKGInfo);
}

void CKeyGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyGenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, ID_BUTTON_GENERATE, m_generateButton);
    DDX_Control(pDX, ID_BUTTON_DELETE, m_deleteButton);
    DDX_Control(pDX, ID_BUTTON_INFO, m_infoButton);
    DDX_Control(pDX, ID_LIST_APPS, m_appList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyGenDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyGenDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(ID_BUTTON_GENERATE, OnClickedButtonGenerate)
  ON_BN_CLICKED(ID_BUTTON_ADD, OnClickedButtonAdd)
  ON_BN_CLICKED(ID_BUTTON_INFO, OnClickedButtonInfo)
  ON_BN_CLICKED(ID_BUTTON_DELETE, OnClickedButtonDelete)
  ON_LBN_SELCHANGE(ID_LIST_APPS, OnSelchangeListApps)
  ON_LBN_DBLCLK(ID_LIST_APPS, OnDblclkListApps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyGenDlg message handlers

BOOL CKeyGenDlg::OnInitDialog()
{
  unsigned short int uApp;

	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

  for(uApp=0; uApp<m_pKGInfo->uNumApps; ++uApp)
     {
     m_appList.AddString(m_pKGInfo->appInfo[uApp].szAppName);
     }
       
  if(m_pKGInfo->uNumApps > 0)
     {
     m_appList.SetCurSel(0);
     }
       
  UpdateButtons();

  return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyGenDlg::OnPaint() 
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

HCURSOR CKeyGenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// Globals

BOOL isOkaySeed(CString sSeed)
{
    int iCount;
    char cCurrent;
    
    if(sSeed.GetLength() < 10)
       {
       return(FALSE);
       }
    
    for(iCount = 0; iCount < 10; ++iCount)
       {
       cCurrent = sSeed[iCount];
       if(!(cCurrent >= 'A' && cCurrent <= 'Z') &&
          !(cCurrent >= '0' && cCurrent <= '9'))
          {
          return(FALSE);
          }
       }
    return(TRUE);
}


void CKeyGenDlg::OnDblclkListApps()
{
    OnClickedButtonGenerate();    
}


void CKeyGenDlg::OnClickedButtonGenerate()
{
    int iCurSel = m_appList.GetCurSel();
    CString appName;
    unsigned short int uAppNum;

    if(iCurSel == LB_ERR)
       {
       MessageBox("No application selected", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
       }
    else
       {
       m_appList.GetText(iCurSel, appName);

       uAppNum = kgIndexOf(appName, m_pKGInfo);

       CGenerateDialog dlg(m_pKGInfo, uAppNum);
       dlg.SetParent(this);
       dlg.DoModal();   
       }
}

void CKeyGenDlg::OnClickedButtonAdd()
{
    CNewAppDialog dlg(m_pKGInfo);

    if(okayToAdd(m_pKGInfo))
       {
       dlg.SetParent(this);
       dlg.DoModal();
       if(dlg.m_added)
          {
          m_appList.AddString(dlg.m_appName);
          m_appList.SetCurSel(m_appList.FindStringExact(-1, dlg.m_appName));
          UpdateButtons();
          }
       }
    else
       {
       MessageBox("Unable to add another application", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
       }
}


void CKeyGenDlg::UpdateButtons()
{
    int iCurSel = m_appList.GetCurSel();
    CWnd *hasFocus = GetFocus();
    
    if(iCurSel == LB_ERR)
       {          
			 if(hasFocus != NULL)
			 {
         if(hasFocus->m_hWnd == m_infoButton.m_hWnd ||
          hasFocus->m_hWnd == m_deleteButton.m_hWnd ||
          hasFocus->m_hWnd == m_generateButton.m_hWnd)
          {
          m_appList.SetFocus();
          }
			 }
       m_infoButton.EnableWindow(FALSE);
       m_deleteButton.EnableWindow(FALSE);
       m_generateButton.EnableWindow(FALSE);
       }
    else
       {
       m_infoButton.EnableWindow(TRUE);
       m_deleteButton.EnableWindow(TRUE);
       m_generateButton.EnableWindow(TRUE);
       }
}


void CKeyGenDlg::OnClickedButtonDelete()
{
    int iCurSel = m_appList.GetCurSel();
    CString messageStr;
    CString appName;
    
    if(iCurSel == LB_ERR)
       {
       MessageBox("No application selected", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
       }
    else
       {
       m_appList.GetText(iCurSel, appName);
       messageStr = "Are you sure you wish to delete \"";
       messageStr += appName;
       messageStr += "\"?";
       if(MessageBox(messageStr, "Delete Application", MB_YESNO|MB_ICONQUESTION) == IDYES)
          {
          if(kgDelete(kgIndexOf(appName, m_pKGInfo), m_pKGInfo) == RK_SUCCESS)
             {
             m_appList.DeleteString(iCurSel);
             UpdateButtons();
             }
          else
             {
             MessageBox("Unable to delete application", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
             }
          }
       }
}

void CKeyGenDlg::OnSelchangeListApps()
{
   UpdateButtons();
}

void CKeyGenDlg::OnClickedButtonInfo()
{
        int iCurSel = m_appList.GetCurSel();
        CString appName;
        CString password = "";
        unsigned short int uAppNum;

        if(iCurSel == LB_ERR)
           {
           MessageBox("No application selected", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
           }
        else
           {
           CPasswordDialog passDlg;
           
           m_appList.GetText(iCurSel, appName);

           uAppNum = kgIndexOf(appName, m_pKGInfo);

           if(passwordNeeded(uAppNum, m_pKGInfo))
              {
              CPasswordDialog passDlg;
           
              passDlg.SetParent(this);
              passDlg.DoModal();
              if(passDlg.m_choseOK)
                 {
                 password = passDlg.m_password;
                 if(!isPasswordValid((const char *)password, uAppNum, m_pKGInfo))
                    {
                    MessageBox("Incorrect password for application", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
                    return;
                    }
                 }
              else
                 {
                 return;
                 }
              }

           CAppInfoDialog infoDlg(uAppNum, password, m_pKGInfo);
           infoDlg.SetParent(this);
           infoDlg.DoModal();
           
           if(infoDlg.m_nameChanged)
              {
              m_appList.DeleteString(iCurSel);
              m_appList.AddString(infoDlg.m_appName);
              m_appList.SetCurSel(m_appList.FindStringExact(-1, infoDlg.m_appName));
              }
           }
}

/////////////////////////////////////////////////////////////////////////////
// CGenerateDialog dialog

CGenerateDialog::CGenerateDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CGenerateDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CGenerateDialog)
    m_password = "";
    m_filename = "";
    m_registrationString = "";
    m_seed = "";
    //}}AFX_DATA_INIT
}


CGenerateDialog::CGenerateDialog(KGINFO *pKGInfo, unsigned short int uAppNum, CWnd* pParent)
    : CDialog(CGenerateDialog::IDD, pParent)
{
    m_pKGInfo = pKGInfo;
    m_uAppNum = uAppNum;

    m_password = "";
    m_registrationString = "";
    m_seed = "";

    m_fileBased = fileBased(m_uAppNum, m_pKGInfo);

    if(m_fileBased)
       {
       m_filename = "REGISTER.KEY";
       }
    else
       {
       m_filename = "";
       }
}


void CGenerateDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGenerateDialog)
    DDX_Control(pDX, IDC_STATIC_PASSWORD, m_passwordStatic);
    DDX_Control(pDX, IDC_STATIC_FILENAME, m_filenameStatic);
    DDX_Control(pDX, IDC_STATIC_NAME, m_appName);
    DDX_Control(pDX, IDOK, m_OKButton);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
    DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
    DDX_Text(pDX, IDC_EDIT_STRING, m_registrationString);
    DDX_Text(pDX, IDC_EDIT_SEED, m_seed);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGenerateDialog, CDialog)
    //{{AFX_MSG_MAP(CGenerateDialog)
    ON_EN_CHANGE(IDC_EDIT_STRING, OnChangeEditString)
    ON_EN_CHANGE(IDC_EDIT_SEED, OnChangeEditSeed)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CGenerateDialog::CheckOK()
{
    UpdateData(TRUE);
                     
    if(m_registrationString.IsEmpty() || !isOkaySeed(m_seed))
       {
       m_OKButton.EnableWindow(FALSE);
       }
    else
       {
       m_OKButton.EnableWindow(TRUE);
       }
}

/////////////////////////////////////////////////////////////////////////////
// CGenerateDialog message handlers

BOOL CGenerateDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    m_OKButton.EnableWindow(FALSE);

    m_appName.SetWindowText(m_pKGInfo->appInfo[m_uAppNum].szAppName);

    if(m_fileBased)
       {
       m_filenameStatic.EnableWindow();
       GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow();
       }
    if(passwordNeeded(m_uAppNum, m_pKGInfo))
       {
       m_passwordStatic.EnableWindow();
       GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow();
       }
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}


void CGenerateDialog::OnChangeEditString()
{
    char szWork[13];

    CheckOK();
    
    if(m_fileBased)
       {
       UpdateData(TRUE);

       filenameFromString(szWork, (const char *)m_registrationString);

       m_filename = szWork;
       
       UpdateData(FALSE);
       }
}


void CGenerateDialog::OnChangeEditSeed()
{
    CheckOK();
}


void CGenerateDialog::OnOK()
{
    char szResult[200];
    char szKey[21];
    char bUserEntered;

    UpdateData(TRUE);

    if(kgGen(m_uAppNum, (const char *)m_registrationString,
       (const char *)m_seed, (const char *)m_password,
       (const char *)m_filename, szResult, m_pKGInfo, szKey,
       &bUserEntered) == RK_SUCCESS)
       {
       if(displayKey(szResult, szKey, (BOOL)bUserEntered))
          {
          if(m_fileBased)
             {
             m_filename = "REGISTER.KEY";
             }
          m_registrationString = "";
          m_seed = "";
          UpdateData(FALSE);
          GetDlgItem(IDC_EDIT_STRING)->SetFocus();
          }
       else
          {    
          CDialog::OnOK();
          }
       }
    else
       {
       MessageBox(szResult, "KeyGen", MB_OK|MB_ICONEXCLAMATION);
       }
}


BOOL CGenerateDialog::displayKey(char *pszMessage, char *pszKey,
    BOOL bUserEntered)
   {
   CRegInfoDialog regInfoDialog;
   
   if(bUserEntered)
      {
      regInfoDialog.SetKey(pszKey, m_registrationString);
      return(regInfoDialog.DoModal() == IDOK);
      }
   else
      {
      return(MessageBox(pszMessage, "KeyGen", MB_YESNO|MB_ICONINFORMATION) == IDYES);
      }
   }



/////////////////////////////////////////////////////////////////////////////
// CNewAppDialog dialog

CNewAppDialog::CNewAppDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CNewAppDialog::IDD, pParent)
{
    AfxDataInit();
}

CNewAppDialog::CNewAppDialog(KGINFO *pKGInfo, CWnd* pParent /*=NULL*/)
    : CDialog(CNewAppDialog::IDD, pParent)
{
    AfxDataInit();
    
    m_pKGInfo = pKGInfo;
}


void CNewAppDialog::AfxDataInit(void)
{
    //{{AFX_DATA_INIT(CNewAppDialog)
    m_appName = "";
    m_password = "";
    m_needsPassword = FALSE;
    m_genCode = "";
    //}}AFX_DATA_INIT
}


void CNewAppDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewAppDialog)
    DDX_Control(pDX, IDOK, m_OKButton);
    DDX_Text(pDX, IDC_EDIT_NAME, m_appName);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
    DDX_Check(pDX, IDC_CHECK_PASSWORD, m_needsPassword);
    DDX_Text(pDX, IDC_EDIT_GENERATION, m_genCode);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewAppDialog, CDialog)
    //{{AFX_MSG_MAP(CNewAppDialog)
    ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
    ON_EN_CHANGE(IDC_EDIT_PASSWORD, OnChangeEditPassword)
    ON_EN_CHANGE(IDC_EDIT_GENERATION, OnChangeEditGeneration)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewAppDialog message handlers

BOOL CNewAppDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    CheckRadioButton(IDC_RADIO_USER, IDC_RADIO_FILE, IDC_RADIO_USER);
    //CheckRadioButton(IDC_RADIO_GENERATE, IDC_RADIO_PREVIOUS, IDC_RADIO_GENERATE);
    m_OKButton.EnableWindow(FALSE);
    
    m_added = FALSE;
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNewAppDialog::OnChangeEditName()
{
    CheckOK();
}


void CNewAppDialog::OnChangeEditGeneration()
{
    CheckOK();  
}


void CNewAppDialog::OnChangeEditPassword()
{
    UpdateData(TRUE);
    
    if(m_password.IsEmpty())
       {
       CheckDlgButton(IDC_CHECK_PASSWORD,0);
       }
    else
       {
       CheckDlgButton(IDC_CHECK_PASSWORD,1);
       }
}


void CNewAppDialog::CheckOK()
{
    UpdateData(TRUE);
                     
    if(m_appName.IsEmpty() || m_genCode.GetLength() < 10)
       {
       m_OKButton.EnableWindow(FALSE);
       }
    else
       {
       m_OKButton.EnableWindow(TRUE);
       }
}
 
 
void CNewAppDialog::OnOK()
{
    char bPasswordNeeded;
    char bFileBased;

    UpdateData(TRUE);

    if(kgIndexOf(m_appName, m_pKGInfo) != 0xffff)
       {
       MessageBox("Application name already used, choose a different name", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
       GetDlgItem(IDC_EDIT_NAME)->SetFocus();
       return;
       }

    bPasswordNeeded = IsDlgButtonChecked(IDC_CHECK_PASSWORD);
    bFileBased = IsDlgButtonChecked(IDC_RADIO_FILE);

    if(m_appName.GetLength() > KG_APP_NAME_LEN-1)
       {
       m_appName = m_appName.Left(KG_APP_NAME_LEN-1);
       }

    if(kgAdd((const char *)m_appName, (const char *)m_password, 
       bPasswordNeeded, bFileBased, m_genCode, m_pKGInfo) == RK_SUCCESS)
       {
       m_added = TRUE;
       CDialog::OnOK();
       }
    else
       {
       MessageBox("Unable to add application", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
       }
}


/////////////////////////////////////////////////////////////////////////////
// CPasswordDialog dialog

CPasswordDialog::CPasswordDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CPasswordDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPasswordDialog)
    m_password = "";
    //}}AFX_DATA_INIT
}

void CPasswordDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPasswordDialog)
    DDX_Control(pDX, IDOK, m_buttonOK);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPasswordDialog, CDialog)
    //{{AFX_MSG_MAP(CPasswordDialog)
    ON_EN_CHANGE(IDC_EDIT_PASSWORD, OnChangeEditPassword)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDialog message handlers


BOOL CPasswordDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_choseOK = FALSE;
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPasswordDialog::OnOK()
{
    m_choseOK = TRUE;
    
    CDialog::OnOK();
}


void CPasswordDialog::OnChangeEditPassword()
{
    UpdateData(TRUE);

    if(m_password.IsEmpty())
       {
       m_buttonOK.EnableWindow(FALSE);
       }
    else
       {
       m_buttonOK.EnableWindow(TRUE);
       }
}


/////////////////////////////////////////////////////////////////////////////
// CAppInfoDialog dialog
CAppInfoDialog::CAppInfoDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CAppInfoDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CAppInfoDialog)
    m_password = "";
    m_appName = "";
    m_generationCode = "";
    //}}AFX_DATA_INIT
}


CAppInfoDialog::CAppInfoDialog(unsigned short int uAppNum, CString password, KGINFO *pKGInfo, CWnd* pParent)
    : CDialog(CAppInfoDialog::IDD, pParent)
{
    m_uAppNum = uAppNum;
    m_originalPassword = password;
    m_password = password;
    m_pKGInfo = pKGInfo;

    getAppInfo(m_uAppNum, (const char *)m_originalPassword, &m_KGAppInfo, m_pKGInfo);

    m_appName = m_KGAppInfo.szAppName;
    m_originalName = m_appName;
    m_generationCode = m_KGAppInfo.szGenerationCode;

    m_nameChanged = FALSE;
}


void CAppInfoDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAppInfoDialog)
    DDX_Control(pDX, IDOK, m_OKButton);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
    DDX_Text(pDX, IDC_EDIT_NAME, m_appName);
    DDX_Text(pDX, IDC_EDIT_GENERATION, m_generationCode);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAppInfoDialog, CDialog)
    //{{AFX_MSG_MAP(CAppInfoDialog)
    ON_EN_CHANGE(IDC_EDIT_GENERATION, OnChangeEditGeneration)
    ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
    ON_EN_CHANGE(IDC_EDIT_PASSWORD, OnChangeEditPassword)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppInfoDialog message handlers

void CAppInfoDialog::checkOK()
{
    UpdateData(TRUE);
                     
    if(m_appName.IsEmpty() || m_generationCode.GetLength() < 10)
       {
       m_OKButton.EnableWindow(FALSE);
       }
    else
       {
       m_OKButton.EnableWindow(TRUE);
       }
}


void CAppInfoDialog::OnOK()
{
    UpdateData(TRUE);

    m_KGAppInfo.bPasswordNeeded = IsDlgButtonChecked(IDC_CHECK_PASSWORD);
    m_KGAppInfo.bFileBased = IsDlgButtonChecked(IDC_RADIO_FILE);

    if(hasGenCodeChanged(m_uAppNum, m_originalPassword, m_generationCode, m_pKGInfo))
       {
       if(MessageBox("Generation code has been changed. Do you wish to continue, calculating a new validation code?", "KeyGen", MB_YESNO|MB_ICONQUESTION) != IDYES)
          {
          return;
          }
       }

    if(m_KGAppInfo.bPasswordNeeded && m_originalPassword != m_password)
       {
       if(MessageBox("Password has been changed. Do you wish to continue?", "KeyGen", MB_YESNO|MB_ICONQUESTION) != IDYES)
          {
          return;
          }
       }
       
    if(m_originalName != m_appName && kgIndexOf(m_appName, m_pKGInfo) != 0xffff)
       {
       MessageBox("New application name already used, choose a different name", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
       GetDlgItem(IDC_EDIT_NAME)->SetFocus();
       return;
       }

    strcpy(m_KGAppInfo.szAppName, m_appName.Left(KG_APP_NAME_LEN-1));
    strcpy(m_KGAppInfo.szGenerationCode, m_generationCode.Left(10));

    if(m_appName.GetLength() > KG_APP_NAME_LEN-1)
       {
       m_appName = m_appName.Left(KG_APP_NAME_LEN-1);
       }

    if(updateAppInfo(m_uAppNum, m_originalPassword, m_password, &m_KGAppInfo, m_pKGInfo) == RK_SUCCESS)
       {    
       m_nameChanged = (m_originalName != m_appName);
       CDialog::OnOK();
       }
    else
       {
       MessageBox("Unable to update application information", "KeyGen", MB_OK|MB_ICONEXCLAMATION);
       }
}

void CAppInfoDialog::OnChangeEditGeneration()
{
    checkOK();  
    
    if(hasGenCodeChanged(m_uAppNum, m_originalPassword, m_generationCode, m_pKGInfo))
       {
       SetDlgItemText(IDC_VALIDATION_CODE, "Uncalculated");
       }
    else
       {
       SetDlgItemText(IDC_VALIDATION_CODE, m_KGAppInfo.szValidationCode);
       }
}

void CAppInfoDialog::OnChangeEditName()
{
    checkOK();  
}

void CAppInfoDialog::OnChangeEditPassword()
{
    UpdateData(TRUE);
    
    if(m_password.IsEmpty())
       {
       CheckDlgButton(IDC_CHECK_PASSWORD,0);
       }
    else
       {
       CheckDlgButton(IDC_CHECK_PASSWORD,1);
       }
}


BOOL CAppInfoDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    SetDlgItemText(IDC_VALIDATION_CODE, m_KGAppInfo.szValidationCode);
    
    if(m_KGAppInfo.bPasswordNeeded)
       {
       CheckDlgButton(IDC_CHECK_PASSWORD,1);
       }
    else
       {
       CheckDlgButton(IDC_CHECK_PASSWORD,0);
       }

    if(m_KGAppInfo.bFileBased)
       {
       CheckRadioButton(IDC_RADIO_USER, IDC_RADIO_FILE, IDC_RADIO_FILE);
       }
    else
       {
       CheckRadioButton(IDC_RADIO_USER, IDC_RADIO_FILE, IDC_RADIO_USER);
       }
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}
/////////////////////////////////////////////////////////////////////////////
// CRegInfoDialog dialog

CRegInfoDialog::CRegInfoDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CRegInfoDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CRegInfoDialog)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CRegInfoDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRegInfoDialog)
    DDX_Control(pDX, IDC_REGKEY, m_regKey);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegInfoDialog, CDialog)
    //{{AFX_MSG_MAP(CRegInfoDialog)
    ON_BN_CLICKED(IDC_BUTTON_COPY, OnClickedButtonCopy)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegInfoDialog message handlers

void CRegInfoDialog::SetKey(const char *pszKey, const char *pszString)
{
    sRegKey = pszKey;
    sRegString = pszString;
}


void CRegInfoDialog::OnClickedButtonCopy()
{
    CString sText = "Registration String: " + sRegString +
                    "\r\n   Registration Key: " + sRegKey + "\r\n\r\n";
    HGLOBAL hMem;
    char far *lpszMem;

    if(OpenClipboard())
    {
        EmptyClipboard();
        if((hMem = GlobalAlloc(GMEM_SHARE, sText.GetLength() + 1)) == NULL)
        {
            MessageBox("Unable to copy registration information to the clipboard because "
                   "there is not enough memory available.",
                   NULL, MB_OK|MB_ICONEXCLAMATION);
        }
        else
        {
            lpszMem = (char far *)GlobalLock(hMem);
            strcpy(lpszMem, sText);
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
        }
        CloseClipboard();
    }
    else
    {
        MessageBox("Unable to copy registration information to the clipboard because "
                   "the clipboard is clipboard in use by another application.",
                   NULL, MB_OK|MB_ICONEXCLAMATION);
    }       
}

BOOL CRegInfoDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    m_regKey.SetWindowText(sRegKey);
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}
