#include <cstdio>
#include <windows.h>
#include <cmath>
#include <ctime>
#include <gdiplus.h>
#pragma comment(lib, "GdiPlus.lib")  

void CreateBmp24(int startColor, int endColor);
void CreateBmp32(int startColor, int endColor);
INT transBMPtoPNG(const WCHAR* bmp, const WCHAR* png);
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
//
//int main(int argc, char **argv)
//{
//	int startColor24 = 255 << 16 | 0 << 8 | 0;
//	int endColor24 = 0 << 16 | 0 << 8 | 255;
//
//	int startColor32 = 255 << 24 | 0 << 16 | 0<<8 | 255;
//	int endColor32 = 0 << 24 | 0 << 16 | 255<<8 | 127;
//
//	CreateBmp24(startColor24, endColor24);
//	CreateBmp32(startColor32, endColor32);
//
//	return 0;
//}

void CreateBmp24(int startColor, int endColor)
{
	char szFileName[32];
	time_t ltime;
	struct tm* pnow;
	HANDLE hFile;
	BYTE *pData = NULL;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	DWORD dwBytesWrite;

	int width = 1024, height = 512;
	int startR = (startColor >> 16) & 0xFF;
	int startG = (startColor >> 8) & 0xFF;
	int startB = startColor & 0xFF;
	int endR = (endColor >> 16) & 0xFF;
	int endG = (endColor >> 8) & 0xFF;
	int endB = endColor & 0xFF;

	// allocate memory for data area
	pData = (BYTE*)malloc(width * height * 3);
	if (pData == NULL)
		return;


	int n = 100;
	float dR = (endR - startR)*1.0f / n;
	float dG = (endG - startG)*1.0f / n;
	float dB = (endB - startB)*1.0f / n;

	float R = startR;
	float G = startG;
	float B = startB;
	float n_height = height*1.0f / n ;
	float h_top = n_height;

	for (int i = 0; i < width  * height * 3; i+=3)
	{
		pData[i] = (BYTE)(pow(B/255, 1 / 2.2)*255);
		pData[i + 1] = (BYTE)(pow(G / 255, 1 / 2.2) * 255);
		pData[i + 2] = (BYTE)(pow(R / 255, 1 / 2.2) * 255);
	
		if ((i+3) / (width * 3) >=h_top)
		{
			R += dR;
			G += dG;
			B += dB;
			h_top += n_height;
		}
	}

	//for (i = 0; i < (int)(width * height * 3); i++)
	//{
	//	do
	//	{
	//		

	//	} while (num < 0 || num > 255);
	//	pData[i] = num;
	//}

	// initialize the structures

	RtlZeroMemory(&bmfHdr, sizeof(bmfHdr));
	RtlZeroMemory(&bi, sizeof(bi));

	// fill the necessary struct fields of the BITMAPFILEHEADER

	bmfHdr.bfType = 0x4d42; // "BM"

	bmfHdr.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
	bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// fill the necessary struct fields of the BITMAPINFOHEADER

	bi.biSize = sizeof(bi);
	bi.biBitCount = 24;
	bi.biHeight = height;
	bi.biWidth = width;
	bi.biCompression = BI_RGB;
	bi.biPlanes = 1;



	// create the bmp file

	hFile = CreateFile("./output24.bmp", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		free(pData);
		return;
	}

	// write the BITMAPFILEHEADER

	WriteFile(hFile, &bmfHdr, sizeof(BITMAPFILEHEADER), &dwBytesWrite, NULL);
	// write the BITMAPINFOHEADER

	WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwBytesWrite, NULL);
	// write the bmp data

	WriteFile(hFile, pData, width * height * 3, &dwBytesWrite, NULL);

	// release resources

	CloseHandle(hFile);
	free(pData);
}


void CreateBmp32(int startColor, int endColor)
{
	char szFileName[32];
	time_t ltime;
	struct tm* pnow;
	HANDLE hFile;
	BYTE *pData = NULL;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	DWORD dwBytesWrite;

	int width = 1024, height = 512;
	int startR = (startColor >> 24) & 0xFF;
	int startG = (startColor >> 16) & 0xFF;
	int startB = (startColor>>8) & 0xFF;
	int startA = startColor & 0xFF;
	int endR = (endColor >> 24) & 0xFF;
	int endG = (endColor >> 16) & 0xFF;
	int endB = (endColor >> 8) & 0xFF;
	int endA = endColor & 0xFF;

	// allocate memory for data area
	pData = (BYTE*)malloc(width * height * 4);
	if (pData == NULL)
		return;

	int n = 100;
	float dR = (endR - startR)*1.0f / n;
	float dG = (endG - startG)*1.0f / n;
	float dB = (endB - startB)*1.0f / n;
	float dA = (endA - startA)*1.0f / n;

	float R = startR;
	float G = startG;
	float B = startB;
	float A = startA;

	float n_height = height*1.0f / n;
	float h_top = n_height;

	for (int i = 0; i < width*height * 4; i += 4)
	{
		pData[i] = (BYTE)(pow(B / 255, 1 / 2.2) * 255);
		pData[i + 1] = (BYTE)(pow(G / 255, 1 / 2.2) * 255);
		pData[i + 2] = (BYTE)(pow(R / 255, 1 / 2.2) * 255);
		pData[i + 3] = (BYTE)(pow(A / 255, 1 / 2.2) * 255);

		if ((i + 4) / (width * 4) >= h_top)
		{
			R += dR;
			G += dG;
			B += dB;
			A += dA;
			h_top += n_height;
		}
	}

	//for (i = 0; i < (int)(width * height * 3); i++)
	//{
	//	do
	//	{
	//		

	//	} while (num < 0 || num > 255);
	//	pData[i] = num;
	//}

	// initialize the structures

	RtlZeroMemory(&bmfHdr, sizeof(bmfHdr));
	RtlZeroMemory(&bi, sizeof(bi));

	// fill the necessary struct fields of the BITMAPFILEHEADER

	bmfHdr.bfType = 0x4d42; // "BM"

	bmfHdr.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 4;
	bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// fill the necessary struct fields of the BITMAPINFOHEADER

	bi.biSize = sizeof(bi);
	bi.biBitCount = 32;
	bi.biHeight = height;
	bi.biWidth = width;
	bi.biCompression = BI_RGB;
	bi.biPlanes = 1;

	// create the bmp file

	hFile = CreateFile("./output32.bmp", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		free(pData);
		return;
	}

	// write the BITMAPFILEHEADER

	WriteFile(hFile, &bmfHdr, sizeof(BITMAPFILEHEADER), &dwBytesWrite, NULL);
	// write the BITMAPINFOHEADER

	WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwBytesWrite, NULL);
	// write the bmp data

	WriteFile(hFile, pData, width * height * 4, &dwBytesWrite, NULL);

	// release resources

	CloseHandle(hFile);
	free(pData);

	transBMPtoPNG(L"./output32.bmp", L"./output32.png");
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

INT transBMPtoPNG(const WCHAR* bmp, const WCHAR* png)
{
	// Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	CLSID   encoderClsid;
	Gdiplus::Status  stat;
	Gdiplus::Image*   image = new Gdiplus::Image(bmp);

	// Get the CLSID of the PNG encoder.
	GetEncoderClsid(L"image/png", &encoderClsid);

	stat = image->Save(png, &encoderClsid, NULL);

	if (stat == Gdiplus::Ok)
		printf("png was saved successfully\n");
	else
		printf("Failure: stat = %d\n", stat);

	delete image;
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}