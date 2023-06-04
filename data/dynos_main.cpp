#include "dynos.cpp.h"
#include "game/camera.h"
extern "C" {
#include "PR/os_cont.h"
#include "types.h"
#include "sm64.h"
#include "level_commands.h"
#include "game/level_update.h"
#include "game/options_menu.h"
#include "game/object_list_processor.h"
extern s16 gMenuMode;
extern s8 gDialogBoxState;
#ifdef OMM_DEFINES_H
extern void omm_opt_init();
#endif

#include "game/game_init.h"
}

//
// Position saving
//

Vec3f PrevPos = { 0, 0, 0 };
Vec3f PrevVel = { 0, 0, 0 };
Vec3s PrevFaceAngle = { 0, 0, 0 };
Vec3s PrevAngleVel = { 0, 0, 0 };
f32 PrevForwardVel = 0;
f32 PrevSlideVelX = 0;
f32 PrevSlideVelZ = 0;
u32 PrevAction = 0;
u32 PrevPrevAction = 0;
u16 PrevActionState = 0;
u16 PrevActionTimer = 0;
u32 PrevActionArg = 0;
s16 PrevHealth = 0;
u8 PrevHurtCounter = 0;
u8 PrevHealCounter = 0;
s16 PrevInvincTimer = 0;
LakituState PrevLakituState = {};

void save_position() {
    memcpy(PrevFaceAngle, gMarioState->faceAngle, sizeof(Vec3s));
    memcpy(PrevAngleVel, gMarioState->angleVel, sizeof(Vec3s));
    memcpy(PrevPos, gMarioState->pos, sizeof(Vec3f));
    memcpy(PrevVel, gMarioState->vel, sizeof(Vec3f));
    PrevForwardVel = gMarioState->forwardVel;
    PrevSlideVelX = gMarioState->slideVelX;
    PrevSlideVelZ = gMarioState->slideVelZ;
    PrevAction = gMarioState->action;
    PrevPrevAction = gMarioState->prevAction;
    PrevActionState = gMarioState->actionState;
    PrevActionTimer = gMarioState->actionTimer;
    PrevActionArg = gMarioState->actionArg;
    PrevHealth = gMarioState->health;
    PrevHurtCounter = gMarioState->hurtCounter;
    PrevHealCounter = gMarioState->healCounter;
    PrevInvincTimer = gMarioState->invincTimer;
    memcpy(&PrevLakituState, &gLakituState, sizeof(LakituState));
    printf("saved pos: %f %f %f\n", PrevPos[0], PrevPos[1], PrevPos[2]);
}

void load_position() {
    memcpy(gMarioState->faceAngle, PrevFaceAngle, sizeof(Vec3s));
    memcpy(gMarioState->angleVel, PrevAngleVel, sizeof(Vec3s));
    memcpy(gMarioState->pos, PrevPos, sizeof(Vec3f));
    memcpy(gMarioState->vel, PrevVel, sizeof(Vec3f));
    gMarioState->forwardVel = PrevForwardVel;
    gMarioState->slideVelX = PrevSlideVelX;
    gMarioState->slideVelZ = PrevSlideVelZ;
    gMarioState->action = PrevAction;
    gMarioState->prevAction = PrevPrevAction;
    gMarioState->actionState = PrevActionState;
    gMarioState->actionTimer = PrevActionTimer;
    gMarioState->actionArg = PrevActionArg;
    gMarioState->health = PrevHealth;
    gMarioState->hurtCounter = PrevHurtCounter;
    gMarioState->healCounter = PrevHealCounter;
    gMarioState->invincTimer = PrevInvincTimer;
    memcpy(&gLakituState, &PrevLakituState, sizeof(LakituState));
    printf("loaded pos: %f %f %f\n", gMarioState->pos[0], gMarioState->pos[1], gMarioState->pos[2]);
}

//
// Main Menu
//

void DynOS_ReturnToMainMenu() {
    optmenu_toggle();
    level_set_transition(0, NULL);
    gDialogBoxState = 0;
    gMenuMode = -1;
    fade_into_special_warp(-2, 0);
}

//
// Init
//

DYNOS_AT_STARTUP void DynOS_Init() {
#ifdef OMM_DEFINES_H
    omm_opt_init();
#endif
    DynOS_Opt_Init();
}

//
// Update
//

void update_bindings() {
    if ((gPlayer1Controller->buttonDown & L_TRIG) && (gPlayer1Controller->buttonDown & R_TRIG)) {
        // Restart level
        DynOS_Warp_RestartLevel();
    } else if ((gPlayer1Controller->buttonDown & START_BUTTON)
               && (gPlayer1Controller->buttonDown & L_TRIG)) {
        // Save current position
        save_position();
    } else if ((gPlayer1Controller->buttonDown & L_TRIG) && (gMenuMode == -1)) {
        // Load saved position
        load_position();
    }
}

static bool sDynosIsLevelEntry = false;
void DynOS_UpdateOpt(void *aPad) {
    if (sDynosIsLevelEntry) {
        DynOS_Warp_SetParam(gCurrLevelNum, -1);
        sDynosIsLevelEntry = false;
    }

    update_bindings();

    DynOS_Opt_Update((OSContPad *) aPad);
    gPrevFrameObjectCount = 0;
}

void *DynOS_UpdateCmd(void *aCmd) {
    static const uintptr_t sCmdLevelEntry[] = { CALL(0, lvl_init_or_update) };
    sDynosIsLevelEntry |= (memcmp(aCmd, sCmdLevelEntry, sizeof(sCmdLevelEntry)) == 0);
    return DynOS_Warp_Update(aCmd, sDynosIsLevelEntry);
}

void DynOS_UpdateGfx() {
    DynOS_Gfx_Update();
}

bool DynOS_IsTransitionActive() {
    return gWarpTransition.isActive;
}
