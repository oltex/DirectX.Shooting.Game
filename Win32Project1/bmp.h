#ifndef __bmp_h__
#define __bmp_h__
#include <ddraw.h>
class CBMP{
private:
	int m_nWidth;
	int m_nHeight;
	unsigned char *m_pBuffer;
public:
	CBMP();
	~CBMP();
	int GetWidth();
	int GetHeight();
	bool LoadBMPFile(char *filename, bool left);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface);
};
#endif