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