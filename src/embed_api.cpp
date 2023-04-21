#include "embed_api.h"

// just dump the cpp files here during compilation hahhaaa
#ifdef _WIN32
#include "winapi.cpp"
#endif

#ifdef __linux__
#include "x11.cpp"
#endif

// put the common params and methods here, like the binding one.
// these are os independent.
EmbedAPI *EmbedAPI::singleton = nullptr;

EmbedAPI *EmbedAPI::get_singleton()
{
	return singleton;
}

void EmbedAPI::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("window_get_parent", "child"), &EmbedAPI::window_get_parent);
	ClassDB::bind_method(D_METHOD("window_get_vscode_hwnd"), &EmbedAPI::window_get_vscode_hwnd);
	ClassDB::bind_method(D_METHOD("window_set_rect", "hwnd", "rect", "flags"), &EmbedAPI::window_set_rect);
	ClassDB::bind_method(D_METHOD("window_set_parent", "child", "new_parent"), &EmbedAPI::window_set_parent);
	ClassDB::bind_method(D_METHOD("window_set_visible", "window", "visible"), &EmbedAPI::window_set_visible);
	ClassDB::bind_method(D_METHOD("window_allow_resizing", "window", "resizing"), &EmbedAPI::window_allow_resizing);
}
