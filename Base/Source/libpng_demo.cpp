

#include <pngconf.h>
#include <setjmp.h>
#include "libpng_demo.h"

void abort_(const char * s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

/*********************************************************************************************************
read from png file (includes opengl)
/*********************************************************************************************************/
RawImageData read_png_file(FILE *fp, const char* filename)
{
	char header[8];    // 8 is the maximum size that can be checked

	if (!fp){
		abort_("[read_png_file] File %s could not be opened for reading", filename);

    }

	fread(header, 1, 8, fp);

	if (png_sig_cmp((png_const_bytep)header, 0, 8))
		abort_("[read_png_file] File %s is not recognized as a PNG file", filename);


	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[read_png_file] png_create_read_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[read_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during init_io");

	//pass open file to png struct------------------------------------------------//
    //PC ver.
	png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

	//get info----------------------------------------------------------------------------------//
	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	//only interested in RGBA data, other bit depths and color types should be converted
	// Ensure 8-bit packing
	if (bit_depth < 8)
		png_set_packing(png_ptr);
	else if (bit_depth == 16)
		png_set_scale_16(png_ptr);

	// if transparency, convert it to alpha----------------------------------------------//
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}

	//get color type (for Opengl)--------------------------------------------------------//
	GLenum format = -1;
	switch (png_get_color_type(png_ptr, info_ptr)) {
	case PNG_COLOR_TYPE_GRAY:
		format = GL_RGB;
		png_set_gray_to_rgb(png_ptr);
		break;

	case PNG_COLOR_TYPE_GRAY_ALPHA:
		format = GL_RGBA;
		png_set_gray_to_rgb(png_ptr);
		break;

	case PNG_COLOR_TYPE_PALETTE:
		format = GL_RGB;
		png_set_expand(png_ptr);
		break;

	case PNG_COLOR_TYPE_RGB:
		format = GL_RGB;
		break;

	case PNG_COLOR_TYPE_RGBA:
		format = GL_RGBA;
		break;
	}

	//we don't want interlaced image data--------------------------------------------------------//
	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);	//skip reading header (done already)

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during read_image");

	//Load pixel buffer--------------------------------------------------------//
	//png_read_image() expects an array of pointers to store row of pixel ata
	//so need pixel buffer and tmp. array of ptrs

	//pixel buffer---//
	const png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
	const int data_length = row_size * height;

	png_byte* pixel_buf = (png_byte*)malloc(data_length);

	//tmp. array of row pointers---//
	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	//row_pointers = new png_bytep[height];

	//iterate through each row
	for (y = 0; y < height; ++y)
		row_pointers[(height - 1) - y] = pixel_buf + y * row_size;

	//read all rows (data goes into pixel_buf)
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, NULL);	//read until end

	fclose(fp);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	delete row_pointers;

	//return data-------------------------------------------------------------------//
	RawImageData img_data(width, height, row_size * height, format, pixel_buf);

	return img_data;
}

/*********************************************************************************************************
Write data to a png file (crate if non)
/*********************************************************************************************************/
void write_png_file(char* file_name)
{
	/* create file */
	//errno_t error;
	FILE *fp = NULL;
	//error = fopen_s(&fp, file_name, "wb");

	if (!fp)
		abort_("[write_png_file] File %s could not be opened for writing", file_name);


	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[write_png_file] png_create_write_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[write_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during init_io");

	png_init_io(png_ptr, fp);
    

	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing header");

	png_set_IHDR(png_ptr, info_ptr, width, height,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing bytes");

	png_write_image(png_ptr, row_pointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during end of write");

	png_write_end(png_ptr, NULL);

	/* cleanup heap allocation */
	for (y = 0; y<height; y++)
		free(row_pointers[y]);
	free(row_pointers);

	fclose(fp);
}

/*********************************************************************************************************
process individual pixels
/*********************************************************************************************************/
void process_file(void)
{
	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
		abort_("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA "
		"(lacks the alpha channel)");

	if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
		abort_("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
		PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));

	cout << "Height: " << height << endl;
	cout << "width: " << width << endl;

	//process the png---------------------------------------------------------------------------//
	for (y = 0; y<height; y++)
	{
		png_byte* row = row_pointers[y];

		for (x = 0; x<width; x++)
		{
			png_byte* ptr = &(row[x * 4]);


			//printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
			//x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

			/* set red value to 0 and green value to the blue one */
			ptr[0] = 255 - ptr[0];
			ptr[1] = 255 - ptr[1];
			ptr[2] = 255 - ptr[2];
		}
	}
}

/*********************************************************************************************************
Cleanup
/*********************************************************************************************************/
void release_raw_image_data(const RawImageData* data)
{
	free((void*)data->data);
}

int libpng_demo()
{
	//read_png_file("train.png");
	process_file();
	write_png_file("asdsad.png");

	cin.get();
	return 0;
}