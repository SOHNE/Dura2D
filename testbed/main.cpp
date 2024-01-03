#if __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Application.h"

Application app;
void Loop(void);

int
main()
{
    app.Setup();

#if __EMSCRIPTEN__
    emscripten_set_main_loop(Loop, 0, 1);
#else
    Loop();
#endif

    app.Destroy();

    return 0;
}

void
Loop()
{
    app.Run();
}