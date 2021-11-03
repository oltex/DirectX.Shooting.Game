#ifndef _gobject_h_
#define _gobject_h_
#include "sprite.h"
#include "timer.h"
#include <ddraw.h>

class CGObject{
public:
	int m_x;
	int m_y;
protected:
	CSprite* m_pSprite;
	CTimer* m_pTimer;
	int m_nFrameInterval;
	int m_nCurrentFrame;
	int m_nLastFrameTime;
	bool m_bIsLive;
public:
	RECT m_rect;
	int m_spriteType;
	int m_min_nCurrentFrame, m_max_nCurrentFrame;
public:
	CGObject();
	~CGObject();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval);
	void Draw(int x, int y, bool isLeft, LPDIRECTDRAWSURFACE7 lpSurface);
	bool IsLive();
	void Kill();
	void SetSprite(int min_nCurrentFrame, int max_nCurrentFrame);
	void SetSprite(int min_nCurrentFrame, int max_nCurrentFrame, int SpriteType);
	void SetRect(int bottom, int left, int right, int top);
	int GetX();
	int GetY();
	int GetCurrentFrame();
	RECT GetRect();
};
#endif