#ifndef _timer_h_
#define _timer_h_
class CTimer{
private:
	int m_nStartTime;
public:
	CTimer();
	~CTimer();
	void start();
	int time();
	bool elapsed(int &stime, int interval);
};
#endif