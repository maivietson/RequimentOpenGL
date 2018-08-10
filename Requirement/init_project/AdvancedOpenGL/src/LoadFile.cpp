#pragma once

#include <learnopengl\LoadFile.h>

bool LoadFile::LoadObj(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
	){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	fclose(file);
	return true;
}

void LoadFile::LoadCompressedImage(char* pDest, char * pSrc, TGA_HEADER * pHeader)
{
	int w = pHeader->width;
	int h = pHeader->height;
	int rowSize = w * pHeader->bits / 8;
	bool bInverted = ((pHeader->descriptor & (1 << 5)) != 0);
	char * pDestPtr = bInverted ? pDest + (h + 1) * rowSize : pDest;
	int countPixels = 0;
	int nPixels = w * h;

	while (nPixels > countPixels)
	{
		unsigned char chunk = *pSrc++;
		if (chunk < 128)
		{
			int chunkSize = chunk + 1;
			for (int i = 0; i < chunkSize; i++)
			{
				if (bInverted && (countPixels % w) == 0)
					pDestPtr -= 2 * rowSize;
				*pDestPtr++ = pSrc[2];
				*pDestPtr++ = pSrc[1];
				*pDestPtr++ = pSrc[0];
				pSrc += 3;
				if (pHeader->bits != 24)
					*pDestPtr++ = *pSrc++;
				countPixels++;
			}
		}
		else
		{
			int chunkSize = chunk - 127;
			for (int i = 0; i < chunkSize; i++)
			{
				if (bInverted && (countPixels % w) == 0)
					pDestPtr -= 2 * rowSize;
				*pDestPtr++ = pSrc[2];
				*pDestPtr++ = pSrc[1];
				*pDestPtr++ = pSrc[0];
				if (pHeader->bits != 24)
					*pDestPtr++ = pSrc[3];
				countPixels++;
			}
			pSrc += (pHeader->bits >> 3);
		}
	}
}

void LoadFile::LoadUncompressedImage(char* pDest, char * pSrc, TGA_HEADER * pHeader)
{
	int w = pHeader->width;
	int h = pHeader->height;
	int rowSize = w * pHeader->bits / 8;
	bool bInverted = ((pHeader->descriptor & (1 << 5)) != 0);
	for (int i = 0; i < h; i++)
	{
		char * pSrcRow = pSrc +
			(bInverted ? (h - i - 1) * rowSize : i * rowSize);
		if (pHeader->bits == 24)
		{
			for (int j = 0; j < w; j++)
			{
				*pDest++ = pSrcRow[2];
				*pDest++ = pSrcRow[1];
				*pDest++ = pSrcRow[0];
				pSrcRow += 3;
			}
		}
		else
		{
			for (int j = 0; j < w; j++)
			{
				*pDest++ = pSrcRow[2];
				*pDest++ = pSrcRow[1];
				*pDest++ = pSrcRow[0];
				*pDest++ = pSrcRow[3];
				pSrcRow += 4;
			}
		}
	}
}


char * LoadFile::LoadTGA(const char * szFileName, int * width, int * height, int * bpp)
{

	FILE * f;

	if (fopen_s(&f, szFileName, "rb") != 0)
		return NULL;

	TGA_HEADER header;
	fread(&header, sizeof(header), 1, f);

	fseek(f, 0, SEEK_END);
	int fileLen = ftell(f);
	fseek(f, sizeof(header)+header.identsize, SEEK_SET);

	if (header.imagetype != IT_COMPRESSED && header.imagetype != IT_UNCOMPRESSED)
	{
		fclose(f);
		return NULL;
	}

	if (header.bits != 24 && header.bits != 32)
	{
		fclose(f);
		return NULL;
	}

	int bufferSize = fileLen - sizeof(header)-header.identsize;
	char * pBuffer = new char[bufferSize];
	fread(pBuffer, 1, bufferSize, f);
	fclose(f);

	*width = header.width;
	*height = header.height;
	*bpp = header.bits;
	char * pOutBuffer = new char[header.width * header.height * header.bits / 8];

	switch (header.imagetype)
	{
	case IT_UNCOMPRESSED:
		LoadUncompressedImage(pOutBuffer, pBuffer, &header);
		break;
	case IT_COMPRESSED:
		LoadCompressedImage(pOutBuffer, pBuffer, &header);
		break;
	}

	delete[] pBuffer;

	return pOutBuffer;
}