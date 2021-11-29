#ifndef PATH_H
#define PATH_H

#include <list>
#include <cassert>
#include "Vector3f.h"

class Path
{
    public:
        Path();
        Path(float, float, float, float, bool);
        virtual ~Path();
        std::list<Vector3f> getPath()const{return m_waypoints;}
        void loopOn();
        void loopOff();
        void clearPath();
        void SetNextWayPoint();
        bool empty();
        Vector3f getCurrentWayPoint()const{return *current_waypoint;}
        void recalculatePath(float,float,float,float);
    protected:

        void createPath(float, float, float, float);

    private:

        std::list<Vector3f>                 m_waypoints;
        std::list<Vector3f>::iterator       current_waypoint;
        bool m_loop;
};


#endif // PATH_H
