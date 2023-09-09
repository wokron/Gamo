#include "gamo/input/input.h"
#include <SDL2/SDL.h>

namespace gamo
{
    Input::Input()
    {
        int num_keys;
        SDL_PumpEvents();                // update the state state
        SDL_GetKeyboardState(&num_keys); // just get the array length
        _keyboard = new uint8_t[num_keys];
        _keyboard_up = new uint8_t[num_keys];
        _keyboard_down = new uint8_t[num_keys];
    }

    Input::~Input()
    {
        delete _keyboard;
        delete _keyboard_up;
        delete _keyboard_down;
    }

    void Input::UpdateInputStates()
    {
        _quit = false;
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                _quit = true;
                break;

            default:
                break;
            }
        }

        int num_keys;
        auto cur_keyboard = SDL_GetKeyboardState(&num_keys);
        for (int i = 0; i < num_keys; i++)
        {
            _keyboard_down[i] = !_keyboard[i] && cur_keyboard[i] ? 1 : 0;
            _keyboard_up[i] = _keyboard[i] && !cur_keyboard[i] ? 1 : 0;
            _keyboard[i] = cur_keyboard[i];
        }

        auto cur_mouse = SDL_GetMouseState(&_mouse_x, &_mouse_y);
        _mouse_down = ~_mouse & cur_mouse;
        _mouse_up = _mouse & ~cur_mouse;
        _mouse = cur_mouse;
    }

    bool Input::GetKey(SDL_Scancode scancode)
    {
        return _keyboard[scancode] == 1;
    }

    bool Input::GetKeyDown(SDL_Scancode scancode)
    {
        return _keyboard_down[scancode] == 1;
    }

    bool Input::GetKeyUp(SDL_Scancode scancode)
    {
        return _keyboard_up[scancode] == 1;
    }

    bool Input::GetMouseButton(MouseButton button)
    {
        return _mouse & SDL_BUTTON(button);
    }

    bool Input::GetMouseButtonDown(MouseButton button)
    {
        return _mouse_down & SDL_BUTTON(button);
    }

    bool Input::GetMouseButtonUp(MouseButton button)
    {
        return _mouse_up & SDL_BUTTON(button);
    }

    void Input::GetMousePosition(int *x, int *y)
    {
        if (x != nullptr)
        {
            *x = _mouse_x;
        }
        if (y != nullptr)
        {
            *y = _mouse_y;
        }
    }
} // namespace gamo
