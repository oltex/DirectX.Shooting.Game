#include <Windows.h>
#include <windowsx.h>
#include "bmp.h"

CBMP::CBMP(){
	m_pBuffer = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
}
CBMP::~CBMP(){
	if (m_pBuffer)
		delete[] m_pBuffer;
}
int CBMP::GetHeight(){
	return (m_nHeight);
}
int CBMP::GetWidth(){
	return (m_nWidth);
}
bool CBMP::LoadBMPFile(char *filename, bool left){
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (hfile == INVALID_HANDLE_VALUE)
		return false;
	BITMAPFILEHEADER bmpfilehead;
	if (!ReadFile(hfile, &bmpfilehead, sizeof(bmpfilehead), &actualRead, NULL)){
		CloseHandle(hfile);
		return false;
	}
	if (bmpfilehead.bfType != 0x4D42){
		CloseHandle(hfile);
		return false;
	}
	BITMAPINFOHEADER bmpinfohead;
	if (!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL)){
		CloseHandle(hfile);
		return false;
	}
	if (bmpinfohead.biBitCount != 24){
		CloseHandle(hfile);
		return false;
	}
	int nHeight = bmpinfohead.biHeight;
	bool bBottomUp;
	if (nHeight > 0){
		bBottomUp = true;
	}
	else{
		bBottomUp = false;
		nHeight = -nHeight;
	}
	m_nHeight = nHeight;
	m_nWidth = bmpinfohead.biWidth;
	if (m_pBuffer)
		delete[] m_pBuffer;
	m_pBuffer = new unsigned char[m_nHeight * m_nWidth * 4];
	struct RBGstruct{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};
	struct RBGstruct rgb24;
	int nStoredLine = (m_nWidth * 3 + 3)&~3;
	unsigned char temp[4];
	int nRemainder = nStoredLine - (m_nWidth * 3);
	int nDestY, nDeltaY;
	if (bBottomUp){
		nDestY = nHeight - 1;
		nDeltaY = -1;
	}
	else{
		nDestY = 0;
		nDeltaY = 1;
	}
	if (left == false){
		for (int y = 0; y < m_nHeight; y++){
			for (int x = 0; x < m_nWidth; x++){
				if (!ReadFile(hfile, &rgb24, 3, &actualRead, NULL)){
					CloseHandle(hfile);
					delete[] m_pBuffer;
					return false;
				}
				CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2), &rgb24, 3);
				*(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2) + 3) = 0;
			}
			nDestY += nDeltaY;
			if (!ReadFile(hfile, temp, nRemainder, &actualRead, NULL)){
				CloseHandle(hfile);
				delete[] m_pBuffer;
				return false;
			}
		}
	}
	else{
		for (int y = 0; y < m_nHeight; y++){
			for (int x = m_nWidth - 1; x >= 0; x--){
				if (!ReadFile(hfile, &rgb24, 3, &actualRead, NULL)){
					CloseHandle(hfile);
					delete[] m_pBuffer;
					return false;
				}
				CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2), &rgb24, 3);
				*(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2) + 3) = 0;
			}
			nDestY += nDeltaY;
			if (!ReadFile(hfile, temp, nRemainder, &actualRead, NULL)){
				CloseHandle(hfile);
				delete[] m_pBuffer;
				return false;
			}
		}
	}
	CloseHandle(hfile);
	return true;
}
bool CBMP::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface){
	if (!m_pBuffer)
		return false;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	if (FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))
		return false;
	unsigned char *pDest, *pSrc;
	pDest = (unsigned char *)ddsd.lpSurface;
	pSrc = m_pBuffer;
	for (int y = 0; y < m_nHeight; y++){
		CopyMemory(pDest, pSrc, m_nWidth << 2);
		pDest += ddsd.lPitch;
		pSrc += (m_nWidth << 2);
	}
	lpSurface->Unlock(NULL);
	return true;
}