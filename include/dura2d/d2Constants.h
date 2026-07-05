#ifndef CONSTANTS_H
#define CONSTANTS_H

const int FPS                 = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

const int PIXELS_PER_METER    = 50;

// Sleeping thresholds
const float TIME_TO_SLEEP           = 0.5f;                        // seconds below tolerance before a body sleeps
const float SLEEP_LINEAR_TOLERANCE  = 3.0f;                        // pixels/second
const float SLEEP_ANGULAR_TOLERANCE = 2.0f / 180.0f * 3.14159265f; // radians/second (~2 deg/s)

#endif
