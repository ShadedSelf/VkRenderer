#ifndef IO
	#define IO

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace io
{
	void WriteBufferToPng(std::string file, void *data, uint32_t width, uint32_t height)
	{
		float *raw = (float*)data;
		uint8_t *pixels = new uint8_t[width * height * 4];
		uint32_t gI = 0;
		for (uint32_t i = 0; i < width; i++)
			for (uint32_t j = 0; j < height; j++)
				for (uint32_t k = 0; k < 4; k++)
				{
					uint32_t tt = j * 4 + (width - 1 - i) * width * 4 + k;
					raw[tt] = pow(raw[tt], 1.0 / 2.2);
					uint8_t c = ((raw[tt] > 1.0) ? 255 : raw[tt] * 255);
					pixels[gI] = c;
					gI++;
				}

		stbi_write_png(file.c_str(), 800, 800, 4, pixels, 0);
	}
}

#endif
