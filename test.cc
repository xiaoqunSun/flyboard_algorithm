#include "Bezier.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>



int gettimeofday(struct timeval * val, struct timezone *)
{
	if (val)
	{
		LARGE_INTEGER liTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liTime);
		val->tv_sec = (long)(liTime.QuadPart / liFreq.QuadPart);
		val->tv_usec = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0);
	}
	return 0;
}
#endif // _WIN32


double gettime()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);

	return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}

int main()
    {
	Vec2 p0 = Vec2(471.20001220703125, 258.6000061035156);
	Vec2 p1 = Vec2(672.2666625976562, 398.3333435058594);
	Vec2 p2 = Vec2(504.5333251953125, 676.7333374023438);
	Vec2 p3 = Vec2(370.3999938964844, 781);
	Vec2 q0 = Vec2(611.2000732421875, 186.59999084472656);
	Vec2 q1 = Vec2(498.13338216145837, 418.60000101725257);
	Vec2 q2 = Vec2(612.8000284830729, 757.000010172526);
	Vec2 q3 = Vec2(698.4000244140625, 868.2000122070312);
	Bezier A = Bezier(p0, p1, p2, p3);
	Bezier B = Bezier(q0, q1, q2, q3);

	double t = gettime();
	std::vector<Vec2> intersections;
	for (int i = 0; i < 1000; ++i)
	{
		intersections.clear();
		B.intersect(&A, intersections);
	}
	printf("cost %f\n", gettime() - t);
	for (unsigned i = 0; i < intersections.size(); ++i)
	{
		printf("%f  %f\n", intersections[i].x, intersections[i].y);
	}
    }
