#include <iostream>
#include <SDL2/SDL.h>

int main() {
    SDL_DisplayMode mode;
    SDL_Init(SDL_INIT_VIDEO);
    int num_displays = SDL_GetNumVideoDisplays();
    for (int i = 0; i < num_displays; ++i) {
        std::cout << SDL_GetDisplayName(i) << std::endl;
        auto num_modes = SDL_GetNumDisplayModes(i);
        for (int j = 0; j < num_modes; ++j) {
            if (!SDL_GetDisplayMode(i, j, &mode)) {
                std::cout << "\t" << mode.w << "x" << mode.h << " " << mode.refresh_rate << "hz" << std::endl;
            }
        }
    }

    return 0;
}