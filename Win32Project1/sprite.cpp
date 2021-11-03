#include <Windows.h>
#include "sprite.h"
static RECT destRect;
static RECT destRect2;
CSprite::CSprite(){
	m_ppSurface = NULL;
	m_nFrame = 0;
	m_pBMPArray = NULL;
}
CSprite::~CSprite(){
	if (m_ppSurface)
		delete[] m_ppSurface;
	if (m_pBMPArray)
		delete[] m_pBMPArray;
}
bool CSprite::InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw){
	if (m_ppSurface)
		delete[] m_ppSurface;
	if (m_pBMPArray)
		delete[] m_pBMPArray;
	m_ppSurface = new LPDIRECTDRAWSURFACE7[nFrame];

	if (!m_ppSurface)
		return false;
	m_pBMPArray = new CBMP[nFrame];
	if (!m_pBMPArray)
		return false;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = nHeight;
	ddsd.dwWidth = nWidth;
	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = nColorKey;
	for (int i = 0; i < nFrame; i++){
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_ppSurface[i], NULL))){
			m_ppSurface[i] = NULL;
			return false;
		}
		else{
			m_ppSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nFrame = nFrame;
	m_leftFrame = nFrame / 2;

	return true;
}
bool CSprite::LoadFrame(int nFrame, char* filename){
	if (!m_pBMPArray[nFrame].LoadBMPFile(filename, false))
		return false;
	if (!m_pBMPArray[nFrame].CopyBufferToSurface(m_ppSurface[nFrame]))
		return false;
	if (!m_pBMPArray[nFrame + m_leftFrame].LoadBMPFile(filename, true))
		return false;
	if (!m_pBMPArray[nFrame + m_leftFrame].CopyBufferToSurface(m_ppSurface[nFrame + m_leftFrame]))
		return false;
	return true;
}
bool CSprite::Drawing(bool isLeft, int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey){

	RECT destRect;
	destRect.left = x - (m_nWidth >> 1);
	destRect.top = y - (m_nHeight >> 1);
	destRect.right = destRect.left + m_nWidth;
	destRect.bottom = destRect.top + m_nHeight;

	HRESULT hResult;
	if (isLeft){
		if (bUsingColorKey){
			if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame + m_leftFrame], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL))){
				if (hResult == DDERR_SURFACELOST)
					return (Restore());
				else
					return false;
			}
		}
		else{
			if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame + m_leftFrame], NULL, DDBLT_WAIT, NULL))){
				if (hResult == DDERR_SURFACELOST)
					return (Restore());
				else
					return false;
			}
		}
	}
	else{
		if (bUsingColorKey){
			if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL))){
				if (hResult == DDERR_SURFACELOST)
					return (Restore());
				else
					return false;
			}
		}
		else{
			if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT, NULL))){
				if (hResult == DDERR_SURFACELOST)
					return (Restore());
				else
					return false;
			}
		}
	}
}
bool CSprite::Restore(){
	if (!m_ppSurface)
		return false;
	for (int i = 0; i < m_nFrame; i++){
		if (!m_ppSurface[i])
			return false;
		if (FAILED(m_ppSurface[i]->Restore()))
			return false;
		if (!m_pBMPArray[i].CopyBufferToSurface(m_ppSurface[i]))
			return false;
	}
	return true;
}
int CSprite::GetNumberOfFrame(){
	return m_nFrame;
}
bool CSprite::ReleaseAll(){
	if (!m_ppSurface)
		return false;
	for (int i = 0; i < m_nFrame; i++){
		if (m_ppSurface[i])
			m_ppSurface[i]->Release();
	}
	return true;
}