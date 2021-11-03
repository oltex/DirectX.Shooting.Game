#include "item.h"
CItem::CItem() {
}
CItem::~CItem() {
}
void CItem::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int itemType) {
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_ySpeed = 0;
	m_itemType = itemType;
	switch (m_itemType){
	case ITEMBULLET:
		SetRect(10, 10, 10, 10);
		SetSprite(0, 3);
		break;
	case ITEMBOMB:
		SetRect(10, 10, 10, 10);
		SetSprite(4, 9);
		break;
	}
}
void CItem::Handle(CBlock* block, int max_block){
	m_ySpeed += m_gravity;
	m_y += m_ySpeed;
	SetRect(10, 10, 10, 10);
	for (int i = 0; i < max_block; i++) {
		if (m_ySpeed > 0 && block[i].GetCollider(m_x, m_y, m_rect.bottom) != -1) {
			m_ySpeed = 0;
			m_y = block[i].GetCollider(m_x, m_y, m_rect.bottom);
		}
	}
}
void CItem::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y){
	CGObject::Draw(m_x - (x - p_x), m_y - (y - p_y), false, lpSurface);
}

int CItem::GetItemType(){
	return m_itemType;
}