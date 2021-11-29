#ifndef APARTICLESYSTEM_H
#define APARTICLESYSTEM_H
#include "AEntity.h"

class AMeshResource;

class AParticleSystem : public AEntity {
    public:
        
        AParticleSystem(AMeshResource* nShape, glm::vec3 nPosition, float nRadius, int nBirthrate, float nParticleLife, glm::vec3 nBirthDirection, float nBirthSize, glm::vec4 nBirthColor);        //Simple emitter contructor, half of the parameters are deduced

        virtual ~AParticleSystem();        //Delete particle emitter and its particles

        void update(float dTime);   //Update particle properties
        void draw(GLuint programID);//Draw current particles

        virtual void beginDraw() {} //This function is unused here
        virtual void endDraw() {}   //This function is unused here

        //Functions to be used by particles
        void setParticleLifeData(float & nCurrentLife, float & nParticleLife);
        void setParticlePositionData(glm::vec3 & nPosition);
        void setParticleDirectionData(glm::vec3 & nBirthDirection, glm::vec3 & nDeathDirection);
        void setParticleSizeData(float & nCurrentSize, float & nBirthSize, float & nDeathSize);
        void setParticleColorData(glm::vec4 & nCurrentColor, glm::vec4 & nBirthColor, glm::vec4 & nDeathColor);

    private:

        class Particle;

        //Variables relared to particles
        AMeshResource*  shape;                  //Shape to be used as particle

        glm::vec3 position;               //3D position of the emitter
        float radius;                 //Radius of the emitter

        int birthrate;              //Maximum number of particles at once
        float particleLife;           //Lifespan of a particle

        glm::vec3 birthDirection;         //Initial direction of a particle
        glm::vec3 deathDirection;         //Final direction of a particle
        float variationDirection;     //Change in direction of particles (from 0 - 1, 1 being completely random)

        float birthSize;              //Initial scale of a particle
        float deathSize;              //Final scale of a particle
        float variationSize;          //Change in size of particles (from 0 - 1, 1 being completely random)

        glm::vec4 birthColor;             //Initial color of a particle
        glm::vec4 deathColor;             //Final color of a particle
        float variationColor;         //Change in color of particles (from 0 - 1, 1 being completely random)

        //Variables related to emitter
        bool emitting = true;        //Is the emitter generating new particles?
        Particle** particlePool;           //Array of particles
        float particlesSecond;        //Particles generated per second 
        float cummulatedFrame = 0;    //When a next particle should be generated

        //Particle class
        class Particle {
            public:

                Particle(AParticleSystem *);           //Create a particle (given the properties of its emitter)         
                ~Particle();                    //Delete particle

                void update(float dTime);       //Update particle properties
                void draw(GLuint programID, AMeshResource* shape);                    //Draw the particle

                float getLife() { return currentLife; } //Returns current life of the particle

            private:

                float currentLife;        //Current life time of the particle
                float particleLife;       //Maximum lifespan of the particle

                glm::vec3 position;           //Current position of the particle
                
                glm::vec3 birthDirection;     //Initial direction of the particle
                glm::vec3 deathDirection;     //Final direction of the particle

                float currentSize;        //Current size of the particle
                float birthSize;          //Initial size of the particle
                float deathSize;          //Final size of the particle

                glm::vec4 currentColor;       //Current color of the particle
                glm::vec4 birthColor;         //Initial color of the particle
                glm::vec4 deathColor;         //Final color of the particle

                //VBO
                GLuint* VBO;

        };

};

#endif // APARTICLESYSTEM_H