/** @file core.c
 *
 *  A portable PICO-8 emulator written in C.
 *
 *  Copyright (c) 2025, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifdef USE_SDL2
#include <SDL.h>
#else
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#endif

#include "app.h"
#include "core.h"

static SDL_Window* window;
static SDL_Renderer* renderer;

#ifdef USE_SDL2
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    if (!init_app((SDL_Renderer**)&renderer, window))
    {
        return 1;
    }

    if (!init_core(renderer))
    {
        destroy_app();
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (!handle_events(renderer, &event))
            {
                running = false;
                break;
            }
        }
        if (running)
        {
            iterate_core(renderer);
        }
    }

    destroy_core();
    destroy_app();
    SDL_Quit();
    return 0;
}
#else
// This function runs once at startup.
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!init_app((SDL_Renderer**)&renderer, window))
    {
        return SDL_APP_FAILURE;
    }

    if (!init_core(renderer))
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

// This function runs when a new event (Keypresses, etc) occurs.
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (!handle_events(renderer, event))
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program.
SDL_AppResult SDL_AppIterate(void* appstate)
{
    iterate_core(renderer);
    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown.
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    destroy_core();
    destroy_app();

    // SDL will clean up the window/renderer for us.
}
#endif
