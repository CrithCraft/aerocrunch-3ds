// AtomUSSR by CrithCraft Apr. 2019 remake for 3ds
#include <citro2d.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <rendering/AeroCrunch.h>
#include <scenes/map_001.h>
#include <scenes/map_002.h>

#define WIDTH    400
#define HEIGHT   240

int main(int argc, char* argv[]) {

		// Prepare Scene
		Aer_PrepareScene();

		// Set upper and bottom screen to Graphics
		C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
		C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
		
		while (aptMainLoop())
		{
			Load_001(top, bottom);
			Load_002(top, bottom);
			if (Aer_BreakCheck()) break; // break in order to return to hbmenu
			
		}

		// Clearing after closing
		Aer_DefineLibs();
		return 0;
}
