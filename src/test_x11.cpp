#include "x11_utils.h"

int main() {
    Display *display;
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        return 1;
    }
    XCloseDisplay(display);
    return 0;
}
