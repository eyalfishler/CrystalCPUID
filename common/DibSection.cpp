//
//	DIB Section
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#include "StdAfx.h"
#include "DibSection.h"

#ifdef _X86_64
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
#else
#include "PngFile.h"
#endif //_X86_64

//
// コンストラクタ
//
CDibSection::CDibSection()
	: hBitmap(0), Bits(0)
{
}

CDibSection::CDibSection(const CDibSection &dib)
	: hBitmap(0), Bits(0)
{
	if (Create(dib.Width(), dib.Height(), dib.Depth()))
		Copy(dib);
}

//
// デストラクタ
//
CDibSection::~CDibSection()
{
	Destroy();
}

//
// DIBセクションの作成
//
BOOL CDibSection::Create(int width, int height, int depth)
{
	Destroy();

	bytes_per_line = ScanBytes(width, depth);
	bytes_per_pixel = PixelBytes(depth);

	Header.Info.biSize			= sizeof(BITMAPINFOHEADER);
	Header.Info.biWidth			= width;
	Header.Info.biHeight		= height;
	Header.Info.biBitCount		= depth;
	Header.Info.biPlanes		= 1;
	Header.Info.biXPelsPerMeter	= 0;
	Header.Info.biYPelsPerMeter	= 0;
	Header.Info.biClrUsed		= 0;
	Header.Info.biClrImportant	= 0;
	Header.Info.biCompression	= depth == 24? BI_RGB: BI_BITFIELDS;
	Header.Info.biSizeImage		= bytes_per_line * height;

	switch (depth) {
	  case 16:
		Header.BitField[0] = 0x7c00;
		Header.BitField[1] = 0x03e0;
		Header.BitField[2] = 0x001f;
		break;

	  case 32:
		Header.BitField[0] = 0xff0000;
		Header.BitField[1] = 0x00ff00;
		Header.BitField[2] = 0x0000ff;
		break;

	  default:
		Header.BitField[0] = 0;
		Header.BitField[1] = 0;
		Header.BitField[2] = 0;
		break;
	}

	HDC dc = ::GetDC(0);
	hBitmap = CreateDIBSection(dc, (BITMAPINFO *)&Header, DIB_RGB_COLORS, &Bits, NULL, 0);
	::ReleaseDC(0, dc);

	return hBitmap != 0;
}

// DIBセクションの破棄
//
void CDibSection::Destroy()
{
	if (hBitmap) {
		GdiFlush();
		::DeleteObject(hBitmap);
		hBitmap = 0;
	}
}

// BMPファイルのロード
//
BOOL CDibSection::LoadBmp(const char *path)
{
	Destroy();

	hBitmap = (HBITMAP)::LoadImage(::GetModuleHandle(0), path, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (!hBitmap)
		return FALSE;

	DIBSECTION	dib;
	if (::GetObject(hBitmap, sizeof(DIBSECTION), &dib) != sizeof(DIBSECTION)) {
		::DeleteObject(hBitmap);
		hBitmap = 0;
		return FALSE;
	}
	Header.Info = dib.dsBmih;
	for (int i=0; i<3; i++)
		Header.BitField[i] = dib.dsBitfields[i];

	bytes_per_pixel = PixelBytes(dib.dsBmih.biBitCount);
	bytes_per_line = ScanBytes(dib.dsBmih.biWidth, dib.dsBmih.biBitCount);
	Bits = dib.dsBm.bmBits;

	return TRUE;
}

// BMPファイルのセーブ
//
BOOL CDibSection::SaveBmp(const char *path)
{
	TRY {
		CFile file(path, CFile::modeCreate | CFile::modeWrite);

		int length = bytes_per_line * Height();
		BITMAPFILEHEADER	header;
		memset(&header, 0, sizeof(header));
		header.bfType = (WORD)('M' << 8) | 'B';
		header.bfSize = sizeof(header) + Header.Info.biSize + length;
		header.bfOffBits = sizeof(header) + Header.Info.biSize;

		file.Write(&header, sizeof(header));
		file.Write(&Header.Info, Header.Info.biSize);
		if (Header.Info.biCompression == BI_BITFIELDS)
			file.Write(Header.BitField, sizeof(Header.BitField));
		file.Write(Bits, length);
		file.Close();
	}
	CATCH(CFileException, e) {
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

#ifdef	USEJPEG

typedef HRESULT (WINAPI * _OleLoadPictureFile_)(VARIANT var, IDispatch** pid);

BOOL CDibSection::LoadJpeg(const char *path)
{
	// ファイル名の変換
	IPicture * pip;
	VARIANT var;
	int n;
	OLE_XSIZE_HIMETRIC xn ;
	OLE_YSIZE_HIMETRIC yn ;
	int xx,yy;

	_OleLoadPictureFile_ OleLoadPictureFile = NULL;
	HINSTANCE hDLL = LoadLibrary( "oleaut32.dll" );
	if(  hDLL != NULL ){
		OleLoadPictureFile = (_OleLoadPictureFile_)GetProcAddress( hDLL, "OleLoadPictureFile" );
	}
	if( OleLoadPictureFile == NULL ){
		FreeLibrary(hDLL);
		return FALSE;
	}

	n = MultiByteToWideChar( CP_ACP, 0, path, strlen( path ), NULL, NULL );
	var.bstrVal = SysAllocStringLen( NULL, n );
	if ( var.bstrVal == NULL ) {
		FreeLibrary(hDLL);
		return FALSE;
	}
	MultiByteToWideChar( CP_ACP, 0, path, strlen( path ), var.bstrVal, n );
	var.vt = VT_BSTR;

	// COMの呼び出し
	IDispatch * pid;
	HRESULT hr = OleLoadPictureFile(var, &pid );
	if ( hr != S_OK ) {
		FreeLibrary(hDLL);
		return FALSE;
	}
	pid -> QueryInterface( IID_IPicture, (void**)&pip ); 

	// 画像の大きさを取り出す
	pip -> get_Width( &xn );
	pip -> get_Height( &yn );
	xx = MulDiv( xn, GetDeviceCaps( GetDC( HWND_DESKTOP ), LOGPIXELSX ), 2540 );
	yy = MulDiv( yn, GetDeviceCaps( GetDC( HWND_DESKTOP ), LOGPIXELSY ), 2540 );

	Destroy();
	Create(xx,yy,24);
	CDibDC cdc(*this);
	pip -> Render(cdc.m_hDC, 0, 0, xx, yy, 0, yn - 1, xn, - yn, NULL );
	pip -> Release();
	pid -> Release();
	SysFreeString(var.bstrVal);

	FreeLibrary(hDLL);
	return TRUE;
}
#endif

#ifdef	USEPNG

#ifndef	_X86_64

// stdioのラッパ
// 　デストラクタでクローズすることで、エラー処理を
// 　簡潔にするため使用
//
class CStdio {
  public:
	CStdio(const char *path, const char *mode="rb");
	~CStdio();
	int Read(void *buf, int len);
	int Write(void *buf, int len);

	FILE *fp;
} ;

// コンストラクタ（ファイルのオープン）
//
inline CStdio::CStdio(const char *path, const char *mode)
{
	fp = fopen(path, mode);
	if (fp == 0)
		throw exception("can't open");
}

// デストラクタ（ファイルのクローズ）
//
inline CStdio::~CStdio()
{
	fclose(fp);
}

// 読み込み
//
inline int CStdio::Read(void *buf, int len)
{
	return fread(buf, 1, len, fp);
}

// 書き出し
//
inline int CStdio::Write(void *buf, int len)
{
	return fwrite(buf, 1, len, fp);
}

// Rowデータクラス
// 　CDibSectionのスキャンラインごとのポインタを配列に格納する
//
class row_data {
  public:
	row_data(const CDibSection *image);
	~row_data();
	operator png_bytepp();

  private:
	png_bytepp row;
} ;

// コンストラクタ
//
inline row_data::row_data(const CDibSection *image)
{
	row = new png_byte *[image->Height()];
	for (png_uint_32 y=0; y<image->Height(); y++)
		row[y] = (png_byte *)image->GetBits(0, y);
}

// デストラクタ
//
inline row_data::~row_data()
{
	delete[] row;
}

// テーブルの取得
//
inline row_data::operator png_bytepp()
{
	return row;
}
#endif //_X86_64

// PNGファイルの読み込み
//
BOOL CDibSection::LoadPng(const char *path, bool alpha_include)
{
#ifdef _X86_64
	int xx,yy;
	WCHAR wpath[256];
	mbstowcs(wpath, path, strlen(path)+1);
	Image image(wpath);

	xx = image.GetWidth();
	yy = image.GetHeight();

	Destroy();
	Create(xx,yy,24);
	CDibDC cdc(*this);

	Graphics graphics( cdc.m_hDC );
	graphics.DrawImage(&image, 0, 0);
#else
	try {
		CStdio	file(path);

		png_byte buf[8];
		if (file.Read(buf, 8) != 8 || !png_check_sig(buf, 8))
			return FALSE;

		cpng_read	png;
		png.init_io(file.fp);
		png.set_sig_bytes(8);
		png.read_info();

		png_uint_32	width, height;
		int depth, color_type;
		png.get_IHDR(&width, &height, &depth, &color_type);

		if (alpha_include) {	// アルファ情報が必要
			if ((color_type & PNG_COLOR_MASK_ALPHA) == 0)	// アルファ情報が無い
				return false;	// アルファ情報が無ければエラー
		}
		else {					// アルファ情報は不要
			if (color_type & PNG_COLOR_MASK_ALPHA)		// アルファ情報がある
				png.set_strip_alpha();		// アルファ情報を取り除く
		}

		// 対応していないフォーマットをlibpngに変換させる

		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png.set_expand();
		if (color_type == PNG_COLOR_TYPE_GRAY && depth < 8)
			png.set_expand();
		if (png.get_valid(PNG_INFO_tRNS))
			png.set_expand();
		if (depth > 8)
			png.set_strip_16();
		if (color_type == PNG_COLOR_TYPE_GRAY)
			png.set_gray_to_rgb();

		// png_read_update_infoを呼び出した後で
		// もう一度ヘッダーを読み込む
		// ここで、読み込む情報は、expand や strip_alpha を
		// 反映した内容になっている。
		png.read_update_info();
		png.get_IHDR(&width, &height, &depth, &color_type);

		// ここで扱えるのは、PNG_COLOR_TYPE_RGB のみ
		// 変換を指示したので、通常のツールでセーブしたファイルならば
		// 読み出せるはず
		if (color_type != PNG_COLOR_TYPE_RGB)
			return FALSE;

		if (!Create(width, height, alpha_include? 32: 24))
			return FALSE;

		row_data row(this);

		// WindowsのDIBは、BGR順にデータを格納するので、
		// png_set_bgrを呼び出す。
		png.set_bgr();
		png.read_image(row);
		png.read_end();
	}
	catch (...) {
		return FALSE;
	}
#endif // _X86_64
	return TRUE;
}

// PNGファイルのセーブ
//
#ifdef _X86_64

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}
#endif

BOOL CDibSection::SavePng(const char *path)
{
#ifdef _X86_64
	WCHAR wpath[256];
	mbstowcs(wpath, path, strlen(path)+1);

	CLSID   encoderClsid;
	Status  stat;

	Bitmap *bitmap = Bitmap::FromHBITMAP(this->Handle(), NULL);

	// Get the CLSID of the PNG encoder.
	GetEncoderClsid(L"image/png", &encoderClsid);
	stat = bitmap->Save(wpath, &encoderClsid, NULL);

#else
	try {
		CStdio	file(path, "wb");

		cpng_write	png;
		png.init_io(file.fp);
		png.set_compression_level();
		png.set_filter(PNG_FILTER_TYPE_BASE, PNG_ALL_FILTERS);
//		png.set_filter(PNG_FILTER_TYPE_BASE, PNG_FILTER_SUB);
		png.set_IHDR(Width(), Height(), 8, PNG_COLOR_TYPE_RGB);
		png.write_info();
		row_data row(this);
		png.set_bgr();
		png.write_image(row);
		png.write_end();
	}
	catch (...) {
		return FALSE;
	}
#endif // _X86_64
	return TRUE;
}

#endif // PNG


// デバイスコンテキストから BitBlt 2003/10/17 by hiyohiyo
void CDibSection::Copy(const HDC hDC)
{
	BitBlt(CDibDC(*this), 0, 0, this->Width(), this->Height(), hDC, 0, 0, SRCCOPY);
}

// イメージのコピー
//
void CDibSection::Copy(const CDibSection &dib)
{
	BitBlt(CDibDC(*this), 0, 0, dib.Width(), dib.Height(), CDibDC(dib), 0, 0, SRCCOPY);
}

void CDibSection::Copy(const CDibSection &dib, CPoint to, CSize size, CPoint from)
{
	BitBlt(CDibDC(*this), to.x, to.y, size.cx, size.cy, CDibDC(dib), from.x, from.y, SRCCOPY);
}

// イメージの合成
//
void CDibSection::Mix(const CDibSection &dib, CPoint to, CSize size, CPoint from, COLORREF tc)
{
	if (Depth() == dib.Depth()) {
		switch (Depth()) {
		  case 16:
			{
				BYTE r = GetRValue(tc);
				BYTE g = GetGValue(tc);
				BYTE b = GetBValue(tc);
				WORD color = (r << 7) & 0x7c00 | (g << 2) & 0x03e0 | (b >> 3) & 0x001f;

				for (int y = 0; y < size.cy; y++) {
					WORD *p = (WORD *)GetBits(to.x, to.y + y);
					const WORD *q = (const WORD *)dib.GetBits(from.x, from.y + y);
					for (int x = 0; x < size.cx; x++) {
						if (*q != color)
							*p = *q;
						p++;
						q++;
					}
				}
			}
			break;

		  case 24:
			{
				BYTE r = GetRValue(tc);
				BYTE g = GetGValue(tc);
				BYTE b = GetBValue(tc);

				for (int y = 0; y < size.cy; y++) {
					BYTE *p = (BYTE *)GetBits(to.x, to.y + y);
					const BYTE *q = (const BYTE *)dib.GetBits(from.x, from.y + y);
					for (int x = 0; x < size.cx; x++) {
						if (q[0] != b || q[1] != g || q[2] != r) {
							p[0] = q[0];
							p[1] = q[1];
							p[2] = q[2];
						}
						p += 3;
						q += 3;
					}
				}
			}
			break;

		  case 32:
			{
				for (int y = 0; y < size.cy; y++) {
					DWORD *p = (DWORD *)GetBits(to.x, to.y + y);
					const DWORD *q = (const DWORD *)dib.GetBits(from.x, from.y + y);
					for (int x = 0; x < size.cx; x++) {
						if ((*q & 0xffffff) != tc)
							*p = *q;
						p++;
						q++;
					}
				}
			}
			break;
		}
	}
	else {
		CDibDC dst(*this);
		CDibDC src(dib);

		for (int y = 0; y < size.cy; y++) {
			for (int x = 0; x < size.cx; x++) {
				COLORREF c = src.GetPixel(from.x + x, from.y + y);
				if (c != tc)
					dst.SetPixelV(to.x + x, to.y + y, c);
			}
		}
	}
}

// 矩形の塗り潰し
//
void CDibSection::FillRect(const CRect &rect, COLORREF color)
{
	CDibDC dc(*this);
	dc.SetBkColor(color);
	dc.ExtTextOut(0, 0, ETO_OPAQUE, &rect, 0, 0, 0);
}

// 代入
//
CDibSection &CDibSection::operator=(const CDibSection &dib)
{
	if (this != &dib) {
		if (Create(dib.Width(), dib.Height(), dib.Depth()))
			Copy(dib);
	}
	return *this;
}

// リージョンデータ構築
//
class CRgnData {
  public:
	CRgnData(int _unit=256);
	~CRgnData();
	void AddRect(int x1, int y1, int x2, int y2);
	DWORD GetSize() const { return sizeof(RGNDATAHEADER) + sizeof(RECT) * nrect; }
	const RGNDATA *GetData() const { return data; }

  private:
	int unit;
	unsigned nrect;
	HANDLE handle;
	RGNDATA *data;
} ;

// コンストラクタ
//
CRgnData::CRgnData(int _unit)
	:unit(_unit), nrect(_unit)
{
	handle = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + sizeof(RECT) * nrect);
	data = (RGNDATA *)::GlobalLock(handle);
	data->rdh.dwSize = sizeof(RGNDATAHEADER);
	data->rdh.iType = RDH_RECTANGLES;
	data->rdh.nCount = 0;
	data->rdh.nRgnSize = 0;
	SetRect(&data->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
}

// デストラクタ
//
CRgnData::~CRgnData()
{
	if (handle)
		::GlobalFree(handle);
}

// リージョンデータに矩形を追加する
//
void CRgnData::AddRect(int x1, int y1, int x2, int y2)
{
	if (data->rdh.nCount >= nrect) {
		::GlobalUnlock(handle);
		nrect += unit;
		handle = ::GlobalReAlloc(handle, sizeof(RGNDATAHEADER) + sizeof(RECT) * nrect, GMEM_MOVEABLE);
		data = (RGNDATA *)::GlobalLock(handle);
	}
	RECT *pr = (RECT *)data->Buffer;
	SetRect(pr + data->rdh.nCount, x1, y1, x2, y2);
	if (x1 < data->rdh.rcBound.left)
		data->rdh.rcBound.left = x1;
	if (y1 < data->rdh.rcBound.top)
		data->rdh.rcBound.top = y1;
	if (x2 > data->rdh.rcBound.right)
		data->rdh.rcBound.right = x2;
	if (y2 > data->rdh.rcBound.bottom)
		data->rdh.rcBound.bottom = y2;
	data->rdh.nCount++;
}

// イメージからリージョンを得る
//
HRGN CDibSection::CreateRgn(COLORREF transparent)
{
	CRgnData rgndata;

	switch (Depth()) {
	  case 16:
		{
			BYTE r = GetRValue(transparent);
			BYTE g = GetGValue(transparent);
			BYTE b = GetBValue(transparent);
			WORD color = (r << 7) & 0x7c00 | (g << 2) & 0x03e0 | (b >> 3) & 0x001f;

			for (int y = 0; y < Height(); y++) {
				const WORD *p = (const WORD *)GetBits(0, y);
				for (int x = 0; x < Width(); x++) {
					int start = x;
					while (x < Width()) {
						if (*p == color)
							break;
						p++;
						x++;
					}
					if (x > start) {
						rgndata.AddRect(start, y, x, y + 1);
					}
					p++;
				}
			}
		}
		break;

	  case 24:
		{
			BYTE r = GetRValue(transparent);
			BYTE g = GetGValue(transparent);
			BYTE b = GetBValue(transparent);

			for (int y = 0; y < Height(); y++) {
				const BYTE *p = (const BYTE *)GetBits(0, y);
				for (int x = 0; x < Width(); x++) {
					int start = x;
					while (x < Width()) {
						if (p[0] == b && p[1] == g && p[2] == r)
							break;
						p += 3;
						x++;
					}
					if (x > start) {
						rgndata.AddRect(start, y, x, y + 1);
					}
					p += 3;
				}
			}
		}
		break;

	  case 32:
		{
			for (int y = 0; y < Height(); y++) {
				const DWORD *p = (const DWORD *)GetBits(0, y);
				for (int x = 0; x < Width(); x++) {
					int start = x;
					while (x < Width()) {
						if ((*p & 0xffffff) == transparent)
							break;
						p++;
						x++;
					}
					if (x > start) {
						rgndata.AddRect(start, y, x, y + 1);
					}
					p++;
				}
			}
		}
		break;

	  default:
		{
			CDibDC dc(*this);

			for (int y = 0; y < Height(); y++) {
				for (int x = 0; x < Width(); x++) {
					int start = x;
					while (x < Width()) {
						if (dc.GetPixel(x, y) == transparent)
							break;
						x++;
					}
					if (x > start) {
						rgndata.AddRect(start, y, x, y + 1);
					}
				}
			}
		}
		break;
	}
	return ::ExtCreateRegion(NULL, rgndata.GetSize(), rgndata.GetData());
}

// イメージからリージョンを得る（方法2）
//
HRGN CDibSection::CreateRgn2(COLORREF transparent)
{
	CRgnData rgndata;

	switch (Depth()) {
	  case 16:
		{
			BYTE r = GetRValue(transparent);
			BYTE g = GetGValue(transparent);
			BYTE b = GetBValue(transparent);
			WORD color = (r << 7) & 0x7c00 | (g << 2) & 0x03e0 | (b >> 3) & 0x001f;

			for (int y = 0; y < Height(); y++) {
				const WORD *p = (const WORD *)GetBits(0, y);
				for (int x = 0; x < Width(); x++) {
					int start = x;
					while (x < Width()) {
						if (*p != color)
							break;
						p++;
						x++;
					}
					if (x > start) {
						rgndata.AddRect(start, y, x, y + 1);
					}
					p++;
				}
			}
		}
		break;

	  case 24:
		{
			BYTE r = GetRValue(transparent);
			BYTE g = GetGValue(transparent);
			BYTE b = GetBValue(transparent);

			for (int y = 0; y < Height(); y++) {
				const BYTE *p = (const BYTE *)GetBits(0, y);
				for (int x = 0; x < Width(); x++) {
					int start = x;
					while (x < Width()) {
						if (p[0] != b || p[1] != g || p[2] != r)
							break;
						p += 3;
						x++;
					}
					if (x > start) {
						rgndata.AddRect(start, y, x, y + 1);
					}
					p += 3;
				}
			}
		}
		break;

	  case 32:
		{
			for (int y = 0; y < Height(); y++) {
				const DWORD *p = (const DWORD *)GetBits(0, y);
				for (int x = 0; x < Width(); x++) {
					int start = x;
					while (x < Width()) {
						if ((*p & 0xffffff) != transparent)
							break;
						p++;
						x++;
					}
					if (x > start) {
						rgndata.AddRect(start, y, x, y + 1);
					}
					p++;
				}
			}
		}
		break;

	  default:
		{
			CDibDC dc(*this);

			for (int y = 0; y < Height(); y++) {
				for (int x = 0; x < Width(); x++) {
					int start = x;
					while (x < Width()) {
						if (dc.GetPixel(x, y) != transparent)
							break;
						x++;
					}
					if (x > start) {
						rgndata.AddRect(start, y, x, y + 1);
					}
				}
			}
		}
		break;
	}

	// イメージ全体の矩形リージョンを作る
	HRGN hRgn1 = ::CreateRectRgn(0, 0, Width(), Height());
	// 透明色部分のリージョンを作る
	HRGN hRgn2 = ::ExtCreateRegion(NULL, rgndata.GetSize(), rgndata.GetData());
	// 全体から、透明色を取り除いた部分を、新しいリージョンにする
	::CombineRgn(hRgn1, hRgn1, hRgn2, RGN_DIFF);
	::DeleteObject(hRgn2);
	return hRgn1;
}

// Gray グレースケール
// 2003/10/18 by hiyohiyo
void CDibSection::Gray()
{
	switch (Depth()) {
	case 16:
	{
	/*
		for (int y = 0; y < Height(); y++) {
			WORD *p = (WORD *)GetBits(0, y);
			for (int x = 0; x < Width(); x++) {
				WORD data = *q++;
				BYTE r = ((data & 0x7c00) >> 10) * level / 256;
				BYTE g = ((data & 0x03e0) >> 5) * level / 256;
				BYTE b = (data & 0x001f) * level / 256;
				*p++ = (r << 10) & 0x7c00 | (g << 5) & 0x03e0 | b & 0x001f;
			}
		}
	*/
	}
	break;
	case 24:
	{
		int h = Height();
		int w = Width();
		for (int y = 0; y < h; y++) {
			BYTE *p = (BYTE *)GetBits(0, y);
			for (int x = 0; x < w; x++) {
				DWORD brightness = ( *p * 10 + *(p+1) * 61 + *(p+2) * 29 ) / 150;// 2/3 に輝度を下げる //
			//	if( brightness > 255 ){brightness = 255;}
				*p++ = (unsigned char)brightness;
				*p++ = (unsigned char)brightness;
				*p++ = (unsigned char)brightness;
			}
		}
	}
	break;
	case 32:
	{
	/*
		for (int y = 0; y < Height(); y++) {
			BYTE *p = (BYTE *)GetBits(0, y);
			const BYTE *q = (const BYTE *)dib.GetBits(0, y);
			for (int x = 0; x < Width(); x++) {
			*p++ = *q++ * level / 256;
			*p++ = *q++ * level / 256;
			*p++ = *q++ * level / 256;
			p++;
			q++;
			}
		}
	*/
	}
	break;
	}
	/*
	}else {
		CDibDC srcDC(dib);
		CDibDC dstDC(*this);

		for (int y = 0; y < Height(); y++) {
			for (int x = 0; x < Width(); x++) {
				COLORREF c = srcDC.GetPixel(x, y);
				BYTE r = GetRValue(c) * level / 256;
				BYTE g = GetGValue(c) * level / 256;
				BYTE b = GetBValue(c) * level / 256;
				dstDC.SetPixelV(x, y, RGB(r, g, b));
			}
		}
	}
	*/
}


// フェードの中間処理（イメージと黒ののアルファ合成を行う）
//
void CDibSection::Fade(const CDibSection &dib, int level)
{
	if (Depth() == dib.Depth()) {
		switch (Depth()) {
		  case 16:
			{
				for (int y = 0; y < Height(); y++) {
					WORD *p = (WORD *)GetBits(0, y);
					const WORD *q = (const WORD *)dib.GetBits(0, y);
					for (int x = 0; x < Width(); x++) {
						WORD data = *q++;
						BYTE r = ((data & 0x7c00) >> 10) * level / 256;
						BYTE g = ((data & 0x03e0) >> 5) * level / 256;
						BYTE b = (data & 0x001f) * level / 256;
						*p++ = (r << 10) & 0x7c00 | (g << 5) & 0x03e0 | b & 0x001f;
					}
				}
			}
			break;

		  case 24:
			{
				for (int y = 0; y < Height(); y++) {
					BYTE *p = (BYTE *)GetBits(0, y);
					const BYTE *q = (const BYTE *)dib.GetBits(0, y);
					for (int x = 0; x < Width(); x++) {
						*p++ = *q++ * level / 256;
						*p++ = *q++ * level / 256;
						*p++ = *q++ * level / 256;
					}
				}
			}
			break;

		  case 32:
			{
				for (int y = 0; y < Height(); y++) {
					BYTE *p = (BYTE *)GetBits(0, y);
					const BYTE *q = (const BYTE *)dib.GetBits(0, y);
					for (int x = 0; x < Width(); x++) {
						*p++ = *q++ * level / 256;
						*p++ = *q++ * level / 256;
						*p++ = *q++ * level / 256;
						p++;
						q++;
					}
				}
			}
			break;
		}
	}
	else {
		CDibDC srcDC(dib);
		CDibDC dstDC(*this);

		for (int y = 0; y < Height(); y++) {
			for (int x = 0; x < Width(); x++) {
				COLORREF c = srcDC.GetPixel(x, y);
				BYTE r = GetRValue(c) * level / 256;
				BYTE g = GetGValue(c) * level / 256;
				BYTE b = GetBValue(c) * level / 256;
				dstDC.SetPixelV(x, y, RGB(r, g, b));
			}
		}
	}
}

// 2つのイメージのアルファ合成を行う
//
void CDibSection::Fade(const CDibSection &dib1, const CDibSection &dib2, int level)
{
	if (Depth() == dib1.Depth() && Depth() == dib2.Depth()) {
		switch (Depth()) {
		  case 16:
			{
				for (int y = 0; y < Height(); y++) {
					WORD *p = (WORD *)GetBits(0, y);
					const WORD *q1 = (const WORD *)dib1.GetBits(0, y);
					const WORD *q2 = (const WORD *)dib2.GetBits(0, y);
					for (int x = 0; x < Width(); x++) {
						WORD data1 = *q1++;
						WORD data2 = *q2++;
						BYTE r1 = (data1 & 0x7c00) >> 10;
						BYTE g1 = (data1 & 0x03e0) >> 5;
						BYTE b1 = data1 & 0x001f;
						BYTE r = r1 + (((data2 & 0x7c00) >> 10) - r1) * level / 256;
						BYTE g = g1 + (((data2 & 0x03e0) >> 5) - g1) * level / 256;
						BYTE b = b1 + ((data2 & 0x001f) - b1) * level / 256;
						*p++ = (r << 10) & 0x7c00 | (g << 5) & 0x03e0 | b & 0x001f;
					}
				}
			}
			break;

		  case 24:
			{
				for (int y = 0; y < Height(); y++) {
					BYTE *p = (BYTE *)GetBits(0, y);
					const BYTE *q1 = (const BYTE *)dib1.GetBits(0, y);
					const BYTE *q2 = (const BYTE *)dib2.GetBits(0, y);
					for (int x = 0; x < Width(); x++) {
						*p++ = q1[0] + (q2[0] - q1[0]) * level / 256;
						*p++ = q1[1] + (q2[1] - q1[1]) * level / 256;
						*p++ = q1[2] + (q2[2] - q1[2]) * level / 256;
						q1 += 3;
						q2 += 3;
					}
				}
			}
			break;

		  case 32:
			{
				for (int y = 0; y < Height(); y++) {
					BYTE *p = (BYTE *)GetBits(0, y);
					const BYTE *q1 = (const BYTE *)dib1.GetBits(0, y);
					const BYTE *q2 = (const BYTE *)dib2.GetBits(0, y);
					for (int x = 0; x < Width(); x++) {
						*p++ = q1[0] + (q2[0] - q1[0]) * level / 256;
						*p++ = q1[1] + (q2[1] - q1[1]) * level / 256;
						*p++ = q1[2] + (q2[2] - q1[2]) * level / 256;
						p++;
						q1 += 4;
						q2 += 4;
					}
				}
			}
			break;
		}
	}
	else {
		CDibDC src1DC(dib1);
		CDibDC src2DC(dib2);
		CDibDC dstDC(*this);

		for (int y = 0; y < Height(); y++) {
			for (int x = 0; x < Width(); x++) {
				COLORREF c1 = src1DC.GetPixel(x, y);
				COLORREF c2 = src2DC.GetPixel(x, y);
				BYTE r = GetRValue(c1) + (GetRValue(c2) - GetRValue(c1)) * level / 256;
				BYTE g = GetGValue(c1) + (GetGValue(c2) - GetGValue(c1)) * level / 256;
				BYTE b = GetBValue(c1) + (GetBValue(c2) - GetBValue(c1)) * level / 256;
				dstDC.SetPixelV(x, y, RGB(r, g, b));
			}
		}
	}
}

// 2つのイメージのアルファ合成を行う（MMX使用）
//
void CDibSection::FadeFast(const CDibSection &dib1, const CDibSection &dib2, int level)
{
#ifndef _AMD64_
	ASSERT(Depth() == dib1.Depth() && Depth() == dib2.Depth());

	switch (Depth()) {
	  case 16:
		{
			static __int64 maskr = 0x7c007c007c007c00;
			static __int64 maskg = 0x03e003e003e003e0;
			static __int64 maskb = 0x001f001f001f001f;
			__int64 lv = (level << 16) | level | ((__int64)((level << 16) | level) << 32);
			int len = Width() / 4;
			for (int y = 0; y < Height(); y++) {
				BYTE *p = (BYTE *)GetBits(0, y);
				const BYTE *q1 = (const BYTE *)dib1.GetBits(0, y);
				const BYTE *q2 = (const BYTE *)dib2.GetBits(0, y);
				__asm	{
					mov			edi, dword ptr p
					mov			ebx, dword ptr q1
					mov			esi, dword ptr q2
					mov			edx, len
					movq		mm6, lv

L16:
					movq		mm0, [esi]
					movq		mm1, mm0
					movq		mm2, mm0
					pand		mm0, maskr			; mm0 = red
					pand		mm1, maskg			; mm1 = green
					pand		mm2, maskb			; mm2 = blue

					movq		mm3, [ebx]
					movq		mm4, mm3
					movq		mm5, mm3
					pand		mm3, maskr			; mm0 = red
					pand		mm4, maskg			; mm1 = green
					pand		mm5, maskb			; mm2 = blue

					psubw		mm0, mm3
					psraw		mm0, 10
					pmullw		mm0, mm6
					psllw		mm0, 2
					pand		mm0, maskr
					paddw		mm0, mm3

					psubw		mm1, mm4
					psraw		mm1, 5
					pmullw		mm1, mm6
					psrlw		mm1, 3
					pand		mm1, maskg
					paddw		mm1, mm4

					psubw		mm2, mm5
					pmullw		mm2, mm6
					psraw		mm2, 8
					paddw		mm2, mm5

					por			mm0, mm1
					por			mm0, mm2

					movq		[edi], mm0
					add			edi, 8
					add			esi, 8
					add			ebx, 8
					dec			edx
					jnz			L16
				}
			}
			__asm	emms
		}
		break;

	  case 24:
	  case 32:
		{
			__int64 lv = (level << 16) | level | ((__int64)((level << 16) | level) << 32);
			int len = Width() * Depth() / 8 / 8;
			for (int y = 0; y < Height(); y++) {
				BYTE *p = (BYTE *)GetBits(0, y);
				const BYTE *q1 = (const BYTE *)dib1.GetBits(0, y);
				const BYTE *q2 = (const BYTE *)dib2.GetBits(0, y);
				__asm	{
					mov			edi, dword ptr p	; EDI = 格納先
					mov			ebx, dword ptr q1	; EBX = 元データ1
					mov			esi, dword ptr q2	; ESI = 元データ2
					mov			edx, len			; EDX = データ数（8Bytes換算）
					movq		mm6, lv				; MM6 = 合成レベル
					pxor		mm7, mm7			; MM7 = 0

L24:
					movq		mm0, [esi]			; MM0 = d1 d2 d3 d4 d5 d6 d7 d8
					movq		mm1, [ebx]			; MM1 = D1 D2 D3 D4 D5 D6 D7 D8
					movq		mm2, mm0			; MM0, 1 は下位４バイト用
					movq		mm3, mm1			; MM2, 3 は上位４バイト用
													; 計算の桁あふれを防ぐため、
													; １６ビットに拡張する
													; 上位と下位４バイトに分けて、
													; 処理する

					punpcklbw	mm0, mm7			; MM0 = 00 d5 00 d6 00 d7 00 d8
					punpcklbw	mm1, mm7			; MM1 = 00 D5 00 D6 00 D7 00 D8
					punpckhbw	mm2, mm7			; MM2 = 00 d1 00 d2 00 d3 00 d4
					punpckhbw	mm3, mm7			; MM3 = 00 D1 00 D2 00 D3 00 D4
													; 上位８ビットは、MM7が入るが
													; MM7を'0'にしているので、読み込
													; んだデータが16ビットに拡張される

													; 以下の計算は、C++版と変わらない
													; ４データ分まとめて計算できるのが
													; MMX命令のメリット
					psubw		mm0, mm1			; d(5,8) - D(5,8)
					pmullw		mm0, mm6			; (d(5,8) - D(5,8)) * level
					psrlw		mm0, 8				; (d(5,8) - D(5,8)) * level / 256
					paddb		mm0, mm1			; D(5,8) + (d(5,8) - D(5,8)) * level / 256
					psubw		mm2, mm3			; d(1,4) - D(1,4)
					pmullw		mm2, mm6			; (d(1,4) - D(1,4)) * level
					psrlw		mm2, 8				; (d(1,4) - D(1,4)) * level / 256
					paddb		mm2, mm3			; D(1,4) + (d(1,4) - D(1,4)) * level / 256

					packuswb	mm0, mm2			; 上位と下位をパックする

					movq		[edi], mm0			;
					add			edi, 8				;
					add			esi, 8				;
					add			ebx, 8				;
					dec			edx					;
					jnz			L24					;
				}
			}
			__asm	emms
		}
		break;
	}
#endif
}
