#include "EDpch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <filesystem>

Shader *bwShader = nullptr;

unsigned int GetTexture(RawData* data, unsigned int imgWidth, unsigned int imgHeight)
{
	unsigned int tex;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return tex;
	//return 0;
}

RawData* EDNegativeHA(RawData* data, unsigned int imgWidth, unsigned int imgHeight, RawData** outData, int nChannels)
{

	RawData* out = nullptr;

	if (outData) // si el usuario quiere llenar un buffer que el maneje
	{
		if (*outData) // si el buffer ya tiene memoria asociada escribimos en el, asumimos que es el mismo tamaño de buffer que la imagen original
		{
			out = *outData;
		}
		else
		{// sino reservamos memoria para ese buffer
			out = new RawData[(imgWidth * imgHeight) * nChannels];
			*outData = out;
		}
	}
	else 
	{
		out = new RawData[(imgWidth * imgHeight) * nChannels];
	}

	

	unsigned int tex = GetTexture(data, imgWidth, imgHeight);
	Object* q = new Object();
	//glActiveTexture(tex):
	bwShader->use();
	glBindTexture(GL_TEXTURE_2D, tex);
	bwShader->setInt("tex", 0);
	bwShader->setInt("mode", 1);
	bwShader->setFloat("imgWidth", imgWidth);
	bwShader->setFloat("imgHeight", imgHeight);
	q->Bind();
	q->Draw();

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, out);
	
	glDeleteTextures(1, &tex);
	return out;
}

RawData* EDGreyscaleHA(RawData* data, unsigned int imgWidth, unsigned int imgHeight, RawData** outData, int nChannels)
{

	RawData* out = nullptr;

	if (outData) // si el usuario quiere llenar un buffer que el maneje
	{
		if (*outData) // si el buffer ya tiene memoria asociada escribimos en el, asumimos que es el mismo tamaño de buffer que la imagen original
		{
			out = *outData;
		}
		else
		{// sino reservamos memoria para ese buffer
			out = new RawData[(imgWidth * imgHeight) * nChannels];
			*outData = out;
		}
	}
	else
	{
		out = new RawData[(imgWidth * imgHeight) * nChannels];
	}



	unsigned int tex = GetTexture(data, imgWidth, imgHeight);
	Object* q = new Object();
	//glActiveTexture(tex):
	bwShader->use();
	glBindTexture(GL_TEXTURE_2D, tex);
	bwShader->setInt("tex", 0);
	bwShader->setInt("mode", 1);
	q->Bind();
	q->Draw();

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, out);

	glDeleteTextures(1, &tex);
	return out;
}






bool EDInit()
{






	// ... <snip> ... more code
	return true;
}