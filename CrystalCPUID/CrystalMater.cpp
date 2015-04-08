// CrystalMater.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CrystalMater.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrystalMater

CCrystalMater::CCrystalMater()
{
	m_Cstr = "";
	m_Width = 0;
	m_MaxWidth = 10000;
	m_FontSize = 240;
	m_FontFace = "Arial";
	m_MaterColor = RGB(240,240,255);
	m_x = 4;
	m_y = 0;

	m_MColor[0] = RGB(0,0,0);
	m_MColor[1] = RGB(0,0,0);
	m_MColor[2] = RGB(0,0,0);
	m_MColor[3] = RGB(0,0,0);

	m_SColor[0] = RGB(128,128,255);
	m_SColor[1] = RGB(128,128,255);
	m_SColor[2] = RGB(128,128,255);
	m_SColor[3] = RGB(64,64,255);
}

CCrystalMater::~CCrystalMater()
{
}

BEGIN_MESSAGE_MAP(CCrystalMater, CStatic)
	//{{AFX_MSG_MAP(CCrystalMater)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrystalMater メッセージ ハンドラ

void CCrystalMater::OnPaint() 
{
	CRect rc;
	GetClientRect(&rc);
	CPaintDC dc(this);

	m_Font.CreatePointFont( m_FontSize, m_FontFace, NULL);
	CFont* oldFont = dc.SelectObject(&m_Font);

	dc.Rectangle(0,0,rc.Width(),rc.Height());

	CBrush myBrush,*pOldBrush;
	CPen myPen,*pOldPen;

	myPen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	pOldPen = dc.SelectObject(&myPen);

	myBrush.CreateSolidBrush(m_MaterColor);
	pOldBrush = dc.SelectObject(&myBrush);

	int width = rc.Width() * m_Width / m_MaxWidth;
	if( width >= rc.Width() ){
		width = rc.Width();
	}

	dc.Rectangle(0,0,width,rc.Height());

	dc.SelectObject(pOldPen);
	myPen.DeleteObject();
	dc.SelectObject(pOldBrush);
	myBrush.DeleteObject();

	dc.SetBkMode(TRANSPARENT);

	dc.SetTextColor(m_SColor[0]);	dc.TextOut(m_x + 0,m_y + 0,m_Cstr,m_Cstr.GetLength());
	dc.SetTextColor(m_SColor[1]);	dc.TextOut(m_x + 0,m_y + 1,m_Cstr,m_Cstr.GetLength());
	dc.SetTextColor(m_SColor[2]);	dc.TextOut(m_x + 1,m_y + 0,m_Cstr,m_Cstr.GetLength());
	dc.SetTextColor(m_SColor[3]);	dc.TextOut(m_x + 1,m_y + 1,m_Cstr,m_Cstr.GetLength());

	dc.SelectObject(oldFont);
	m_Font.DeleteObject();
}

void CCrystalMater::SetString(CString cstr) 
{
	m_Cstr = cstr;
	Invalidate(TRUE);
}

void CCrystalMater::SetWidth(int width)
{
	m_Width = width;
	m_Cstr.Format("%d",width);
	Invalidate(TRUE);
}

void CCrystalMater::SetFontSize(int size)
{
	m_FontSize = size;
	Invalidate(TRUE);
}

void CCrystalMater::SetFontFace(CString face)
{
	m_FontFace = face;
	Invalidate(TRUE);
}

void CCrystalMater::SetMaxWidth(int maxwidth)
{
	m_MaxWidth = maxwidth;
	Invalidate(TRUE);
}

void CCrystalMater::SetFontColor(COLORREF color1,COLORREF color2,COLORREF color3,COLORREF color4)
{
	m_SColor[0] = color1;
	m_SColor[1] = color2;
	m_SColor[2] = color3;
	m_SColor[3] = color4;
	Invalidate(TRUE);
}

void CCrystalMater::SetMaterColor(COLORREF color)
{
	m_MaterColor = color;
	Invalidate(TRUE);
}

