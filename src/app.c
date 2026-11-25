/** @file app.c
 *
 *  A portable PICO-8 emulator written in C.
 *
 *  Copyright (c) 2025-2026, Michael Fitzmayer. All rights reserved.
 *  SPDX-License-Identifier: MIT
 *
 **/

#ifdef USE_SDL2
#include <SDL.h>
#else
#include "SDL3/SDL.h"
#endif

#include "app.h"
#include "core.h"

static SDL_AudioDeviceID audio_device;

bool init_app(SDL_Renderer** renderer, SDL_Window* window)
{
    SDL_SetHint("SDL_RENDER_VSYNC", "1");
    SDL_SetHint("SDL_RENDER_NGAGE_SHOW_FPS", "1");
#ifndef __EMSCRIPTEN__
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
#endif
#ifdef USE_SDL2
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
#else
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_INFO);
#endif
#ifndef USE_SDL2
    SDL_SetAppMetadata("open8", "1.0", "de.ngagesdk.open8");

    SDL_SetAppMetadataProperty(
        SDL_PROP_APP_METADATA_URL_STRING,
        "https://ngagesdk.de/open8"
    );

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD))
    {
        SDL_Log("Couldn't initialize gamepad subsystem: %s", SDL_GetError());
    }
#else
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
    {
        SDL_Log("Couldn't initialize gamepad subsystem: %s", SDL_GetError());
    }
#endif

    int window_w, window_h;

#if defined(__DJGPP__)
    window_w = 320;
    window_h = 200;
#elif __SYMBIAN__
    window_w = 176;
    window_h = 208;
#else
    window_w = 160 * 2;
    window_h = 205 * 2;
#endif

#ifdef USE_SDL2
    window = SDL_CreateWindow("open8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_w, window_h, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
#else
    window = SDL_CreateWindow("open8", window_w, window_h, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
#endif
    if (!window)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return false;
    }

#ifdef USE_SDL2
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
#else
    *renderer = SDL_CreateRenderer(window, "software");
#endif
    if (!*renderer)
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return false;
    }

    handle_resize(*renderer);

    SDL_AudioSpec spec;
#ifdef USE_SDL2
    SDL_memset(&spec, 0, sizeof(spec));
    spec.channels = 1;
    spec.format = AUDIO_S16SYS;
    spec.freq = 8000;
    spec.samples = 512;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
#else
    spec.channels = 1;
    spec.format = SDL_AUDIO_S16;
    spec.freq = 8000;

    audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
#endif
    if (audio_device == 0)
    {
        SDL_Log("SDL_OpenAudioDevice: %s", SDL_GetError());
        return false;
    }

    return true;
}

void destroy_app(void)
{
    SDL_CloseAudioDevice(audio_device);
}
