#include "InputCore.h"
#include "stdio.h"

InputCore::InputCore() {
    // Get keyboard state array
    keyboardState = SDL_GetKeyboardState(NULL);
}

InputCore::~InputCore() {
    // Nothing to clean up
}

void InputCore::handleEvent(SDL_Event& e) {
    // Update keyboard state on key events
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        keyboardState = SDL_GetKeyboardState(NULL);

        switch (e.key.keysym.sym) {
        case SDLK_w:
            printf("W tusuna basildi\n");
            break;
        case SDLK_a:
            printf("A tusuna basildi\n");
            break;
        case SDLK_s:
            printf("S tusuna basildi\n");
            break;
        case SDLK_d:
            printf("D tusuna basildi\n");
            break;
        }
    }
}

bool InputCore::isKeyPressed(SDL_Scancode key) {
    return keyboardState[key];
}