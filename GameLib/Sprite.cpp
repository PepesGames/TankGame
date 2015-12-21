#include "stdafx.h"

#include "Sprite.h"
#include "Matrix3x3.h" 

#include "sdl_openGL.h"
#include "sdl.h"
#include "sdl_image.h"


Sprite::Sprite(const char *szFilename)
{
	m_loaded	= false;
	m_width		= 0.0f;
	m_height	= 0.0f;
	m_handle	= 0;

	LoadTexture( szFilename );

	points[0].m_X = -32;
	points[0].m_Y = -32;
	points[0].m_Z = 1;	//need to set 1 for ignore the Z value for translate etc

	points[1].m_X = 32;
	points[1].m_Y = -32;
	points[1].m_Z = 1;	//need to set 1 for ignore the Z value for translate etc

	points[2].m_X = 32;
	points[2].m_Y = 32;
	points[2].m_Z = 1;	//need to set 1 for ignore the Z value for translate etc

	points[3].m_X = -32;
	points[3].m_Y = 32;
	points[3].m_Z = 1;	//need to set 1 for ignore the Z value for translate etc


}

Sprite::~Sprite()
{
	if( m_loaded == true )
		FreeTexture();
}

void Sprite::LoadTexture(const char *szFilename)
{
	SDL_Surface *image = IMG_Load( szFilename );

	// if the image failed to load, get out now
	if( image == NULL )
		return;

	//convert to the correct image format
	SDL_PixelFormat Format;

	Format.colorkey		= 0;
	Format.alpha		= 255;
	Format.palette		= NULL;

	Format.Rloss = 0;
	Format.Gloss = 0;
	Format.Bloss = 0;
	Format.Aloss = 0;

	Format.BitsPerPixel		= 32;
	Format.BytesPerPixel	= 4;

	Format.Rshift		= 0;
	Format.Gshift		= 8;
	Format.Bshift		= 16;
	Format.Ashift		= 24;

#ifdef SDL_BIG_ENDIAN
	Format.Rmask = 0x000000FF;
	Format.Gmask = 0x0000FF00;
	Format.Bmask = 0x00FF0000;
	Format.Amask = 0xFF000000;
#elif
	Format.Rmask = 0xFF000000;
	Format.Gmask = 0x00FF0000;
	Format.Bmask = 0x0000FF00;
	Format.Amask = 0x000000FF;
#endif

	// convert the image
	SDL_Surface *convImage = SDL_ConvertSurface( image, &Format, 0 );

	// free the old one
	SDL_FreeSurface( image );


	// Load image into OpenGL
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4);

	glGenTextures( 1, &m_handle );			// will put a texture handle into m_handle
	glBindTexture(GL_TEXTURE_2D, m_handle); // bind the texture for use with OpenGL

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		convImage->w,
		convImage->h,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		convImage->pixels);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_width		= convImage->w;
	m_height	= convImage->h;
	m_loaded	= true;

	// finally clean up the sdl surface
	SDL_FreeSurface( convImage );
}


void Sprite::FreeTexture()
{
	if( m_loaded == false )
		return;

	glDeleteTextures(1, &m_handle);

	m_loaded	= false;
	m_handle	= 0;
	m_width		= 0.0f;
	m_height	= 0.0f;
}


void Sprite::Draw(Matrix3x3 a_worldTrans)
{

	Vector3 p[4];
	for(int i=0; i<4;i++)
	{
		p[i] = a_worldTrans * points[i];
	}


	glBindTexture( GL_TEXTURE_2D, m_handle );
	glBegin(GL_QUADS);
	// top left
	glTexCoord2f(	0.0f,		0.0f		);
	glVertex2f	(	p[0].m_X,			p[0].m_Y			);

	// top right
	glTexCoord2f(	1.0f,		0.0f		);
	glVertex2f	(	p[1].m_X,	p[1].m_Y			);

	// bottom right
	glTexCoord2f(	1.0f,		1.0f		);
	glVertex2f	(	p[2].m_X,	p[2].m_Y	);

	// bottom left
	glTexCoord2f(	0.0f,		1.0f		);
	glVertex2f	(	p[3].m_X,			p[3].m_Y	);

	glEnd();
}

void Sprite::DrawImage(float a_x, float a_y, float a_w, float a_h)
{

	glBindTexture( GL_TEXTURE_2D, m_handle );
	glBegin(GL_QUADS);
	// top left
	glTexCoord2f(	0.0f,		0.0f		);
	glVertex2f	(	a_x,		a_y			);

	// top right
	glTexCoord2f(	1.0f,		0.0f		);
	glVertex2f	(	a_x + a_w,	a_y			);

	// bottom right
	glTexCoord2f(	1.0f,		1.0f		);
	glVertex2f	(	a_x + a_w,	a_y + a_h	);

	// bottom left
	glTexCoord2f(	0.0f,		1.0f		);
	glVertex2f	(	a_x,		a_y + a_h	);

	glEnd();

}


void Sprite::DrawChar(const char a_cinput, float a_fx, float a_fy, float a_fwidth, float a_fheight, float a_fscale)
{

	glBindTexture( GL_TEXTURE_2D, m_handle );
	glBegin(GL_QUADS);

	char c = a_cinput;

	int cIdx = c - 32;

	int col = cIdx % 28;
	int row = cIdx / 28;

	float u1 = col * (18.0f/512.0f);
	float v1 = row * (34.0f/512.0f);

	float u2 = (col + 1) * (18.0f/512.0f);
	float v2 = (row + 1) * (34.0f/512.0f);


	glTexCoord2f(	u1,		v1		);
	glVertex2f	(	a_fx ,			a_fy 			);

	// top right
	glTexCoord2f(	u2,		v1		);
	glVertex2f	(	a_fx + a_fwidth * a_fscale,	a_fy			);

	// bottom right
	glTexCoord2f(	u2,		v2		);
	glVertex2f	(	a_fx + a_fwidth * a_fscale,	a_fy + a_fheight * a_fscale	);


	// bottom left
	glTexCoord2f(	u1,		v2		);
	glVertex2f	(	a_fx,			a_fy + a_fheight * a_fscale	);

	glEnd();
}


void Sprite::DrawString(const char* a_text,  float a_x, float a_y, float a_width, float a_height, float a_scale)
{
	//int textsize = sizeof(a_text) * strlen(a_text);
	int textsize = strlen(a_text);
	for (int i=0; i<textsize; i++)
	{
		if (a_text[i] >= 33 && a_text[i] <= 126)
			DrawChar(a_text[i], a_x + a_width * i, a_y, a_width, a_height, a_scale);
	}
}
