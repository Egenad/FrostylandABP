#include "AParticleSystem.h"
#include "AMeshResource.h"
#include <stdlib.h>
#include <time.h> 
#include <climits>

//Simple emitter contructor, half of the parameters are deduced
AParticleSystem::AParticleSystem(AMeshResource* nShape, glm::vec3 nPosition, float nRadius, int nBirthrate, float nParticleLife,glm::vec3 nBirthDirection, float nBirthSize, glm::vec4 nBirthColor) {

    //Variables relared to particles
    shape = nShape;              

    position = nPosition;           
    radius = nRadius;             

    birthrate = nBirthrate;          
    particleLife = nParticleLife;       

    birthDirection = nBirthDirection;     
    deathDirection = nBirthDirection;     
    variationDirection = 0; 

    birthSize = nBirthSize;          
    deathSize = nBirthSize;          
    variationSize = 0;      

    birthColor = nBirthColor;         
    deathColor = nBirthColor;         
    variationColor = 0;     

    //Variables related to emitter  
    particlePool = new Particle*[nBirthrate];
    for(int i = 0; i < nBirthrate; i++){
        particlePool[i] = nullptr;
    }      

    srand(time(nullptr)); 

    particlesSecond = birthrate/particleLife;

}                                 

//Delete particle emitter and its particles
AParticleSystem::~AParticleSystem() {

    for(int i = 0; i < birthrate; i++){
        if(particlePool[i] != nullptr)
            delete particlePool[i];
    }      
    delete [] particlePool;

}

//Update particle properties
void AParticleSystem::update(float dTime){
    float particlesFrame  = particlesSecond * dTime; //Particles generated per frame

    cummulatedFrame += particlesFrame;

    int pendingFrames = (int)cummulatedFrame;

    cummulatedFrame -= pendingFrames;

    for(int i = 0; i < birthrate; i++) {
        //If the particle is dead, delete it
        if(particlePool[i] != nullptr && particlePool[i]->getLife() > particleLife) {
            delete particlePool[i];
            particlePool[i] = nullptr;
        }
        //If the particle is not dead, update it
        else if(particlePool[i] != nullptr && particlePool[i]->getLife() < particleLife) {
            particlePool[i]->update(dTime);
        }
        //If there's no particle and there's particles yet to be emitted, create new particle
        else if(particlePool[i] == nullptr && pendingFrames > 0 && emitting) {

            particlePool[i] = new Particle(this);
            pendingFrames--;

        }
    }
 
}


//Draw current particles               
void AParticleSystem::draw(GLuint programID) {
    update(0.01f);
    for(int i = 0; i <birthrate; i++){
        if(particlePool[i] != nullptr) {
            particlePool[i]->draw(programID, shape);
        }
    }
}            

//Set particle life data
void AParticleSystem::setParticleLifeData(float & nCurrentLife, float & nParticleLife) {

    nCurrentLife = 0;
    nParticleLife = particleLife;

}
//Randomize particle initial position
void AParticleSystem::setParticlePositionData(glm::vec3 & nPosition) {

    float randX = ((int)rand()) / (float)RAND_MAX;
    float randY = ((int)rand()) / (float)RAND_MAX;
    float randZ = ((int)rand()) / (float)RAND_MAX;

    int signX = (rand() % 2 == 0) ? 1 : -1;
    int signY = (rand() % 2 == 0) ? 1 : -1;
    int signZ = (rand() % 2 == 0) ? 1 : -1;

    nPosition = glm::vec3( position.x + radius*randX*signX, position.y + radius*randY*signY, position.z + radius*randZ*signZ);

}

//Randomize particle direction
void AParticleSystem::setParticleDirectionData(glm::vec3 & nBirthDirection, glm::vec3 & nDeathDirection) {

    float randX = ((int)rand()) / (float)RAND_MAX;
    float randY = ((int)rand()) / (float)RAND_MAX;
    float randZ = ((int)rand()) / (float)RAND_MAX;

    int signX = (rand() % 2 == 0) ? 1 : -1;
    int signY = (rand() % 2 == 0) ? 1 : -1;
    int signZ = (rand() % 2 == 0) ? 1 : -1;

    float lenght = glm::length(birthDirection);

    nBirthDirection   = glm::vec3( birthDirection.x + lenght*variationDirection*randX*signX, birthDirection.y + lenght*variationDirection*randY*signY, birthDirection.z + lenght*variationDirection*randZ*signZ);

    randX = ((int)rand()) / (float)RAND_MAX;
    randY = ((int)rand()) / (float)RAND_MAX;
    randZ = ((int)rand()) / (float)RAND_MAX;

    signX = (rand() % 2 == 0) ? 1 : -1;
    signY = (rand() % 2 == 0) ? 1 : -1;
    signZ = (rand() % 2 == 0) ? 1 : -1;

    nDeathDirection   = glm::vec3( deathDirection.x + lenght*variationDirection*randX*signX, deathDirection.y + lenght*variationDirection*randY*signY, deathDirection.z + lenght*variationDirection*randZ*signZ);

}

//Randomize particle size
void AParticleSystem::setParticleSizeData(float & nCurrentSize, float & nBirthSize, float & nDeathSize) {

    int sign = (rand() % 2 == 0) ? 1 : -1;
    float randX = ((int)rand()) / (float)RAND_MAX;

    nBirthSize   = birthSize + birthSize*sign*variationSize*randX;
    nCurrentSize = nBirthSize;

    sign = (rand() % 2 == 0) ? 1 : -1;
    randX = ((int)rand()) / (float)RAND_MAX;
    nDeathSize   = deathSize + deathSize*sign*variationSize*randX;

}

//Randomize color
void AParticleSystem::setParticleColorData(glm::vec4 & nCurrentColor, glm::vec4 & nBirthColor, glm::vec4 & nDeathColor) {

    float randX = ((int)rand()) / (float)RAND_MAX;
    float randY = ((int)rand()) / (float)RAND_MAX;
    float randZ = ((int)rand()) / (float)RAND_MAX;
    float randA = ((int)rand()) / (float)RAND_MAX;

    int signX = (rand() % 2 == 0) ? 1 : -1;
    int signY = (rand() % 2 == 0) ? 1 : -1;
    int signZ = (rand() % 2 == 0) ? 1 : -1;
    int signA = (rand() % 2 == 0) ? 1 : -1;

    float r = birthColor.x + signX*variationColor*randX;
    float g = birthColor.y + signY*variationColor*randY;
    float b = birthColor.z + signZ*variationColor*randZ;
    float a = birthColor.w + birthColor.w*signA*variationColor*randA;

    if (r > 1)
        r = 1;
    else if (r < 0)
        r = 0;

    if (g > 1)
        g = 1;
    else if (g < 0)
        g = 0;

    if (b > 1)
        b = 1;
    else if (b < 0)
        b = 0;

    if (a > 1)
        a = 1;
    else if (a < 0)
        a = 0;

    nCurrentColor = glm::vec4(r,g,b,a);
    nBirthColor = glm::vec4(r,g,b,a);

    randX = ((int)rand()) / (float)RAND_MAX;
    randY = ((int)rand()) / (float)RAND_MAX;
    randZ = ((int)rand()) / (float)RAND_MAX;
    randA = ((int)rand()) / (float)RAND_MAX;

    signX = (rand() % 2 == 0) ? 1 : -1;
    signY = (rand() % 2 == 0) ? 1 : -1;
    signZ = (rand() % 2 == 0) ? 1 : -1;
    signA = (rand() % 2 == 0) ? 1 : -1;

    r = deathColor.x + signX*variationColor*randX;
    g = deathColor.y + signY*variationColor*randY;
    b = deathColor.z + signZ*variationColor*randZ;
    a = deathColor.w + deathColor.w*signA*variationColor*randA;

    if (r > 1)
        r = 1;
    else if (r < 0)
        r = 0;

    if (g > 1)
        g = 1;
    else if (g < 0)
        g = 0;

    if (b > 1)
        b = 1;
    else if (b < 0)
        b = 0;

    if (a > 1)
        a = 1;
    else if (a < 0)
        a = 0;

    nDeathColor = glm::vec4(r,g,b,a);

}


//Create a particle (given the properties of its emitter)   
AParticleSystem::Particle::Particle(AParticleSystem * emitter) {
    emitter->setParticleLifeData(currentLife, particleLife);
    emitter->setParticlePositionData(position);
    emitter->setParticleDirectionData(birthDirection, deathDirection);
    emitter->setParticleSizeData(currentSize, birthSize, deathSize);
    emitter->setParticleColorData(currentColor, birthColor, deathColor);

    VBO = (unsigned int *)malloc(sizeof(unsigned int) * 2);
    glGenBuffers(2, VBO);

}      

//Delete particle    
AParticleSystem::Particle::~Particle() {

}               

//Update particle properties
void AParticleSystem::Particle::update(float dTime) {
    currentLife += dTime;

    float lifeState = currentLife / particleLife;

    position += birthDirection*(1-lifeState) + deathDirection*lifeState;
    currentSize = birthSize*(1-lifeState) + deathSize*lifeState;
    currentColor = birthColor*(1-lifeState) + deathColor*lifeState;

}

//Draw the particle          
void AParticleSystem::Particle::draw(GLuint programID, AMeshResource* shape) {
    //Create model matrix for the given particle
    glm::mat4 model(1.0);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(currentSize));

    model = AEntity::projectionMatrix() * AEntity::viewMatrix() * model;

    //Give the model matrix to the shader
    GLuint modelID = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

    //Give the model matrix to the shader
    GLuint colorID = glGetUniformLocation(programID, "v_Color");
    glUniform4fv(colorID, 1, &currentColor[0]);

    //Draw shape
    shape->draw();

}