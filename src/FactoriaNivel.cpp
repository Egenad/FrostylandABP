#include "FactoriaNivel.h"
#include <iostream>
#include <sys/stat.h>
#include "Seal_Normal.h"
#include "Seal_Quest.h"
#include "P_Soldier.h"
#include "P_General.h"
#include "P_KPongo.h"
#include "DragBox.h"
#include "Trigger_Enemies.h"
#include "Trigger_NEnemies.h"
#include "Trigger_Level.h"
#include "Trigger_Camera.h"
#include "Trigger_Fall.h"
#include "Trigger_Dialogo.h"
#include "Trigger_KPongo.h"
#include "Trigger_Door.h"
#include "Platform.h"
#include "Chest.h"
#include "Plate.h"
#include "Interactible.h"
#include "Player.h"
#include "Globals.h"
#include "Dungeon.h"
#include "Fox.h"
#include "Puzzle.h"
 
template <typename T> std::string tostr(const T& t) { 
   std::ostringstream os; 
   os<<t; 
   return os.str(); 
} 

FactoriaNivel* FactoriaNivel::Instance(){
    static FactoriaNivel pinstance;
    return &pinstance;
}

FactoriaNivel::FactoriaNivel() {
    doc = new TiXmlDocument("../assets/config/WorldData.xml");
    bool ok = doc->LoadFile();
    if(!ok){
        exit(0);
    }
}

void FactoriaNivel::clearInstance(){
	if(doc){
		delete doc;
		doc = nullptr;
	}
}

FactoriaNivel::~FactoriaNivel() {

}

void FactoriaNivel::destroySaveData(){
	struct stat buffer;   
  	if (stat("../assets/config/PlayerData.xml", &buffer) == 0){
  		remove("../assets/config/PlayerData.xml");
  	}
  	std::string slvl;
  	std::string stdlvl;
  	for(int i=1; i<4; i++){
  		stdlvl = std::to_string(i);
		slvl = "../assets/config/Level" + stdlvl + ".xml";
  		if (stat(slvl.c_str(), &buffer) == 0){
	  		remove(slvl.c_str());
	  	}
  	}
}

void FactoriaNivel::loadPlayer(){
	//Load Player data

	TiXmlDocument* loader = new TiXmlDocument("../assets/config/PlayerData.xml");

	bool ok = loader->LoadFile();
    if(!ok){
        return;
    }
	
	TiXmlElement* data = loader->RootElement();
 
	auto* pl = Player::Instance();

   	const char* lifes = data->FirstChildElement("Lifes")->GetText();
    int ilifes = 0;

   	std::stringstream ssl;
    ssl << lifes;
    ssl >> ilifes;

    pl->setLifes(ilifes);

    const char* money = data->FirstChildElement("Money")->GetText();
    int imoney = 0;

    std::stringstream ssm;
    ssm << money;
    ssm >> imoney;

    pl->setMoney(imoney);

    const char* keys = data->FirstChildElement("Keys")->GetText();
    int ikeys = 0;

    std::stringstream ssk;
    ssk << keys;
    ssk >> ikeys;

    pl->setKeys(ikeys);

    const char* bosskey = data->FirstChildElement("BossKey")->GetText();

    if(strcmp(bosskey, "1") == 0){
    	pl->setBossKey(true);
    }else{
    	pl->setBossKey(false);
    }

    TiXmlElement* weapons = data->FirstChildElement("Weapons");
  
    const char* sword = weapons->FirstChildElement("Sword")->GetText();

    if(strcmp(sword, "1") == 0){
    	pl->addWeapon(0);
    }

    const char* hammer = weapons->FirstChildElement("Hammer")->GetText();

    if(strcmp(hammer, "1") == 0){
    	pl->addWeapon(2);
    }

    const char* slingshot = weapons->FirstChildElement("Slingshot")->GetText();

    if(strcmp(slingshot, "1") == 0){
    	pl->addWeapon(1);
    }

    const char* food = data->FirstChildElement("Food")->GetText();

	std::stringstream ss(food);
    std::string item;
    std::vector<std::string> splittedStrings;
    char delimeter = ',';

    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }

	for(long unsigned int i=0; i<splittedStrings.size(); i++){
		int ifood = 0;

	    std::stringstream ssm;
	    ssm << splittedStrings[i];
	    ssm >> ifood;

	    pl->addFood(ifood);
	}
}

void FactoriaNivel::loadGame(int level, std::vector<Room*>& rooms, std::vector<Door*>& doors, std::vector<Enemy*>& enemies, std::vector<Villager*>& seals){
	
	std::string stdlvl = std::to_string(level);
	std::string path = "../assets/config/Level" + stdlvl + ".xml";
	TiXmlDocument* loader = new TiXmlDocument(path.c_str());
	
	bool ok = loader->LoadFile();
    if(!ok){
        return;
    }
	
	TiXmlElement* data = loader->RootElement();

    //Load environment data

    //Load doors data

    TiXmlElement* door = data->FirstChildElement("Door");

    while(door != nullptr){

    	const char* dindex1 = door->FirstChildElement("Index1")->GetText();
    	const char* dindex2 = door->FirstChildElement("Index2")->GetText();
    	const char* dclosed = door->FirstChildElement("Closed")->GetText();

    	int diindex1, diindex2;

    	std::stringstream ssd1;
	    ssd1 << dindex1;
	    ssd1 >> diindex1;

	    std::stringstream ssd2;
	    ssd2 << dindex2;
	    ssd2 >> diindex2;

    	bool done = false;

    	for(long unsigned int i=0; i<doors.size() && !done; i++){
    		if(doors[i]->getINDEX1() == diindex1 && doors[i]->getINDEX2() == diindex2){
    			done = true;
    			if(strcmp(dclosed, "1") == 0){
    				doors[i]->setClosed(true);
    			}else{
    				doors[i]->setClosed(false);
    			}
    		}
    	}

    	door = door->NextSiblingElement("Door");  
    }

    //Load Room data

    TiXmlElement* room = data->FirstChildElement("Room");

    while(room != nullptr){

    	const char* rcenterx = room->FirstChildElement("CenterX")->GetText();
    	const char* rcenterz = room->FirstChildElement("CenterZ")->GetText();

    	float icenterx, icenterz;

    	std::stringstream sscx;
	    sscx << rcenterx;
	    sscx >> icenterx;

	    std::stringstream sscz;
	    sscz << rcenterz;
	    sscz >> icenterz;

    	bool done = false;

    	for(long unsigned int i=0; i<rooms.size() && !done; i++){
    		if(rooms[i]->getCenterX() == icenterx && rooms[i]->getCenterZ() == icenterz){

    			//Load interactibles

    			TiXmlElement* interactible = room->FirstChildElement("Interactible");
    			std::vector<Interactible*> interactibles = rooms[i]->getInteractibles();

    			while(interactible != nullptr){

    				const char* intype = interactible->FirstChildElement("Type")->GetText();
    				const char* intx = interactible->FirstChildElement("X")->GetText();
    				const char* intz = interactible->FirstChildElement("Z")->GetText();

    				float iintx, iintz;

    				std::stringstream ssinx;
				    ssinx << intx;
				    ssinx >> iintx;

				    std::stringstream ssinz;
				    ssinz << intz;
				    ssinz >> iintz;

				    if(strcmp(intype, "Chest") == 0){

				    	const char* copen = interactible->FirstChildElement("Open")->GetText();

				    	int open;

				    	std::stringstream ssop;
					    ssop << copen;
					    ssop >> open;

					    bool encontrado = false;

	    				for(long unsigned int j=0; j<interactibles.size(); j++){
	    					if(interactibles[j]->getX() == iintx && interactibles[j]->getZ() == iintz){
	    						if(interactibles[j]->getType() == CHEST_INTERACTIBLE){
	    							Chest* chest = dynamic_cast<Chest*> (interactibles[j]);
									if(chest != nullptr){
										if(open == 1){
											chest->setOpen(true);
										}else{
											chest->setOpen(false);
										}
										encontrado = true;
									}
	    						}
	    					}
	    				}

	    				if(!encontrado){
	    					Chest* chsst = new Chest(iintx,3,iintz,CHEST_NORMAL_KEY_ENTITY, 1);
	    					chsst->setOpen(true);
	    					chsst->setColor('k');
	    					rooms[i]->addinteractible(chsst);
	    				}
	    			}

    				interactible = interactible->NextSiblingElement("Interactible");
    			}

    			//Load triggers

    			TiXmlElement* trigger = room->FirstChildElement("Trigger");
    			std::vector<Trigger*> triggers = rooms[i]->getTriggers();

    			int k = 0;

    			if(trigger != nullptr){
    				k = 1;
    				while(trigger != nullptr){
	    				trigger = trigger->NextSiblingElement("Trigger");
	    				k++;
	    			}
    			}

    			int tris=0;

    			for(long unsigned int i=0; i<triggers.size(); i++){
    				EntityType ttri = triggers[i]->getType();
    				if(ttri != TRIGGER_CAMERA && ttri != TRIGGER_FALL && ttri != TRIGGER_LEVEL){
    					tris++;
    				}
    			}

    			while(k < tris){
    				rooms[i]->eraseTrigger();
    				tris--;
    			}

    			//Load puzzles

				TiXmlElement* puzzle = room->FirstChildElement("Puzzle");
    			std::vector<Puzzle*> puzzles = rooms[i]->getPuzzles();

    			while(puzzle != nullptr){

    				TiXmlElement* object2 = puzzle->FirstChildElement("Object");

    				bool llel = false;

    				while(object2 != nullptr){

	    				const char* intx = object2->FirstChildElement("XC")->GetText();
	    				const char* intz = object2->FirstChildElement("ZC")->GetText();

	    				float iintx, iintz;

	    				std::stringstream ssinx;
					    ssinx << intx;
					    ssinx >> iintx;

					    std::stringstream ssinz;
					    ssinz << intz;
					    ssinz >> iintz;

	    				for(long unsigned int c=0; c<puzzles.size() && !llel; c++){
	    					std::vector<EntityFlag> conditions = puzzles[c]->getConditions();
	    					for(long unsigned int x=0; x<conditions.size() && !llel; x++){
	    						if(conditions[x].type == DRAGGABLE_INTERACTIBLE){
	    							for(long unsigned int e=0; e<interactibles.size(); e++){
	    								if(interactibles[e]->getType() == DRAGGABLE_INTERACTIBLE){
	    									DragBox* dragbox = dynamic_cast<DragBox*> (interactibles[e]);
	    									dragbox->reposition(iintx, iintz);
	    									llel = true;
	    								}
	    							}
	    						}
	    					}
	    				}
	    				object2 = object2->NextSiblingElement("Object");
	    			}
	    			puzzle = puzzle->NextSiblingElement("Puzzle");	
    			}

    			done = true;

    		}
    	}

    	room = room->NextSiblingElement("Room");  
    }
}

bool FactoriaNivel::saveDataAvailable(){
	TiXmlDocument* loader = new TiXmlDocument("../assets/config/PlayerData.xml");
    bool ok = loader->LoadFile();
    if(!ok){
        return false;
    }

    return true;
}

void FactoriaNivel::saveGame(int lvl){

	TiXmlDocument save, psave;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlDeclaration *decl2 = new TiXmlDeclaration("1.0", "", "");

	//Save information of the player

	auto* pl = Player::Instance();

	TiXmlElement * playertag = new TiXmlElement("Player");
	TiXmlElement * lifetag = new TiXmlElement("Lifes");
	TiXmlElement * keytag = new TiXmlElement("Keys");
	TiXmlElement * moneytag = new TiXmlElement("Money");
	TiXmlElement * bosskeytag = new TiXmlElement("BossKey");
	TiXmlElement * weaponstag = new TiXmlElement("Weapons");
	TiXmlElement * swordtag = new TiXmlElement("Sword");
	TiXmlElement * slingshottag = new TiXmlElement("Slingshot");
	TiXmlElement * hammertag = new TiXmlElement("Hammer");
	TiXmlElement * foodtag = new TiXmlElement("Food");

	std::string lifes = tostr(pl->getLifes());
	std::string keys = tostr(pl->getKeys());
	std::string money = tostr(pl->getMoney());

	TiXmlText * textbosskey;

	if(!pl->getBossKey()){
		textbosskey = new TiXmlText("0");
	}else{
		textbosskey = new TiXmlText("1");
	}

	bool* weapons = pl->getWeapons();
	
	TiXmlText * textsword;
	TiXmlText * textslingshot;
	TiXmlText * texthammer;

	if(weapons[0]){
		textsword = new TiXmlText("1");
	}else{
		textsword = new TiXmlText("0");
	}

	swordtag->LinkEndChild(textsword);

	if(weapons[1]){
		textslingshot = new TiXmlText("1");
	}else{
		textslingshot = new TiXmlText("0");
	}

	slingshottag->LinkEndChild(textslingshot);

	if(weapons[2]){
		texthammer = new TiXmlText("1");
	}else{
		texthammer = new TiXmlText("0");
	}

	hammertag->LinkEndChild(texthammer);

	int* food = pl->getFood();
	std::string foodstring = "";

	for(int i=0; i<GLOBAL_FOOD_SIZE; i++){
		std::string foodi = tostr(food[i]);
		foodstring += foodi + ",";
	}

	TiXmlText * foodtext = new TiXmlText(foodstring);
	foodtag->LinkEndChild(foodtext);

	TiXmlText * textlifes = new TiXmlText(lifes);
	TiXmlText * textkeys = new TiXmlText(keys);
	TiXmlText * textmoney = new TiXmlText(money);

	lifetag->LinkEndChild(textlifes);
	moneytag->LinkEndChild(textmoney);
	keytag->LinkEndChild(textkeys);
	bosskeytag->LinkEndChild(textbosskey);
	weaponstag->LinkEndChild(swordtag);
	weaponstag->LinkEndChild(slingshottag);
	weaponstag->LinkEndChild(hammertag);

	playertag->LinkEndChild(lifetag);
	playertag->LinkEndChild(moneytag);
	playertag->LinkEndChild(keytag);
	playertag->LinkEndChild(bosskeytag);
	playertag->LinkEndChild(weaponstag);
	playertag->LinkEndChild(foodtag);

	pl = nullptr;
	psave.LinkEndChild(decl);
	save.LinkEndChild(decl2);
	psave.LinkEndChild(playertag);

	//Save information of the level

	TiXmlElement * savedata = new TiXmlElement("SaveData");

	//World info --> doors and interactibles states

	auto* dl = Dungeon::Instance();

	std::vector<Door*> doors = dl->getDoors();

	for(long unsigned int i=0; i<doors.size(); i++){

		//Save doors data

		TiXmlElement * doortag = new TiXmlElement("Door");
		TiXmlElement * closed = new TiXmlElement("Closed");
		TiXmlElement * index1 = new TiXmlElement("Index1");
		TiXmlElement * index2 = new TiXmlElement("Index2");

		std::string sindex1 = tostr(doors[i]->getINDEX1());
		std::string sindex2 = tostr(doors[i]->getINDEX2());

		std::string sclosed = "";

		if(doors[i]->getClosed()){
			sclosed += "1";
		}else{
			sclosed += "0";
		}

		TiXmlText * sclosed_text = new TiXmlText(sclosed);
		TiXmlText * sindex1_text = new TiXmlText(sindex1);
		TiXmlText * sindex2_text = new TiXmlText(sindex2);

		closed->LinkEndChild(sclosed_text);
		index1->LinkEndChild(sindex1_text);
		index2->LinkEndChild(sindex2_text);

		doortag->LinkEndChild(closed);
		doortag->LinkEndChild(index1);
		doortag->LinkEndChild(index2);

		savedata->LinkEndChild(doortag);
	}

	std::vector<Room*> rooms = dl->getRooms();

	for(long unsigned int i=0; i<rooms.size(); i++){

		if(rooms[i] != nullptr){

			//Save the important data to know what room is it

			TiXmlElement * roomtag = new TiXmlElement("Room");
			TiXmlElement * cxtag = new TiXmlElement("CenterX");
			TiXmlElement * cztag = new TiXmlElement("CenterZ");

			std::string cx = tostr(rooms[i]->getCenterX());
			std::string cz = tostr(rooms[i]->getCenterZ());

			TiXmlText * cx_text = new TiXmlText(cx);
			TiXmlText * cz_text = new TiXmlText(cz);

			cxtag->LinkEndChild(cx_text);
			cztag->LinkEndChild(cz_text);

			roomtag->LinkEndChild(cxtag);
			roomtag->LinkEndChild(cztag);

			//Save interactibles data

			std::vector<Interactible*> interactibles = rooms[i]->getInteractibles();

			for(long unsigned int j=0; j<interactibles.size(); j++){
				if(interactibles[j] != nullptr){
					TiXmlElement * interactibletag = new TiXmlElement("Interactible");
					TiXmlElement * typetag = new TiXmlElement("Type");
					//Save interactible data

					EntityType intype = interactibles[j]->getType();

					if(intype == CHEST_INTERACTIBLE){
						Chest* chest = dynamic_cast<Chest*> (interactibles[j]);
						if(chest != nullptr){
							if(chest->getInteractibleType() == CHEST_NORMAL_KEY_ENTITY || chest->getInteractibleType() == CHEST_LEVEL_KEY_ENTITY || chest->getInteractibleType() == CHEST_HAMMER_ENTITY
								|| chest->getInteractibleType() == CHEST_SWORD_ENTITY  || chest->getInteractibleType() == CHEST_JAIL_KEY_ENTITY || chest->getInteractibleType() == CHEST_SLINGSHOT_ENTITY
								|| chest->getInteractibleType() == CHEST_BOSS_KEY_ENTITY){

								TiXmlElement * opentag = new TiXmlElement("Open");
								TiXmlText * chest_text = new TiXmlText("Chest");
								TiXmlElement * chxtag = new TiXmlElement("X");
								TiXmlElement * chztag = new TiXmlElement("Z");
								
								std::string chx = tostr(chest->getX());
								std::string chz = tostr(chest->getZ());

								TiXmlText * chx_text = new TiXmlText(chx);
								TiXmlText * chz_text = new TiXmlText(chz);

								chxtag->LinkEndChild(chx_text);
								chztag->LinkEndChild(chz_text);

								std::string sopen = "";

								if(chest->getOpen()){
									sopen += "1";
								}else{
									sopen += "0";
								}

								TiXmlText * open_text = new TiXmlText(sopen);

								opentag->LinkEndChild(open_text);
								typetag->LinkEndChild(chest_text);

								interactibletag->LinkEndChild(typetag);
								interactibletag->LinkEndChild(chxtag);
								interactibletag->LinkEndChild(chztag);
								interactibletag->LinkEndChild(opentag);

								roomtag->LinkEndChild(interactibletag);
							}
						}
					}
				}
			}

			//Save triggers data

			std::vector<Trigger*> triggers = rooms[i]->getTriggers();

			for(long unsigned int j=0; j<triggers.size(); j++){
				if(triggers[j] != nullptr){
					EntityType tritype = triggers[j]->getType();
					if(tritype != TRIGGER_FALL && tritype != TRIGGER_CAMERA && tritype != TRIGGER_LEVEL){
						TiXmlElement * triggertag = new TiXmlElement("Trigger");
						TiXmlElement * typetag = new TiXmlElement("Type");

						std::string stype = "";

						if(tritype == TRIGGER_ENEMIES){
							stype += "TRIGGER_ENEMIES";
						}else if(tritype == TRIGGER_NENEMIES){
							stype += "TRIGGER_NENEMIES";
						}else if(tritype == TRIGGER_DIALOGO){
							stype += "TRIGGER_DIALOGO";
						}else if(tritype == TRIGGER_DOOR){
							stype += "TRIGGER_DOOR";
						}else if(tritype == TRIGGER_KPONGO){
							stype += "TRIGGER_KPONGO";
						}

						TiXmlText * ttype_text = new TiXmlText(stype);
						typetag->LinkEndChild(ttype_text);
						triggertag->LinkEndChild(typetag);
						roomtag->LinkEndChild(triggertag);
					}
				}
			}

			//Save puzzles data

			std::vector<Puzzle*> puzzles = rooms[i]->getPuzzles();

			for(long unsigned int j=0; j<puzzles.size(); j++){
				if(puzzles[j]->getResolved()){
					TiXmlElement * puzzletag = new TiXmlElement("Puzzle");
					std::vector<EntityFlag> conditions = puzzles[j]->getConditions();

					for(long unsigned int s=0; s<conditions.size(); s++){
						if(conditions[s].type == DRAGGABLE_INTERACTIBLE){
							TiXmlElement * objecttag = new TiXmlElement("Object");
							TiXmlElement * cxtag = new TiXmlElement("XC");
							TiXmlElement * cztag = new TiXmlElement("ZC");
							
							std::string cx = tostr(conditions[s].p_x);
							std::string cz = tostr(conditions[s].p_z);

							TiXmlText * cx_text = new TiXmlText(cx);
							TiXmlText * cz_text = new TiXmlText(cz);

							cxtag->LinkEndChild(cx_text);
							cztag->LinkEndChild(cz_text);

							objecttag->LinkEndChild(cxtag);
							objecttag->LinkEndChild(cztag);

							puzzletag->LinkEndChild(objecttag);
						}
					}
					roomtag->LinkEndChild(puzzletag);
				}
			}

			savedata->LinkEndChild(roomtag);
		}
	}

	dl = nullptr;
	save.LinkEndChild(savedata);
	std::string stdlvl = std::to_string(lvl);
	std::string path = "../assets/config/Level" + stdlvl + ".xml";
	save.SaveFile(path.c_str());
	
	std::string path2 = "../assets/config/PlayerData.xml";
	psave.SaveFile(path2.c_str());

	std::cout << "Saved Game!" << std::endl;
}

void FactoriaNivel::readLevel(std::string map, std::vector<Room*>& rooms, std::vector<Door*>& doors, std::vector<Enemy*>& enemies, std::vector<Villager*>& seals){

 	TiXmlElement* mapp = doc->RootElement();
    
   	TiXmlElement* level = mapp->FirstChildElement(map);
    const char* nrooms = level->FirstChildElement("NRooms")->GetText();
    const char* mediaY = level->FirstChildElement("MediaY")->GetText();
    const char* ndb = level->FirstChildElement("NDB")->GetText();
    const char* nplates = level->FirstChildElement("NP")->GetText();

    int inrooms, imediay, inp, indb;

    std::stringstream ssr;
    ssr << nrooms;
    ssr >> inrooms;

    std::stringstream smy;
    smy << mediaY;
    smy >> imediay;

    std::stringstream sndb;
    sndb << ndb;
    sndb >> indb;

    std::stringstream snp;
    snp << nplates;
    snp >> inp;

    Dungeon::Instance()->setLevelMediaY(imediay);

    int array[inrooms];
    int arraydb[indb];
    int arrayp[inp];

    Vector2f vectorarray[inrooms];

    //Set Graph

    std::vector<ConexionP> cVec;

    TiXmlElement* graph = level->FirstChildElement("Graph");

    if(graph != nullptr){
    	TiXmlElement* node = graph->FirstChildElement("Node");

    	int index = 0;

    	while(node != nullptr){

    		const char* _x = node->FirstChildElement("X")->GetText();
    		const char* _z = node->FirstChildElement("Z")->GetText();

    		std::string::size_type s;

			float x = std::stof (_x,&s);
			float z = std::stof (_z,&s);

			vectorarray[index] = Vector2f(x,z);

			index++;

    		node = node->NextSiblingElement("Node");
    	}

    	TiXmlElement* connection = graph->FirstChildElement("Connection");

    	while(connection != nullptr){

    		const char* _c1 = connection->FirstChildElement("C1")->GetText();
    		const char* _c2 = connection->FirstChildElement("C2")->GetText();
    		const char* _p = connection->FirstChildElement("P")->GetText();

    		int c1, c2;

			std::stringstream sc1;
		    sc1 << _c1;
		    sc1 >> c1;

		    std::stringstream sc2;
		    sc2 << _c2;
		    sc2 >> c2;

		    std::string::size_type s;

			float p = std::stof (_p,&s);

		    ConexionP c(vectorarray[c1],vectorarray[c2],p);
		    cVec.push_back(c);
		    ConexionP cc(vectorarray[c2],vectorarray[c1],p);
		    cVec.push_back(cc);

    		connection = connection->NextSiblingElement("Connection");
    	}

    	Dungeon::Instance()->setGrafo(GrafoP(cVec));
    }

    //Load Rooms

	level = level->FirstChildElement("Room");

	int i=0;
	int idb = 0;
	int ip = 0;

	while (level != nullptr)
	{

		//Load from xml

		const char* min_x = level->FirstChildElement("MinX")->GetText();
		const char* min_z = level->FirstChildElement("MinZ")->GetText();
		const char* max_x = level->FirstChildElement("MaxX")->GetText();
		const char* max_z = level->FirstChildElement("MaxZ")->GetText();
		const char* type = level->FirstChildElement("Type")->GetText();

		std::string::size_type sz;

		float minx = std::stof (min_x,&sz);
		float minz = std::stof (min_z,&sz);
		float maxx = std::stof (max_x,&sz);
		float maxz = std::stof (max_z,&sz);
	
		int itype;

		std::stringstream ss;
	    ss << type;
	    ss >> itype;

		//Create room

		Room* room = new Room(minx, minz, maxx, maxz);
	    array[i] = room->getID();
	    switch(itype){
	    	case 0:
	    		room->setType(BOSS_ROOM);
	    	break;
	    	case 1:
	    		room->setType(SOLID_ROOM);
	    	break;
	    	case 2:
	    		room->setType(HOLLOW_ROOM);
	    	break;
	    	case 3:
	    		room->setType(TRAP_ROOM);
	    	break;
	    }

	    rooms.push_back(room);

	    //Load Decoration / Triggers / Platforms / Interactibles

	    //Decoration

	     //Set decoration

	    TiXmlElement* additions = level->FirstChildElement("Decoration");

	    while(additions != nullptr){

	    	const char* d_type = additions->FirstChildElement("Type")->GetText();
	    	const char* d_x = additions->FirstChildElement("X")->GetText();
	    	const char* d_y = additions->FirstChildElement("Y")->GetText();
	    	const char* d_z = additions->FirstChildElement("Z")->GetText();
	    	const char* d_ang = additions->FirstChildElement("Angle")->GetText();

	    	std::string::size_type s1;
			float fx = std::stof (d_x,&s1);
			float fy = std::stof (d_y,&s1);
			float fz = std::stof (d_z,&s1);
			float fang = std::stof (d_ang,&s1);

			if (strcmp(d_type,"Torch") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_TORCH,fang);
				room->addDecoration(d);
			}else if (strcmp(d_type,"Table") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_TABLE,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Barrel") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_BARREL,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Box") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_BOX,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Bed") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_BED,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Chair") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_CHAIR,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Chain") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_CHAIN,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"PS") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_SOLDIER,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Flag") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_FLAG,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Column") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_COLUMN,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Statue1") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_STATUE1,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Statue2") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_STATUE2,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Throne") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_THRONE,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Painting") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_PAINTING,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"SwordDeco") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_SWORDDECO,fang);
				room->addDecoration(d);
			}else if(strcmp(d_type,"Lance") == 0){
				Decoration* d = new Decoration(fx,fy,fz,DECORATION_LANCE,fang);
				room->addDecoration(d);
			}

	    	additions = additions->NextSiblingElement("Decoration");
	    }

	    //Interactibles
	   
	    additions = level->FirstChildElement("Interactible");

	    while(additions != nullptr){

	    	//Load xml

	    	const char* object_type = additions->FirstChildElement("Object")->GetText();
	    	const char* ps_x = additions->FirstChildElement("X")->GetText();
	    	const char* ps_y = additions->FirstChildElement("Y")->GetText();
	    	const char* ps_z = additions->FirstChildElement("Z")->GetText();
	    	const char* type = additions->FirstChildElement("Type")->GetText();
			const char* valor = additions->FirstChildElement("Value")->GetText();

			float fps_x = std::stof (ps_x,&sz);
			float fps_y = std::stof (ps_y,&sz);
			float fps_z = std::stof (ps_z,&sz);

			//Create interactible

			int ival;
			std::stringstream ssmv;
	   		ssmv << valor;
	    	ssmv >> ival;

	    	if (strcmp(object_type,"Chest") == 0){

	    		EntityType ctype=CHEST_SWORD_ENTITY;

	    		if(strcmp(type,"CHEST_SWORD_ENTITY") == 0){
	    			ctype = CHEST_SWORD_ENTITY;
	    		}else if(strcmp(type,"CHEST_FOOD_ICE_ENTITY") == 0){
	    			ctype = CHEST_FOOD_ICE_ENTITY;
	    		}else if(strcmp(type,"CHEST_FOOD_FISH_ENTITY") == 0){
	    			ctype = CHEST_FOOD_FISH_ENTITY;
	    		}else if(strcmp(type,"CHEST_FOOD_MEAT_ENTITY") == 0){
	    			ctype = CHEST_FOOD_MEAT_ENTITY;
	    		}else if(strcmp(type,"CHEST_FOOD_BERRY_ENTITY") == 0){
	    			ctype = CHEST_FOOD_BERRY_ENTITY;
	    		}else if(strcmp(type,"CHEST_NORMAL_KEY_ENTITY") == 0){
	    			ctype = CHEST_NORMAL_KEY_ENTITY;
	    		}else if(strcmp(type,"CHEST_LEVEL_KEY_ENTITY") == 0){
	    			ctype = CHEST_LEVEL_KEY_ENTITY;
	    		}else if(strcmp(type,"CHEST_JAIL_KEY_ENTITY") == 0){
	    			ctype = CHEST_JAIL_KEY_ENTITY;
	    		}else if(strcmp(type,"CHEST_BOSS_KEY_ENTITY") == 0){
	    			ctype = CHEST_BOSS_KEY_ENTITY;
	    		}else if(strcmp(type,"CHEST_HAMMER_ENTITY") == 0){
	    			ctype = CHEST_HAMMER_ENTITY;
	    		}else if(strcmp(type,"CHEST_SLINGSHOT_ENTITY") == 0){
	    			ctype = CHEST_SLINGSHOT_ENTITY;
	    		}

	    		Interactible* chest = new Chest(fps_x,fps_y,fps_z,ctype, ival);
    			room->addinteractible(chest);

	    	}else if(strcmp(object_type,"Plate") == 0){

	    		EntityType ctype;

	    		if(strcmp(type,"PLATE_1_2_INTERACTIBLE") == 0){
	    			ctype = PLATE_1_2_INTERACTIBLE;
	    		}

	    		Interactible* plate = new Plate(fps_x,fps_y,fps_z,ctype, ival);
	    		arrayp[ip] = plate->getID();
	    		ip++;
    			room->addinteractible(plate);

	    	}else if(strcmp(object_type,"DragBox") == 0){


	    		Interactible* d_box = new DragBox(fps_x,fps_y,fps_z, ival);
	    		arraydb[idb] = d_box->getID();
	    		idb++;
    			room->addinteractible(d_box);
	    	}

			additions = additions->NextSiblingElement("Interactible");	    	
	    }

	    //Platforms

	    additions = level->FirstChildElement("Platform");

	    while(additions != nullptr){

	    	//Load xml

	    	const char* ps_x = additions->FirstChildElement("X")->GetText();
	    	const char* ps_y = additions->FirstChildElement("Y")->GetText();
	    	const char* ps_z = additions->FirstChildElement("Z")->GetText();

	    	float fps_x = std::stof (ps_x,&sz);
			float fps_y = std::stof (ps_y,&sz);
			float fps_z = std::stof (ps_z,&sz);

	    	//Create platform

	    	Platform* platform = new Platform(fps_x,fps_y,fps_z);

	    	//Load path of platform

	    	TiXmlElement* path = additions->FirstChildElement("Path");

	    	const char* pathmin_x = path->FirstChildElement("MinX")->GetText();
	    	const char* pathmin_z = path->FirstChildElement("MinZ")->GetText();
	    	const char* pathmax_x = path->FirstChildElement("MaxX")->GetText();
	    	const char* pathmax_z = path->FirstChildElement("MaxZ")->GetText();
	    	const char* loop = path->FirstChildElement("Loop")->GetText();

	    	float fpathmin_x = std::stof (pathmin_x,&sz);
			float fpathmin_z = std::stof (pathmin_z,&sz);
			float fpathmax_x = std::stof (pathmax_x,&sz);
			float fpathmax_z = std::stof (pathmax_z,&sz);

			int iloop;

			std::stringstream ssloop;
		    ssloop << loop;
		    ssloop >> iloop;

		    //Create path of platform

		    platform->generatePath(fpathmin_x,fpathmin_z,fpathmax_x,fpathmax_z,iloop);
		    room->addPlatform(platform);

	    	additions = additions->NextSiblingElement("Platform");
	    }

	    //Triggers
	    
	    additions = level->FirstChildElement("Trigger");

	    while(additions != nullptr){

	    	//Load xml
			const char* tri_object = additions->FirstChildElement("Object")->GetText();
	    	const char* tri_x = additions->FirstChildElement("X")->GetText();
	    	const char* tri_y = additions->FirstChildElement("Y")->GetText();
	    	const char* tri_w = additions->FirstChildElement("Width")->GetText();

	    	float ftri_x = std::stof (tri_x,&sz);
			float ftri_y = std::stof (tri_y,&sz);
			float ftri_w = std::stof (tri_w,&sz);

	    	//Create trigger
	    	if(strcmp(tri_object,"Trigger_Enemies") == 0){
	    		Trigger_Enemies* tre = new Trigger_Enemies();
			    tre->addRectangularTriggerRegion(Vector2f(ftri_x,ftri_y), ftri_w);
			    room->addTrigger(tre);
	    	}else if(strcmp(tri_object,"Trigger_NEnemies") == 0){
	    		const char* valor = additions->FirstChildElement("Value")->GetText();
				int ivalD;
				std::stringstream ssmv;
				ssmv << valor;
				ssmv >> ivalD;
	    		Trigger_NEnemies* ntre = new Trigger_NEnemies();
	    		ntre->setValor(ivalD);
    			room->addTrigger(ntre);
	    	}else if(strcmp(tri_object,"Trigger_Fall") == 0){
	    		Trigger_Fall* trf = new Trigger_Fall();
			    trf->addRectangularTriggerRegion(Vector2f(ftri_x,ftri_y),ftri_w);
			    room->addTrigger(trf);
	    	}else if(strcmp(tri_object,"Trigger_Level") == 0){
	    		const char* tri_lvl = additions->FirstChildElement("Level")->GetText();
	    		std::string::size_type slevel;
	    		float ftri_lvl = std::stof (tri_lvl,&slevel);
	    		Trigger_Level* trl = new Trigger_Level();
	    		trl->addRectangularTriggerRegion(Vector2f(ftri_x,ftri_y),ftri_w);
	    		trl->setToLevel(ftri_lvl);
			    room->addTrigger(trl);
	    	}else if(strcmp(tri_object, "Trigger_Camera") == 0){
	    		Trigger_Camera * tc = new Trigger_Camera();
		        tc->addRectangularTriggerRegion(Vector2f(ftri_x, ftri_y), ftri_w, 50.f);
		        room->addTrigger(tc); 
	    	}else if(strcmp(tri_object, "Trigger_Dialogo") == 0){
				const char* valor = additions->FirstChildElement("Value")->GetText();
				int ivalD;
				std::stringstream ssmv;
				ssmv << valor;
				ssmv >> ivalD;
	    		Trigger_Dialogo * td = new Trigger_Dialogo();
				td->setChat(ivalD);
		        td->addRectangularTriggerRegion(Vector2f(ftri_x, ftri_y), ftri_w, 50.f);
		        room->addTrigger(td);
	    	}else if(strcmp(tri_object, "Trigger_KPongo") == 0){
				Trigger_KPongo * tc = new Trigger_KPongo();
		        tc->addRectangularTriggerRegion(Vector2f(ftri_x, ftri_y), ftri_w, 50.f);
		        room->addTrigger(tc); 
	    	}else if(strcmp(tri_object, "Trigger_Door") == 0){
	    		Trigger_Door * tc = new Trigger_Door();
		        tc->addRectangularTriggerRegion(Vector2f(ftri_x, ftri_y), ftri_w, 50.f);
		        room->addTrigger(tc); 
	    	}
	    	
	    	additions = additions->NextSiblingElement("Trigger"); 	
	    }
	    
	    //Puzzles
	    
	    additions = level->FirstChildElement("Puzzle");

	    while(additions != nullptr){
	    	Puzzle* puzzle = new Puzzle();

	    	TiXmlElement* pz_object = additions->FirstChildElement("Object");

	    	while(pz_object != nullptr){
	    		const char* type = pz_object->FirstChildElement("Type")->GetText();
	    		const char* pzid = pz_object->FirstChildElement("ID")->GetText();
	    		const char* pzpx = pz_object->FirstChildElement("XC")->GetText();
	    		const char* pzpy = pz_object->FirstChildElement("YC")->GetText();
	    		const char* pzpz = pz_object->FirstChildElement("ZC")->GetText();

	    		int ipzpx, ipzpz, ipzpy, ipzpid;
				EntityType v = PRESSED_INTERACTIBLE;

				std::stringstream sspid;
				sspid << pzid;
				sspid >> ipzpid;

				std::stringstream sspx;
				sspx << pzpx;
				sspx >> ipzpx;

				std::stringstream sspy;
				sspy << pzpy;
				sspy >> ipzpy;

				std::stringstream sspz;
				sspz << pzpz;
				sspz >> ipzpz;

				int k=0;

				if(strcmp(type,"Plate") == 0){
					v = PRESSED_INTERACTIBLE;
					k = 1;
				}else if(strcmp(type,"DragBox") == 0){
					v = DRAGGABLE_INTERACTIBLE;
					k = 2;
				}
		
				EntityFlag e;

				switch(k){
					case 1:
					e.id = arrayp[ipzpid];
					break;
					case 2:
					e.id = arraydb[ipzpid];
					break;
				}
				
				e.p_x = ipzpx;
				e.p_y = ipzpy;
				e.p_z = ipzpz;
				e.type = v;

				puzzle->addCondition(e); 

	    		pz_object = pz_object->NextSiblingElement("Object");
	    	}

	    	room->addPuzzle(puzzle);

	    	additions = additions->NextSiblingElement("Puzzle");
	    }
	    i++;
	    level = level->NextSiblingElement("Room");
	}

	//Load Doors

    level = mapp->FirstChildElement(map);
	level = level->FirstChildElement("Door");

	while(level != nullptr){

		//Load from xml

		const char* ps_x = level->FirstChildElement("X")->GetText();
		const char* ps_z = level->FirstChildElement("Z")->GetText();
		const char* down = level->FirstChildElement("Down")->GetText();
		const char* closed = level->FirstChildElement("Closed")->GetText();
		const char* type = level->FirstChildElement("Type")->GetText();
		const char* in1 = level->FirstChildElement("Index1")->GetText();
		const char* in2 = level->FirstChildElement("Index2")->GetText();

		std::string::size_type sz;

		float px = std::stof (ps_x,&sz);
		float pz = std::stof (ps_z,&sz);
		int idown;
		int iclosed;
		int iin1;
		int iin2;

		std::stringstream ss;
	    ss << down;
	    ss >> idown;

	    std::stringstream ss2;
	    ss2 << closed;
	    ss2 >> iclosed;

	    EntityType val=BOSS_DOOR_ENTITY;

		if(strcmp(type,"BOSS_DOOR_ENTITY") == 0){
    		val = BOSS_DOOR_ENTITY;
    	}else if(strcmp(type,"NORMAL_DOOR_ENTITY") == 0){
    		val = NORMAL_DOOR_ENTITY;
    	}else if(strcmp(type,"NORMAL2_DOOR_ENTITY") == 0){
    		val = NORMAL2_DOOR_ENTITY;
    	}else if(strcmp(type,"LEVEL_DOOR_ENTITY") == 0){
    		val = LEVEL_DOOR_ENTITY;
    	}else if(strcmp(type,"JAIL_DOOR_ENTITY") == 0){
    		val = JAIL_DOOR_ENTITY;
    	}else if(strcmp(type,"NORMAL3_DOOR_ENTITY") == 0){
    		val = NORMAL3_DOOR_ENTITY;
    	}
    	
	    std::stringstream ss4;
	    ss4 << in1;
	    ss4 >> iin1;

	    std::stringstream ss5;
	    ss5 << in2;
	    ss5 >> iin2;
 
		//Create door
		Door* door = new Door(px,pz,idown,iclosed,val);
	    door->setIDs(array[iin1], array[iin2]);
	    door->setIndex(iin1,iin2);
	    if(iclosed){
	    	if(val != LEVEL_DOOR_ENTITY && val != NORMAL_DOOR_ENTITY){
	    		door->setY(imediay + 3);
	    	}else{
	    		door->setY(imediay + 7);
	    	}
	    	door->repositionRigidBody();
	    }else{
	    	door->setY(imediay + 100);
	    	door->repositionRigidBody();
	    }  
	    doors.push_back(door);
		
		level = level->NextSiblingElement("Door");
	}
	
	//Load Enemies

	level = mapp->FirstChildElement(map);
	level = level->FirstChildElement("Enemy");

	while(level != nullptr){

		//Load from xml

		const char* ps_x = level->FirstChildElement("X")->GetText();
		const char* ps_y = level->FirstChildElement("Y")->GetText();
		const char* ps_z = level->FirstChildElement("Z")->GetText();
		const char* type = level->FirstChildElement("Type")->GetText();

		std::string::size_type sz;

		float px = std::stof (ps_x,&sz);
		float py = std::stof (ps_y,&sz);
		float pz = std::stof (ps_z,&sz);

		int itype;

		std::stringstream ss;
	    ss << type;
	    ss >> itype;

		//Create enemy

		switch(itype){
			case 1:{
				P_Soldier* soldier = new P_Soldier(Vector3f(px,py,pz));
        		soldier->setType(P_SOLDIER_ENTITY);
        		enemies.push_back(soldier);
				break;
			}
			case 2:{
				P_General* soldier = new P_General(Vector3f(px,py,pz));
        		soldier->setType(P_GENERAL_ENTITY);
        		enemies.push_back(soldier);
				break;
			}
			case 3:{
				P_KPongo* soldier = new P_KPongo(Vector3f(px,py,pz));
        		soldier->setType(P_KPONGO_ENTITY);
        		enemies.push_back(soldier);
				break;
			}
			case 4:{
				Fox* fox = new Fox(Vector3f(px,py,pz));
				fox->setType(FOX_ENTITY);
				enemies.push_back(fox);
				break;
			}
			default:
			break;
		}

        level = level->NextSiblingElement("Enemy");
	}

	//Load VILLAGERS

	level = mapp->FirstChildElement(map);
	level = level->FirstChildElement("Villager");

	while(level != nullptr){

		//Load from xml

		const char* ps_x = level->FirstChildElement("X")->GetText();
		const char* ps_y = level->FirstChildElement("Y")->GetText();
		const char* ps_z = level->FirstChildElement("Z")->GetText();
		const char* type = level->FirstChildElement("Type")->GetText();
		const char*	dial = level->FirstChildElement("Dialog")->GetText();
		const char*	mxdi = level->FirstChildElement("MaxDia")->GetText();

		std::string::size_type sz;

		float px = std::stof (ps_x,&sz);
		float py = std::stof (ps_y,&sz);
		float pz = std::stof (ps_z,&sz);

		int itype;

		std::stringstream ss;
	    ss << type;
	    ss >> itype;

		int idial;

		std::stringstream ss1;
	    ss1 << dial;
	    ss1 >> idial;

		int imxdi;

		std::stringstream ss2;
	    ss2 << mxdi;
	    ss2 >> imxdi;

		//Create SEAL

		switch(itype){
			case 0:{
				Seal_Normal* seal = new Seal_Normal(Vector3f(px,py,pz), idial, imxdi);
        		seal->setType(S_NORMAL_ENTITY);
        		seals.push_back(seal);
				break;
			}
			case 1:{
				int irew;
				const char*	rew = level->FirstChildElement("Reward")->GetText();
				std::stringstream ss;
	   			 	ss << rew;
	    			ss >> irew;

				Seal_Quest* seal = new Seal_Quest(Vector3f(px,py,pz), idial, imxdi,irew);
        		seal->setType(S_QUEST_ENTITY);
        		seals.push_back(seal);
				break;
			}
		}
        level = level->NextSiblingElement("Villager");
	}
}

void FactoriaNivel::putPlayerPosition(int actual, int anterior){
	std::string actstd = std::to_string(actual);
	std::string antstd = std::to_string(anterior);
	std::string path = "l" + antstd + actstd;

	TiXmlDocument* loader = new TiXmlDocument("../assets/levels/LevelDoors.xml");

	bool ok = loader->LoadFile();
    if(!ok){
        exit(0);
    }
	
	TiXmlElement* data = loader->RootElement();
	//Load Player data
 
	auto* pl = Player::Instance();
	
   	TiXmlElement* door = data->FirstChildElement(path.c_str());
   	const char* _x = door->FirstChildElement("X")->GetText();
   	int xi = 0;
   	std::stringstream ssl;
    ssl << _x;
    ssl >> xi;

    const char* _y = door->FirstChildElement("Y")->GetText();

   	int yi = 0;
   	std::stringstream ssy;
    ssy << _y;
    ssy >> yi;

    const char* _z = door->FirstChildElement("Z")->GetText();

   	int zi = 0;
   	std::stringstream ssz;
    ssz << _z;
    ssz >> zi;

    pl->setPositionEntity(Vector3f(xi, yi, zi));
    //pl->setPositionPhysic();
    delete loader;
}