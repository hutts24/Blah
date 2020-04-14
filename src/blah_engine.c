/* blah_engine.c - defines main engine framework */

#include <stdio.h>

#include "blah_engine.h"
#include "blah_video.h"
#include "blah_input.h"
#include "blah_draw.h"
#include "blah_entity.h"
#include "blah_debug.h"
#include "blah_signal.h"

/* Global variables */

static Blah_Debug_Log blah_engine_log = { .filePointer = NULL };

/* Function Declarations */

void blah_engine_exit()
{
	fprintf(stderr, "exiting draw\n");
	blah_draw_exit();	//Shutdown drawing component
	Blah_Debug_Log_message(&blah_engine_log, "Call to draw exit successful");
	fprintf(stderr, "exiting video\n");
	blah_video_exit();
	Blah_Debug_Log_message(&blah_engine_log, "Call to video exit successful");
	fprintf(stderr, "exiting input\n");
	blah_input_exit();  //shutdown the input component
	Blah_Debug_Log_message(&blah_engine_log, "Call to input exit successful");
	/* Do garbage collection */
	fprintf(stderr, "garbage collection\n");
	fprintf(stderr, "collecting entities\n");
	blah_entity_destroyAll();  //destroy all entities and free memory
	fprintf(stderr, "collecting fonts\n");
	blah_font_destroyAll(); //Destroy all remaining fonts in memory
	// fprintf(stderr,"collecting images\n");
	// blah_image_destroyAll(); //destroy all remaining images
	fprintf(stderr, "collecting models\n");
	blah_model_destroyAll(); //destroy all models in memory
	fprintf(stderr, "collecting textures\n");
	blah_texture_destroyAll(); //Garbage collection on textures
	Blah_Debug_Log_message(&blah_engine_log, "End of engine exit");
	fprintf(stderr, "collecting logs\n");
	blah_debug_log_destroyAll();	//Destroy all debugging logs
}

bool blah_engine_init()
{
    // initialises all engine components
    blah_signal_init(); // Install signal handlers
	Blah_Debug_Log_init(&blah_engine_log, "blah_engine");

	Blah_Debug_Log_message(&blah_engine_log, "Call to video init");
	if (!blah_video_init()) {
		Blah_Debug_Log_message(&blah_engine_log, "Failed to initialise video");
		Blah_Debug_Log_disable(&blah_engine_log);
		return false;
	}
	Blah_Debug_Log_message(&blah_engine_log, "Video init successful");

	Blah_Debug_Log_message(&blah_engine_log, "Call to draw init");
	if (!blah_draw_init()) {
		Blah_Debug_Log_message(&blah_engine_log, "Failed to initialise drawing");
		Blah_Debug_Log_disable(&blah_engine_log);
		return false;
	}
	Blah_Debug_Log_message(&blah_engine_log, "Draw init successful");

	Blah_Debug_Log_message(&blah_engine_log, "Call to input init");
	if (!blah_input_init()) {
		Blah_Debug_Log_message(&blah_engine_log, "Failed to initialise input");
		Blah_Debug_Log_disable(&blah_engine_log);
		return false;
	}
	Blah_Debug_Log_message(&blah_engine_log, "Input init successful");

	return true;
}

void blah_engine_main()
{	//main loop
	blah_input_main();  //Call main input processing
	blah_entity_main();  //Call main entity processing
	blah_video_main();	//Call main drawing routine to draw to display
}


