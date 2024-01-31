#ifndef SETTINGS_H
#define SETTINGS_H


struct Settings
{
    Settings() = default;

    void Reset()
    {
        targetFPS = 60;
        positionIterations = 3;
    }

    int targetFPS {60};
    int positionIterations {3};
};


#endif //SETTINGS_H
