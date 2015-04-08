//
//	libpng ÉâÉbÉpÅ[
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef	__pngfile_h__
#define	__pngfile_h__

#include "png.h"
#include "exception"

#ifndef _X86_64
//#pragma comment(lib, "libpng64.lib")
//#pragma comment(lib, "zlib64.lib")
#else

#ifdef	_DEBUG
#pragma comment(lib, "libpngd.lib")
#pragma comment(lib, "zlibd.lib")
#else
#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "zlib.lib")
#endif

#endif

class cpng {
  public:
	cpng();
	virtual ~cpng() = 0
	{}

	bool create_write_struct(png_const_charp user_png_ver=PNG_LIBPNG_VER_STRING);
	void destroy_write_struct();
	void set_IHDR(png_uint_32 width, png_uint_32 height, int bit_depth,
		int color_type=PNG_COLOR_TYPE_RGB,
		int interlace_type=PNG_INTERLACE_NONE,
		int compression_type=PNG_COMPRESSION_TYPE_DEFAULT,
		int filter_type=PNG_FILTER_TYPE_DEFAULT);
	void set_compression_level(int level=Z_BEST_COMPRESSION);
	void set_filter(int method=PNG_FILTER_TYPE_BASE, int filters=PNG_ALL_FILTERS);
	void write_info();
	void write_image(png_bytepp image);
	void write_end();
	bool create_read_struct(png_const_charp user_png_ver=PNG_LIBPNG_VER_STRING);
	void destroy_read_struct(png_infopp end_info_ptr_ptr=0);
	bool create_info_struct();
	void init_io(png_FILE_p fp);
	void set_sig_bytes(int num_bytes);
	void read_info();
	png_uint_32 get_IHDR(png_uint_32 *width, png_uint_32 *height,
		int *bit_depth, int *color_type, int *interlace_type=0,
		int *compression_type=0, int *filter_type=0);
	void set_expand();
	void set_strip_16();
	void set_strip_alpha();
	void set_gray_to_rgb();
	png_uint_32 get_valid(png_uint_32 flag);
	void read_update_info();
	void set_bgr();
	void read_image(png_bytepp image);
	void read_end();

  protected:
	static void error_func(png_structp png_ptr, png_const_charp message);
	static void warning_func(png_structp png_ptr, png_const_charp message);

  protected:
	png_structp png_ptr;
	png_infop info_ptr;
} ;

inline cpng::cpng(): png_ptr(0), info_ptr(0)
{
}

inline bool cpng::create_write_struct(png_const_charp user_png_ver)
{
	png_ptr = png_create_write_struct(user_png_ver,
		(png_voidp)this, &error_func, &warning_func);
	return png_ptr != 0;
}

inline void cpng::destroy_write_struct()
{
	if (png_ptr)
		png_destroy_write_struct(&png_ptr, info_ptr? &info_ptr: 0);
	png_ptr = 0;
	info_ptr = 0;
}

inline void cpng::set_compression_level(int level)
{
	png_set_compression_level(png_ptr, level);
}

inline void cpng::set_filter(int method, int filters)
{
	png_set_filter(png_ptr, method, filters);
}

inline void cpng::set_IHDR(png_uint_32 width, png_uint_32 height, int bit_depth,
	int color_type, int interlace_type, int compression_type, int filter_type)
{
	png_set_IHDR(png_ptr, info_ptr,
		width, height, bit_depth, color_type,
		interlace_type, compression_type, filter_type);
}

inline void cpng::write_info()
{
	png_write_info(png_ptr, info_ptr);
}

inline void cpng::write_image(png_bytepp image)
{
	png_write_image(png_ptr, image);
}

inline void cpng::write_end()
{
	png_write_end(png_ptr, info_ptr);
}

inline bool cpng::create_read_struct(png_const_charp user_png_ver)
{
	png_ptr = png_create_read_struct(user_png_ver,
		(png_voidp)this, &error_func, &warning_func);
	return png_ptr != 0;
}

inline void cpng::destroy_read_struct(png_infopp end_info_ptr_ptr)
{
	if (png_ptr)
		png_destroy_read_struct(&png_ptr, info_ptr? &info_ptr: 0, end_info_ptr_ptr);
	png_ptr = 0;
	info_ptr = 0;
}

inline bool cpng::create_info_struct()
{
	info_ptr = png_create_info_struct(png_ptr);
	return info_ptr != 0;
}

inline void cpng::init_io(png_FILE_p fp)
{
	png_init_io(png_ptr, fp);
}

inline void cpng::set_sig_bytes(int num_bytes)
{
	png_set_sig_bytes(png_ptr, num_bytes);
}

inline void cpng::read_info()
{
	png_read_info(png_ptr, info_ptr);
}

inline png_uint_32 cpng::get_IHDR(png_uint_32 *width, png_uint_32 *height,
	int *bit_depth, int *color_type, int *interlace_type,
	int *compression_type, int *filter_type)
{
	return png_get_IHDR(png_ptr, info_ptr,
		width, height, bit_depth, color_type, interlace_type,
		compression_type, filter_type);
}

inline void cpng::set_expand()
{
	png_set_expand(png_ptr);
}

inline void cpng::set_strip_16()
{
	png_set_strip_16(png_ptr);
}

inline void cpng::set_strip_alpha()
{
	png_set_strip_alpha(png_ptr);
}

inline void cpng::set_gray_to_rgb()
{
	png_set_gray_to_rgb(png_ptr);
}

inline png_uint_32 cpng::get_valid(png_uint_32 flag)
{
	return png_get_valid(png_ptr, info_ptr, flag);
}

inline void cpng::read_update_info()
{
	png_read_update_info(png_ptr, info_ptr);
}

inline void cpng::set_bgr()
{
	png_set_bgr(png_ptr);
}

inline void cpng::read_image(png_bytepp image)
{
	png_read_image(png_ptr, image);
}

inline void cpng::read_end()
{
	png_read_end(png_ptr, info_ptr);
}

class cpng_write: public cpng {
  public:
	cpng_write();
	~cpng_write();
} ;

inline cpng_write::cpng_write()
{
	if (create_write_struct()) {
		if (create_info_struct())
			return;
		destroy_write_struct();
	}
//	throw exception("not enough memory");
}

inline cpng_write::~cpng_write()
{
	destroy_write_struct();
}

class cpng_read: public cpng {
  public:
	cpng_read();
	~cpng_read();
} ;

inline cpng_read::cpng_read()
{
	if (create_read_struct()) {
		if (create_info_struct())
			return;
		destroy_read_struct();
	}
//	throw exception("not enough memory");
}

inline cpng_read::~cpng_read()
{
	destroy_read_struct();
}

#endif
