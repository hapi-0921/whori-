#pragma once

#include "TargetManager.h"

class GameContext
{
public:
    void Initialize()
    {
        targetManager = new TargetManager();
    }

    void Finalize()
    {
        delete targetManager;
        targetManager = nullptr;
    }

    TargetManager* GetTargetManager() const { return targetManager; }

private:
    TargetManager* targetManager = nullptr;
};