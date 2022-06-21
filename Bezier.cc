#include <stddef.h> // for size_t
// #include <sys/stdtypes.h> // for size_t, on some systems
#include "Bezier.h"
#define _USE_MATH_DEFINES
#include <math.h>
/* The value of 1.0 / (1L<<23) is float machine epsilon. */

#ifdef FLOAT_ACCURACY
#define INV_EPS (1L<<23)
#else
/* The value of 1.0 / (1L<<14) is enough for most applications */
#define INV_EPS (1L<<14)
#endif
#define log2(x) (log(x)/log(2.))
inline double log4(double x) { return 0.5 * log2(x); }


void  Bezier::split(Bezier** l, Bezier** r)
{
 
	Vec2 l_p1 = vmid(p0, p1);
	Vec2 r_p2 = vmid(p2, p3);
	Vec2 r_p1 = vmid(p1, p2);
	Vec2 l_p2 = vmid(l_p1, r_p1);
	r_p1 = vmid(r_p1, r_p2);
	
	Vec2 l_p3 = vmid(l_p2, r_p1);
	Vec2 r_p0 = l_p3;

	*l = new Bezier(p0, l_p1, l_p2, l_p3);
	*r = new Bezier(r_p0, r_p1, r_p2, p3);
}

    
/*
* Test the bounding boxes of two Bezier curves for interference.
* Several observations:
*	First, it is cheaper to compute the bounding box of the second curve
*	and test its bounding box for interference than to use a more direct
*	approach of comparing all control points of the second curve with 
*	the various edges of the bounding box of the first curve to test
* 	for interference.
*	Second, after a few subdivisions it is highly probable that two corners
*	of the bounding box of a given Bezier curve are the first and last 
*	control point.  Once this happens once, it happens for all subsequent
*	subcurves.  It might be worth putting in a test and then short-circuit
*	code for further subdivision levels.
*	Third, in the final comparison (the interference test) the comparisons
*	should both permit equality.  We want to find intersections even if they
*	occur at the ends of segments.
*	Finally, there are tighter bounding boxes that can be derived. It isn't
*	clear whether the higher probability of rejection (and hence fewer
*	subdivisions and tests) is worth the extra work.
*/
inline bool intersectBB( Bezier* a, Bezier* b )
    {
    if( ( a->minx > b->maxx ) || ( a->miny > b->maxy )  // Not >= : need boundary case
	|| ( b->minx > a->maxx ) || ( b->miny > a->maxy ) )
		return false; // they don't intersect
    else
		return true; // they intersect
}
	
/* 
* Recursively intersect two curves keeping track of their real parameters 
* and depths of intersection.
* The results are returned in a 2-D array of doubles indicating the parameters
* for which intersections are found.  The parameters are in the order the
* intersections were found, which is probably not in sorted order.
* When an intersection is found, the parameter value for each of the two 
* is stored in the index elements array, and the index is incremented.
* 
* If either of the curves has subdivisions left before it is straight
*	(depth > 0)
* that curve (possibly both) is (are) subdivided at its (their) midpoint(s).
* the depth(s) is (are) decremented, and the parameter value(s) corresponding
* to the midpoints(s) is (are) computed.
* Then each of the subcurves of one curve is intersected with each of the 
* subcurves of the other curve, first by testing the bounding boxes for
* interference.  If there is any bounding box interference, the corresponding
* subcurves are recursively intersected.
* 
* If neither curve has subdivisions left, the line segments from the first
* to last control point of each segment are intersected.  (Actually the 
* only the parameter value corresponding to the intersection point is found).
*
* The apriori flatness test is probably more efficient than testing at each
* level of recursion, although a test after three or four levels would
* probably be worthwhile, since many curves become flat faster than their 
* asymptotic rate for the first few levels of recursion.
*
* The bounding box test fails much more frequently than it succeeds, providing
* substantial pruning of the search space.
*
* Each (sub)curve is subdivided only once, hence it is not possible that for 
* one final line intersection test the subdivision was at one level, while
* for another final line intersection test the subdivision (of the same curve)
* was at another.  Since the line segments share endpoints, the intersection
* is robust: a near-tangential intersection will yield zero or two
* intersections.
*/
int count = 0;
void recursivelyIntersect( Bezier* a, double t0, double t1, int deptha,
			   Bezier* b, double u0, double u1, int depthb,
			   std::vector<Vec2>& intersections)
{
    if( deptha > 0 )
	{
		Bezier* a_l;
		Bezier* a_r;
		count++;
		a->split(&a_l, &a_r);
		double tmid = (t0+t1)*0.5;
		deptha--;
		if( depthb > 0 )
		{
			Bezier* b_l;
			Bezier* b_r;
			b->split(&b_l, &b_r);
			double umid = (u0+u1)*0.5;
			depthb--;
			if( intersectBB( a_l, b_l ) )
				recursivelyIntersect(a_l, t0, tmid, deptha,
								b_l, u0, umid, depthb,
							  intersections );
			if( intersectBB(a_r, b_l) )
				recursivelyIntersect(a_r, tmid, t1, deptha,
					b_l, u0, umid, depthb,
					intersections);
			if( intersectBB(a_l, b_r ) )
				recursivelyIntersect(a_l, t0, tmid, deptha,
					b_r, umid, u1, depthb,
					intersections);
			if( intersectBB(a_r, b_r) )
				recursivelyIntersect(a_r, tmid, t1, deptha,
					b_r, umid, u1, depthb,
							  intersections );
			
			delete b_l;
			delete b_r;
		}
		else
		{
			if( intersectBB( a_l, b ) )
				recursivelyIntersect(a_l, t0, tmid, deptha,
							  b, u0, u1, depthb,
							  intersections );
			if( intersectBB(a_r, b ) )
				recursivelyIntersect(a_r, tmid, t1, deptha,
					b, u0, u1, depthb,
					intersections);
		}
		delete a_l;
		delete a_r;
	}
    else if( depthb > 0 )
	{
		Bezier* b_l;
		Bezier* b_r;
		b->split(&b_l, &b_r);
	    double umid = (u0 + u1)*0.5;
	    depthb--;
	    if( intersectBB( a, b_l) )
			recursivelyIntersect(a, t0, t1, deptha,
				b_l, u0, umid, depthb,
				intersections);
	    if( intersectBB( a, b_r) )
			recursivelyIntersect( a, t0, t1, deptha,
				b_r, umid, u1, depthb,
						  intersections );
		delete b_l;
		delete b_r;
	}
	else // Both segments are fully subdivided; now do line segments
	{
	    double xlk = a->p3.x - a->p0.x;
	    double ylk = a->p3.y - a->p0.y;
	    double xnm = b->p3.x - b->p0.x;
	    double ynm = b->p3.y - b->p0.y;
	    double xmk = b->p0.x - a->p0.x;
	    double ymk = b->p0.y - a->p0.y;
	    double det = xnm * ylk - ynm * xlk;
	    if( 1.0 + det == 1.0 )
			return;
	    else
		{
			double detinv = 1.0 / det;
			double s = (xnm * ymk - ynm * xmk) * detinv;
			double t = (xlk * ymk - ylk * xmk) * detinv;
			if ((s < 0.0) || (s > 1.0) || (t < 0.0) || (t > 1.0))
				return;
			intersections.push_back(Vec2(t0 + s * (t1 - t0), u0 + t * (u1 - u0)));
		}
	}
}


/*
 * Wang's theorem is used to estimate the level of subdivision required,
 * but only if the bounding boxes interfere at the top level.
 * Assuming there is a possible intersection, RecursivelyIntersect is
 * used to find all the parameters corresponding to intersection points.
 * these are then sorted and returned in an array.
 */

void findIntersections(Bezier* a, Bezier* b, std::vector<Vec2>& intersections)
{
    if( intersectBB( a, b ) )
	{
		Vec2 la1 = vabs( ( (a->p2) - (a->p1) ) - ( (a->p1) - (a->p0) ) );
		Vec2 la2 = vabs( ( (a->p3) - (a->p2) ) - ( (a->p2) - (a->p1) ) );
		Vec2 la;
		if( la1.x > la2.x ) la.x = la1.x; else la.x = la2.x;
		if( la1.y > la2.y ) la.y = la1.y; else la.y = la2.y;
		Vec2 lb1 = vabs( ( (b->p2) - (b->p1) ) - ( (b->p1) - (b->p0) ) );
		Vec2 lb2 = vabs( ( (b->p3) - (b->p2) ) - ( (b->p2) - (b->p1) ) );
		Vec2 lb;
		if( lb1.x > lb2.x ) lb.x = lb1.x; else lb.x = lb2.x;
		if( lb1.y > lb2.y ) lb.y = lb1.y; else lb.y = lb2.y;
		double l0;
		if( la.x > la.y ) 
			l0 = la.x;
		else 
			l0 = la.y;
		int ra;
		if( l0 * 0.75 * M_SQRT2 + 1.0 == 1.0 ) 
			ra = 0;
		else
			ra = (int)ceil( log4( M_SQRT2 * 6.0 / 8.0 * INV_EPS * l0 ) );
		if( lb.x > lb.y ) 
			l0 = lb.x;
		else 
			l0 = lb.y;
		int rb;
		if( l0 * 0.75 * M_SQRT2 + 1.0 == 1.0 ) 
			rb = 0;
		else
			rb = (int)ceil(log4( M_SQRT2 * 6.0 / 8.0 * INV_EPS * l0 ) );
		recursivelyIntersect( a, 0., 1., ra, b, 0., 1., rb, intersections );
	}
}

    


void Bezier::intersect( Bezier* b, std::vector<Vec2>& intersections)
{
	findIntersections( this, b,intersections );
 
}
