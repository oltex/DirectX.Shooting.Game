#ifndef __bossmissile_h__
#define __bossmissile_h__
#include "gobject.h"
#include "block.h"

enum { BOSSBULLET1, BOSSBULLET2 };

class CBossMissile : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int m_xSpeed, m_ySpeed;
	int m_bulletType;
	int m_damage;
	bool m_bIsLeft;
public:
	CBossMissile();
	~CBossMissile();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int s_x, int s_y, bool bIsLeft, int bulletType);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y);
	void Handle(CBlock* block, int max_block, int x, int y);
	int GetDamage();
};
#endif