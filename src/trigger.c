//
// trigger.c
///

#include <assert.h>
#include "log.h"
#include "trigger.h"
#include "lua/lua.h"

//
// Tries to call lua block then C function
//
void trigger_pull(const trigger * tr, void * ud)
{
	// call lua block
	if (tr->lua_block) {
		lua_checkstack(prim_lstate, 3);
		lua_rawgeti(prim_lstate, LUA_REGISTRYINDEX, tr->lua_block);
		lua_pushlightuserdata(prim_lstate, ud);

		if (lua_isfunction(prim_lstate, -2)) {
			if (lua_pcall(prim_lstate, 1, 0, 0)) {
				wrlog("Error in trigger: %s.\n", lua_tostring(prim_lstate, -1));
				lua_pop(prim_lstate, 1);
			}
		} else {
			wrlog("Error: Trigger index does not point to lua function.");
		}
	}

	// call c function
	if (tr->c_func != NULL) {
		tr->c_func(ud);
	}
}