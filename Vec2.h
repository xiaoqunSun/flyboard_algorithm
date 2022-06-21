// Floating point vector library
#ifndef VECTORS_INCLUDED__
#include <math.h>

struct Vec2
{
	double x, y;
    Vec2()
	{
		x = y = 0.0f;
	}
    Vec2( const Vec2 &v )
	{
		x = v.x, y = v.y;
	}
    Vec2( double _x, double _y )
	{
		x = _x, y = _y;
	}
};

inline Vec2 operator-(const Vec2& a, const Vec2& b ) // p - p = v
{
    return Vec2( a.x - b.x, a.y - b.y );
}
      
inline Vec2 operator+(const Vec2& a, const Vec2& b ) // p + v = p
{
    return Vec2( a.x + b.x, a.y + b.y );
}
    
inline Vec2 vabs( const Vec2& a )
{
    return Vec2( fabs( a.x ), fabs( a.y ) );
};
inline Vec2 vmid(const Vec2& a, const Vec2& b)
{
	return Vec2((a.x + b.x) / 2, (a.y + b.y) / 2);
};


#define VECTORS_INCLUDED__
#endif
