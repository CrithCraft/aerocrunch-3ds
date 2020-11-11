#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <scenes/map_001.h>

void LoadMap(int map_id, C2D_SpriteSheet spriteSheet_players, C2D_SpriteSheet spriteSheet_map, int screen_id){
    // Top screen
    if (screen_id == 0){
        if (map_id == 0)  Draw_001_top(spriteSheet_players, spriteSheet_map);
    }
    if (screen_id == 1){
         if (map_id == 1)  Draw_001_bottom(spriteSheet_players, spriteSheet_map);
    }
}
