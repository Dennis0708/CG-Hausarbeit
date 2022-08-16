#include "rgbimage.h"
#include "color.h"
#include "assert.h"

RGBImage::RGBImage(unsigned int Width, unsigned int Height)
{
	this->m_Width = Width;
	this->m_Height = Height;
	this->m_Image = new Color[Width * Height];
}

RGBImage::~RGBImage()
{
	delete [] m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c)
{
	if (x >= this->m_Width && y >= this->m_Height) {
		cerr << "x und y liegen nicht im Werte bereich" << endl;
		return;
	}

	this->m_Image[x + y * this->m_Width] = c;
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const
{
	if (x >= this->m_Width || y >= this->m_Height) {
		//cerr << "x und y liegen nicht im Werte bereich" << endl;
		return Color();
	}

	return this->m_Image[x + y * this->m_Width];
}

unsigned int RGBImage::width() const
{
	return this->m_Width;
}
unsigned int RGBImage::height() const
{
	return this->m_Height;
}

RGBImage& RGBImage::SobelFilter(RGBImage& dst, const RGBImage& src, float factor)
{
	assert(src.width() == dst.width() && src.height() == dst.height());

	int K[3][3] = { {1, 0, -1},{2, 0, -2},{1, 0, -1} };
	int Kt[3][3] = { {1, 2, 1},{0, 0, 0},{-1, -2, -1} };

	for (int y = 0; y < src.height(); y++) {
		for (int x = 0; x < src.width(); x++) {
		
			Color U;
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 3; i++) {
					U += src.getPixelColor(x - i - 1, y - j - 1) * K[i][j];
				}
			}

			Color V;
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 3; i++) {
					V += src.getPixelColor(x - i - 1, y - j - 1) * Kt[i][j];
				}
			}

			Color S = Color(sqrt((pow(U.R, 2) + pow(V.R, 2))), sqrt((pow(U.G, 2) + pow(V.G, 2))), sqrt((pow(U.B, 2) + pow(V.B, 2))));

			dst.setPixelColor(x, y, S * factor);
		}
	}

	return dst;
}

unsigned char RGBImage::convertColorChannel(float v)
{
	if (v < 0.0f) {
		v = 0.0f;
	}
	if (v > 1.0f) {
		v = 1.0f;
	}
	return 255.0f * v;
}

bool RGBImage::saveToDisk(const char* Filename)
{
	FILE* fp;

	if (!(fp = fopen(Filename, "wb"))) {
		cerr << "Datei konnte nicht geoeffnet werden" << endl;
		return false;
	}

	uint16_t bfType1[1] = { 'B' };
	uint16_t bfType2[1] = { 'M' };
	uint32_t bfSize[1] = { this->m_Width * this->m_Height * 3 + 54 };
	uint32_t bfReserved[1] = { 0 };
	uint32_t bfOffBits[1] = { 54 };
	
	uint32_t biSize[1] = { 40 };
	uint32_t biWidth[1] = { m_Width };
	uint32_t biHeight[1] = { m_Height };
	uint16_t biPlanes[1] = { 1 };
	uint16_t biBitCount[1] = { 24 };
	uint32_t biCompression[1] = { 0 };
	uint32_t biSizeImage[1] = { 0 };
	uint32_t biXPelsPerMeter[1] = { 0 };
	uint32_t biYPelsPerMeter[1] = { 0 };
	uint32_t biClrUsed[1] = { 0 };	
	uint32_t biClrImportant[1] = { 0 };

	fwrite(bfType1, 1, 1, fp);
	fwrite(bfType2, 1, 1, fp);
	fwrite(bfSize, 4, 1, fp);
	fwrite(bfReserved, 4, 1, fp);
	fwrite(bfOffBits, 4, 1, fp);

	fwrite(biSize, 4, 1, fp);
	fwrite(biWidth, 4, 1, fp);
	fwrite(biHeight, 4, 1, fp);
	fwrite(biPlanes, 2, 1, fp);
	fwrite(biBitCount, 2, 1, fp);
	fwrite(biCompression, 4, 1, fp);
	fwrite(biSizeImage, 4, 1, fp);
	fwrite(biXPelsPerMeter, 4, 1, fp);
	fwrite(biYPelsPerMeter, 4, 1, fp);
	fwrite(biClrUsed, 4, 1, fp);
	fwrite(biClrImportant, 4, 1, fp);

	for (int y = this->m_Height - 1; y >= 0; y--) {
		for (int x = 0; x < this->m_Width; x++) {

			Color tmp = this->m_Image[x + y * this->m_Width];
			uint8_t b = convertColorChannel(tmp.B);
			uint8_t g = convertColorChannel(tmp.G);
			uint8_t r = convertColorChannel(tmp.R);

			fwrite(&b, 1, 1, fp);
			fwrite(&g, 1, 1, fp);
			fwrite(&r, 1, 1, fp);
		}
	}

	if (fclose(fp) != 0) {
		cerr << "Datei konnte nicht geschlossen werden" << endl;
	}

	return true;
}
