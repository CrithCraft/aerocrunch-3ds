#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <rendering/AeroCrunch.h>

#define WIDTH       400
#define HEIGHT      200

int weapon_rotate = 0;
int weapon_speed = 4;
int max_weapons = 6;
int max_enemy = 20;
int player_hearts = 3;

bool firstTurn = true;

// Update top graphics
void Draw_001_top(C2D_SpriteSheet spriteSheet_players, C2D_SpriteSheet spriteSheet_map){
    // Setup graphic
    if (firstTurn == true){
        srand(time(NULL));

        // Map sprite
        Aer_CreateSprite(1, spriteSheet_map, 0, 0, 0);

        // Weapon sprite
        Aer_CreateSprite(2, spriteSheet_players, 10, -100, -100);
        Aer_SpriteCenter(2, 0, 1);
        Aer_SpriteScale(2, 2, 2);

        // Player sprite
        Aer_CreateSprite(3, spriteSheet_players, 0, 70, 70);
        Aer_SpriteCollisionBox(3,32,56);

        // Generate weapons
        for(int i=4; i<=4+max_weapons-1; i++){
          Aer_CreateSprite(i, spriteSheet_players, 7+(i-3), (((rand()%6)+1)*32*4)-32+12, (((rand()%6)+1)*32*4)-32+4);
          Aer_SpriteCollisionBox(i,16,28);
        }

        // Generate enemy
        for(int i=10; i<=10+max_enemy; i++){
          Aer_CreateSprite(i, spriteSheet_players, 14, (((rand()%6)+1)*32*4)-32-12, (((rand()%6)+1)*32*4)-32-12);
          Aer_SpriteScale(i, 2, 2);
          Aer_SpriteCollisionBox(i,32, 40);
        }

        // Initilize indicator
        Aer_InitIndicator(31, 5, spriteSheet_players);

        firstTurn = false;
    }

    // Write event and render graphics

    // Set camera for follow to player
    Aer_CameraFollow(3);
    Aer_CameraShiftCenter(-32/2, -56/2);

    // Set move player with DPad
    Aer_SpriteDPadMove(3, 1);

    // We clay to sprites
    Aer_SpriteConnects(2,3,15,35);

    // Set animation for player weapon
    if(Aer_KeyHeld(KEY_A)){
        if(weapon_rotate >= 90)
            weapon_rotate = 0;
        else
            weapon_rotate=weapon_rotate+weapon_speed;
    }
    else{
        if (weapon_rotate > 0)
          weapon_rotate=weapon_rotate-weapon_speed;
        else
          weapon_rotate = 0;
    }
    // Set rotate from animation cycle
    Aer_SpriteRotate(2, weapon_rotate);

    // Set player animation for walk and stand
    if(Aer_DPadMove())
        Aer_SpriteAnimate(3, spriteSheet_players, 4, 4, 5);
    else
        Aer_SpriteAnimate(3, spriteSheet_players, 0, 4, 10);

    Aer_MoveTimer(0, 100);
    if(Aer_TimerDone(0)){
        Aer_MoveTimer(1,100);
        if(Aer_TimerDone(1)){
            Aer_MoveTimer(2,100);
            if(Aer_TimerDone(2)){
                Aer_MoveTimer(3,100);
                if(Aer_TimerDone(3)){
                    for(int i=0; i <= 3; i++)
                        Aer_ResetTimer(i); 
                }
            }
        }
    }


    // Set animation and check touch with player
    for(int i = 10; i<=10+max_enemy; i++){
        Aer_SpriteAnimate(i, spriteSheet_players, 14, 4, 6);
        
        // Restore enemy position if player touched it
        if(Aer_SpriteTouched(3,i)){
            Aer_SpriteSmartPlaceRestore(i, Aer_spr_x(3), Aer_spr_y(3), 32, 56);
            // Restore player position if he move into enemy 
            Aer_SpriteSmartPlaceRestore(3, Aer_spr_x(i), Aer_spr_y(i), 32, 40);
        }

        if(!(Aer_TimerDone(0)))
            Aer_SpritePosition(i, Aer_spr_x(i)+1, Aer_spr_y(i));
        else if(!(Aer_TimerDone(1)))
            Aer_SpritePosition(i, Aer_spr_x(i), Aer_spr_y(i)+1);
        else if(!(Aer_TimerDone(2)))
            Aer_SpritePosition(i, Aer_spr_x(i)-1, Aer_spr_y(i));
        else if(!(Aer_TimerDone(3)))
            Aer_SpritePosition(i, Aer_spr_x(i), Aer_spr_y(i)-1);
        else{
            for(int i=0; i <= 3; i++)
                Aer_ResetTimer(i);  
        }
    }

    // Create invisible collider solid boxes
    for(int i = 0; i < 3; i++){
        Aer_SolidSpaceChecker(32+(i*32*4), 32+(i*32*4), 32, 1024-64-2*(i*32*4));
        Aer_SolidSpaceChecker(32+(i*32*4), 32+(i*32*4), 1024-64-2*(i*32*4), 32);
        Aer_SolidSpaceChecker(1024-32-i*32*4-32, 32+(i*32*4), 32, 1024-64-2*(i*32*4));
        Aer_SolidSpaceChecker(32+(i*32*4), 1024-32-i*32*4-32, 1024-64-2*(i*32*4), 32);
    }

    // Check player contact with new player and swap it
    for(int i = 4; i<=4+max_weapons-1; i++){
        if((Aer_SpriteTouched(3, i)) && (Aer_KeyHeld(KEY_B)))
            Aer_ChangeSprite(2, spriteSheet_players, 7+(i-3));
    }

    // Draw map
    Aer_DrawSprite(1);

    // Draw weapons  and enemies with render distanse optimization
    Aer_RenderDistaneOptimize(true, 64);
    for(int i = 4; i<=4+max_weapons-1; i++)
        Aer_DrawSprite(i);
    for(int i = 10; i<=10+max_enemy-1; i++)
        Aer_DrawSprite(i);
    Aer_RenderDistaneOptimize(false, 64);

    // Draw player and his weapon
    for(int i = 2; i<=3; i++)
        Aer_DrawSprite(i);

    // Draw hearts indicator
    Aer_DrawIndicator(31, player_hearts, 5, spriteSheet_players, 18, 19, Aer_spr_x(3)-WIDTH/2+40, Aer_spr_y(3)-HEIGHT/2+24, 16);
}

// Update bottom graphics
void Draw_001_bottom(C2D_SpriteSheet spriteSheet_players){
    // Later
}
