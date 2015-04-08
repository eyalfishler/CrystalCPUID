//	Based on CButtonST by Davide Calabro http://www.softechsoftware.it

#include "stdafx.h"
#include "StaticTT.h"
#include "GetFileVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStaticTT::CStaticTT()
{
	// No tooltip created
	m_ToolTip.m_hWnd = NULL;
	m_dwToolTipStyle = 0;
} // End of CStaticTT

CStaticTT::~CStaticTT()
{
} // End of ~CStaticTT

BEGIN_MESSAGE_MAP(CStaticTT, CStatic)
    //{{AFX_MSG_MAP(CStaticTT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CStaticTT::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CStatic::PreTranslateMessage(pMsg);
} // End of PreTranslateMessage

void CStaticTT::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this, m_dwToolTipStyle);
		// Create inactive
		m_ToolTip.Activate(FALSE);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	} // if
} // End of InitToolTip

// This function sets the text to show in the control tooltip.
//
// Parameters:
//		[IN]	nText
//				ID number of the string resource containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CStaticTT::SetTooltipText(int nText, BOOL bActivate)
{
	CString sText;

	// Load string resource
	sText.LoadString(nText);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetTooltipText((LPCTSTR)sText, bActivate);
} // End of SetTooltipText

// This function sets the text to show in the control tooltip.
//
// Parameters:
//		[IN]	lpszText
//				Pointer to a null-terminated string containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CStaticTT::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (lpszText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// IE 3.0 or later
	if(GetFileVersion("Comctl32.dll") < 470){
		bActivate = FALSE;
	}
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rect;
		GetClientRect(rect);
		m_ToolTip.AddTool(this, lpszText, rect, 1);
	} // if

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} // End of SetTooltipText

// This function enables or disables the control tooltip.
//
// Parameters:
//		[IN]	bActivate
//				If TRUE the tooltip will be activated.
//
void CStaticTT::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip

// This function enables the tooltip to be displayed using the balloon style.
// This function must be called before any call to SetTooltipText is made.
//
// Return value:
//		TRUE
//			Function executed successfully.
//
DWORD CStaticTT::EnableBalloonTooltip()
{
	m_dwToolTipStyle |= TTS_BALLOON;
	return TRUE;
} // End of EnableBalloonTooltip
