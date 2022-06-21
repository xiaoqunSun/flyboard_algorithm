#ifndef _BEZIER_INCLUDED_
#include "Vec2.h"
#include <vector>
struct Bezier{
	Bezier(){}
    Bezier(const Vec2& _p0, const Vec2& _p1
		, const Vec2& _p2, const Vec2& _p3 ):p0(_p0), p1(_p1), p2(_p2), p3(_p3)
	{
		if (p0.x > p3.x)	
			minx = p3.x, maxx = p0.x;
		else
			minx = p0.x, maxx = p3.x;
		if (p2.x < minx)
			minx = p2.x;
		else if (p2.x > maxx)
			maxx = p2.x;
		if (p1.x < minx)
			minx = p1.x;
		else if (p1.x > maxx)
			maxx = p1.x;
		if (p0.y > p3.y)
			miny = p3.y, maxy = p0.y;
		else
			miny = p0.y, maxy = p3.y;
		if (p2.y < miny)
			miny = p2.y;
		else if (p2.y > maxy)
			maxy = p2.y;
		if (p1.y < miny)
			miny = p1.y;
		else if (p1.y > maxy)
			maxy = p1.y;
	}
    void  split(Bezier** l, Bezier** r);

	void intersect(Bezier* b,std::vector<Vec2>& intersections);

	Vec2 p0, p1, p2, p3;
	double minx, maxx, miny, maxy;
};
#define _BEZIER_INCLUDED_
#endif
