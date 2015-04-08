//
//	libpng ÉâÉbÉpÅ[
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#include "StdAfx.h"
#include "PngFile.h"

void cpng::error_func(png_structp png_ptr, png_const_charp message)
{
	TRACE("libpng error: %s\n", message);
//	throw exception(message);
}

void cpng::warning_func(png_structp png_ptr, png_const_charp message)
{
	TRACE("libpng warning: %s\n", message);
}
