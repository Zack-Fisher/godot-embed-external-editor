#include "embed_api.h"

#include "x11_utils.h"
#include <cstring>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

EmbedAPI::EmbedAPI()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
	display = XOpenDisplay(NULL);
	ERR_FAIL_COND(display == nullptr);
}

EmbedAPI::~EmbedAPI()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
	XCloseDisplay(display);
}

bool is_vscode_window(Display *display, Window window) {
    XTextProperty window_name;
    if (XGetWMName(display, window, &window_name) == 0) {
        return false;
    }

	bool is_vscode = strstr(reinterpret_cast<const char*>(window_name.value), "Visual Studio Code") != nullptr;
    XFree(window_name.value);
    return is_vscode;
}


Window find_vscode_window(Display *display, Window root_window) {
    Window root_return, parent_return;
    Window *children;
    unsigned int num_children;

    if (XQueryTree(display, root_window, &root_return, &parent_return, &children, &num_children) == 0) {
        return 0;
    }

    for (unsigned int i = 0; i < num_children; i++) {
        if (is_vscode_window(display, children[i])) {
            XFree(children);
            return children[i];
        }
        Window vscode_child = find_vscode_window(display, children[i]);
        if (vscode_child != 0) {
            XFree(children);
            return vscode_child;
        }
    }

    XFree(children);
    return 0;
}

uint64_t EmbedAPI::window_get_vscode_window() const {
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        return 0;
    }

    Window root_window = XDefaultRootWindow(display);
    Window vscode_window = find_vscode_window(display, root_window);
    XCloseDisplay(display);

    return static_cast<uint64_t>(vscode_window);
}


void EmbedAPI::window_set_rect(uint64_t window, Rect2i rect) {
	XMoveResizeWindow(display, (Window)window, rect.position.x, rect.position.y, rect.size.x, rect.size.y);
}

uint64_t EmbedAPI::window_get_parent(uint64_t child) const {
	Window root, parent;
	Window *children;
	unsigned int nchildren;

	XQueryTree(display, (Window)child, &root, &parent, &children, &nchildren);
	XFree(children);
	return (uint64_t)parent;
}

void EmbedAPI::window_set_parent(uint64_t child, uint64_t new_parent) {
	XReparentWindow(display, (Window)child, (Window)new_parent, 0, 0);
}

void EmbedAPI::window_set_visible(uint64_t window, bool visible) {
	if (visible) {
		XMapWindow(display, (Window)window);
	} else {
		XUnmapWindow(display, (Window)window);
	}
}

void EmbedAPI::window_allow_resizing(uint64_t p_window, bool p_allow) {
    Display *display = XOpenDisplay(nullptr);
    if (!display) {
        return;
    }

    Window window = static_cast<Window>(p_window);
    XWindowAttributes attrs;
    XGetWindowAttributes(display, window, &attrs);

    XSizeHints size_hints;
    long supplied;
    XGetWMNormalHints(display, window, &size_hints, &supplied);

    if (p_allow) {
        size_hints.flags &= ~(PMinSize | PMaxSize);
    } else {
        size_hints.flags |= (PMinSize | PMaxSize);
		size_hints.max_width = DisplayWidth(display, XScreenNumberOfScreen(attrs.screen));
		size_hints.max_height = DisplayHeight(display, XScreenNumberOfScreen(attrs.screen));
    }

    XSetWMNormalHints(display, window, &size_hints);
    XFlush(display);
    XCloseDisplay(display);
}

uint64_t EmbedAPI::window_get_vscode_hwnd() const {
	// TODO
	return 0;
}