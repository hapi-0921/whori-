#include "GameManager.h"  

//À‘Ì¶¬Aƒ[ƒh
void GameManager::CreateTargetManager()
{
    if (targetManager == nullptr)
    {
        targetManager = new TargetManager();
        targetManager->LoadTargets("Data/targetData/target.json");  
    }
}
//”jŠü
void GameManager::ReleaseTargetManager()
{
    if (targetManager != nullptr)
    {
        delete targetManager;
        targetManager = nullptr;
    }
}

TargetManager* GameManager::GetTargetManager()
{
    return targetManager;
}