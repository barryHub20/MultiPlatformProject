#ifndef LIBDEMO_H
#define LIBDEMO_H

#include "AssetManager.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
using namespace std;

#define PNG_DEBUG 3

struct RawImageData{

	const int width;
	const int height;
	const int size;
	const GLenum gl_color_format;
	GLvoid* data;	//double pointer to data

	RawImageData() :
		width(0),
		height(0),
		size(0),
		gl_color_format(0),
		data(NULL){}

	RawImageData(const int width, const int height, const int size, const GLenum gl_color_format, GLvoid* data) :
		width(width),
		height(height),
		size(size),
		gl_color_format(gl_color_format),
		data(data){}

	~RawImageData(){}
};

/***************************************************************************************************************
Image
Tutorial: http://zarb.org/~gc/html/libpng.html, http://devcry.heiho.net/html/2015/20150517-libpng.html
libpng setup tutorial: https://www.youtube.com/watch?v=GsgSnSG0O0U

Author: Tan Yie Cher
Date: 24/1/17

Note:
-Reads and writes PNG raw data
/***************************************************************************************************************/
RawImageData read_png_file(FILE *fp, const char* filename);;
void process_file(void);
void write_png_file(char* file_name);

void release_raw_image_data(const RawImageData* data);

#endif