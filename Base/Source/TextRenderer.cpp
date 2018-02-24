#include "TextRenderer.h"
#include "CoreUtilities.h"

FreeType_Text::FreeType_Text(){}
FreeType_Text::~FreeType_Text(){}

/******************************************************************************
Init
******************************************************************************/
void FreeType_Text::Init()
{
	//Init FT library--------------------------------------------------//
	if (FT_Init_FreeType(&ft))
        CU::fuck_flag = true;

	//Load and init font as face---------------------------------------//
    //PC: fonts/
    //Android: sdcard/
    string mama = CU::assets_path + "fonts/Minecraftia.ttf";
    const char* address2 = mama.c_str();

	if (FT_New_Face(ft, address2, 0, &minecraft))
		CU::fuck_flag = true;

	//extract font size--------------------------------------------------//
    FT_Set_Pixel_Sizes(minecraft, 0, 48);	//setting widh to 0 lets face dynamically cal. width based on given height

	//FT face hosts collection of glyphs, set one of those glyphs as active----------------------------------//
	if (FT_Load_Char(minecraft, 'X', FT_LOAD_RENDER))	//tell FT to create an 8-bit grayscale bitmap image for us
	    CU::fuck_flag = true;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	//For each character, generate a character and store relevant data to Character struct------------------------------------------------//

	bool first = false;
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(minecraft, c, FT_LOAD_RENDER))
		{
			CU::fuck_flag = true;
			continue;
		}

		// Generate texture and store alpha value in R component of texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels
        //https://stackoverflow.com/questions/27430811/text-not-rendering-correctly-opengl-using-freetype2
        //change the 3rd variable from GL_RED to GL_R8 (Fix for Android glTexImage2D 1282 error)
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_R8,	// number of color components in the texture
			minecraft->glyph->bitmap.width,	//pixel width
			minecraft->glyph->bitmap.rows,	//pixel height
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			minecraft->glyph->bitmap.buffer
			);

		// Set texture options-------------------------------------------------------------//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use-----------------------------------------------//
		TextCharacter character;

		character.init(texture, Vector2(minecraft->glyph->bitmap.width, minecraft->glyph->bitmap.rows),
			Vector2(minecraft->glyph->bitmap_left, minecraft->glyph->bitmap_top),
			minecraft->glyph->advance.x);

		//Add new data-------------------------------------------------------------//
		characters.insert(std::pair<GLchar, TextCharacter>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	//cleanup-------------------------------------------------------------//
	FT_Done_Face(minecraft);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 0); // Enable byte-alignment restriction
	FT_Done_FreeType(ft);
}

/******************************************************************************
Exit: cleanup
******************************************************************************/
void FreeType_Text::Exit()
{
	//cleanup textures----------------------------------//
	for (GLubyte c = 0; c < 128; c++)
		characters[c].Exit();

}