#ifndef _exploding_h_
#define _exploding_h_
#include "gobject.h"
class CExploding : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
	bool m_bIsLeft;
public:
	CExploding();
	~CExploding();
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval, int min_nCurrentFrame, int max_nCurrentFrame, int bIsLeft, int spriteType);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y);
	void Handle();
};
#endif