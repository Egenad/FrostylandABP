#include "Path.h"
#include <math.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

const float TWO_PI = 2.0 * 3.1415926535897932384626433832795;

Path::Path(){
    m_loop = false;
}

Path::Path(float min_x, float min_z, float max_x, float max_z, bool loop)
{
    //ctor
    m_loop = loop;
    createPath(min_x, min_z, max_x, max_z);
    std::srand(std::time(nullptr)); // use current time as seed for random generator
}

Path::~Path()
{
    //dtor
    clearPath();
}

void Path::loopOff(){
    m_loop = false;
}

void Path::loopOn(){
    m_loop = true;
}

void Path::clearPath(){
    m_waypoints.clear();
}

void Path::SetNextWayPoint(){
    if(m_waypoints.size() > 0){
        if(++current_waypoint == m_waypoints.end()){
            if(m_loop){
                current_waypoint = m_waypoints.begin();
            }
        }
    }
}

void Path::createPath(float min_x, float min_z, float max_x, float max_z){

    clearPath();

    Vector3f temp(max_x,min_z);
    m_waypoints.push_back(temp);
    temp.setZ(max_z);
    m_waypoints.push_back(temp);
    temp.setX(min_x);
    m_waypoints.push_back(temp);
    temp.setZ(min_z);
    m_waypoints.push_back(temp);

    current_waypoint = m_waypoints.begin();
}

bool Path::empty(){
    if(m_waypoints.size() > 0){
        return false;
    }
    return true;
}

void Path::recalculatePath(float c_x, float c_z, float w, float h){

    clearPath();
    
    int random_variable = (std::rand()%4);
    
    int min_x;
    int max_x;
    int min_z;
    int max_z;

    switch(random_variable){
        case 0:
            min_x = c_x - (w/2) + 100;
            max_x = c_x + (w/2) - 100;
            min_z = c_z - (h/2) + 100;
            max_z = c_z - (h/2) + 200;
        break;
        case 1:
            min_x = c_x - (w/2) + 100;
            max_x = c_x + (w/2) - 100;
            min_z = c_z + (h/2) - 100;
            max_z = c_z + (h/2) - 200;
        break;
        case 2:
            min_x = c_x - (w/2) + 100;
            max_x = c_x - (w/2) + 200;
            min_z = c_z - (h/2) + 100;
            max_z = c_z + (h/2) - 100;
        break;
        case 3:
            min_x = c_x + (w/2) - 100;
            max_x = c_x + (w/2) - 200;
            min_z = c_z - (h/2) + 100;
            max_z = c_z + (h/2) - 100;
        break;
        default:
            min_x = c_x - (w/2) + 100;
            max_x = c_x + (w/2) - 100;
            min_z = c_z - (h/2) + 100;
            max_z = c_z - (h/2) + 200;
        break;
    }

    Vector3f temp(min_x,min_z);
    m_waypoints.push_back(temp);
    temp.setX(max_x);
    m_waypoints.push_back(temp);
    temp.setZ(max_z);
    m_waypoints.push_back(temp);
    temp.setX(min_x);
    m_waypoints.push_back(temp);

    /*std::cout << "Path: \n";
    std::cout << min_x << "," << min_z << "\n";
    std::cout << max_x << "," << min_z << "\n";
    std::cout << max_x << "," << max_z << "\n";
    std::cout << min_x << "," << max_z << "\n";*/

    current_waypoint = m_waypoints.begin();
}


