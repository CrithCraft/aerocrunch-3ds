// AtomUSSR by CrithCraft Apr. 2019 (C) remake for 3ds
#include <citro2d.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <rendering/AeroCrunch.h>
#include <rendering/scenes_connect.h>

#define WIDTH    400
#define HEIGHT   240

int SaveScene_ID = 0;

// SpriteSheet Initilize
static C2D_SpriteSheet spriteSheet_players;
static C2D_SpriteSheet spriteSheet_map;

int main(int argc, char* argv[]) {

		// Prepare Scene
		Aer_PrepareScene();

		// Set upper and bottom screen to Graphics
		C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
		C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
		

		// Set spritesheet
		spriteSheet_players = C2D_SpriteSheetLoad("romfs:/gfx/sprites_players.t3x");
		if (!spriteSheet_players) svcBreak(USERBREAK_PANIC);
		spriteSheet_map = C2D_SpriteSheetLoad("romfs:/gfx/map.t3x");
		if (!spriteSheet_map) svcBreak(USERBREAK_PANIC);

		while (aptMainLoop())
		{
				if (Aer_BreakCheck())
						break; // break in order to return to hbmenu



				// Begin render frame the scene
				C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

				// Draw top screen
				Aer_RenderScreen(top);
				LoadMap(SaveScene_ID, spriteSheet_players, spriteSheet_map, 0);

				// Draw top screen
				Aer_RenderScreen(bottom);
				LoadMap(SaveScene_ID, spriteSheet_players, spriteSheet_map, 1);

				// End render frame the scene
				C3D_FrameEnd(0);
		}

		// Clearing after closing
		C2D_SpriteSheetFree(spriteSheet_players);
		Aer_DefineLibs();
		return 0;
}
