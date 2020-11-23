// AeroCrunch - graphic engine by CrithCraft
#include <rendering/AeroCrunch.h>

#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPRITES   999
#define WIDTH       400
#define HEIGHT      240

typedef struct {
	C2D_Sprite spr;
	float x,y,speed, col_box_x, col_box_y, prev_x, prev_y, animation, scale_x, scale_y, center_x, center_y, rotate;
} Sprite;

float camera_x = 0;
float camera_y = 0;

int sprite_id_arr[MAX_SPRITES];
int sprites_used = 0;
bool RenderDistanceOptimize = false;
int distanse = 0;

static Sprite sprites[MAX_SPRITES];

int timer[99];
bool timerdone[99];


void Aer_CreateSprite(size_t id, C2D_SpriteSheet sheet, size_t sprite_id, float x, float y){
	Sprite* sprite = &sprites[id];
	sprite->x = x;
	sprite->y = y;
	sprite->col_box_x = 0;
	sprite->col_box_y = 0;
	sprite->prev_x = x;
	sprite->prev_y = y;
	sprite->animation = 0;
	sprite->scale_x = 1;
	sprite->scale_y = 1;
	sprite->center_x = 0;
	sprite->center_y = 0;
	sprite->rotate = 0;
	sprites_used++;
	sprite_id_arr[sprites_used] = id;
	C2D_SpriteFromSheet(&sprite->spr, sheet, sprite_id);
	C2D_SpriteSetCenter(&sprite->spr, sprite->center_x, sprite->center_x);
	C2D_SpriteSetPos(&sprite->spr, sprite->x, sprite->y);
}

void Aer_SpriteCenter(size_t id, float center_x, float center_y){
	Sprite* sprite = &sprites[id];
	sprite->center_x = center_x;
	sprite->center_y = center_y;
	C2D_SpriteSetCenter(&sprite->spr, center_x, center_y);
}

void Aer_DrawSprite(size_t id){
	if(RenderDistanceOptimize == true){
			if(Aer_SpriteTouchPlace(id, camera_x-distanse, camera_y-distanse, WIDTH+2*distanse, HEIGHT+2*distanse)){
				Sprite* sprite = &sprites[id];
				C2D_SpriteSetPos(&sprite->spr, sprite->x-camera_x, sprite->y-camera_y);
				C2D_DrawSprite(&sprite->spr);
			}
	}
	else{
		Sprite* sprite = &sprites[id];
		C2D_SpriteSetPos(&sprite->spr, sprite->x-camera_x, sprite->y-camera_y);
		C2D_DrawSprite(&sprite->spr);
	}
}

void Aer_PrepareScene(){
	// Init libs
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
}

void Aer_RenderScreen(C3D_RenderTarget* screen){
	C2D_TargetClear(screen, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	C2D_SceneBegin(screen);
}

void Aer_DefineLibs(){
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
}

void Aer_SpritePosition(size_t id, float x, float y){
	Sprite* sprite = &sprites[id];
	sprite->prev_x = sprite->x;
	sprite->prev_y = sprite->y;
	sprite->x = x;
	sprite->y = y;
	C2D_SpriteSetPos(&sprite->spr, sprite->x, sprite->y);
}

void Aer_ChangeSprite(size_t id, C2D_SpriteSheet sheet, size_t sprite_id){
	Sprite* sprite = &sprites[id];
	C2D_SpriteFromSheet(&sprite->spr, sheet, sprite_id);
	C2D_SpriteSetPos(&sprite->spr, sprite->x, sprite->y);
	Aer_SpriteScale(id, sprite->scale_x, sprite->scale_y);
	Aer_SpriteCenter(id, sprite->center_x, sprite->center_y);
	Aer_SpriteRotate(id, sprite->rotate);
}

void Aer_SpriteDPadMove(size_t id, size_t speed){
	Sprite* sprite = &sprites[id];
	hidScanInput();
	sprite->prev_x = sprite->x;
	sprite->prev_y = sprite->y;
	if(hidKeysHeld() & KEY_RIGHT)
		sprite->x = sprite->x + speed;
	if(hidKeysHeld() & KEY_LEFT)
		sprite->x = sprite->x - speed;
	if(hidKeysHeld() & KEY_UP)
		sprite->y = sprite->y - speed;
	if(hidKeysHeld() & KEY_DOWN)
		sprite->y = sprite->y + speed;
	C2D_SpriteSetPos(&sprite->spr, sprite->x, sprite->y);
}

void Aer_SpriteCollisionBox(size_t id, float box_x, float box_y){
	Sprite* sprite = &sprites[id];
	sprite->col_box_x = box_x;
	sprite->col_box_y = box_y;
}

bool Aer_SpriteTouched(size_t id, size_t id2){
	Sprite* sprite = &sprites[id];
	float first_x = sprite->x;
	float first_y = sprite->y;
	float width = sprite->col_box_x;
	float height = sprite->col_box_y;

	Sprite* sprite2 = &sprites[id2];
	float second_x = sprite2->x;
	float second_y = sprite2->y;
	float second_width = sprite2->col_box_x;
	float second_height = sprite2->col_box_y;

	if ((first_x + width >= second_x) && (first_x <= second_x + second_width) 
	&& (first_y + height >= second_y) && (first_y <= second_y + second_height))
		return true;
	
	return false;
}

bool Aer_SpriteTouchPlace(size_t id, float second_x, float second_y, float second_width, float second_height){
	Sprite* sprite = &sprites[id];
	if ((sprite->x + sprite->col_box_x >= second_x) && (sprite->x <= second_x + second_width) 
	&& (sprite->y + sprite->col_box_y >= second_y) && (sprite->y <= second_y + second_height))
		return true;
	
	return false;
}

void Aer_SpriteScale(size_t id, float scale_x, float scale_y){
	Sprite* sprite = &sprites[id];
	sprite->scale_x = scale_x;
	sprite->scale_y = scale_y;
	C2D_SpriteSetScale(&sprite->spr, scale_x, scale_y);
}

void Aer_SpriteRotate(size_t id, float degrees){
	Sprite* sprite = &sprites[id];
	C2D_SpriteSetRotation(&sprite->spr, C3D_AngleFromDegrees(degrees));
}

void Aer_SpritePlaceRestore(size_t id){
	Sprite* sprite = &sprites[id];
	sprite->x = sprite->prev_x;
	sprite->y = sprite->prev_y;
	C2D_SpriteSetPos(&sprite->spr, sprite->x, sprite->y);
}

bool Aer_BreakCheck(){
	hidScanInput();
	if(hidKeysHeld() & KEY_START) return true;
	
	return false;
}

void Aer_SpriteAnimate(size_t id, C2D_SpriteSheet sheet, int start_frame, int max_frame, int speed){
	Sprite* sprite = &sprites[id];
	if(sprite->animation < max_frame*speed){
		if(fmod(sprite->animation, speed) == 0){}
				Aer_ChangeSprite(id, sheet, start_frame+sprite->animation/speed);
		sprite->animation = sprite->animation + 1;
	} 
	else sprite->animation = 0;
}

bool Aer_DPadMove(){
	hidScanInput();
	if((hidKeysHeld() & KEY_LEFT) || (hidKeysHeld() & KEY_RIGHT) 
	|| (hidKeysHeld() & KEY_UP) || (hidKeysHeld() & KEY_DOWN))
		return true;
	else
		return false;
}

void Aer_MoveCamera(float x, float y){
	camera_x = camera_x + x;
	camera_y = camera_y + y;
}

void Aer_SetCamera(float x, float y){
	camera_x = x;
	camera_y = y;
}

void Aer_CameraFollow(size_t id){
	Sprite* sprite = &sprites[id];
	Aer_SetCamera(sprite->x-WIDTH/2, sprite->y-HEIGHT/2);
}

float Aer_spr_x(size_t id){
	Sprite* sprite = &sprites[id];
	return sprite->x;
}

float Aer_spr_y(size_t id){
	Sprite* sprite = &sprites[id];
	return sprite->y;
}

void Aer_SpriteConnects(size_t id, size_t id2, float x, float y){
	Sprite* sprite = &sprites[id2];
	Aer_SpritePosition(id, sprite->x+x, sprite->y+y);
}

bool Aer_KeyHeld(u32 key){
	hidScanInput();
	if(hidKeysHeld() & key)
		return true;
	else
		return false;
}

bool Aer_KeyRelesed(u32 key){
	hidScanInput();
	if(hidKeysHeld() & key)
		return false;
	else
		return false;
}

void Aer_SolidSpaceChecker(float second_x, float second_y, float second_width, float second_height){
	for(int i=1; i<=sprites_used; i++){
		Sprite* sprite = &sprites[sprite_id_arr[i]];
		if(!((sprite->col_box_x == 0)&&(sprite->col_box_y == 0))){
			if((sprite->x + sprite->col_box_x >= second_x) && (sprite->x <= second_x + second_width) 
			&& (sprite->y + sprite->col_box_y >= second_y) && (sprite->y <= second_y + second_height)){
				if(!((sprite->prev_x + sprite->col_box_x >= second_x) && (sprite->prev_x <= second_x + second_width) 
				&& (sprite->y + sprite->col_box_y >= second_y) && (sprite->y <= second_y + second_height))){
					sprite->x = sprite->prev_x;
				}
				else if(!((sprite->x + sprite->col_box_x >= second_x) && (sprite->x <= second_x + second_width) 
				&& (sprite->prev_y + sprite->col_box_y >= second_y) && (sprite->prev_y <= second_y + second_height))){
					sprite->y = sprite->prev_y;
				}
				else{
					sprite->x = sprite->prev_x;
					sprite->y = sprite->prev_y;
				}
			}
		}
	}
}

void Aer_SpriteSmartPlaceRestore(size_t id, float second_x, float second_y, float second_width, float second_height){
	Sprite* sprite = &sprites[id];
	if((sprite->x + sprite->col_box_x >= second_x) && (sprite->x <= second_x + second_width) 
	&& (sprite->y + sprite->col_box_y >= second_y) && (sprite->y <= second_y + second_height)){
		if(!((sprite->prev_x + sprite->col_box_x >= second_x) && (sprite->prev_x <= second_x + second_width) 
		&& (sprite->y + sprite->col_box_y >= second_y) && (sprite->y <= second_y + second_height))){
			sprite->x = sprite->prev_x;
		}
		else if(!((sprite->x + sprite->col_box_x >= second_x) && (sprite->x <= second_x + second_width) 
		&& (sprite->prev_y + sprite->col_box_y >= second_y) && (sprite->prev_y <= second_y + second_height))){
			sprite->y = sprite->prev_y;
		}
		else{
			sprite->x = sprite->prev_x;
			sprite->y = sprite->prev_y;
		}
	}
}

void Aer_RenderDistaneOptimize(bool mode, int dist){
	RenderDistanceOptimize = mode;
	distanse = dist;
}

void Aer_CameraShiftCenter(float x, float y){
	camera_x = camera_x - x;
	camera_y = camera_y - y;
}

void Aer_InitIndicator(size_t id, int max_indicator_varible, C2D_SpriteSheet sheet){
	for(int i = 0; i < max_indicator_varible; i++) Aer_CreateSprite(id+i, sheet, 0, 0, 0);
}

void Aer_DrawIndicator(size_t id, int varible, int max_indicator_varible, C2D_SpriteSheet sheet, int empty_indicator_part_id, int full_indicator_part_id,  float x, float y, float distanse){
	for(int i = 0; i < varible; i++){
		Aer_SpritePosition(id+i, x+(distanse*i), y);
		Aer_ChangeSprite(id+i, sheet, full_indicator_part_id);
		Aer_DrawSprite(id+i);
	}

	for(int i = varible; i < max_indicator_varible; i++){
		Aer_SpritePosition(id+i, x+(distanse*i), y);
		Aer_ChangeSprite(id+i, sheet, empty_indicator_part_id);
		Aer_DrawSprite(id+i);
	}
}

void Aer_MoveTimer(size_t id, int ticktime){
	if(timer[id] <= ticktime)
		timer[id]++;
	else
		timerdone[id] = true;
}

bool Aer_TimerDone(size_t id){
	if(timerdone[id] == true)
		return true;
	else
		return false;
}

void Aer_ResetTimer(size_t id){
	timer[id] = 0;
	timerdone[id] = false;
}

bool Aer_RoomLoop() {
	return aptMainLoop();
	// Checking room teleport
}