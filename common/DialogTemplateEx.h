/////////////////////////////////////////////////////////////////////////////
// CDialogTemplate

class CDialogTemplateEx
{
// Constructors
public:
	CDialogTemplateEx(const DLGTEMPLATE* pTemplate = NULL);
	CDialogTemplateEx(HGLOBAL hGlobal);

// Attributes
	BOOL HasFont() const;
	BOOL SetFont(LPCTSTR lpFaceName, WORD nFontSize);
	BOOL SetSystemFont(WORD nFontSize = 0);
	BOOL GetFont(CString& strFaceName, WORD& nFontSize) const;
	void GetSizeInDialogUnits(SIZE* pSize) const;
	void GetSizeInPixels(SIZE* pSize) const;

	static BOOL AFX_CDECL GetFont(const DLGTEMPLATE* pTemplate,
		CString& strFaceName, WORD& nFontSize);

// Operations
	BOOL Load(LPCTSTR lpDialogTemplateID);
	HGLOBAL Detach();

// Implementation
public:
	~CDialogTemplateEx();

	HGLOBAL m_hTemplate;
	DWORD m_dwTemplateSize;
	BOOL m_bSystemFont;

protected:
	static BYTE* AFX_CDECL GetFontSizeField(const DLGTEMPLATE* pTemplate);
	static UINT AFX_CDECL GetTemplateSize(const DLGTEMPLATE* pTemplate);
	BOOL SetTemplate(const DLGTEMPLATE* pTemplate, UINT cb);
};
