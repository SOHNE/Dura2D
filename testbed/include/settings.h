#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings
{
    Settings() = default;

    void
    Reset()
    {
        targetFPS          = 60;
        positionIterations = 3;
        allowSleep         = true;
    }

    int  targetFPS { 60 };
    int  positionIterations { 3 };
    bool allowSleep { true };
};

#endif //SETTINGS_H
