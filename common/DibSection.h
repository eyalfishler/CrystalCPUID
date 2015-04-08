//
//	DIB Section
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef	__DibSection_h__
#define	__DibSection_h__

#define USEPNG
#define USEJPEG

//
//	DIB セクションクラス
//
class CDibSection {
  public:
	CDibSection();
	CDibSection(const CDibSection &dib);
	~CDibSection();

	BOOL Create(int width, int height, int depth);
	void Destroy();

	BOOL LoadImage(const char *path);

	BOOL LoadBmp(const char *path);
	BOOL SaveBmp(const char *path);

#ifdef	USEJPEG
	BOOL LoadJpeg(const char *path);
#endif

#ifdef	USEPNG
	BOOL LoadPng(const char *path, bool alpha_include=false);
	BOOL SavePng(const char *path);
#endif
	void Gray(); // by hiyohiyo

	void CDibSection::Copy(const HDC hDC);
	void Copy(const CDibSection &dib);
	void Copy(const CDibSection &dib, CPoint to, CSize size, CPoint from);
	void Mix(const CDibSection &dib, CPoint to, CSize size, CPoint from,
		COLORREF transparent = RGB(0, 255, 0));
	void Fade(const CDibSection &dib, int level);
	void Fade(const CDibSection &dib1, const CDibSection &dib2, int level);
	void FadeFast(const CDibSection &dib1, const CDibSection &dib2, int level);
	HRGN CreateRgn(COLORREF transparent = RGB(0, 255, 0));
	HRGN CreateRgn2(COLORREF transparent = RGB(0, 255, 0));
	void FillRect(const CRect &rect, COLORREF color);

	void Draw(HDC dc, int x, int y, int w, int h, int ox=0, int oy=0);
	void Draw(HDC dc, const CRect &rect, CPoint point);
	void Draw(HDC dc, const CRect &rect);

	const void *GetBits() const { return Bits; }
	void *GetBits() { return Bits; }

	const void *GetBits(int x, int y) const;
	void *GetBits(int x, int y);

	BOOL IsOK() const { return Bits != 0; }
	int Width() const { return Header.Info.biWidth; }
	int Height() const { return Header.Info.biHeight; }
	int Depth() const { return Header.Info.biBitCount; }
	int BytesPerLine() const { return bytes_per_line; }
	int BytesPerPixel() const { return bytes_per_pixel; }
	HBITMAP Handle() const { return hBitmap; }

	CDibSection &operator=(const CDibSection &dib);

	static int ScanBytes(int width, int depth);
	static int PixelBytes(int depth);

  protected:
	struct	{
		BITMAPINFOHEADER	Info;
		DWORD				BitField[3];
	} Header;
	HBITMAP	hBitmap;
	void *Bits;
	int bytes_per_line;
	int bytes_per_pixel;
} ;

class CDibDC: public CDC {
  public:
	CDibDC(const CDibSection &dib);
	CDibDC(const CDibSection &dib, HDC hDC);
	~CDibDC();

  protected:
	HDC hScreenDC;
	HGDIOBJ hOldBitmap;
} ;

inline CDibDC::CDibDC(const CDibSection &dib)
{
	hScreenDC = ::GetDC(0);
	HDC	memDC = ::CreateCompatibleDC(hScreenDC);
	hOldBitmap = ::SelectObject(memDC, dib.Handle());
	Attach(memDC);
}

inline CDibDC::CDibDC(const CDibSection &dib, HDC hDC)
{
	hScreenDC = 0;
	HDC	memDC = ::CreateCompatibleDC(hDC);
	hOldBitmap = ::SelectObject(memDC, dib.Handle());
	Attach(memDC);
}

inline CDibDC::~CDibDC()
{
	::GdiFlush();
	HDC memDC = Detach();
	::SelectObject(memDC, hOldBitmap);
	::DeleteDC(memDC);
	if (hScreenDC)
		::ReleaseDC(0, hScreenDC);
}

//	インラインメンバ関数

// 1ピクセルに必要なバイト数を得る

inline int CDibSection::PixelBytes(int depth)
{
	return (depth + 7) / 8;
}

// 1ラインに必要なバイト数を得る

inline int CDibSection::ScanBytes(int width, int depth)
{
	return ((width * PixelBytes(depth) + 3) / 4) * 4;
}

// 与えた座標のポインタを得る

inline const void *CDibSection::GetBits(int x, int y) const
{
	return (const void *)((const char *)GetBits()
		+ (Height() - y - 1) * bytes_per_line + x * bytes_per_pixel);
}

inline void *CDibSection::GetBits(int x, int y)
{
	return (void *)((char *)GetBits()
		+ (Height() - y - 1) * bytes_per_line + x * bytes_per_pixel);
}

//	描画
//
//		３種類あるが、引数の型が異なるだけで動作は同じ
//
inline void CDibSection::Draw(HDC dc, int x, int y, int w, int h, int ox, int oy)
{
	BitBlt(dc, x, y, w, h, CDibDC(*this, dc), ox, oy, SRCCOPY);
}

inline void CDibSection::Draw(HDC dc, const CRect &rect, CPoint point)
{
	Draw(dc, rect.left, rect.top, rect.Width(), rect.Height(), point.x, point.y);
}

inline void CDibSection::Draw(HDC dc, const CRect &rect)
{
	Draw(dc, rect.left, rect.top, rect.Width(), rect.Height(), rect.left, rect.top);
}

#endif
