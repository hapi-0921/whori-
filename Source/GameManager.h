#include "targetManager.h"  


class GameManager
{
public:
    static GameManager& Instance()
    {
        static GameManager instance;
        return instance;
    }

    //ゲーム中かどうか（入力）
    void SetPlaying(bool playing)
    {
        isPlaying = playing;
    }
    //（結果）
    bool IsPlaying() const
    {
        return isPlaying;
    }

    //targetの読み込み等管理
    void CreateTargetManager();
    void ReleaseTargetManager();
    TargetManager* GetTargetManager();

private:
    bool isPlaying = false;


    TargetManager* targetManager = nullptr;

public:
    bool needCameraReset = false;

};