#ifndef __PLANE_MESH__
#define __PLANE_MESH__

#include "Mesh.h"

class PlaneMesh : public Mesh
{
public:
    static const int XY = 0;
    static const int XZ = 1;
    static const int YZ = 2;

    int dir;

    float orgWid = 0;
    float orgLen = 0;
    float width = 1.0;
    float length = 1.0;
    int resWid = 5.0;
    int resLen = 5.0;

    float leapWid;
    float leapLen;


    PlaneMesh(int direction, float wid, float len,
        int res_width = 5, int res_length = 5,
        float orig_width = 0, float orig_length = 0)
    {

        dir = direction;
        width = wid;
        length = len;
        
        resWid = res_width;
        resLen = res_length;

        orgWid = orig_width;
        orgLen = orig_length;


        if (XY == dir) {
            createXY();
        }
        else if (XZ == dir) {
            createXZ();
        }
        else {
            createYZ();
        }
    }

    void createXZ(float height = 0.0);
    void createXY(float height = 0.0) {}
    void createYZ(float height = 0.0) {}

};

#endif