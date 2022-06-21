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
#else
#include <sys/time.h>
#endif // _WIN32

extern "C" 
{ 
	const char *opus_get_version_string(void)
	{
		return "asdafdfad";
	}
		int *multiply(int a, int b)
	{
		// Allocates native memory in C.
		int *mult = (int *)malloc(sizeof(int));
		*mult = a * b;

		//printf("malloc_pointer%p\n",mult);
		return mult;
	}

	void free_pointer(int *int_pointer)
	{
		// Free native memory in C which was allocated in C.
		free(int_pointer);

		//printf("free_pointer%p\n",int_pointer);
	}

	
}

// double gettime()
// {
// 	struct timeval tv;
// 	gettimeofday(&tv, nullptr);

// 	return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
// }

// int main()
//     {
// 	point p0 = point(471.20001220703125, 258.6000061035156);
// 	point p1 = point(672.2666625976562, 398.3333435058594);
// 	point p2 = point(504.5333251953125, 676.7333374023438);
// 	point p3 = point(370.3999938964844, 781);
// 	point q0 = point(611.2000732421875, 186.59999084472656);
// 	point q1 = point(498.13338216145837, 418.60000101725257);
// 	point q2 = point(612.8000284830729, 757.000010172526);
// 	point q3 = point(698.4000244140625, 868.2000122070312);
// 	Bezier A = Bezier(&p0, &p1, &p2, &p3);
// 	Bezier B = Bezier(&q0, &q1, &q2, &q3);

// 	double t = gettime();
// 	double** r;
// 	for (int i = 1;i<1000;++i)
// 	{


// 		r = B.Intersect(A);
// 	}
// 	printf("cost %f\n", gettime() - t);
// 	for (int i= 0;i < 9;++i)
// 	{
// 		printf("%f  %f\n", r[0][i], r[1][i]);
// 	}
	
//     }
