// MatrixStatic.cpp : implementation file
//
// Copyright (C) 2002 by Nic Wilson  http://www.nicsoft.com.au
// Written by Nic Wilson nicw@bigpond.net.au
// All rights reserved
//
// This is free software.
// This code may be used in compiled form in any way you desire. This  
// file may be redistributed unmodified by any means PROVIDING it is   
// not sold for profit without the authors written consent, and   
// providing that this notice and the authors name and all copyright   
// notices remains intact. If the source code in this file is used in   
// any  commercial application then a statement along the lines of   
// "Portions Copyright © 2002 Nic Wilson MUST be included in   
// the startup banner, "About" dialog & printed documentation.   
//
// No warrantee of any kind, expressed or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Release Date and Version:
// Version: 1.0 November 2002
//
// SEE FEATURES:
//
// Single or multilined displays.
// Three fixed sizes for performance.
// Display can be any colors of your choice.
// CStatic objects can be auto sized.
// Horizontal left or right scrolling for single lined display .
// Vertical up or down scrolling for mulilined displays.
// Display characters can be customised.

// USAGE:

// Add the matrixstatic.cpp and matrixstatic.h files to your project using 
// "Insert Files into Project" function of Visual C++ or Studio.
// This can be accessed from the workspace by right clicking on the "Source 
// Files" and "Header Files" folders.
//
//
// Add the three bitmaps supplied as resources.  THEY MUST BE GIVEN THE ID's of
// IDB_MATRIXLARGE,  IDB_MATRIXSMALL,  IDBMATRIXTINY.  These are obviously for the
// Large, medium and smaller bitmaps respectively.
//
//
// Add an  #include "MatrixStatic.h" to your dialogs .h file.
// Add a CStatic control to your dialog.
// Add a variable of "Category:  Control",  "Variable Type: CMatrixStatic".
// If CMatrixStatic is does not appear then you did not follow the above 
// correctly.  Find your projects  .CLW file and delete it.  Choose ClassWizard
// view and when prompted rebuild the CLW file and include the MatrixStatic files.
//
// If you are feeling adventurous, these are some of the things I would like to see.
// Feel free to post me the code and I will add the best to the class and give you
// credit for it here.

// Optional pixel scrolling rather than character scrolling, maybe with ScrollWindowEx()
// Some extra character graphic bitmaps
// An intelligent resing routine for the characters.
// Any improvements you can think of.

// See the project example for a working demonstration.

#include "stdafx.h"
#include "MatrixStatic.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHARWIDTH 19
#define CHARHEIGHT 27
#define XSPACING 3	
#define YSPACING 3
#define CHARWIDTHSMALL 14
#define CHARHEIGHTSMALL 20
#define XSPACINGSMALL 2	
#define YSPACINGSMALL 2

#define CHARWIDTHTINY 10
#define CHARHEIGHTINY 14
#define XSPACINGTINY 1	
#define YSPACINGTINY 1

/////////////////////////////////////////////////////////////////////////////
// CMatrixStatic

CMatrixStatic::CMatrixStatic()
{
	SetNumberOfLines(0);
	SetXCharsPerLine(0);
	m_crOnColor = RGB(63, 181, 255);
	m_crOffColor = RGB(23, 64,  103);
	m_crBackColor = RGB(0,  0,   0);
	m_btimer = false;
	m_bAutoPad = false;
}

CMatrixStatic::~CMatrixStatic()
{

}


BEGIN_MESSAGE_MAP(CMatrixStatic, CStatic)
	//{{AFX_MSG_MAP(CMatrixStatic)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatrixStatic message handlers

bool CMatrixStatic::GetCharBmpOffset(RECT *rc, char ch)
{
	int val = (int)ch;
	rc->left = (val - 32);
	if (rc->left < 32)
		rc->left *= (m_icharwidth + m_ixspacing);
	else if (rc->left < 64)
		rc->left = (rc->left - 32) * (m_icharwidth + m_ixspacing);
	else
		rc->left = (rc->left - 64) * (m_icharwidth + m_ixspacing);
	rc->top = ((val - 32)/ 32) * (m_icharheight + m_iyspacing);
	rc->right = m_icharwidth + rc->left;
	rc->bottom = m_icharheight + rc->top;
	return 0;
}

void CMatrixStatic::SetText(LPCTSTR lpszText)
{
	if (m_csStrText != lpszText)
	{
		m_csStrText = lpszText;
		m_bModified = true;
		if (m_iMaxXChars == 0)
			SetXCharsPerLine();
		if (m_iMaxYChars == 0)
			SetNumberOfLines();
		if (m_bImmediateUpdate)
			RedrawWindow(NULL, NULL,  RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
		else
			Invalidate();
	}
}

int CMatrixStatic::GetText(LPTSTR string, int size)
{
	int length = m_csStrText.GetLength();
	if (size < length || length == 0)
		return 0;
	_tcscpy((LPTSTR)string, m_csStrText.GetBuffer(length));
	return length;
}

void CMatrixStatic::SetSize(int size)
{
	if (size == SMALL)
	{
		m_icharwidth  = CHARWIDTHSMALL;
		m_icharheight = CHARHEIGHTSMALL;
		m_ixspacing   = XSPACINGSMALL;
		m_iyspacing   = YSPACINGSMALL;
		m_iImagematrix = IDB_MATRIXSMALL;
	}
	else if (size == TINY)
	{
		m_icharwidth  = CHARWIDTHTINY;
		m_icharheight = CHARHEIGHTINY;
		m_ixspacing   = XSPACINGTINY;
		m_iyspacing   = YSPACINGTINY;
		m_iImagematrix = IDB_MATRIXTINY;
	}
	else
	{
		m_icharwidth  = CHARWIDTH;
		m_icharheight = CHARHEIGHT;
		m_ixspacing   = XSPACING;
		m_iyspacing   = YSPACING;
		m_iImagematrix = IDB_MATRIXLARGE;
	}

}

void CMatrixStatic::SetBitmapResource(UINT bitmap)
{
	m_iImagematrix = bitmap;
}

void CMatrixStatic::OnTimer(UINT nIDEvent) 
{ 
	if (m_iMaxYChars == 1)
	{
		if (m_bAutoPad == true)
		{
			if (m_csStrText.GetLength() < m_iMaxXChars)
			{
				int diff =  m_iMaxXChars - m_csStrText.GetLength();
				for (;diff > 0; diff--)
					m_csStrText += m_cPadChar;
			}
		}
		if (m_iDirection  == LEFT)
			m_csStrText = m_csStrText.Mid(1) + m_csStrText[0]; //Move the first character to the end.
		else
		{
			int len = m_csStrText.GetLength();
			m_csStrText = m_csStrText.Right(1) + m_csStrText.Left(len-1); //Move the end character to the start.
		}
	}
	else if (m_iMaxYChars > 1)
	{
		if (m_bAutoPad == true)
		{
			if (m_csStrText.GetLength() < (m_iMaxXChars * m_iMaxYChars))
			{
				int diff = m_iMaxXChars * m_iMaxYChars - m_csStrText.GetLength();
				for (;diff > 0; diff--)
					m_csStrText += m_cPadChar;
			}
		}
		if (m_iDirection  == DOWN)
		{
			CString tmp = m_csStrText.Right(m_iMaxXChars);
			m_csStrText = tmp + m_csStrText.Left(m_csStrText.GetLength() - m_iMaxXChars);
		}
		else if (m_iDirection  == UP)
		{
			CString tmp = m_csStrText.Left(m_iMaxXChars);
			m_csStrText = m_csStrText.Mid(m_iMaxXChars) + tmp;
		}
		else if (m_iDirection  == LEFT)
		{
			int loop = m_csStrText.GetLength() / m_iMaxXChars;
			if (loop == 0 || m_csStrText.GetLength() < m_iMaxXChars)
				loop = 1;
			int offset = 0;
			for (;loop > 0; loop--)
			{
				m_csStrText.Insert((offset + m_iMaxXChars), m_csStrText.GetAt(offset));
				m_csStrText.Delete(offset, 1);
				offset += m_iMaxXChars;
			}
		}
		else if (m_iDirection  == RIGHT)
		{
			int loop = m_csStrText.GetLength() / m_iMaxXChars;
			if (loop == 0 || m_csStrText.GetLength() < m_iMaxXChars)
				return;
			int offset = 0;
			for (;loop > 0; loop--)
			{
				m_csStrText.Insert(offset, m_csStrText.GetAt(offset+(m_iMaxXChars-1)));
				m_csStrText.Delete(offset+(m_iMaxXChars));
				offset += m_iMaxXChars;
			}
		}
	}
	Invalidate();
} 

void CMatrixStatic::DoScroll(int speed, BOOL dir)
{
	if (m_btimer == false)
	{
		m_btimer = true;
		m_iDirection  = dir;
		m_iTimer = SetTimer(1, speed, NULL); 
		ASSERT(m_iTimer);
	}
}

void CMatrixStatic::StopScroll()
{
	if (m_btimer == true)
	{
		KillTimer(m_iTimer);
		m_btimer = false;
	}
}


void CMatrixStatic::OnPaint() 
{

COLORREF SEGM_COLORS[3] = {
	RGB(63, 181, 255),	// BLUE ON
	RGB(23, 64,  103),	// BLUE OFF
	RGB(0,  0,   0),	// BLACK
};
	RECT rc;
	CRect m_rect;
	GetClientRect(&m_rect);
	int x = 0, y = 0;
	int strlen = m_csStrText.GetLength();
	if (strlen == 0)
		return;
	CPaintDC dc(this); // device context for painting
	HBITMAP bmp;
	COLORMAP	ColorMap[4];;
	ColorMap[0].from = SEGM_COLORS[0]; 
	ColorMap[0].to  = m_crOnColor;
	ColorMap[1].from = SEGM_COLORS[1]; 
	ColorMap[1].to  = m_crOffColor;
	ColorMap[2].from = SEGM_COLORS[2]; 
	ColorMap[2].to  = m_crBackColor;
	bmp = ::CreateMappedBitmap(AfxGetInstanceHandle(), m_iImagematrix, 0, ColorMap, 3);
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);	// Select the bitmap into the in-memory DC
	CBitmap* pOldBitmap = dcMemory.SelectObject(CBitmap::FromHandle(bmp));
	int charcount = 0;
	int linecount = 1;
	HBRUSH hbBkBrush = CreateSolidBrush(m_crBackColor);
	FillRect(dc.m_hDC, &m_rect, hbBkBrush);
	for (int ix = 0; ix < strlen; ix++)
	{
		GetCharBmpOffset(&rc, (char)m_csStrText[ix]);
		dc.BitBlt(x, y, (m_icharwidth + m_ixspacing), (m_icharheight+m_iyspacing), &dcMemory, rc.left, rc.top, SRCCOPY);
		x += m_icharwidth + m_ixspacing;
		charcount++;
		if ((charcount == m_iMaxXChars) && m_iMaxYChars == 1)
			break;
		else if ((charcount == m_iMaxXChars) && m_iMaxYChars > 1)
		{
			if (linecount == m_iMaxYChars)
				break;
			x = charcount = 0;
			y += m_icharheight + m_iyspacing;
			linecount++;
		}
	}
	dcMemory.SelectObject(pOldBitmap);
	dcMemory.DeleteDC();
	DeleteObject(bmp);
	DeleteObject(hbBkBrush);
}

BOOL CMatrixStatic::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void CMatrixStatic::SetCustomCharSizes(int width, int height, int xspace, int yspace)
{
	m_icharwidth  = width;
	m_icharheight = height;
	m_ixspacing = xspace;
	m_iyspacing = yspace;
}

void CMatrixStatic::SetXCharsPerLine(int max)
{
	if (max == 0)
		m_iMaxXChars = m_csStrText.GetLength();
	else
		m_iMaxXChars = max;
}

void CMatrixStatic::SetNumberOfLines(int max)
{
	if (max == 0)
		if (m_iMaxXChars != 0)
			m_iMaxYChars = (int) m_csStrText.GetLength() / m_iMaxXChars;
		else
			m_iMaxYChars = 1;
	m_iMaxYChars = max;
}

void CMatrixStatic::AdjustClientXToSize(int size )
{
	if (m_iMaxXChars == 0)
		m_iMaxXChars = size;
	RECT rc;
	GetClientRect(&rc);
	int width =  ((m_icharwidth + m_ixspacing) * size);
	int height = rc.bottom - rc.top;
	SetWindowPos(NULL, 0, 0, width+4, height+4, SWP_NOMOVE);  
}

void CMatrixStatic::AdjustClientYToSize(int size)
{
	if (m_iMaxYChars == 0)
		m_iMaxYChars = size;
	RECT rc;
	GetClientRect(&rc);
	int width =  rc.right - rc.left;
	int height = ((m_icharheight+m_iyspacing) * size);
	SetWindowPos(NULL, 0, 0, width+4, height+4, SWP_NOMOVE);   
}

void CMatrixStatic::SetDisplayColors(COLORREF bk, COLORREF on, COLORREF off)
{
	SetBkColor(bk);
	SetColor(off, on);
}

void CMatrixStatic::SetBkColor(COLORREF bk)
{
	m_crBackColor = bk;
	Invalidate();
}

void CMatrixStatic::SetColor(COLORREF off,  COLORREF on)
{
	m_crOffColor = off;
	m_crOnColor = on;
	Invalidate();
}



void CMatrixStatic::SetAutoPadding(bool pad, char ch)
{
	m_bAutoPad = pad;
	m_cPadChar = ch;
}
