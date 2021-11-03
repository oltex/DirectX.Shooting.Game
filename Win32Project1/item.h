#ifndef _item_h_
#define _item_h_
#include "gobject.h"
#include "block.h"

enum { ITEMBULLET, ITEMBOMB };

class CItem : public CGObject {
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int m_ySpeed;
	int m_gravity = 1;
	int m_itemType;
public:
	CItem();
	~CItem();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int itemType);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y);
	void Handle(CBlock* block,int max_block);
	int GetItemType();
};
#endif