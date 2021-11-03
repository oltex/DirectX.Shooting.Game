#ifndef _random_h_
#define _random_h_
class CRandom{
private:
	int m_nCount;
	void sowseed();
public:
	CRandom();
	~CRandom();
	int number(int m, int n);
};
#endif