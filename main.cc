#include "Bezier.h"
#include <stdio.h>
extern "C" 
{ 
	const char* get_version(void)
	{
		return "1.0.0";
	}

    double* bezier_intersect(double* p)
    {

        Bezier A = Bezier(Vec2(p[0], p[1]), Vec2(p[2], p[3])
                        , Vec2(p[4], p[5]), Vec2(p[6], p[7]));
        Bezier B = Bezier(Vec2(p[8], p[9]), Vec2(p[10], p[11])
                        , Vec2(p[12], p[13]), Vec2(p[14], p[15]));

        std::vector<Vec2> intersections;
        B.intersect(&A, intersections);    

        int double_count = intersections.size() * 2 + 1;

        double* result = (double*)malloc(sizeof(double)*double_count);
        result[double_count-1] = -1.0f;
	    for (unsigned i = 0; i < intersections.size(); ++i)
	    {
            result[i*2] = intersections[i].x;
            result[i*2+1] = intersections[i].y;
	    }
        printf("bezier_intersect return %p\n",result);
        return result;
    }

    void free_pointer(void* p)
	{
        printf("free_pointer %p\n",p);
	    free(p);
	}

}

