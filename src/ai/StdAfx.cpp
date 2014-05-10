#include <cstdlib>
#include <vector>
#include <cassert>
#include <ctime>
#include <sys/time.h>
using namespace std;

void ZeroMemory(void * data, int n)
{
	memset(data, 0, n);
}

int GetTickCount()
{
	struct  timezone   tz;
	struct  timeval    tv;
	gettimeofday(&tv, &tz);
	return tv.tv_sec*1000 + tv.tv_usec/1000;
}
