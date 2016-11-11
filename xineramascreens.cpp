#include <X11/extensions/Xinerama.h>
#include <X11/Xlib.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include <iostream>
#include <memory>
#include <iterator>
#include <algorithm>

int main(int argc, char **argv) {
  gtk_init(&argc, &argv);
  GdkScreen *screen = gdk_screen_get_default();
  auto gdk_num_monitors = gdk_screen_get_n_monitors(screen);
  std::cout << "There are " << gdk_num_monitors << " monitors, according to gdk"
            << std::endl;

  for (gint gdk_mon = 0; gdk_mon < gdk_num_monitors; ++gdk_mon) {
    GdkRectangle rect = {0};
    gdk_screen_get_monitor_geometry(screen, gdk_mon, &rect);
    std::cout << "GDK monitor " << gdk_mon << ": " << rect.width << "x"
              << rect.height << " origin: (" << rect.x << ", "
              << rect.y << ")" << std::endl;
  }

  auto d = std::unique_ptr<Display, decltype(&XCloseDisplay)>(
      XOpenDisplay(nullptr), XCloseDisplay);

  int event_base = 0;
  int error_base = 0;
  if (XineramaQueryExtension(d.get(), &event_base, &error_base)) {
    if (XineramaIsActive(d.get())) {
      int num_screens = 0;
      XineramaScreenInfo *screenInfo =
          XineramaQueryScreens(d.get(), &num_screens);
      std::cout << "There are " << num_screens << " Xinerama screens"
                << std::endl;
      for (int i = 0; i < num_screens; ++i) {
        XineramaScreenInfo &cur = screenInfo[i];
        std::cout << "screen " << cur.screen_number << ": " << cur.width << "x"
                  << cur.height << " origin: (" << cur.x_org << ", "
                  << cur.y_org << ")" << std::endl;
      }
    } else {
      std::cout << "Xinerama is not active" << std::endl;
    }
  } else {
    std::cout << "Xinerama extension not available" << std::endl;
  }

  return 0;
}
