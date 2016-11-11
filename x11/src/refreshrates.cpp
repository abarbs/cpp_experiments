#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>
#include <X11/Xlib.h>

#include <iostream>
#include <memory>
#include <iterator>
#include <algorithm>

int main() {
    auto d = std::unique_ptr<Display, decltype(&XCloseDisplay)>(XOpenDisplay(nullptr), XCloseDisplay);

    std::cout << "Default Screen is " << DefaultScreen(d.get()) << std::endl;

    int maj_opcode = 0, fst_evt = 0, fst_err;
    XQueryExtension(d.get(), "RANDR", &maj_opcode, &fst_evt, &fst_err);
    std::cout << "Major opcode is " << maj_opcode << std::endl;
    std::cout << "First event num is " << fst_evt << std::endl;
    std::cout << "First error num is " << fst_err << std::endl;

    unsigned int screen_count = XScreenCount(d.get());
    std::cout << "Screen count is " << XScreenCount(d.get()) << ":" << std::endl;
    for (unsigned int i = 0; i < screen_count; ++i) {
        int nsizes = 0;
        XRRScreenSize *sizes = XRRSizes(d.get(), i, &nsizes);
        for (unsigned int sz_idx = 0; sz_idx < nsizes; ++sz_idx) {
            XRRScreenSize *sz = &sizes[sz_idx];
            std::cout << "Screen " << i << " " << sz->width << "x" << sz->height << std::endl;
            std::cout << "\t" << "Refresh rates: ";
            int nrates = 0;
            short *rates = XRRRates(d.get(), i, sz_idx, &nrates);
            std::copy(rates, rates + nrates, std::ostream_iterator<int>(std::cout, " "));
            std::cout << std::endl;
        }
    }

    int event_base = 0;
    int error_base = 0;
    if (XineramaQueryExtension(d.get(), &event_base, &error_base)) {
        if (XineramaIsActive(d.get())) {
            int num_screens = 0;
            XineramaScreenInfo *screenInfo = XineramaQueryScreens(d.get(), &num_screens);
            std::cout << "There are " << num_screens << " screens" << std::endl;
            for (int i = 0; i < num_screens; ++i) {
                XineramaScreenInfo &cur = screenInfo[i];
                std::cout << "screen " << cur.screen_number << ": " << cur.width << "x" << cur.height << " origin: (" <<
                cur.x_org << ", " << cur.y_org << ")" << std::endl;
            }
        } else {
            std::cout << "Xinerama is not active" << std::endl;
        }
    } else {
        std::cout << "Xinerama extension not available" << std::endl;
    }

    auto root_window = XDefaultRootWindow(d.get());
    auto screen_resources = std::unique_ptr<XRRScreenResources, decltype(&XRRFreeScreenResources)>(XRRGetScreenResourcesCurrent(d.get(), root_window), XRRFreeScreenResources);
        std::for_each(screen_resources->outputs, screen_resources->outputs + screen_resources->noutput,
                  [&](RROutput &out) {
                      auto out_info = std::unique_ptr<XRROutputInfo, decltype(&XRRFreeOutputInfo)> (XRRGetOutputInfo(d.get(), screen_resources.get(), out), XRRFreeOutputInfo);


                  });
    return 0;
}

