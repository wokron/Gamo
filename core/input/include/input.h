#pragma once
#include "utils.h"
#include <SDL2/SDL.h>

namespace gamo
{
    enum MouseButton
    {
        Left = SDL_BUTTON_LEFT,
        Middle = SDL_BUTTON_MIDDLE,
        Right = SDL_BUTTON_RIGHT,
        X1 = SDL_BUTTON_X1,
        X2 = SDL_BUTTON_X2,
    };

    SINGLETON_BEGIN(Input)
    private:
        bool _quit = false;
        uint8_t *_keyboard_down = nullptr;
        uint8_t *_keyboard = nullptr;
        uint8_t *_keyboard_up = nullptr;
        uint32_t _mouse_down;
        uint32_t _mouse;
        uint32_t _mouse_up;
        int _mouse_x, _mouse_y;

    public:
        bool Quit() { return _quit; }

        /// @brief update the input states of keyboard and mouse,
        /// this function should be called in every game step
        void UpdateInputStates();

        /// @brief check if the key with the scancode is being pressed now
        /// @param scancode the scancode of the key
        /// @return if the key is pressed
        bool GetKey(SDL_Scancode scancode);

        /// @brief check if the key with the scancode has just been pressed
        /// @param scancode the scancode of the key
        /// @return if the key has just been pressed
        bool GetKeyDown(SDL_Scancode scancode);

        /// @brief check if the keyboard keys has been released
        /// @param scancode the scancode of the key
        /// @return if the keyboard key has been released
        bool GetKeyUp(SDL_Scancode scancode);

        /// @brief check if the mouse button with the button number is being pressed now
        /// @param scancode the button number
        /// @return if the button is pressed
        bool GetMouseButton(MouseButton button);

        /// @brief check if the mouse button with the button number has just been pressed
        /// @param scancode the button number
        /// @return if the button has just been pressed
        bool GetMouseButtonDown(MouseButton button);

        /// @brief check if the mouse button with the button number has been released
        /// @param scancode the button number
        /// @return if the button has been released
        bool GetMouseButtonUp(MouseButton button);

        /// @brief get the current mouse position,
        /// regardless of whether the mouse is shown or not.
        /// null can be passed for param x and y
        /// @return the positon of the mouse, relative to the focus window
        void GetMousePosition(int *x, int *y);

    private:
        Input();
        ~Input();
    SINGLETON_END

} // namespace gamo
