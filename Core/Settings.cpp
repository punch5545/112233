#include "framework.h"
#include "Settings.h"

Settings::Settings()
    : hInstance(nullptr)
    , handle(nullptr)
    , windowSize(0, 0)
    , bVsync(false)
{
}
