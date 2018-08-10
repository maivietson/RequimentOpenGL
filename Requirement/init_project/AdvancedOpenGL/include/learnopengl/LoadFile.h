#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>

// include library of openGL
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#pragma pack(push,x1)					// Byte alignment (8-bit)
#pragma pack(1)

typedef struct
{
	unsigned char  identsize;			// size of ID field that follows 18 byte header (0 usually)
	unsigned char  colourmaptype;		// type of colour map 0=none, 1=has palette
	unsigned char  imagetype;			// type of image 2=rgb uncompressed, 10 - rgb rle compressed

	short colourmapstart;				// first colour map entry in palette
	short colourmaplength;				// number of colours in palette
	unsigned char  colourmapbits;		// number of bits per palette entry 15,16,24,32

	short xstart;						// image x origin
	short ystart;						// image y origin
	short width;						// image width in pixels
	short height;						// image height in pixels
	unsigned char  bits;				// image bits per pixel 24,32
	unsigned char  descriptor;			// image descriptor bits (vh flip bits)

	// pixel data follows header

} TGA_HEADER;

#pragma pack(pop,x1)

const int IT_COMPRESSED = 10;
const int IT_UNCOMPRESSED = 2;

using namespace std;

class LoadFile
{
public:
	vector<unsigned int> vertexIndices, uvIndicates, normalIndicates;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	bool LoadObj(const char* fileName, vector<glm::vec3> &vertices, vector<glm::vec2> &uvs, vector<glm::vec3> &normals);
	
	void LoadCompressedImage(char* pDest, char * pSrc, TGA_HEADER * pHeader);
	void LoadUncompressedImage(char* pDest, char * pSrc, TGA_HEADER * pHeader);
	char * LoadTGA(const char * szFileName, int * width, int * height, int * bpp);
};

