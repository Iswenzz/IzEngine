#include "Declaration.hpp"

namespace IW3SR::Game
{
	// Binaries
	std::string COD4X_BIN = "";
	std::string IW3MP_BIN = "iw3mp.exe";
	std::string IW3XO_BIN = "iw3xo.dll";

	// Modules
	uintptr_t COD4X = 0;
	uintptr_t IW3MP = reinterpret_cast<uintptr_t>(GetModuleHandle(IW3MP_BIN.c_str()));
	uintptr_t IW3XO = reinterpret_cast<uintptr_t>(GetModuleHandle(IW3XO_BIN.c_str()));

	// Game
	CmdArgs* cmd_args = reinterpret_cast<CmdArgs*>(0x1410B40);
	cmd_function_s** cmds = reinterpret_cast<cmd_function_s**>(0x1410B3C);
	clientActive_t* clients = reinterpret_cast<clientActive_t*>(0xC5F930);
	clientStatic_t* cls = reinterpret_cast<clientStatic_t*>(0x956D80);
	clientConnection_t& clc = *reinterpret_cast<clientConnection_t*>(0x8F4CE0);
	cg_s* cgs = reinterpret_cast<cg_s*>(0x74E338);
	centity_s* cg_entities = reinterpret_cast<centity_s*>(0x84F2D8);
	GfxBuffers* gfx_buf = reinterpret_cast<GfxBuffers*>(0xD2B0840);
	GfxWorld* gfx_world = reinterpret_cast<GfxWorld*>(0xD0701E0);
	GfxScene* scene = reinterpret_cast<GfxScene*>(0xCF10280);
	GfxViewParms* viewParms = reinterpret_cast<GfxViewParms*>(0xD540220);
	clipMap_t* cm = reinterpret_cast<clipMap_t*>(0x14098C0);
	ComWorld* com = reinterpret_cast<ComWorld*>(0x1435CB8);
	int* com_frameTime = reinterpret_cast<int*>(0x1476EFC);
	float* com_timescaleValue = reinterpret_cast<float*>(0x1435D68);
	int* g_entities = reinterpret_cast<int*>(0x12885C4);
	int* g_clients = reinterpret_cast<int*>(0x13255A8);
	pmove_t* pmove = reinterpret_cast<pmove_t*>(0x8C9C90);
	const char* g_entityBeginParsePoint = reinterpret_cast<const char*>(0x1113674);
	const char* g_entityEndParsePoint = reinterpret_cast<const char*>(0x1113678);
	int* client_activeNumber = reinterpret_cast<int*>(0xCC5FF8);
	XZone* g_zones = reinterpret_cast<XZone*>(0xFFEFD0);
	XAssetEntry* g_assetEntryPool = reinterpret_cast<XAssetEntry*>(0xF0D640);
	infoParm_t* info_params = reinterpret_cast<infoParm_t*>(0x71FBD0);
	WeaponDef** bg_weaponNames = reinterpret_cast<WeaponDef**>(0x736DB8);

	// Render
	DxGlobals* dx = reinterpret_cast<DxGlobals*>(0xCC9A400);
	MaterialLoadGlob* mtl_loadGlob = reinterpret_cast<MaterialLoadGlob*>(0xD541330);
	GfxRenderTarget* gfx_renderTargets = reinterpret_cast<GfxRenderTarget*>(0xD573EB0);
	float& v_aspectRatioWindow = *reinterpret_cast<float*>(0xCC9D0E4);
	float& v_postProcessAspect = *reinterpret_cast<float*>(0xCC9D0FC);
	Material* floatz_display = reinterpret_cast<Material*>(0xFA5378);
	GfxCmdBufSourceState* gfx_cmdBufSourceState = reinterpret_cast<GfxCmdBufSourceState*>(0xD53F5F0);
	GfxCmdBufState* gfx_cmdBufState = reinterpret_cast<GfxCmdBufState*>(0xD5404F0);
	GfxBackEndData* gfx_frontEndDataOut = reinterpret_cast<GfxBackEndData*>(0xCC9827C);
	GfxBackEndData* gfx_backEndData = reinterpret_cast<GfxBackEndData*>(0xD0704BC);
	materialCommands_t* tess = reinterpret_cast<materialCommands_t*>(0xD085EE0);
	r_global_permanent_t* rgp = reinterpret_cast<r_global_permanent_t*>(0xCC98280);
	clientDebugLineInfo_t* clientDebugLineInfo_client = reinterpret_cast<clientDebugLineInfo_t*>(0xC5B054);
	clientDebugLineInfo_t* clientDebugLineInfo_server = reinterpret_cast<clientDebugLineInfo_t*>(0xC5B074);

	// UI
	uintptr_t* ui_white_material = reinterpret_cast<uintptr_t*>(0xCAF06F0);
	UiContext* ui_cg_dc = reinterpret_cast<UiContext*>(0x746FA8);
	UiContext* ui_context = reinterpret_cast<UiContext*>(0xCAEE200);
	WinMouseVars_t* s_wmv = reinterpret_cast<WinMouseVars_t*>(0xCC147C4);
	PlayerKeyState* player_keys = reinterpret_cast<PlayerKeyState*>(0x8F1DB8);
	clientUIActive_t* client_ui = reinterpret_cast<clientUIActive_t*>(0xC5F8F4);
	int* map_nameEnum = reinterpret_cast<int*>(0xCAF2330);
	int* g_typeEnum = reinterpret_cast<int*>(0xCAF1820);
	Fonts_t* g_fonts = reinterpret_cast<Fonts_t*>(0xCAF06F8);

	// GSC
	uintptr_t* gScrMemTreePub = reinterpret_cast<uintptr_t*>(0x14E8A04);
	uintptr_t* scrVarPub = reinterpret_cast<uintptr_t*>(0x15CA61C);
	int* scrVarPub_p4 = reinterpret_cast<int*>(0x15CA620);
	char* error_message = reinterpret_cast<char*>(0x1798378);
	char* error_type = reinterpret_cast<char*>(0x1798777);
	int* scr_numParam = reinterpret_cast<int*>(0x1794074);
	gentity_s* scr_g_entities = reinterpret_cast<gentity_s*>(0x1288500);
	level_locals_t* level_locals = reinterpret_cast<level_locals_t*>(0x13EB6A8);

	// Console
	float* con_matchtxtColor_currentDvar = reinterpret_cast<float*>(0x6BDF14);
	float* con_matchtxtColor_currentValue = reinterpret_cast<float*>(0x6BDF24);
	float* con_matchtxtColor_defaultValue = reinterpret_cast<float*>(0x6BDF34);
	float* con_matchtxtColor_dvarDescription = reinterpret_cast<float*>(0x6BDF54);
	float* con_matchtxtColor_domainDescription = reinterpret_cast<float*>(0x6BDF44);
	ConDrawInputGlob* con_drawInputGlob = reinterpret_cast<ConDrawInputGlob*>(0x8CC2C8);
	Con* con = reinterpret_cast<Con*>(0x8DC8C0);
	bool* con_isCatcherActive = reinterpret_cast<bool*>(0xC5F8F8);
	float* con_screenMinLeft = reinterpret_cast<float*>(0x8ECB14);
	float* con_screenMinTop = reinterpret_cast<float*>(0x8ECB18);
	float* con_screenMaxRight = reinterpret_cast<float*>(0x8ECB1C);
	float* con_screenMaxBottom = reinterpret_cast<float*>(0x8ECB20);
	bool* con_ignoreMatchPrefixOnly = reinterpret_cast<bool*>(0x736BB1);
	cmd_function_s* cmd_functions = reinterpret_cast<cmd_function_s*>(0x1410B3C);
	field_t* g_consoleField = reinterpret_cast<field_t*>(0x8F1B88);

	// Database
	HANDLE db_handle = reinterpret_cast<HANDLE>(0x14E89A4);
	unsigned short* db_hashTable = reinterpret_cast<unsigned short*>(0xE62A80);
	XAssetHeader* db_xassetPool = reinterpret_cast<XAssetHeader*>(0x7265E0);
	const char** zone_code_post_gfx_mp = reinterpret_cast<const char**>(0xCC9D128);
	const char** zone_localized_code_post_gfx_mp = reinterpret_cast<const char**>(0xCC9D134);
	const char** zone_ui_mp = reinterpret_cast<const char**>(0xCC9D12C);
	const char** zone_common_mp = reinterpret_cast<const char**>(0xCC9D130);
	const char** zone_localized_common_mp = reinterpret_cast<const char**>(0xCC9D138);
	const char** zone_mod = reinterpret_cast<const char**>(0xCC9D13C);
	unsigned int* g_poolSize = reinterpret_cast<unsigned int*>(0x7263A0);

	// Filesystem
	const char* fs_game = reinterpret_cast<const char*>(0xCB19898);
	searchpath_s* fs_searchPaths = reinterpret_cast<searchpath_s*>(0xD5EC4DC);
}
