#ifndef INPUT_CORE_H
#define INPUT_CORE_H

#include <SDL.h>

class InputCore {
public:
    // Constructor/Destructor
    InputCore();
    ~InputCore();

    // Handle SDL events
    void handleEvent(SDL_Event& e);

    // Check if a key is pressed
    bool isKeyPressed(SDL_Scancode key);

private:
    // Current keyboard state
    const Uint8* keyboardState;
};

#endif // INPUT_CORE_H
