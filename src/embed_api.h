#pragma once
// the X11 interface class that allows the editor
// to pull in the vscode interface.

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class EmbedAPI : public Object {
	GDCLASS(EmbedAPI, Object);

private:
	static EmbedAPI *singleton;

    #ifdef __linux__
    #include "x11_utils.h"
	Display *display;
    #endif

protected:
	static void _bind_methods();

public:
	static EmbedAPI *get_singleton();

	EmbedAPI();
	~EmbedAPI();

	uint64_t window_get_vscode_window() const;
	void window_set_rect(uint64_t window, Rect2i rect);
	uint64_t window_get_parent(uint64_t child) const;
	void window_set_parent(uint64_t child, uint64_t new_parent);
	void window_set_visible(uint64_t window, bool visible);
	void window_allow_resizing(uint64_t window, bool resizing);
    uint64_t window_get_vscode_hwnd() const;
};