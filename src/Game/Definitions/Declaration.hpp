#pragma once
#include "Structs.hpp"

#include <cstdint>
#include <Windows.h>

// Modules
extern const uintptr_t IW3MP;
extern const uintptr_t COD4X;

// Game
extern CmdArgs* cmd_args;
extern cmd_function_s** cmds;
extern clientActive_t* clients;
extern clientStatic_t* cls;
extern clientConnection_t& clc;
extern cg_s* cgs;
extern GfxBuffers* gfx_buf;
extern GfxWorld* gfx_world;
extern GfxScene* scene;
extern clipMap_t* cm;
extern ComWorld* com;
extern int* com_frameTime;
extern float* com_timescaleValue;
extern int* g_entities;
extern int* g_clients;
extern playerState_s* ps_loc;
extern pmove_t* pmove;
extern const char* g_entityBeginParsePoint;
extern const char* g_entityEndParsePoint;
extern int* clientActiveCmdNumber;
extern XZone* g_zones;
extern XAssetEntry* g_assetEntryPool;
extern infoParm_t* info_params;
extern WeaponDef** bg_weaponNames;

// Render
extern MaterialLoadGlob* mtl_loadGlob;
extern GfxRenderTarget* gfx_renderTargets;
extern float& v_aspectRatioWindow;
extern float& v_postProcessAspect;
extern IDirect3DDevice9** dx9_device;
extern Material* floatz_display;
extern GfxCmdBufSourceState* gfx_cmdBufSourceState;
extern GfxCmdBufState* gfx_cmdBufState;
extern materialCommands_t* tess;
extern GfxBackEndData* gfx_frontEndDataOut;
extern GfxBackEndData* gfx_backEndData;
extern r_global_permanent_t* rgp;
extern clientDebugLineInfo_t* clientDebugLineInfo_client;
extern clientDebugLineInfo_t* clientDebugLineInfo_server;

// UI
extern uintptr_t* ui_white_material;
extern int* g_typeEnum;
extern int* map_nameEnum;
extern UiContext* ui_cg_dc;
extern UiContext* ui_context;
extern WinMouseVars_t* s_wmv;
extern PlayerKeyState* player_keys;
extern clientUIActive_t* client_ui;
extern ScreenPlacement* scr_place;
extern ScreenPlacement* scr_placeFull;

// GSC
extern uintptr_t* gScrMemTreePub;
extern uintptr_t* scrVarPub;
extern int* scrVarPub_p4;
extern char* errorMessage;
extern char* errorType;
extern int* scr_numParam;
extern gentity_s* scr_g_entities;
extern level_locals_t* level_locals;

// Console
extern float* con_matchtxtColor_currentDvar;
extern float* con_matchtxtColor_currentValue;
extern float* con_matchtxtColor_defaultValue;
extern float* con_matchtxtColor_dvarDescription;
extern float* con_matchtxtColor_domainDescription;
extern ConDrawInputGlob* con_drawInputGlob;
extern Console* con;
extern field_t* g_consoleField;
extern bool* key_isCatcherActive;
extern float* con_screenMinLeft;
extern float* con_screenMinTop;
extern float* con_screenMaxRight;
extern float* con_screenMaxBottom;
extern bool* con_ignoreMatchPrefixOnly;
extern cmd_function_s* cmd_functions;

// Database
extern HANDLE db_handle;
extern unsigned short* db_hashTable;
extern const char** zone_code_post_gfx_mp;
extern const char** zone_localized_code_post_gfx_mp;
extern const char** zone_ui_mp;
extern const char** zone_common_mp;
extern const char** zone_localized_common_mp;
extern const char** zone_mod;
extern XAssetHeader* db_xassetPool;
extern unsigned int* g_poolSize;

// Filesystem
extern const char* fs_game;
extern searchpath_s* fs_searchPaths;
