#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <rendering/AeroCrunch.h>

#define WIDTH       400
#define HEIGHT      200

// SpriteSheet Initilize
static C2D_SpriteSheet spriteSheet_players;

void Setup_002() {
    srand(time(NULL));

    // Weapon sprite
    Aer_CreateSprite(2, spriteSheet_players, 10, -100, -100);
    Aer_SpriteCenter(2, 0, 1);
    Aer_SpriteScale(2, 2, 2);

    // Player sprite
    Aer_CreateSprite(3, spriteSheet_players, 0, 70, 70);
    Aer_SpriteCollisionBox(3,32,56);
}

// Update top graphics
void Draw_002_top(){
    // Set camera for follow to player
    Aer_CameraFollow(3);
    Aer_CameraShiftCenter(-32/2, -56/2);

    // Set move player with DPad
    Aer_SpriteDPadMove(3, 2);

    // We clay to sprites
    Aer_SpriteConnects(2,3,15,35);

    if(Aer_KeyHeld(KEY_X)){
        // Aer_NextRoom();
    }

    // Set player animation for walk and stand
    if(Aer_DPadMove())
        Aer_SpriteAnimate(3, spriteSheet_players, 4, 4, 5);
    else
        Aer_SpriteAnimate(3, spriteSheet_players, 0, 4, 10);

    // Draw player and his weapon
    for(int i = 2; i<=3; i++)
        Aer_DrawSprite(i);
}

// Update bottom graphics
void Draw_002_bottom(){
    // Later
}

void Load_002(C3D_RenderTarget* top, C3D_RenderTarget* bottom){
    // Set spritesheet
    spriteSheet_players = C2D_SpriteSheetLoad("romfs:/gfx/sprites_players.t3x");
    if (!spriteSheet_players) svcBreak(USERBREAK_PANIC);

    Setup_002();

    while(Aer_RoomLoop()) {
        // Begin render frame the scene
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // Draw top screen
        Aer_RenderScreen(top);
        Draw_002_top();

        // Draw top screen
        Aer_RenderScreen(bottom);
        Draw_002_bottom();

        // End render frame the scene
        C3D_FrameEnd(0);

        // if (Aer_BreakCheck()) break;
    }
    C2D_SpriteSheetFree(spriteSheet_players);
}