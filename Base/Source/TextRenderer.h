#ifndef TEXT_FT_H
#define TEXT_FT_H
#include "Header.h"
#include "Vector2.h"

/******************************************************************************
Store info for charcter glyph
******************************************************************************/
struct TextCharacter
{
	GLuint textureID;
	Vector2 size;
	Vector2 bearing;
	GLuint advance;

	void init(GLuint textureID, Vector2 size, Vector2 bearing, GLuint advance)
	{
		this->textureID = textureID; this->size = size; this->bearing = bearing; this->advance = advance;
	}

	void Exit()
	{
		glDeleteTextures(1, &textureID);
	}
};

/******************************************************************************
FreeType text library wrapper.
Credits:
-Tutorial: http://learnopengl.com/#!In-Practice/Text-Rendering
credit goes to FreeType for this awesome text library and learnopengl.xom for the comprehensive tutorial :)

Author: Tan Yie Cher
Date: 25/6/16
******************************************************************************/
class FreeType_Text
{
	FT_Library ft;

	//font------------------------//
	FT_Face minecraft;

public:
	map<GLchar, TextCharacter> characters;

	/********************** constructor/destructor *****************************/
	FreeType_Text();
	~FreeType_Text();

	/********************** Core functions *****************************/
	void Init();
	void Exit();
};

#endif
