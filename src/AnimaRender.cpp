#include "AnimaRender.h"
#include "Enemy.h"
#include "Dungeon.h"
#include "Inventory.h"
#include "Player.h"
#include <iostream>

AnimaRender::AnimaRender(){
	camera = nullptr;
	device = new AnimaEngine();
	device->init();
	createCamara(0,50,250,0,50,-100/*440,25,90, 440, 0, 0*/); // v3(position)v3(target)
	createLight(5,1,-5,0.7,0.7,0.7); // v3(position)v3(intensity)
	frameDeltaTime = 0;
	now = 0;
	then = 0;
	timeelapsed = 0;
    lastop = 0;
    actualSprite = 0;
    timerLS = 0;

    chr_start = std::chrono::system_clock::now();
    chr_then = std::chrono::system_clock::now();
    chr_delta = 0;

    r_left = nullptr;
    r_right = nullptr;
    v_up = nullptr;
    v_down = nullptr;
	continue_button = nullptr;
    b_pause = nullptr;
	newgame_button = nullptr;
	exit_button = nullptr;
    options_button = nullptr;
	menu_background = nullptr;
    inventory_background = nullptr;
    item_selector = nullptr;
    option_selector = nullptr;
    option_back = nullptr;
    option_use = nullptr;
    option_erase = nullptr;
    input_buttons = nullptr;
    equiped_weapon = nullptr;
    dialog_background = nullptr;
    dialog_icon = nullptr;
    dialog_text0 = nullptr;
    dialog_text1 = nullptr;
    dialog_text2 = nullptr;
    dialog_text3 = nullptr;
    pl_debug1 = nullptr;
    pl_debug2 = nullptr;
    pl_debug3 = nullptr;
    pl_debug4 = nullptr;
    pl_debug5 = nullptr;
    money_text = nullptr;
    enemy_text1 = nullptr;
    enemy_text2 = nullptr;
    enemy_text3 = nullptr;
    enemy_text4 = nullptr;
    enemy_text5 = nullptr;
    inventory_desc0 = nullptr;
    inventory_desc1 = nullptr;
    inventory_desc2 = nullptr;
    inventory_desc3 = nullptr;
    loadingscreen = nullptr;
    dungeon_mesh = nullptr;
    restart_button = nullptr;
    exitmenu_button = nullptr;
    gameover_background = nullptr;
    castle_ground = nullptr;
    axes = nullptr;
    buttons = nullptr;
    object_received = nullptr;
    bg_object_received = nullptr;
    eika3d = nullptr;
    bg_eika_debug = nullptr;
    enemy_infobg = nullptr;
    enemy_selector = nullptr;
    options = nullptr;
    music = nullptr;
    musiconoff = nullptr;
    volumebill = nullptr;
    resolution = nullptr;
    resolution_text = nullptr;

    lifes.reserve(GLOBAL_MAX_LIFE_PLAYER);
    food.reserve(GLOBAL_FOOD_SIZE);
    weapons.reserve(GLOBAL_WEAPONS_SIZE);
    keys.reserve(GLOBAL_MAX_KEYS);
}

AnimaRender::~AnimaRender(){
    if(device){
        device->drop();
	    delete device;
        device = nullptr;
    }
    r_left = nullptr;
    r_right = nullptr;
    v_up = nullptr;
    v_down = nullptr;
    options = nullptr;
    b_pause = nullptr;
    music = nullptr;
    musiconoff = nullptr;
    volumebill = nullptr;
    resolution = nullptr;
    resolution_text = nullptr;
    continue_button = nullptr;
    newgame_button = nullptr;
    exit_button = nullptr;
    options_button = nullptr;
    menu_background = nullptr;
    inventory_background = nullptr;
    item_selector = nullptr;
    input_buttons = nullptr;
    castle_ground = nullptr;
    equiped_weapon = nullptr;
    dialog_background = nullptr;
    dialog_icon = nullptr;
    dialog_text0 = nullptr;
    dialog_text1 = nullptr;
    dialog_text2 = nullptr;
    dialog_text3 = nullptr;
    pl_debug1 = nullptr;
    pl_debug2 = nullptr;
    pl_debug3 = nullptr;
    pl_debug4 = nullptr;
    pl_debug5 = nullptr;
    money_text = nullptr;
    enemy_text1 = nullptr;
    enemy_text2 = nullptr;
    enemy_text3 = nullptr;
    enemy_text4 = nullptr;
    enemy_text5 = nullptr;
    inventory_desc0 = nullptr;
    inventory_desc1 = nullptr;
    inventory_desc2 = nullptr;
    inventory_desc3 = nullptr;
    dungeon_mesh = nullptr;
    exitmenu_button = nullptr;
    loadingscreen = nullptr;
    restart_button = nullptr;
    gameover_background = nullptr;
    axes = nullptr;
    buttons = nullptr;
    option_back = nullptr;
    option_use = nullptr;
    option_erase = nullptr;
    option_selector = nullptr;
    object_received = nullptr;
    bg_object_received = nullptr;
    eika3d = nullptr;
    enemy_infobg = nullptr;
    enemy_selector = nullptr;
    bg_eika_debug = nullptr;
}

void AnimaRender::createCamara(float p_x, float p_y, float p_z, float t_x, float t_y, float t_z){
	if(camera != nullptr){
		delete camera;
		camera = nullptr;
	}
	camera = device->createCamera(glm::vec3(p_x,p_y,p_z),glm::vec3(t_x,t_y,t_z));
}

void AnimaRender::createLight(float p_x, float p_y, float p_z, float i_x, float i_y, float i_z){
	device->createLight(glm::vec3(p_x,p_y,p_z), 0, glm::vec3(i_x,i_y,i_z));
}

void AnimaRender::drawLoadingScreen(){

    if(sigueActivo()){
        
        if(!created){
            loadingscreen = device->createBillboard(glm::vec3(0.75,-0.9,0), "../assets/textures/pingu.png", glm::vec3(0.00055, 0.0007, 1));
            created = true;
        }

        if(timerLS < now){ //update animation

            switch(actualSprite){
                case 0:
                    device->setBillboardTexture("../assets/textures/pingu.png", loadingscreen);
                break;
                case 1:
                    device->setBillboardTexture("../assets/textures/pingu2.png", loadingscreen);
                break;
                case 2:
                    device->setBillboardTexture("../assets/textures/pingu3.png", loadingscreen);
                break;
            }

            if(actualSprite < 3){
                actualSprite += 1;
            }else{
                actualSprite = 0;
            }

            timerLS = now + 0.2;
        }


        device->drawBillboard(loadingscreen);
    }
}

void AnimaRender::loadTextures(int state){
	
    //DELETE BILLBOARDS
	//CLEAR TEXTURE MEMORY

    device->clearEngine();

    created = false;

    //-----clear inventory
    for(int i=0;i<GLOBAL_WEAPONS_SIZE;i++){
        weapons[i] = nullptr;
    }
    weapons.resize(0);
    for(int i=0;i<GLOBAL_FOOD_SIZE;i++){
        food[i] = nullptr;
    }
    food.resize(0);
    for(int i=0;i<GLOBAL_MAX_KEYS;i++){
        keys[i] = nullptr;
    }
    keys.resize(0);
    for(int i=0; i<GLOBAL_MAX_LIFE_PLAYER; i++){
        lifes[i] = nullptr;
    }
    lifes.resize(0);

    dialog_background = nullptr;
    dialog_icon = nullptr;
    dialog_text0 = nullptr;
    dialog_text1 = nullptr;
    dialog_text2 = nullptr;
    dialog_text3 = nullptr;
    pl_debug1 = nullptr;
    pl_debug2 = nullptr;
    pl_debug3 = nullptr;
    pl_debug4 = nullptr;
    pl_debug5 = nullptr;
    money_text = nullptr;
    enemy_text1 = nullptr;
    enemy_text2 = nullptr;
    enemy_text3 = nullptr;
    enemy_text4 = nullptr;
    enemy_text5 = nullptr;
    inventory_desc0 = nullptr;
    inventory_desc1 = nullptr;
    inventory_desc2 = nullptr;
    inventory_desc3 = nullptr;
    resolution_text = nullptr;
    //---------------------

    if(dungeon_mesh){
        device->eraseNode(dungeon_mesh);
        dungeon_mesh = nullptr;
    }

    if(castle_ground){
        device->eraseNode(castle_ground);
        castle_ground = nullptr;
    }

    if(eika3d){
        device->eraseNode(eika3d);
        eika3d = nullptr;
    }

    device->eraseSkybox();
    device->eraseParticleSystem();

    //HUD LOADING
	if(state == 0){
        device->setCameraParams(glm::vec3(0,50,250),glm::vec3(0,50,-100));
        device->createSkybox();
        // std::string shape, float px, float py, float pz, float nRadius, int nBirthrate, float nParticleLife, float dx, float dy, float dz, float nBirthSize, float r, float g, float b, float a
        device->createParticleSystem("../assets/models/copo_Nieve.obj",0,-300,0,500,500,10,0,0.5f,0,1.0f,1,1,1,1);
        dungeon_mesh = device->createMesh(glm::vec3(0,200,0), 0,"../assets/models/castillo.obj", "../assets/textures/castillo.png");
		castle_ground = device->createMesh(glm::vec3(0,200,0), 0,"../assets/models/suelo.obj", "../assets/textures/suelo.png");
        continue_button = device->createBillboard(glm::vec3(-0.3,-0.37,0), "../assets/textures/continue1.png", glm::vec3(0.0018, 0.0025, 1));
		newgame_button = device->createBillboard(glm::vec3(-0.3,-0.57,0/*388,530,0*/), "../assets/textures/newgame1.png", glm::vec3(0.0018, 0.0025, 1));
        exit_button = device->createBillboard(glm::vec3(-0.3,-0.77,0/*388,610,0*/), "../assets/textures/exit1.png", glm::vec3(0.0018, 0.0025, 1));
		menu_background = device->createBillboard(glm::vec3(-0.3,0.1,0), "../assets/textures/cartel.png", glm::vec3(0.00189,0.0028,1.0));
    }else if(state == 2){

        //game over
        gameover_background = device->createBillboard(glm::vec3(-0.34,0.32,0), "../assets/textures/gameover.png", glm::vec3(0.00169,0.0022,1.0));
        gameover_background->setActive(true);

        exitmenu_button = device->createBillboard(glm::vec3(-0.28,-0.80,0/*388,610,0*/), "../assets/textures/exit1.png", glm::vec3(0.0018, 0.0025, 1));
        exitmenu_button->setActive(true);

        restart_button = device->createBillboard(glm::vec3(-0.28,-0.60,0), "../assets/textures/newgame2.png", glm::vec3(0.0018, 0.0025, 1));
        restart_button->setActive(true);

        eika3d = device->createAnimation(glm::vec3(-4,0,-30), 0,"../assets/animations/eika/EikaMuerte/eikamuerte", "../assets/textures/eika-textura.png",81);
        device->playAnimation(eika3d);
        device->rotateNode(eika3d, glm::vec3(1,0,0), -0.5);

        dungeon_mesh = device->createMesh(glm::vec3(0,0,0), 0,"../assets/models/foco.obj", "../assets/textures/foco.png");
        device->setMeshCartoon(dungeon_mesh,false);
        device->rotateNode(dungeon_mesh, glm::vec3(1,0,0), -0.3);

        device->setCameraParams(glm::vec3(0,60,80),glm::vec3(0,0,0));

    }else{

        drawLoadingScreen();
        
        exitmenu_button = device->createBillboard(glm::vec3(-0.26,-0.80,0/*388,610,0*/), "../assets/textures/exit1.png", glm::vec3(0.0018, 0.0025, 1));
        exitmenu_button->setActive(false);

        drawLoadingScreen();

        //Options state
        music =  device->createBillboard(glm::vec3(-0.4,0.12,0), "../assets/textures/music_noselect.png", glm::vec3(0.0018, 0.0025, 1));
        music->setActive(false);
        musiconoff =  device->createBillboard(glm::vec3(-0.37,-0.03,0), "../assets/textures/music_on.png", glm::vec3(0.0018, 0.0025, 1));
        musiconoff->setActive(false);
        volumebill =  device->createBillboard(glm::vec3(0,0.12,0), "../assets/textures/volumen_noselect.png", glm::vec3(0.0018, 0.0025, 1));
        volumebill->setActive(false);
        resolution =  device->createBillboard(glm::vec3(-0.2,-0.2,0), "../assets/textures/resol_noselect.png", glm::vec3(0.0018, 0.0025, 1));
        resolution->setActive(false);
        b_pause =  device->createBillboard(glm::vec3(0.17,-0.5,0), "../assets/textures/exit_nosel.png", glm::vec3(0.0018, 0.0025, 1));
        b_pause->setActive(false);
        
        drawLoadingScreen();

        r_left = device->createBillboard(glm::vec3(-0.3,-0.35,0), "../assets/textures/left_nosel.png", glm::vec3(0.0018, 0.0025, 1));
        r_left->setActive(false);
        r_right = device->createBillboard(glm::vec3(0.1,-0.35,0), "../assets/textures/right_nosel.png", glm::vec3(0.0018, 0.0025, 1));
        r_right->setActive(false);
        v_up = device->createBillboard(glm::vec3(0.06,-0.05,0), "../assets/textures/minus_nosel.png", glm::vec3(0.0018, 0.0025, 1));
        v_up->setActive(false);
        v_down = device->createBillboard(glm::vec3(0.15,-0.05,0), "../assets/textures/plus_nosel.png", glm::vec3(0.0018, 0.0025, 1));
        v_down->setActive(false);
        options = device->createBillboard(glm::vec3(-0.48,-0.60,0), "../assets/textures/options.png", glm::vec3(0.0018, 0.0025, 1));
        options->setActive(false);

        drawLoadingScreen();

        device->loadTexture("../assets/textures/exit_sel.png");
        device->loadTexture("../assets/textures/resolution_select.png");
        device->loadTexture("../assets/textures/music_select.png");
        device->loadTexture("../assets/textures/music_off.png");
        device->loadTexture("../assets/textures/volumen_select.png");
        device->loadTexture("../assets/textures/left_sel.png");
        device->loadTexture("../assets/textures/right_sel.png");
        device->loadTexture("../assets/textures/plus_sel.png");
        device->loadTexture("../assets/textures/minus_sel.png");

        //Pause state
        
        drawLoadingScreen();
        
        continue_button = device->createBillboard(glm::vec3(-0.32,0.12,0), "../assets/textures/resume2.png", glm::vec3(0.0018, 0.0025, 1));
        continue_button->setActive(false);
        newgame_button = device->createBillboard(glm::vec3(-0.32,-0.08,0/*388,530,0*/), "../assets/textures/save1.png", glm::vec3(0.0018, 0.0025, 1));
        newgame_button->setActive(false);
        
        drawLoadingScreen();
        
        exit_button = device->createBillboard(glm::vec3(-0.32,-0.48,0/*388,610,0*/), "../assets/textures/exit1.png", glm::vec3(0.0018, 0.0025, 1));
        exit_button->setActive(false);
        options_button = device->createBillboard(glm::vec3(-0.32,-0.28,0/*388,610,0*/), "../assets/textures/options1.png", glm::vec3(0.0018, 0.0025, 1));
        options_button->setActive(false);
        
        drawLoadingScreen();
        
        menu_background = device->createBillboard(glm::vec3(-0.5,-0.7,0), "../assets/textures/pause.png", glm::vec3(0.00189,0.0028,1.0));
        menu_background->setActive(false);
        
        drawLoadingScreen();
        
        dialog_icon = device->createBillboard(glm::vec3(-0.56,-0.86,0), "../assets/textures/dialogs/IconoDialogo_ReinaWho.png", glm::vec3(0.0018, 0.0025, 1));
        dialog_icon->setActive(false);
        dialog_background = device->createBillboard(glm::vec3(-0.6,-0.9,0), "../assets/textures/fondodialog.png", glm::vec3(0.0018, 0.0025, 1));
        dialog_background->setActive(false);
        bg_eika_debug = device->createBillboard(glm::vec3(0,-0.95,0), "../assets/textures/datospj.png", glm::vec3(0.0018, 0.0025, 1));
        bg_eika_debug->setActive(false);
        enemy_infobg = device->createBillboard(glm::vec3(-0.95,-0.95,0), "../assets/textures/datospj.png", glm::vec3(0.0018, 0.0025, 1));
        enemy_infobg->setActive(false);
        enemy_selector = device->createBillboard(glm::vec3(0,0,0), "../assets/textures/flecha.png", glm::vec3(0.0018, 0.0025, 1));
        enemy_selector->setActive(false);

        drawLoadingScreen();
        
        interact_info = device->createBillboard(glm::vec3(0.1,-0.15,0), "../assets/textures/pulsa p.png", glm::vec3(0.0015, 0.0020, 1));
        interact_info->setActive(false);

        //init inventory
        option_selector = device->createBillboard(glm::vec3(-0.45,0.38,0), "../assets/textures/optionSelector.png", glm::vec3(0.0021,0.0032,1.0));
        option_selector->setActive(false);
        option_back = device->createBillboard(glm::vec3(-0.45,0.38,0), "../assets/textures/backOption.png", glm::vec3(0.0021,0.0032,1.0));
        option_back->setActive(false);

        drawLoadingScreen();


        object_received = device->createBillboard(glm::vec3(0.89,0.42,0), "../assets/textures/sword.png", glm::vec3(0.0021,0.0032,1.0));
        object_received->setActive(false);
        bg_object_received = device->createBillboard(glm::vec3(0.63,0.38,0), "../assets/textures/objObtenido.png", glm::vec3(0.0012,0.0019,1.0));
        bg_object_received->setActive(false);

        drawLoadingScreen();

        option_use = device->createBillboard(glm::vec3(-0.45,0.38,0), "../assets/textures/useOption.png", glm::vec3(0.0021,0.0032,1.0));
        option_use->setActive(false);
        option_erase = device->createBillboard(glm::vec3(-0.45,0.38,0), "../assets/textures/eraseOption.png", glm::vec3(0.0021,0.0032,1.0));
        option_erase->setActive(false);

        drawLoadingScreen();

        item_selector = device->createBillboard(glm::vec3(-0.45,0.38,0), "../assets/textures/selector.png", glm::vec3(0.0021,0.0032,1.0));
        item_selector->setActive(false);
        
        drawLoadingScreen();
        
        for(int i=0;i<GLOBAL_WEAPONS_SIZE;i++){
            ABillboard* b = device->createBillboard(glm::vec3(-0.46+(i*0.1),0.38,0), "../assets/textures/sword.png", glm::vec3(0.00189,0.0028,1.0));
            b->setActive(false);
            weapons.push_back(b);
            b = nullptr;
        }
        
        drawLoadingScreen();
        
        int cont = 0;
        float x = 0.0;
        float y = 0.21;
        for(int i=0;i<GLOBAL_FOOD_SIZE;i++){
            ABillboard* b = device->createBillboard(glm::vec3(-0.46+(x*0.095),y,0), "../assets/textures/icecream.png", glm::vec3(0.00189,0.0028,1.0));
            b->setActive(false);
            food.push_back(b);
            b = nullptr;

            cont++;
            if(cont == 6){
                cont = 0;
                x = 0.0;
                y -= 0.145;
            }else{
                x++;
            }
        }
        
        drawLoadingScreen();
        
        for(int i=0;i<GLOBAL_MAX_KEYS;i++){
            ABillboard* b = device->createBillboard(glm::vec3(-0.45+(i*0.092),-0.39,0), "../assets/textures/key.png", glm::vec3(0.00189,0.0028,1.0));
            b->setActive(false);
            keys.push_back(b);
            b = nullptr;
        }   
        
        drawLoadingScreen();
        
        inventory_background = device->createBillboard(glm::vec3(-0.5,-0.7,0), "../assets/textures/inventario.png", glm::vec3(0.00189,0.0028,1.0));
        inventory_background->setActive(false);

        float par = 0;

        for(int i=0; i<GLOBAL_MAX_LIFE_PLAYER; i++){
            if(i%2==0){
                lifes.push_back(device->createBillboard(glm::vec3(-0.96+(par*0.11),0.82,0), "../assets/textures/life_l.png", glm::vec3(0.0010, 0.0015, 1)));
            }else{
                lifes.push_back(device->createBillboard(glm::vec3(-0.91+(par*0.11),0.82,0), "../assets/textures/life_r.png", glm::vec3(0.0010, 0.0015, 1)));
                par++;
            }
        }
        
        drawLoadingScreen();
        
        equiped_weapon = device->createBillboard(glm::vec3(0.75,0.78,0), "../assets/textures/espada.png", glm::vec3(0.0012, 0.0015, 1));
        equiped_weapon->setActive(false);
        control_info = device->createBillboard(glm::vec3(0.87,0.78,0), "../assets/textures/hablar.png", glm::vec3(0.0022, 0.0025, 1));
        control_info->setActive(false);
        
        drawLoadingScreen();
        
        input_buttons = device->createBillboard(glm::vec3(0.57,0.78,0), "../assets/textures/controles.png", glm::vec3(0.0010,0.0015,1.0));
        input_buttons->setActive(true);
        
        drawLoadingScreen();

        //LOAD CHARACTERS TEXTURES
        device->loadTexture("../assets/textures/eika.png");
        device->loadTexture("../assets/textures/eika2.png");
        device->loadTexture("../assets/textures/eika2_e.png");
        device->loadTexture("../assets/textures/eika3.png");
        device->loadTexture("../assets/textures/eika3_e.png");
        device->loadTexture("../assets/textures/eika_a.png");
        device->loadTexture("../assets/textures/eika_e.png");

        drawLoadingScreen();

        device->loadTexture("../assets/textures/psoldier_sleep1.png");
        device->loadTexture("../assets/textures/psoldier_sleep2.png");
        device->loadTexture("../assets/textures/psoldier_sleep3.png");

        drawLoadingScreen();

        device->loadTexture("../assets/textures/pinguG1.png");
        device->loadTexture("../assets/textures/pinguG2.png");
        device->loadTexture("../assets/textures/pinguG3.png");

        drawLoadingScreen();

        device->loadTexture("../assets/textures/zorro1.png");
        device->loadTexture("../assets/textures/zorro2.png");
        device->loadTexture("../assets/textures/zorro3.png");

        drawLoadingScreen();

        device->loadTexture("../assets/textures/kpongo.png");
        device->loadTexture("../assets/textures/kpongocaida.png");
        device->loadTexture("../assets/textures/kpongodesliz.png");
        device->loadTexture("../assets/textures/kpongomareo1.png");
        device->loadTexture("../assets/textures/kpongomareo2.png");
        device->loadTexture("../assets/textures/kpongomareo3.png");

        drawLoadingScreen();

        device->loadTexture("../assets/textures/seal.png");

        //LOAD INTERACTIBLES TEXTURES

        device->loadTexture("../assets/textures/cofre.png");
        device->loadTexture("../assets/textures/normal_door.jpg");
        device->loadTexture("../assets/textures/level_door.jpg");

        drawLoadingScreen();
        eika3d = device->createPostProcessAnimation(glm::vec3(0,-1000,-1000),"../assets/animations/eika/eikastanding/eikastanding", "../assets/textures/eika-textura2.png",406);
        drawLoadingScreen();

        device->rotateNode(eika3d,glm::vec3(1,0,0), -0.5);
        //device->playAnimation(eika3d);
        device->setLoopAnimation(eika3d, true);

        drawLoadingScreen();

        device->loadAnimation("../assets/animations/eika/eikadebil/eikadebil",181);

        drawLoadingScreen();

        device->loadAnimation("../assets/animations/eika/eikacomer/eikacomer",121);

        drawLoadingScreen();

        device->loadAnimation("../assets/animations/eika/eikaequip/eikaequip",123);

        drawLoadingScreen();
	}
}

void AnimaRender::redibujarOptions(int a,int b){
    switch(a){
        case 0:
            if(b == 0){
                device->setBillboardTexture("../assets/textures/music_on.png", musiconoff);
            }else{
                device->setBillboardTexture("../assets/textures/music_off.png", musiconoff);
            }
        break;
        case 1:
            if(b == 0){
                device->setBillboardTexture("../assets/textures/minus_sel.png", v_up);
                device->setBillboardTexture("../assets/textures/plus_nosel.png", v_down);
            }else{
                device->setBillboardTexture("../assets/textures/minus_nosel.png", v_up);
                device->setBillboardTexture("../assets/textures/plus_sel.png", v_down);
            }
        break;
        case 2:
            if(lastop == 4 && b == 0){
                device->setBillboardTexture("../assets/textures/left_nosel.png", r_left);
                device->setBillboardTexture("../assets/textures/right_sel.png", r_right);
            }else if(b == 4 && lastop == 0){
                device->setBillboardTexture("../assets/textures/left_sel.png", r_left);
                device->setBillboardTexture("../assets/textures/right_nosel.png", r_right);
            }else if(lastop > b){
                device->setBillboardTexture("../assets/textures/left_sel.png", r_left);
                device->setBillboardTexture("../assets/textures/right_nosel.png", r_right);
            }else{
                device->setBillboardTexture("../assets/textures/left_nosel.png", r_left);
                device->setBillboardTexture("../assets/textures/right_sel.png", r_right);
            }

            lastop = b;

            std::string str = "";

            if(resolution_text != nullptr){
                switch(b){
                    case 0:
                        device->setText(resolution_text,"640/480");
                        device->setTextPosition(resolution_text,glm::vec3(-0.1,-0.28,0));
                    break;
                    case 1:
                        device->setText(resolution_text,"1080/720");
                        device->setTextPosition(resolution_text,glm::vec3(-0.12,-0.28,0));
                    break;
                    case 2:
                        device->setText(resolution_text,"1280/720");
                        device->setTextPosition(resolution_text,glm::vec3(-0.12,-0.28,0));
                    break;
                    case 3:
                        device->setText(resolution_text,"1600/1200");
                        device->setTextPosition(resolution_text,glm::vec3(-0.13,-0.28,0));
                    break;
                    case 4:
                        device->setText(resolution_text,"2048/1080");
                        device->setTextPosition(resolution_text,glm::vec3(-0.13,-0.28,0));
                    break;
                    default:
                        device->setText(resolution_text,"");
                    break;
                }
            }
        break;
    }
}

void AnimaRender::redibujarOptions(int a){
    switch(a){
        case 0:
            device->setBillboardTexture("../assets/textures/volumen_noselect.png", volumebill);
            device->setBillboardTexture("../assets/textures/resol_noselect.png", resolution);
            device->setBillboardTexture("../assets/textures/exit_nosel.png", b_pause);
            device->setBillboardTexture("../assets/textures/music_select.png", music);
        break;
        case 1:
            device->setBillboardTexture("../assets/textures/volumen_select.png", volumebill);
            device->setBillboardTexture("../assets/textures/resol_noselect.png", resolution);
            device->setBillboardTexture("../assets/textures/exit_nosel.png", b_pause);
            device->setBillboardTexture("../assets/textures/music_noselect.png", music);
        break;
        case 2:
            device->setBillboardTexture("../assets/textures/volumen_noselect.png", volumebill);
            device->setBillboardTexture("../assets/textures/resolution_select.png", resolution);
            device->setBillboardTexture("../assets/textures/music_noselect.png", music);
            device->setBillboardTexture("../assets/textures/exit_nosel.png", b_pause);
        break;
        case 3:
            device->setBillboardTexture("../assets/textures/volumen_noselect.png", volumebill);
            device->setBillboardTexture("../assets/textures/resol_noselect.png", resolution);
            device->setBillboardTexture("../assets/textures/music_noselect.png", music);
            device->setBillboardTexture("../assets/textures/exit_sel.png", b_pause);
        break;
        default:
        break;
    }
    if(resolution_text == nullptr){
        resolution_text = device->createText(glm::vec3(-0.1,-0.28,0), "640/480", 0.0008, glm::vec3(0.0,1.0,0.0));
    }
    device->setBillboardTexture("../assets/textures/minus_nosel.png", v_up);
    device->setBillboardTexture("../assets/textures/plus_nosel.png", v_down);
    device->setBillboardTexture("../assets/textures/left_nosel.png", r_left);
    device->setBillboardTexture("../assets/textures/right_nosel.png", r_right);
}

void AnimaRender::setLoadingInactive(bool f){
    loadingscreen->setActive(!f);
}

void AnimaRender::setWindowSize(float w, float h){
    device->setWindowSize(w,h);
}

void AnimaRender::setRadius(float r){
    device->setRadius(r);
}

void AnimaRender::setTransition(bool t){
    device->setPostProcessing(t);
}

void AnimaRender::clearDungeonMesh(){
    if(dungeon_mesh){
        device->eraseMeshNode(dungeon_mesh);
        dungeon_mesh = nullptr;
    }
}

void AnimaRender::setBillboardActives(){
	menu_background->setActive(false);
}

void AnimaRender::setMeshTexture(Node& n, std::string t){
    device->setMeshTexture(n.getNode(),t);
}

void AnimaRender::drawEntity(float r, float x, float y, float z, char c){ //en realidad lo que hace es añadir un nodo a la escena

}

void AnimaRender::drawBillBoardEntity(Node& n,Node& shadow,float x, float y, float z, EntityType type){
	switch(type){
        case PLAYER_ENTITY:
            n.getNode() = device->createMesh(glm::vec3(x, y, z), 0,"../assets/models/eika.obj", "../assets/textures/eika.png");
            break;
        case P_SOLDIER_ENTITY:
            n.getNode() = device->createMesh(glm::vec3(x, y, z), 0,"../assets/models/pinguS.obj", "../assets/textures/pingu.png");
            break;
        case P_GENERAL_ENTITY:
            n.getNode() = device->createMesh(glm::vec3(x, y, z), 0,"../assets/models/pgeneral.obj", "../assets/textures/pinguG1.png");
            break;
        case S_NORMAL_ENTITY:
            n.getNode() = device->createMesh(glm::vec3(-x, y, z), 0,"../assets/models/seal.obj", "../assets/textures/seal.png");
            break;
        case S_QUEST_ENTITY:
            n.getNode() = device->createMesh(glm::vec3(-x, y, z), 0,"../assets/models/seal.obj", "../assets/textures/seal.png");
            break;
        case FOX_ENTITY:
            n.getNode() = device->createMesh(glm::vec3(x, y, z), 0,"../assets/models/zorro.obj", "../assets/textures/zorro1.png");
            break;
        case P_KPONGO_ENTITY:
            n.getNode() = device->createMesh(glm::vec3(x, y, z), 0,"../assets/models/kpong.obj", "../assets/textures/kpongo.png");
            break;
        case QUEEN_ENTITY:
            n.getNode() = device->createMesh(glm::vec3(x, y, z), 0,"../assets/models/queen.obj", "../assets/textures/reina.png");
            break;
        default:
        break;
    }
    shadow.getNode() = device->createMesh(glm::vec3(x, y, z), 0,"../assets/models/shadow.obj", "../assets/textures/shadow.png");
    
    if(type == P_KPONGO_ENTITY){
        device->scaleNode(shadow.getNode(), glm::vec3(1.7,1.7,1.7));
    }

    device->setMeshCartoon(n.getNode(), false);
}

void AnimaRender::beginScene(bool a, bool b, int c, int d, int e, int f){
 
}

void AnimaRender::drawScene(){
	device->drawScene();
}

void AnimaRender::drawDoor(Node& n, float dx, float dy, float dz, bool d, EntityType type){
    switch(type){
        case JAIL_DOOR_ENTITY:{
            n.getNode() = device->createMesh(glm::vec3(-dx, -(dy-10), dz), 0,"../assets/models/jail_door.obj", "../assets/textures/grey.jpg");
            break;
        }
        case NORMAL_DOOR_ENTITY:{
            n.getNode() = device->createMesh(glm::vec3(-dx, -(dy-10), dz), 0,"../assets/models/normal_door.obj", "../assets/textures/normal_door.jpg");
            break;
        }
        case LEVEL_DOOR_ENTITY:{
            n.getNode() = device->createMesh(glm::vec3(-dx, -(dy-10), dz), 0,"../assets/models/level_door.obj", "../assets/textures/level_door.jpg");
            break;
        }
        case NORMAL2_DOOR_ENTITY:{
            n.getNode() = device->createMesh(glm::vec3(-dx, -(dy-15), dz), 0,"../assets/models/door2.obj", "../assets/textures/door2.jpg");
            break;
        }
        case NORMAL3_DOOR_ENTITY:{
            n.getNode() = device->createMesh(glm::vec3(-dx, -(dy-15), dz), 0,"../assets/models/door2.obj", "../assets/textures/door2.jpg");
            break;
        }
        case BOSS_DOOR_ENTITY:{
            n.getNode() = device->createMesh(glm::vec3(-dx, -(dy-15), dz), 0,"../assets/models/bossdoor.obj", "../assets/textures/bossdoor.jpg");
            break;
        }
        default:
        break;
    }

    if(!d){ 
        device->rotateNode(n.getNode(), glm::vec3(0,1,0), -1.571);
    }
}

void AnimaRender::drawChest(Node& n, float dx, float dy, float dz){
    n.getNode() = device->createAnimation(glm::vec3(-dx, -(dy), dz), 0,"../assets/animations/cofre/cofre-riggin", "../assets/textures/cofre.png", 51);
}

void AnimaRender::drawFlag(Node& n, float x, float y, float z, int angle){

    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/flag.obj", "../assets/textures/bandera.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(0,1,0), aux);
    }
}

void AnimaRender::drawColumn(Node& n, float x, float y, float z, int angle){

    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/columna.obj", "../assets/textures/columna.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(1,0,0), aux);
    }
}

void AnimaRender::drawS1(Node& n, float x, float y, float z, int angle){
    n.getNode() = device->createMeshLOD(glm::vec3(-x,-y,z), 0,"../assets/models/estatua1", "../assets/textures/grey1");
    
    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(1,0,0), aux);
    }
}

void AnimaRender::drawThrone(Node& n, float x, float y, float z, int angle){
    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/trono.obj", "../assets/textures/trono.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(0,1,0), aux);
    }
}

void AnimaRender::drawS2(Node& n, float x, float y, float z, int angle){
    n.getNode() = device->createMeshLOD(glm::vec3(-x,-y,z), 0,"../assets/models/estatua2", "../assets/textures/grey2");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(1,0,0), aux);
    }
}

void AnimaRender::drawSwordDeco(Node& n, float x, float y, float z, int angle){
    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/espada2.obj", "../assets/textures/espadadeco.png");
    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(0,1,0), aux);
    }
}

void AnimaRender::drawLance(Node& n, float x, float y, float z, int angle){
    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/lanza.obj", "../assets/textures/lanza.png");
    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(0,0,1), aux);
    }
}

void AnimaRender::drawPlate(Node&n, float dx, float dy, float dz){
    n.getNode() = device->createMesh(glm::vec3(-dx, -(dy), dz), 0,"../assets/models/plate.obj", "../assets/textures/plate.jpg");
}

void AnimaRender::drawPlatform(Node& n, float dx, float dy, float dz){
    n.getNode() = device->createMesh(glm::vec3(-dx, -(dy), dz), 0,"../assets/models/platform.obj", "../assets/textures/platform.jpg");
}


void AnimaRender::drawBox(Node& n, float dx, float dy, float dz){
    n.getNode() = device->createMesh(glm::vec3(-dx, -(dy), dz), 0,"../assets/models/dbox.obj", "../assets/textures/dragbox.png");
}

void AnimaRender::drawLine(float x1, float y1, float z1, float x2, float y2, float z2, char c){

}

void AnimaRender::drawInventory(int f[], bool w[], int k, bool master_key, bool level_key, bool jail_key, float x_, float y_, int dinero){
    
    //weapons
    //then we check how many weapons the player has and then we show it
    for(int i=0;i<GLOBAL_WEAPONS_SIZE;i++){
        if(w[i]==true){
            //we need to add different types of weapons
            switch(i){
                case 0:
                    //guienv->addImage(sword,core::position2d<int>(325+(50*i),220));
                    device->setBillboardTexture("../assets/textures/sword.png", weapons[i]);
                    break;
                case 1:
                    //guienv->addImage(slingshot,core::position2d<int>(325+(50*i),220));
                    device->setBillboardTexture("../assets/textures/slingshot.png", weapons[i]);
                    break;
                case 2:
                    //guienv->addImage(hammer,core::position2d<int>(325+(50*i),220));
                    device->setBillboardTexture("../assets/textures/hammer.png", weapons[i]);
                    break;
            }
            weapons[i]->setActive(true);
        }
    }

    //food
    //same process for the food
    for(int i=0;i<GLOBAL_FOOD_SIZE;i++){
        if(f[i]!=-1){
            switch(f[i]){
                case 0:
                    //guienv->addImage(icecream,core::position2d<int>(325+(50*x),y));
                    device->setBillboardTexture("../assets/textures/icecream.png", food[i]);
                    break;
                case 1:
                    //guienv->addImage(fish,core::position2d<int>(325+(50*x),y));
                    device->setBillboardTexture("../assets/textures/fish.png", food[i]);
                    break;
                case 2:
                    //guienv->addImage(meat,core::position2d<int>(325+(50*x),y));
                    device->setBillboardTexture("../assets/textures/meat.png", food[i]);
                    break;
                case 3:
                    //guienv->addImage(berry,core::position2d<int>(325+(50*x),y));
                    device->setBillboardTexture("../assets/textures/berry.png", food[i]);
                    break;
            }
            food[i]->setActive(true);
        }else{
            food[i]->setActive(false);
        }
    }

    //keys
    int i;
    for(i=0;i<k;i++){
        device->setBillboardTexture("../assets/textures/key.png", keys[i]);
        keys[i]->setActive(true);
    }
    if(level_key==true){
        device->setBillboardTexture("../assets/textures/LevelKey.png", keys[i]);
        keys[i]->setActive(true);
        i++;
    }
    if(jail_key==true){
        device->setBillboardTexture("../assets/textures/JailKey.png", keys[i]);
        keys[i]->setActive(true);
        i++;
    }
    if(master_key==true){
        device->setBillboardTexture("../assets/textures/masterKey.png", keys[i]);
        keys[i]->setActive(true);
    }

    //done drawing the items, now draw the selector
    item_selector->setPosition(glm::vec3(x_,y_,0));
    item_selector->setActive(true);
    RenderInventoryDescription();

    //model of eika
    auto* p = Player::Instance();
    glm::vec3 position;

    float y = 0;
    float x = 0;

    if(p->getLifes()<=4){
        if(p->getCamera()->getY() > 40){
            x = -p->getCamera()->getX()-55;
            y = -p->getCamera()->getY()+115;
        }else{
            x = -p->getCamera()->getX()-50;
            y = -p->getCamera()->getY()+75;
        }
        position = glm::vec3(x,y,p->getCamera()->getZ()+90);
        device->changeAnimation(eika3d,"../assets/animations/eika/eikadebil/eikadebil", "../assets/textures/eika-textura3.png",181);
    }else{

        if(p->getCamera()->getY() > 40){
            x = -p->getCamera()->getX()-60;
            y = -p->getCamera()->getY()+110;
        }else{
            x = -p->getCamera()->getX()-55;
            y = -p->getCamera()->getY()+70;
        }
        position = glm::vec3(x,y,p->getCamera()->getZ()+90);
        device->changeAnimation(eika3d,"../assets/animations/eika/eikastanding/eikastanding", "../assets/textures/eika-textura2.png",406);
    }

    device->translateNode(eika3d,position);
    //device->translateNode(eika3d,glm::vec3(-p->getX()-40,-p->getY()+20,p->getZ()));
    device->playAnimation(eika3d);
    device->setLoopAnimation(eika3d,true);
}

void AnimaRender::setPositionEntity(Node& entity, float x, float y, float z){
    device->translateNode(entity.getNode(), glm::vec3(-x,-y,z));
}

void AnimaRender::rotateEntity(Node& entity, float ang){
    float rad = ang * 0.0174533;
    device->rotateNode(entity.getNode(), glm::vec3(0,1,0), rad);
}

void AnimaRender::dibujarMenu(int option){
	if(continue_button != nullptr && newgame_button != nullptr && exit_button != nullptr){
		switch(option){
			case 0:
	            device->setBillboardTexture("../assets/textures/continue2.png", continue_button);
	            device->setBillboardTexture("../assets/textures/newgame1.png", newgame_button);
	            device->setBillboardTexture("../assets/textures/exit1.png", exit_button);
	        break;
	        case 1:
	        	device->setBillboardTexture("../assets/textures/continue1.png", continue_button);
	            device->setBillboardTexture("../assets/textures/newgame2.png", newgame_button);
	            device->setBillboardTexture("../assets/textures/exit1.png", exit_button);
	        break;
	        case 2:
	        	device->setBillboardTexture("../assets/textures/continue1.png", continue_button);
	            device->setBillboardTexture("../assets/textures/newgame1.png", newgame_button);
	            device->setBillboardTexture("../assets/textures/exit2.png", exit_button);
	        break;
	        default:
	        	device->setBillboardTexture("../assets/textures/continue2.png", continue_button);
	            device->setBillboardTexture("../assets/textures/newgame1.png", newgame_button);
	            device->setBillboardTexture("../assets/textures/exit1.png", exit_button);
	        break;
		}
	}
}

void AnimaRender::drawGameOverMenu(int option){
    if(newgame_button !=nullptr && exit_button!=nullptr){
        switch(option){
            case 0:
                device->setBillboardTexture("../assets/textures/newgame2.png", restart_button);
                device->setBillboardTexture("../assets/textures/exit1.png", exitmenu_button);
            break;
            case 1:
                device->setBillboardTexture("../assets/textures/newgame1.png", restart_button);
                device->setBillboardTexture("../assets/textures/exit2.png", exitmenu_button);
            break;
            default:
                device->setBillboardTexture("../assets/textures/newgame2.png", restart_button);
                device->setBillboardTexture("../assets/textures/exit1.png", exitmenu_button);
            break;
        }
    }
}

void AnimaRender::dibujarGO(){
    dibujarMenu(0);      
}

void AnimaRender::activateBillboards(Estado::StateType t){
    switch(t){
        case Estado::StateType::DUNGEON_STATE:{
            continue_button->setActive(false);
            newgame_button->setActive(false);
            dialog_background->setActive(false);
            dialog_icon->setActive(false);
            options_button->setActive(false);
            exit_button->setActive(false);
            menu_background->setActive(false);
            inventory_background->setActive(false);
            item_selector->setActive(false);
            for(int i=0;i<GLOBAL_WEAPONS_SIZE;i++){
                weapons[i]->setActive(false);
            }
            for(int i=0;i<GLOBAL_MAX_KEYS;i++){
                keys[i]->setActive(false);
            }
            for(int i=0;i<GLOBAL_FOOD_SIZE;i++){
                food[i]->setActive(false);
            }
            device->translateNode(eika3d,glm::vec3(glm::vec3(0,-1000,-1000)));
            device->stopAnimation(eika3d);
            device->resetAnimation(eika3d);
            break;
        }
        case Estado::StateType::PAUSE_MENU_STATE:{
            continue_button->setActive(true);
            newgame_button->setActive(true);
            options_button->setActive(true);
            exit_button->setActive(true);
            menu_background->setActive(true);
            interact_info->setActive(false);
            bg_object_received->setActive(false);
            object_received->setActive(false);
            options->setActive(false);
            music->setActive(false);
            musiconoff->setActive(false);
            b_pause->setActive(false);
            volumebill->setActive(false);
            resolution->setActive(false);
            r_left->setActive(false);
            r_right->setActive(false);
            v_up->setActive(false);
            v_down->setActive(false);
            if(resolution_text) device->setText(resolution_text, "");
            break;
        }
        case Estado::StateType::INVENTORY_STATE:{
            inventory_background->setActive(true);
            interact_info->setActive(false);
            option_selector->setActive(false);
            option_erase->setActive(false);
            option_use->setActive(false);
            option_back->setActive(false);
            bg_object_received->setActive(false);
            object_received->setActive(false);
            break;
        }
        case Estado::StateType::DIALOG_STATE:{
            dialog_background->setActive(true);
            dialog_icon->setActive(true);
            interact_info->setActive(false);
            bg_object_received->setActive(false);
            object_received->setActive(false);
            break;
        }
        case Estado::StateType::GAME_OVER_STATE:{
            gameover_background->setActive(true);
            restart_button->setActive(true);
            exitmenu_button->setActive(true);
            break;
        }
        case Estado::StateType::OPTIONS_STATE:{
            menu_background->setActive(false);
            options->setActive(true);
            music->setActive(true);
            musiconoff->setActive(true);
            volumebill->setActive(true);
            resolution->setActive(true);
            r_left->setActive(true);
            r_right->setActive(true);
            v_up->setActive(true);
            v_down->setActive(true);
            b_pause->setActive(true);
            if(resolution_text){
                device->setText(resolution_text,"640/480");
                device->setTextPosition(resolution_text,glm::vec3(-0.1,-0.28,0));
            }
            break;
        }
        default:
        break;
    }
}

void AnimaRender::drawPauseButtons(int option){
    if(continue_button && newgame_button && exit_button && options_button){
        switch(option){
            case 0:
                device->setBillboardTexture("../assets/textures/resume2.png", continue_button);
                device->setBillboardTexture("../assets/textures/save1.png", newgame_button);
                device->setBillboardTexture("../assets/textures/options1.png", options_button);
                device->setBillboardTexture("../assets/textures/exit1.png", exit_button);
            break;
            case 1:
                device->setBillboardTexture("../assets/textures/resume1.png", continue_button);
                device->setBillboardTexture("../assets/textures/save2.png", newgame_button);
                device->setBillboardTexture("../assets/textures/options1.png", options_button);
                device->setBillboardTexture("../assets/textures/exit1.png", exit_button);
            break;
            case 2:
                device->setBillboardTexture("../assets/textures/resume1.png", continue_button);
                device->setBillboardTexture("../assets/textures/save1.png", newgame_button);
                device->setBillboardTexture("../assets/textures/options2.png", options_button);
                device->setBillboardTexture("../assets/textures/exit1.png", exit_button);
            break;
            case 3:
                device->setBillboardTexture("../assets/textures/resume1.png", continue_button);
                device->setBillboardTexture("../assets/textures/save1.png", newgame_button);
                device->setBillboardTexture("../assets/textures/options1.png", options_button);
                device->setBillboardTexture("../assets/textures/exit2.png", exit_button);
            break;
            default:
                device->setBillboardTexture("../assets/textures/resume2.png", continue_button);
                device->setBillboardTexture("../assets/textures/save1.png", newgame_button);
                device->setBillboardTexture("../assets/textures/options1.png", options_button);
                device->setBillboardTexture("../assets/textures/exit1.png", exit_button);
            break;
        }
    }
}

void AnimaRender::drawItemOptions(float x, float y, int selector, int item){

    //use button
    option_use->setPosition(glm::vec3(x+0.05,y-0.05,0));
    option_use->setActive(true);

    if(item < GLOBAL_WEAPONS_SIZE){
        option_back->setPosition(glm::vec3(x+0.05,y-0.13,0));

    }else{
        //erase button
        option_erase->setPosition(glm::vec3(x+0.05,y-0.13,0));
        option_erase->setActive(true);

        option_back->setPosition(glm::vec3(x+0.05,y-0.21,0));

    }

    option_back->setActive(true);

    //selector
    float y_;
    switch(selector){
        case 0:
            y_ = y-0.05;
        break;
        case 1:
            y_ = y-0.13;
        break;
        case 2:
            y_ = y-0.21;
        break;
        default:
            y_ = y;
        break;
    }

    option_selector->setPosition(glm::vec3(x+0.05,y_,0));
    option_selector->setActive(true);

}

void AnimaRender::drawEquipedWeapon(int w){
    switch(w){
        case 0:
            device->setBillboardTexture("../assets/textures/espada.png",equiped_weapon);
            equiped_weapon->setActive(true);
            break;
        case 1:
            device->setBillboardTexture("../assets/textures/tirachinas.png",equiped_weapon);
            equiped_weapon->setActive(true);
            break;
        case 2:
            device->setBillboardTexture("../assets/textures/martillo.png",equiped_weapon);
            equiped_weapon->setActive(true);
            break;
        default:
            equiped_weapon->setActive(false);
            break;
    }
}


float AnimaRender::getDeltaTime(){
    return chr_delta;
}

float AnimaRender::getNow(){
    return now;
}

void AnimaRender::updatePlCamera(float c_x, float c_y, float c_z, float pl_x, float pl_y, float pl_z){
    device->setCameraParams(glm::vec3(c_x, c_y, -c_z), glm::vec3(pl_x, pl_y, -pl_z));
}

void AnimaRender::drawTorch(Node& n, float x, float y, float z, int angle){

    n.getNode() = device->createAnimation(glm::vec3(-x,-y,z), 0,"../assets/animations/torch/antorcha", "../assets/textures/antorcha.png",41);

    /*if(angle != 0){
    	float aux = angle*(3.14/180);
    	device->rotateNode(n.getNode(), glm::vec3(1,0,0), aux);
    }*/
    device->playAnimation(n.getNode());
    device->setLoopAnimation(n.getNode(), true);
    device->setAnimationCartoon(n.getNode(), false);
    device->setAnimationActive(n.getNode(), false);
}

void AnimaRender::drawChair(Node& n, float x, float y, float z, int angle){

    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/silla.obj", "../assets/textures/silla.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(1,0,0), aux);
    }
}

void AnimaRender::drawPainting(Node& n, float x, float y, float z, int angle){

    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/cuadro.obj", "../assets/textures/cuadro.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(0,1,0), aux);
    }
}

void AnimaRender::drawChain(Node& n, float x, float y, float z, int angle){
    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/grilletes.obj", "../assets/textures/grilletes.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(0,1,0), aux);
    } 
}

void AnimaRender::drawBed(Node& n, float x, float y, float z, int angle){

    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/cama.obj", "../assets/textures/cama.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(0,1,0), aux);
    }
}

void AnimaRender::drawBox(Node& n, float x, float y, float z, int angle){

    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/caja.obj", "../assets/textures/cajadeco.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(1,0,0), aux);
    }
}

void AnimaRender::drawBarrel(Node& n, float x, float y, float z, int angle){

    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/barril.obj", "../assets/textures/barril.png");

    if(angle != 0){
        float aux = angle*(3.14/180);
        device->rotateNode(n.getNode(), glm::vec3(1,0,0), aux);
    }
}

void AnimaRender::drawPSD(Node& n, float x, float y, float z, int angle){
    n.getNode() = device->createMesh(glm::vec3(-x,-y,z), 0,"../assets/models/pinguS.obj", "../assets/textures/psoldier_sleep1.png");
    device->setMeshCartoon(n.getNode(), false);
}

void AnimaRender::drawTable(Node& n, float x, float y, float z, int angle){
    n.getNode() = device->createMeshLOD(glm::vec3(-x,-y,z), 0,"../assets/models/table", "../assets/textures/mesa");
    if(angle != 0){
    	float aux = angle*(3.14/180);
    	device->rotateNode(n.getNode(), glm::vec3(1,0,0), aux);
    }
}

void AnimaRender::inicializarDungeon(int lvl){
	std::string ints = std::to_string(lvl);
	dungeon_mesh = device->createMesh(glm::vec3(0,0,0), 0,"../assets/levels/level"+ints+".obj", "../assets/textures/TLevel"+ints+".png");
	device->scaleNode(dungeon_mesh, glm::vec3(1,1,-1));
	device->rotateNode(dungeon_mesh, glm::vec3(1,0,0), 1.571);
    device->setMeshCartoon(dungeon_mesh, false);
}

void AnimaRender::resetFrameDelta(){
	timeelapsed=0;
}

float AnimaRender::getTimeElapsed(){
	return timeelapsed;
}

int AnimaRender::sigueActivo(){

	if(device){
		if(device->isActive()){
			now = glfwGetTime();

            chr_start = std::chrono::system_clock::now();

	        frameDeltaTime = (float)(now - then);

            chr_delta = std::chrono::duration_cast<std::chrono::microseconds> (chr_start-chr_then).count();
            chr_delta = chr_delta/1000000.f;
            
	        timeelapsed += frameDeltaTime;
	        then = now;
            chr_then = chr_start;

			return 2;
		}
	}

	return 0;
}

void AnimaRender::insertLifes(int aux){
    for(long unsigned int i=0; i<lifes.size(); i++){
        if((int)i<aux){
            lifes[i]->setActive(true);
        }else{
            lifes[i]->setActive(false);
        }
    }
}

void AnimaRender::setAnimationActive(Node& n, bool f){
    device->setAnimationActive(n.getNode(), f);
}

void AnimaRender::setDebugDrawer(ADebug* debug){
    if(debug!=nullptr)
        device->setDebugDrawer(debug);
}

void AnimaRender::drawObjectReceived(int obj){
    
    if(obj > 0 ){
        bg_object_received->setActive(true);
        switch(obj){
            case 1:
                device->setBillboardTexture("../assets/textures/icecream.png", object_received);
            break;
            case 2:
                device->setBillboardTexture("../assets/textures/fish.png", object_received);
            break;
            case 3:
                device->setBillboardTexture("../assets/textures/meat.png", object_received);
            break;
            case 4:
                device->setBillboardTexture("../assets/textures/berry.png", object_received);
            break;
            case 5:
                device->setBillboardTexture("../assets/textures/key.png", object_received);
            break;
            case 6:
                device->setBillboardTexture("../assets/textures/LevelKey.png", object_received);
            break;
            case 7:
                device->setBillboardTexture("../assets/textures/JailKey.png", object_received);
            break;
            case 8:
                device->setBillboardTexture("../assets/textures/masterKey.png", object_received);
            break;
            case 9:
                device->setBillboardTexture("../assets/textures/sword.png", object_received);
            break;
            case 10:
                device->setBillboardTexture("../assets/textures/slingshot.png", object_received);
            break;
            case 11:
                device->setBillboardTexture("../assets/textures/hammer.png", object_received);
            break;
            default:
                device->setBillboardTexture("../assets/textures/dialogs/IconoDialogo_ReinaWho.png", object_received);
            break;
        }
        object_received->setActive(true);        
    }else{
        bg_object_received->setActive(false);
        object_received->setActive(false);
    }

}

//      TEXTOS

void AnimaRender::showMoney(int d){
    std::string str_Money = "";
    str_Money += std::to_string(d);
    if(money_text == nullptr){
        money_text = device->createText(glm::vec3(-0.02,0.52,0.0), str_Money, 0.0008, glm::vec3(0.0,1.0,0.0));
    }else{
        device->setText(money_text, str_Money);
    }
}

void AnimaRender::cleanDialogText(){
    if(dialog_text0 != nullptr){
        device->setText(dialog_text0, "");
    }
    if(dialog_text1 != nullptr){
        device->setText(dialog_text1, "");
    }
    if(dialog_text2 != nullptr){
        device->setText(dialog_text2, "");
    }
    if(dialog_text3 != nullptr){
        device->setText(dialog_text3, "");
    }
    if(inventory_desc0 != nullptr){
        device->setText(inventory_desc0, "");
    }
    if(inventory_desc1 != nullptr){
        device->setText(inventory_desc1, "");
    }
    if(inventory_desc2 != nullptr){
        device->setText(inventory_desc2, "");
    }
    if(inventory_desc3 != nullptr){
        device->setText(inventory_desc3, "");
    }
    if(money_text != nullptr){
        device->setText(money_text, "");
    }
    if(pl_debug1 != nullptr){
        device->eraseTextEntity(pl_debug1);
        pl_debug1 = nullptr;
    }
    if(pl_debug2 != nullptr){
        device->eraseTextEntity(pl_debug2);
        pl_debug2 = nullptr;
    }
    if(pl_debug3 != nullptr){
        device->eraseTextEntity(pl_debug3);
        pl_debug3 = nullptr;
    }
    if(pl_debug4 != nullptr){
        device->eraseTextEntity(pl_debug4);
        pl_debug4 = nullptr;
    }
    if(pl_debug5 != nullptr){
        device->eraseTextEntity(pl_debug5);
        pl_debug5 = nullptr;
    }
    if(enemy_text1 != nullptr){
        device->eraseTextEntity(enemy_text1);
        enemy_text1 = nullptr;
    }
    if(enemy_text2 != nullptr){
        device->eraseTextEntity(enemy_text2);
        enemy_text2 = nullptr;
    }
    if(enemy_text3 != nullptr){
        device->eraseTextEntity(enemy_text3);
        enemy_text3 = nullptr;
    }
    if(enemy_text4 != nullptr){
        device->eraseTextEntity(enemy_text4);
        enemy_text4 = nullptr;
    }
    if(enemy_text5 != nullptr){
        device->eraseTextEntity(enemy_text5);
        enemy_text5 = nullptr;
    }
}

void AnimaRender::RenderDialog(int a, EntityType tipo){   
    //AÑADO LA IMAGEN DE FONDO
    //guienv->addImage(fondodialog, core::position2d<int>(200, 490)); //70

    //cleanDialogText();

    std::string strname = "";
    //AÑADO LA CARA DE QUIEN HABLA
    switch (tipo) {
        case P_KPONGO_ENTITY: //P_KPONGO
            device->setBillboardTexture("../assets/textures/dialogs/IconoDialogo_KPongo.png", dialog_icon);
            strname += "KING PONGO";
        break;

        case S_NORMAL_ENTITY: //S_NORMAL
            device->setBillboardTexture("../assets/textures/dialogs/IconoDialogo_FocaA.png", dialog_icon);
            strname += "VILLAGER";
        break;

        case S_QUEST_ENTITY: //S_QUEST
            device->setBillboardTexture("../assets/textures/dialogs/IconoDialogo_FocaA.png", dialog_icon);
            strname += "PRISONER";
        break;

        case TRIGGER_DIALOGO: //DIALOGO NORMAL
            device->setBillboardTexture("../assets/textures/dialogs/IconoDialogo_ReinaWho.png", dialog_icon);
            strname += "      ???";
        break;
        case QUEEN_ENTITY:
            device->setBillboardTexture("../assets/textures/dialogs/IconoDialogo_Reina.png", dialog_icon);
            strname += "QUEEN";
        break;
        default:
        break; 
    }

    //CREO EL TEXTO
    std::string str = "";
    std::string str1 = "";
    std::string str2 = "";
    switch (a) {
        
        case 0:
            str += "Eika... Wake up... Eika!";
            str1 += "Are you ok? Can you move?";
            str2 += "There must be a way to get out of this jail."; 
        break;

        case 1:
            str += "You need to hurry up! I'm trapped at";
            str1 += "the top of the castle... King Pongo";
            str2 += "and it's soldiers have conquered Frostyland!"; 
        break;

        case 2:
            str += "";
        	str1 += "You're the only one who can help us!";
        	str2 += "";
        break;

        case 3:
            str += "Oh! Can you see that chest?";
            str1 += "Try to find what's inside using 'P'."; 
            str2 += "I hid the jail's key."; 
        break;

        case 4:
            str += "Let's check what's in your bag using 'I'";
            str1 += "and look what you have found"; 
            str2 += "I suspect that it will be useful"; 
        break;

        case 5:
            str += "Don't forget to check the object's description.";
            str1 += "This could help you to know how to use them."; 
            str2 += "Oh...here the come...see you soon."; 
        break;

        case 6:
            str += "Be careful!";
            str1 += "There is a guard right there!";
            str2 += "Look!";
        break;

        case 7:
            str += "Do you have a weapon?"; 
            str1 += "Hurry!";
            str2 += "Open your bag and equip it.";
        break;

        case 8:
            str += "Are you ready?"; 
            str1 += "Let's go! Use 'O'to attack him.";
            str2 += "";
        break;

        case 9:
            str += "Oh, so you finally came."; 
            str1 += "So what now? Do you really";
            str2 += "think you can stop me?";
        break;

        case 10:
            str += "Get that idea out of your head."; 
            str1 += "I did what I had to, in order";
            str2 += "to save my people! My citizens!";
        break;

        case 11:
            str += "None can nor will stop me!"; 
            str1 += "And I will show you right now!";
            str2 += "Get ready, hollow-headed human!";
        break;

        case 12:
            str += "   CONGRATULATIONS!!"; 
            str1 += "You have completed the demo";
            str2 += "";
        break;

        case 13:
            str += "This is only the beggining. "; 
            str1 += "We hope you enjoyed this mini adventure!";
            str2 += "Get the full game to enjoy more!";
        break;

        case 14:
            str += "Thanks for playing Frostyland!"; 
            str1 += "Follow us for more info on Instagram";
            str2 += "and Twitter: @OvercodedGames";
        break;
        
        case 17:
            str += "Hello? Is anybody there?"; 
            str1 += "Ohh you are not a guard, thanks god!"; 
            str2 += "I need to break out of here."; 
        break;

        case 18:
            str += "They caught me yesterday and"; 
            str1 += "I'm going to be executed soon!"; 
            str2 += "snif snif"; 
        break;

        case 19:
            str += "Please you have to help me!";
            str1 += "My family is waiting for me!"; 
            str2 += "They need me! snif snif";
        break;

        case 20:
            str += "Thank you so much, I owe you!";
            str1 += "You will be always welcome to our hearth"; 
            str2 += "My family will be greatful to see you."; 
        break;

        case 21:
            str += "Eika! Thanks for coming!"; 
            str1 += "You came to help me and I will always"; 
            str2 += "be grateful for what you have done."; 
        break;

        case 22:
            str += "Not only for me, but for all";
            str1 += "the seals that got tortured."; 
            str2 += "None of us deserved that cruel fate.";
        break;

        case 23:
            str += "It might be inappropriate from me";
            str1 += "to ask you this but..."; 
            str2 += "Would you stay by my side from now on?"; 
        break;

        default:
            str += "Hola DEFAULT";
        break;
    }
    
    //AÑADO FINALMENTE ELTEXTO

    if(dialog_text0 == nullptr){
        dialog_text0 = device->createText(glm::vec3(-0.27,-0.65,0.0), str, 0.0008, glm::vec3(0.0,1.0,0.0));
        dialog_text1 = device->createText(glm::vec3(-0.27,-0.7,0.0), str1, 0.0008, glm::vec3(0.0,1.0,0.0));
        dialog_text2 = device->createText(glm::vec3(-0.27,-0.75,0.0), str2, 0.0008, glm::vec3(0.0,1.0,0.0));
        dialog_text3 = device->createText(glm::vec3(-0.15,-0.52,0.0), strname, 0.0008, glm::vec3(0.0,1.0,0.0));
    }else{
        device->setText(dialog_text0, str);
        device->setText(dialog_text1, str1);
        device->setText(dialog_text2, str2);
        device->setText(dialog_text3, strname);
    }

    //device->setTextPosition(dialog_text0, glm::vec3(-0.27,0.65,0.0));
}

void AnimaRender::RenderHistory(int a){

}

void AnimaRender::RenderInventoryDescription(){

    auto* pl = Player::Instance();
    int aux = Inventory::Instance()->getItemSelected();
    std::string str = "";
    std::string str2 = "";
    std::string str4 = "";
    std::string str3 = "EIKA";

    switch (aux) {

        case 0:
            str += "Nothing better than a sword to deal with";
            str2 += "your enemies."; 
            str4 += "(Damage: 2 , Range: 60)";
        break;
        
        case 1:
            str += "There's no bows in Frostylands";
            str2 += "so we have Slingshots.";
            str4 += "(Damage: 1 , Range: 100)";
        break;

        case 2:
            str += "Those hammers can break the strongest Ice."; 
            str2 += "(Damage: 3 , Range: 30)";
        break;

        case 3:
            str += "Ice creams are so good, the also have"; 
            str2 += "lots of flavors.";
            str4 += "(Heal: 3)";
        break;

        case 4:
            str += "The penguin's favorite food.";
            str2 += "(Heal: 5)";
        break;

        case 5:
            str += "Baked penguin meat is to die for.";
            str2 += "(Heal: 2)";
        break;

        case 6:
            str += "Fruits of the forest are so tasty.";
            str2 += "(Heal: 1)";
        break;
        
        case 7:
            str += "This key may open basic doors.";
        break;

        case 8:
            str += "A special key for an important door";
            str2 += "find the star to go to the next level.";
        break;
        
        case 9:
            str += "A strong key to open the cell door.";
        break;

        case 10:
            str += "This key opens the final room.";
            str2 += "Are you ready to combat King Pongo?"; 
        break;

        default:
            str += "There's no selected object";
        break;
    }

    if(inventory_desc0 == nullptr){
        inventory_desc0 = device->createText(glm::vec3(-0.45,-0.51,0.0), str, 0.0008, glm::vec3(0.0,1.0,0.0));
        inventory_desc1 = device->createText(glm::vec3(0.24,0.52,0.0), str3, 0.0008, glm::vec3(0.0,1.0,0.0));
        inventory_desc2 = device->createText(glm::vec3(-0.45,-0.55,0.0), str2, 0.0008, glm::vec3(0.0,1.0,0.0));
        inventory_desc3 = device->createText(glm::vec3(-0.45,-0.59,0.0), str4, 0.0008, glm::vec3(0.0,1.0,0.0));
    }else{
        device->setText(inventory_desc0, str);
        device->setText(inventory_desc1, str3);
        device->setText(inventory_desc2, str2);
        device->setText(inventory_desc3, str4);
    }

    showMoney(pl->getMoney());
}

void AnimaRender::RenderDatosEnemy(int id){

    //METODOS DE COGER DATOS DEL ENEMIGO
    //LLAMAR AL MÁS CERCANO Y PASARLE 
    //CREO EL TEXTO A MOSTRAR
    Enemy* npc = Dungeon::Instance()->getEnemyPerID(id);
    std::string str1 = "PINGUINO :";
    std::string str2 = "Pos(x,y,z): ";
    std::string str3 = "Vel(x,z): ";
    std::string str4 = "Estado : ";
    std::string str5 = "Enemigo del Tipo: ";
    if(npc != nullptr){
        
        str1 += std::to_string(static_cast<int>(id));
        str1 += " , Vidas: ";
        str1 += std::to_string(npc->getLife());

        str2 += std::to_string(static_cast<int>(npc->getX()));
        str2 += " , ";
        str2 += std::to_string(static_cast<int>(npc->getY()));
        str2 += " , ";
        str2 += std::to_string(static_cast<int>(npc->getZ()));

        str3 += std::to_string(static_cast<int>(npc->getVelocityX()));
        str3 += " ,";
        str3 += std::to_string(static_cast<int>(npc->getVelocityZ()));

        if(npc->getType() != P_KPONGO_ENTITY){
            str3 += "  Dir(x,y,z): ";
            str3 += "";
            str3 += std::to_string(static_cast<int>(npc->getVisionVectorX()));
            str3 += " ,";
            str3 += std::to_string(static_cast<int>(npc->getVisionVectorY()));
            str3 += " ,";
            str3 += std::to_string(static_cast<int>(npc->getVisionVectorZ()));
        }

        //SWITCH PARA MIRAR EL ESTADO EN EL QUE SE ENCUENTRA, ARREGLAR
        switch(npc->getState()){
            case 0:
                str4 += "PATRULLANDO";
            break;
            case 1:
                str4 += "PERSIGUIENDO";
            break;
            case 2:
                str4 += "ATACANDO";
            break;
            case 3:
                str4 += "SIGUIENDO";
            break;
            case 4:
                str4 += "AVISANDO";
            break;
            default:
                str4 += "ERROR";
            break;
        }


        switch(npc->getType()){
            case P_SOLDIER_ENTITY:
                str5 += "SOLDADO";
            break;
            case P_GENERAL_ENTITY:
                str5 += "GENERAL";
            break;
            case P_KPONGO_ENTITY:
                str5 += "KING PONGO";
            break;
            case FOX_ENTITY:
                str5 += "ZORRO";
            break;
            default:
                str5 += "ERROR";
            break;
        }

            //PASO LAS COORDENADAS 3D A 2D
        glm::vec2 pos = glm::vec2(device->getScreenCoords(npc->getNode().getNode()));
        pos.x = pos.x;
        pos.y = pos.y;
        glm::vec3 pos3 = glm::vec3(pos.x, pos.y, 0);
            // ACTIVO EL SPRITE SOBRE EL QUE SE PINTA
        device->setBillboardPosition(enemy_selector, pos3);
            //AÑADO FINALMENTE ELTEXTO
        if(enemy_text1 == nullptr){
            enemy_text1 = device->createText(glm::vec3(-0.85,-0.6,0.0), str1, 0.0008, glm::vec3(0.0,1.0,0.0));
            enemy_text2 = device->createText(glm::vec3(-0.85,-0.7,0.0), str2, 0.0008, glm::vec3(0.0,1.0,0.0));
           enemy_text3 = device->createText(glm::vec3(-0.85,-0.8,0.0), str3, 0.0008, glm::vec3(0.0,1.0,0.0));
            enemy_text4 = device->createText(glm::vec3(-0.85,-0.9,0.0), str4, 0.0008, glm::vec3(0.0,1.0,0.0));
            enemy_text5 = device->createText(glm::vec3(-0.85,-0.5,0.0), str5, 0.0008, glm::vec3(0.0,1.0,0.0));
        }else{
            device->setText(enemy_text1, str1);
            device->setText(enemy_text2, str2);
            device->setText(enemy_text3, str3);
            device->setText(enemy_text4, str4);
           device->setText(enemy_text5, str5);
        }
    }
    

    //FALTA SET POSITION DEL ENEMY INFOBG
    npc = nullptr;
}

void AnimaRender::setdebugEnemies(bool sn){
    enemy_infobg->setActive(sn);
    enemy_selector->setActive(sn);
    if(sn == false && enemy_text1 != nullptr){
        cleanDialogText();
    }
}

void AnimaRender::clear(){

}

void AnimaRender::clearNode(Node& n){
    device->eraseNode(n.getNode());
}

void AnimaRender::clearScene(){

}

void AnimaRender::RenderDatosPlayer(){

    auto* pl = Player::Instance();
    //CREO EL TEXTO A MOSTRAR
    std::string str1 = "DEBUG VISUAL DE EIKA --- Vidas: ";
    std::string str2 = "Pos(x,y,z): ";
    std::string str3 = "Velocidad: ";
    std::string str4 = "Arma equipada: ";
    std::string str5 = "Sala actual: ";

    Wearable* w = pl->getWearable();
    // PLAYER -- POSICION -- VECTOR DIRECTOR -- VELOCIDAD //ARMA -- TIPO -- DAÑO -- RANGO -- ANGULO
    str1 += std::to_string(pl->getLifes());
    str2 += std::to_string(static_cast<int>(pl->getX()));
    str2 += " , ";
    str2 += std::to_string(static_cast<int>(pl->getY()));
    str2 += " , ";
    str2 += std::to_string(static_cast<int>(pl->getZ()));

    str3 += std::to_string(static_cast<int>(pl->getVelocity()));
    str3 += "  Direccion(x,y,z): ";
    str3 += "";
    str3 += std::to_string(static_cast<int>(pl->getVisionVectorX()));
    str3 += " ,";
    str3 += std::to_string(static_cast<int>(pl->getVisionVectorY()));
    str3 += " ,";
    str3 += std::to_string(static_cast<int>(pl->getVisionVectorZ()));

    if(w != nullptr){
        if(static_cast<int>(w->getType()) == 25){
            str4 += "Espada";
        }
        if(w->getType() == 35){
            str4 += "Tirachinas";
        }
        if(static_cast<int>(w->getType()) == 45){
            str4 += "Martillo";
        }
    }else{
        str4 += "  ---  ";
    }

    str5 += std::to_string(pl->getSalaActual());
    str5 += "  FPS: ";
    str5 += std::to_string(Dungeon::Instance()->getFPS());
    //AÑADO FINALMENTE ELTEXTO

    if(pl_debug1 == nullptr){
        pl_debug1 = device->createText(glm::vec3(0.1,-0.5,0.0), str1, 0.0008, glm::vec3(0.0,1.0,0.0));
        pl_debug2 = device->createText(glm::vec3(0.1,-0.6,0.0), str2, 0.0008, glm::vec3(0.0,1.0,0.0));
        pl_debug3 = device->createText(glm::vec3(0.1,-0.7,0.0), str3, 0.0008, glm::vec3(0.0,1.0,0.0));
        pl_debug4 = device->createText(glm::vec3(0.1,-0.8,0.0), str4, 0.0008, glm::vec3(0.0,1.0,0.0));
        pl_debug5 = device->createText(glm::vec3(0.1,-0.9,0.0), str5, 0.0008, glm::vec3(0.0,1.0,0.0));
    }else{
        device->setText(pl_debug1, str1);
        device->setText(pl_debug2, str2);
        device->setText(pl_debug3, str3);
        device->setText(pl_debug4, str4);
        device->setText(pl_debug5, str5);
    }

}

void AnimaRender::setdebugIA(bool sn){
    bg_eika_debug->setActive(sn);
    if(sn == false){
        enemy_infobg->setActive(false);
    }
    cleanDialogText();
}

bool AnimaRender::isKeyDown(char key){

	bool down = false;

    for(int i=0; i<14 && !down; i++){
        if(myMap[i].c == key){
            if(glfwGetKey(device->getWindow(), myMap[i].keynum) == GLFW_PRESS){
                down = true;
            }
        }
    }
    
    return down;
}

bool AnimaRender::isKeyReleased(char key){
    return !isKeyDown(key);
}

int  AnimaRender::isJoystickPresent(){
    return glfwJoystickPresent( GLFW_JOYSTICK_1 );
}

void AnimaRender::initJoystick(){
    if(isJoystickPresent()==1){
        axes = glfwGetJoystickAxes( GLFW_JOYSTICK_1 , &axesCount);
        buttons = glfwGetJoystickButtons( GLFW_JOYSTICK_1, &buttonCount );
    }
}

void AnimaRender::checkJoystickAxes(float& x,float& y){
    x = axes[0];
    y = axes[1];
}

bool AnimaRender::onButtonPressed(char button){
    bool pressed = false;

    if(button == 'A'){
        if(GLFW_PRESS==buttons[0]){
            pressed = true;
        }
    }
    if(button == 'B'){
        if(GLFW_PRESS==buttons[1]){
            pressed = true;
        }
    }
    if(button == 'X'){
        if(GLFW_PRESS==buttons[2]){
            pressed = true;
        }
    }
    if(button == 'Y'){
        if(GLFW_PRESS==buttons[3]){
            pressed = true;
        }
    }
    if(button == '-'){
        if(GLFW_PRESS==buttons[7]){
            pressed = true;
        }
    }
    return pressed;
}

void AnimaRender::drawInfoInteract(bool a){
    interact_info->setActive(a);

}
void AnimaRender::drawInfoControl(int a){
    
    if(a==-1){
        //desactive
        control_info->setActive(false);

    }else if(a==0){
    //Chest
        control_info->setActive(true);
        device->setBillboardTexture("../assets/textures/abrir_cofre.png", control_info);

    }else if(a==1){
    //Dragbox
        control_info->setActive(true);
        device->setBillboardTexture("../assets/textures/empujar.png", control_info);

    }else if(a==2){
    //Door
        control_info->setActive(true);
        device->setBillboardTexture("../assets/textures/abrir.png", control_info);


    }else if(a==3){
    //Talk
        control_info->setActive(true);
        device->setBillboardTexture("../assets/textures/hablar.png", control_info);


    }


}

void AnimaRender::playAnimation(Node& n){
    device->playAnimation(n.getNode());
}

void AnimaRender::setOpenedChest(Node& n){
    device->setActFrame(n.getNode(), 50);
}

void AnimaRender::setLODdecoration(Node& n, bool sn){
    device->setActiveLOD(n.getNode(), sn);
}

void AnimaRender::createParticleSystem(std::string shape, float px, float py, float pz, float nRadius, int nBirthrate, float nParticleLife, float dx, float dy, float dz, float nBirthSize, float r, float g, float b, float a){
    device->createParticleSystem(shape, px, py, pz, nRadius, nBirthrate, nParticleLife, dx, dy, dz, nBirthSize, r, g, b, a);
}

void AnimaRender::playEatingEika(){
    auto* p = Player::Instance();

    float y = 0;
    float x = 0;
    if(p->getCamera()->getY() > 40){
        y = -p->getCamera()->getY()+105;
        x = -p->getCamera()->getX()-65;
    }else{
        y = -p->getCamera()->getY()+63;
        x = -p->getCamera()->getX()-57;
    }

    glm::vec3 position = glm::vec3(x,y,p->getCamera()->getZ()+100);
    device->changeAnimation(eika3d,"../assets/animations/eika/eikacomer/eikacomer", "../assets/textures/eika-textura2.png",121);
    device->translateNode(eika3d,position);
    //device->translateNode(eika3d,glm::vec3(-p->getX()-40,-p->getY()+20,p->getZ()));
    device->playAnimation(eika3d);
}

void AnimaRender::playEquipEika(){
    auto* p = Player::Instance();

    float y = 0;
    float x = 0;
    if(p->getCamera()->getY() > 40){
        y = -p->getCamera()->getY()+112;
        x = -p->getCamera()->getX()-60;
    }else{
        y = -p->getCamera()->getY()+70;
        x = -p->getCamera()->getX()-50;
    }

    glm::vec3 position = glm::vec3(x,y,p->getCamera()->getZ()+95);
    device->changeAnimation(eika3d,"../assets/animations/eika/eikaequip/eikaequip", "../assets/textures/eika-textura2.png",123);
    device->translateNode(eika3d,position);
    //device->translateNode(eika3d,glm::vec3(-p->getX()-40,-p->getY()+20,p->getZ()));
    device->playAnimation(eika3d);
}