#include "StrategyA.h"
#include "Dungeon.h"
#include "Player.h"
#include "AudioManager.h"
#include <math.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

StrategyA::StrategyA(){
	std::srand(std::time(nullptr)); // use current time as seed for random generator
	timer = 9.f;
	option = 1;
	collision = -1;
	confused_timer = 0.f;
	confused = false;
	change = true;
	number = 0;
	a_time = 9.f;
	init_pos = -1;
}

StrategyA::~StrategyA(){}

void StrategyA::execute(int id){
	auto* m = m3D::Motor3D::Instance();
	auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);

    float ps_x = npc->getX();
    float ps_z = npc->getZ();

	if(npc && !confused){
		if(option == 0){
			timer = 0;
			int random_variable = (std::rand()%2);

			if(random_variable == 0){
				option = 1;
				collision = 0;
			}else{
				option = 2;
			}
		}
		if(option != 0){
			if(option == 1){ //move so fast and collide with walls
				if(number > 8){
					npc->setAnimState(2);
					confused = true;
					npc->playAudio(5);
					npc->setVelocityX(0);
					npc->setVelocityZ(0);
					npc->setDeactivated(1);
					option = 3;
					number = 0;
					change = true;
				}else{
					npc->setAnimState(1);
					if(change){
						float aux = (std::rand()%4);
						if(aux != collision || collision == -1){
							collision = aux;
						}else{
							if(collision < 3){
								collision++;
							}else{
								collision = 0;
							}
						}
						change = false;
						npc->setDeactivated(1);
						number++;
					}
					if(collision == 0){
						npc->setVelocityX(-100);
						npc->setVelocityZ(-100);
						if(ps_x < -90){
							npc->playAudio(6);
							change = true;
						}else if(ps_z < -200){
							npc->playAudio(6);
							change = true;
						}
					}else if(collision == 1){
						npc->setVelocityX(100);
						npc->setVelocityZ(-100);
						if(ps_z < -200){
							npc->playAudio(6);
							change = true;
						}else if(ps_x > 110){
							npc->playAudio(6);
							change = true;
						}
					}else if(collision == 2){
						npc->setVelocityX(100);
						npc->setVelocityZ(100);
						if(ps_x > 110){
							npc->playAudio(6);
							change = true;
						}
						else if(npc->getZ() > 200){
							npc->playAudio(6);
							change = true;
						}
					}else{
						if(ps_x < -110){
							npc->playAudio(6);
							change = true;
						}else{
							npc->setVelocityX(-100);
							npc->setVelocityZ(60);
							if(ps_z > 200){
								npc->playAudio(6);
								change = true;
							}
						}
					}
				}

				//Try to do damage to the player

				if(!confused && a_time > 1){
					makeDamage(50, id);
				}else{
					a_time += m->getDeltaTime();
				}

				timer += m->getDeltaTime();

			}else if(option == 2){
				option = 1;
			}else if(option == 3){

				float aux = npc->getY();

				if(init_pos == -1){
					init_pos = aux;
				}

				float delta = m->getDeltaTime();

				confused_timer += delta;
				
				if(confused_timer < 0.5){
					npc->setAnimState(0);
					float aux2 = aux + 90 * delta;
					npc->setY(aux2);
				}else if(confused_timer < 1.0){
					npc->setAnimState(3);
					float aux2 = aux + 50 * delta;
					npc->setY(aux2);
				}else{
					if(aux > init_pos){
						float aux2 = aux - 300 * delta;
						npc->setY(aux2);
					}else{
						npc->playAudio(1);
						option = 0;
						confused_timer = 0;
						npc->setY(init_pos);
						makeDamage(120, id);
					}
				}
			}
		}
	}else{
		confused_timer += m->getDeltaTime();
		npc->playAudio(5);
		if(confused_timer >= 3){
			if(AudioManager::Instance()->isPlaying("HardFX/KingPongo/Mareo")){
				AudioManager::Instance()->stop("HardFX/KingPongo/Mareo");
			}
			confused_timer = 0.f;
			confused = false;
		}
	}
}

void StrategyA::makeDamage(int r, int id){

	auto* d = Dungeon::Instance();
    Enemy* npc = d->getEnemyPerID(id);

    float ps_x = npc->getX();
    float ps_z = npc->getZ();

	auto* pl = Player::Instance();

	float pl_x = pl->getX();
	float pl_z = pl->getZ();

	int x = ps_x - pl_x;
    x = pow(x,2);
    int z = ps_z - pl_z;
    z = pow (z,2);
    int xz = x+z;

    int dist = sqrt(xz);

    if(dist <= r){
    	if(r == 50){
        	npc->playAudio(4);
        	pl->receiveDamage(1);
        }else{
        	npc->playAudio(4);
        	pl->receiveDamage(2);
        }
        a_time = 0.f;

        pl->setPushed(1);

        if(pl_x <= ps_x){
        	if(pl_z > ps_z){
        		pl->setPushedVelocity(0,1);
        	}else{
        		pl->setPushedVelocity(1,1);
        	}
        }else{
        	if(pl_z > ps_z){
        		pl->setPushedVelocity(0,0);
        	}else{
        		pl->setPushedVelocity(1,0);
        	}
        }
    }
}