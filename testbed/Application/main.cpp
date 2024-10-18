#include "Application.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#endif

void
MainLoop(void* arg) //void* arg __attribute__((unused)))
{
	auto *game = static_cast<Application*>(arg);
	game->Input();
	game->Update();
	game->Render();

#ifdef __EMSCRIPTEN__
	if (!game->IsRunning())
    {
        emscripten_cancel_main_loop();
    }
#endif
}

int
main(int argc, char **argv)
{
    Application game { };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(MainLoop, &game, 0, 1);
#else
    do MainLoop(&game); while (game.IsRunning());
#endif

    game.Destroy();

    return 0;
}