#include "Common.h"
namespace TSM
{
    double SCENE_INTRO_TIME;
    double SCENE_MAIN_TIME;
    double SCENE_ENDCREDITS_TIME;

    double g_current_scene_time = 10.0;
    double g_delta_time = 10.0;
    unsigned int selectedScene = SCENE::INTRO;
}

float objX = 0.0f;
float objY = 0.0f;
float objZ = 0.0f;
float objIncrement = 0.1f;

float scaleX = 0.0f;
float scaleY = 0.0f;
float scaleZ = 0.0f;
float scaleIncrement = 0.1f;
