// AeroCrunch functions
#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Aer_CreateSprite(size_t id, C2D_SpriteSheet sheet, size_t sprite_id, float x, float y);
void Aer_SpriteCenter(size_t id, float center_x, float center_y);
void Aer_DrawSprite(size_t id);
void Aer_PrepareScene();
void Aer_RenderScreen(C3D_RenderTarget* screen);
void Aer_DefineLibs();
void Aer_SpritePosition(size_t id, float x, float y);
void Aer_ChangeSprite(size_t id, C2D_SpriteSheet sheet, size_t sprite_id);
void Aer_SpriteDPadMove(size_t id, size_t speed);
void Aer_SpriteCollisionBox(size_t id, float box_x, float box_y);
bool Aer_SpriteTouched(size_t id, size_t id2);
bool Aer_SpriteTouchPlace(size_t id, float second_x, float second_y, float second_width, float second_height);
void Aer_SpriteScale(size_t id, float scale_x, float scale_y);
void Aer_SpriteRotate(size_t id, float degrees);
void Aer_SpritePlaceRestore(size_t id);
bool Aer_BreakCheck();
void Aer_SpriteAnimate(size_t id, C2D_SpriteSheet sheet, int start_frame, int max_frame, int speed);
bool Aer_DPadMove();
void Aer_MoveCamera(float x, float y);
void Aer_SetCamera(float x, float y);
void Aer_CameraFollow(size_t id);
float Aer_spr_x(size_t id);
float Aer_spr_y(size_t id);
void Aer_SpriteConnects(size_t id, size_t id2, float x, float y);
bool Aer_KeyHeld(u32 key);
bool Aer_KeyRelesed(u32 key);
void Aer_SolidSpaceChecker(float second_x, float second_y, float second_width, float second_height);
void Aer_SpriteSmartPlaceRestore(size_t id, float second_x, float second_y, float second_width, float second_height);
void Aer_RenderDistaneOptimize(bool mode, int dist);
void Aer_CameraShiftCenter(float x, float y);
void Aer_InitIndicator(size_t id, int max_indicator_varible, C2D_SpriteSheet sheet);
void Aer_DrawIndicator(size_t id, int varible, int max_indicator_varible, C2D_SpriteSheet sheet, int empty_indicator_part_id, int full_indicator_part_id,  float x, float y, float distanse);
void Aer_MoveTimer(size_t id, int ticktime);
bool Aer_TimerDone(size_t id);
void Aer_ResetTimer(size_t id);
bool Aer_RoomLoop();