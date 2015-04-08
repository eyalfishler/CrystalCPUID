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

#if !defined(AFX_MATRIXSTATIC_H__EA81982F_BB69_4A1A_870D_E62D412FB448__INCLUDED_)
#define AFX_MATRIXSTATIC_H__EA81982F_BB69_4A1A_870D_E62D412FB448__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MatrixStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMatrixStatic window
class CMatrixStatic : public CStatic
{
// Construction
public:

	CMatrixStatic();
enum {SMALL = 0, LARGE, TINY};
enum {LEFT = 0, RIGHT, UP, DOWN};


// Attributes
public:

// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatrixStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetAutoPadding(bool pad, char ch = ' ');
	int  GetText(LPTSTR string,  int size);
	void SetDisplayColors(COLORREF bk,  COLORREF on,  COLORREF off);
	void SetBkColor(COLORREF bk);
	void SetColor(COLORREF off,  COLORREF on);
	void SetBitmapResource(UINT bitmap);
	void SetSize(int size = SMALL);
	void SetText(LPCTSTR lpszText);
	void StopScroll();
	void DoScroll(int speed, BOOL dir);
	void SetCustomCharSizes(int width, int height, int xspace,  int yspace);
	void AdjustClientYToSize(int size);
	void AdjustClientXToSize(int size);
	void SetNumberOfLines(int max = 0);
	void SetXCharsPerLine(int max = 0);

	virtual ~CMatrixStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMatrixStatic)
	// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:

	bool GetCharBmpOffset(RECT *rc, char ch);

	char		m_cPadChar;
	CString		m_csStrText;
	bool		m_bModified, m_bImmediateUpdate, m_btimer, m_bAutoPad;	
	COLORREF	m_crOffColor,	m_crOnColor, m_crBackColor;
	UINT		m_iImagematrix, m_iTimer;
	int			m_icharwidth, m_icharheight, m_ixspacing, m_iyspacing;
	int			m_iMaxYChars, m_iMaxXChars, m_iDirection;
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATRIXSTATIC_H__EA81982F_BB69_4A1A_870D_E62D412FB448__INCLUDED_)
