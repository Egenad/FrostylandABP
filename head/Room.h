#ifndef Room_H
#define Room_H

#include <vector>
#include "BaseGameEntity.h"
#include "Vector3f.h"
#include "Interactible.h"
#include "Trigger.h"
#include "Platform.h"
#include "Puzzle.h"
#include "Decoration.h"
#include "Node.h"

class Room : public BaseGameEntity
{
    public:
        Room(float, float, float, float);
        virtual ~Room();
        void updateEntity();
        
        //Setters
        void addAlert(int);
        void quitAlert(int);
        void addTrigger(Trigger*);
        void addinteractible(Interactible*);
        void addPlatform(Platform*);
        void addPuzzle(Puzzle*);
        void addDecoration(Decoration*);
        void setTorchsActive(bool);

        //Getters
        int   getAlert();
        float getHeight();
        float getWidth();
        float getCenterX();
        float getCenterZ();
        float getMaxX();
        Vector3f getClosestDecoration(float,float);
        float getMinX();
        float getMaxZ();
        float getMinZ();
        std::vector<Interactible*>& getInteractibles();
        std::vector<Trigger*>& getTriggers();
        std::vector<Puzzle*>& getPuzzles();
        Interactible* getInteractiblePerID(int);
        Decoration* getTable();
        Decoration* getStatue1();
        Decoration* getStatue2();
 
        //Main Methods
        void eraseInteractible(int);
        void drawInteractuables();
        bool interactibleHere();   
        bool interactueWithObjectInRange(float, float);
        bool interactibleInRange(float, float);
        bool checkCollisionWithBox(Interactible*);
        int IDwichCollides(Interactible*);
        void eraseTrigger();

    private:
    	float height;
    	float width;
    	float center_x;
    	float center_z;
    	int alert;
        std::vector<Interactible*> interactibles;
        std::vector<Platform*> platforms; 
        std::vector<Trigger*> triggers; //a room can have (or not) triggers
        std::vector<Puzzle*> puzzles;
        std::vector<Decoration*> decorations;
};

#endif // Room_H