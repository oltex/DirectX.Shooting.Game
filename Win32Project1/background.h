#ifndef _background_h_
#define _background_h_
#include "gobject.h"
class CBackground : public CGObject {
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int p_y;
public:
	CBackground();
	~CBackground();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x);
	void Handle(int x);
	int GetPY();
};
#endif