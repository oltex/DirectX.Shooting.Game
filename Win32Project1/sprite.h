#ifndef _sprite_h_
#define _sprite_h_
#include <ddraw.h>
#include "bmp.h"
class CSprite{
private:
	int m_nFrame;
	int m_nWidth;
	int m_nHeight;
	int m_leftFrame;
	LPDIRECTDRAWSURFACE7* m_ppSurface;
	CBMP* m_pBMPArray;
public:
	CSprite();
	~CSprite();
	int GetNumberOfFrame();
	bool InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadFrame(int nFrame, char* filename);
	bool Drawing(bool isLeft, int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Restore();
	bool ReleaseAll();
};
#endif