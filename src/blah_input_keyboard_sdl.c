/* blah_input_keyboard_sdl.c
	SDL Specific Keyboard functions.
	Functions to handle keyboard input via SDL */

#include <stdio.h>

#include "blah_input_keyboard_sdl.h"
#include "blah_input_keyboard.h"
#include "blah_video_sdl.h"

/* Externally Referenced Variables */
extern Blah_Input_Key blahInputKeys[BLAH_INPUT_KEYBOARD_NUM_KEYS];


/* Globals */

blah_input_key_symbol blahInputKeyboardSDLMap[SDLK_EURO]; // FIXME an array to map SDL key symbols to engine key syms

/* Private Function Prototypes */

static int blah_input_keyboard_sdl_eventFilter(const SDL_Event *event);

/* Function Declarations */

static int blah_input_keyboard_sdl_eventFilter(const SDL_Event *event) {
	//Filters out all but keyboard events from the SDL event queue
	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
		return 1;
	else
		return 0;
}

void blah_input_keyboard_sdl_init() { //initialises SDL keyboard input
	fprintf(stderr,"Checking if sdl video inited\n");

	// This is pretty simple.  SDL Video is required for SDL keyboard input
	// If SDL Video hasn't been initialised, initialise SDL with video subsystem and we get keyboard also
	if (!SDL_WasInit(SDL_INIT_VIDEO)) { blah_video_init(); }

	fprintf(stderr, "initing mappings\n");

	//Set all mappings to -1, meaning not mapped to engine key symbols
	for (int symCount = 0; symCount < SDLK_EURO; symCount++) { blahInputKeyboardSDLMap[symCount] = BLAH_INPUT_KEY_NONE; };

	fprintf(stderr,"setting up key mappings\n");
	//Set up active mappings
	int activeKeyIndex = 0;
	int sdlActiveKeys[] = {SDLK_UP,BLAH_INPUT_KEY_UP, SDLK_DOWN,BLAH_INPUT_KEY_DOWN, SDLK_LEFT,BLAH_INPUT_KEY_LEFT,
	SDLK_RIGHT,BLAH_INPUT_KEY_RIGHT, SDLK_SPACE,BLAH_INPUT_KEY_SPACE, SDLK_LSHIFT,BLAH_INPUT_KEY_LEFT_SHIFT,
	SDLK_RETURN,BLAH_INPUT_KEY_RETURN, SDLK_ESCAPE,BLAH_INPUT_KEY_ESCAPE, SDLK_v,BLAH_INPUT_KEY_V,
	SDLK_d,BLAH_INPUT_KEY_D, SDLK_x,BLAH_INPUT_KEY_X, SDLK_t,BLAH_INPUT_KEY_T, SDLK_z,BLAH_INPUT_KEY_Z,
	SDLK_EQUALS,BLAH_INPUT_KEY_EQUALS, SDLK_MINUS,BLAH_INPUT_KEY_MINUS,
	SDLK_h,BLAH_INPUT_KEY_H,		(-1)};

	while (sdlActiveKeys[activeKeyIndex] !=-1) { //while end of mapping list hasn't been reached
		blahInputKeyboardSDLMap[sdlActiveKeys[activeKeyIndex]] = sdlActiveKeys[activeKeyIndex+1];
		activeKeyIndex += 2;
	}

	SDL_SetEventFilter(blah_input_keyboard_sdl_eventFilter);  //Set up event filter
}

void blah_input_keyboard_sdl_exit() { // shutdown SDL keyboard input component
	SDL_SetEventFilter(NULL); // remove event filter
}

void blah_input_keyboard_sdl_main() {  // update keyboard status via SDL
	SDL_Event sdlEvent;
	blah_input_key_symbol keySym;

	SDL_PumpEvents(); // Grab all events and whack them on the SDL event queue
	/* Select specific events to remove off queue */
	while (SDL_PeepEvents(&sdlEvent, 1, SDL_GETEVENT, SDL_KEYDOWNMASK | SDL_KEYUPMASK))
		switch (sdlEvent.type) {
			case SDL_KEYDOWN:  // key pressed down
				keySym = blahInputKeyboardSDLMap[sdlEvent.key.keysym.sym];
				if (keySym != BLAH_INPUT_KEY_NONE) { blahInputKeys[keySym].depressed = true; } // update key status if the SDL key symbol is actively mapped
				break;

			case SDL_KEYUP:  // key pressed down
				keySym = blahInputKeyboardSDLMap[sdlEvent.key.keysym.sym];
				if (keySym != BLAH_INPUT_KEY_NONE) { blahInputKeys[keySym].depressed = false; } // update key status if the SDL key symbol is actively mapped
				break;
		}

}
