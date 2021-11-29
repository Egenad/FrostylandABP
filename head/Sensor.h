#ifndef SENSOR_H
#define SENSOR_H

#include "Vector3f.h"

class Sensor
{
    public:
        Sensor();
        virtual ~Sensor();
        bool detectEntity(int, Vector3f); //int = id of the entity that has this sensor, Vector3f = position
        //of the entity is looking at
};

#endif // SENSOR_H