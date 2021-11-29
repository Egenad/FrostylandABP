#ifndef AANIMATION_H
#define AANIMATION_H

#include "AEntity.h"
#include "AAnimationResource.h"
#include <string>

class AAnimation: public AEntity
{
    public:
        AAnimation(int);
        virtual ~AAnimation();
        //AAnimation(const AAnimation&);
        void loadAnimation(std::string);
        void beginDraw();
        void endDraw();
        void play();
        void stop();
        void reset();
        void update();
        void setTexture();
        //----------SETTER----------
        void setProgramId(GLuint);
        void setTextureResource(ATextureResource*);
        void setMaterialResource(AMaterialResource*);
        void setAnimationResource(AAnimationResource*);
        void setFrames(int);                                //total frames of the animation
        void setFrame(int);                                 //actual frame
        void setLoop(bool);
        void setAnimationSilhouette(bool);
        void setAnimationActive(bool);

    private:
    	AAnimationResource* anim;
    	GLuint programID;
        bool loop, playing;
        unsigned int pointer;
        float animation_clock;
        unsigned int frames;
        bool silhouette;
        bool active;
};

#endif // AANIMATION_H