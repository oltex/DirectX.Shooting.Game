#ifndef __enemymissile_h__
#define __enemymissile_h__
#include "gobject.h"
#include "block.h"

enum { THROWBOMB, ROLLBOMB, TANKBULLET, HELICOPTERBULLET };

class CEnemyMissile : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
	bool m_bIsLeft;
	int m_xSpeed = 0, m_ySpeed = 0, m_bulletType;
	int m_gravity = 1;
	int m_damage;
public:
	CEnemyMissile();
	~CEnemyMissile();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int s_x, int s_y, bool bIsLeft, int bulletType);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y);
	void Handle(CBlock* block, int max_block, int x, int y);
	int GetDamage();
};
#endif