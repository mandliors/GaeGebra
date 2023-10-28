#ifdef _WIN32
    #include <SDL.h.h>
#elif defined(__unix__) || defined(__linux__)
    #include <SDL2/SDL.h>
#endif

typedef SDL_Color Color;
typedef SDL_Point Vector2;