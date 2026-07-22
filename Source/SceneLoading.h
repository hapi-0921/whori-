#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include <thread>

#include "targetManager.h"
#include <CameraController.h>

// ローディングシーン
class SceneLoading : public Scene
{
public:
    SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
    ~SceneLoading() override {};

    // 初期化
    void Initialize() override;

    // 終了化
    void Finalize() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render() override;

    // GUI描画
    void DrawGUI() override;


    struct LoadingTarget
    {
        int pos_x;
        int pos_y;
        std::string path;
        Sprite* sprite;
    };

private:


    Sprite* sprite = nullptr;

    // ローディング中のオブジェクト用配列
    // LoadingTarget LoadItemFood[40] = {};
    // LoadingTarget LoadItemAnimal[40] = {};
    Sprite* LoadItemFood[45];
    Sprite* LoadItemAnimal[41];
    int ItemNum[4]; // ランダムに出すアイテム番号
    float ItemNumPosY[4]; // ランダムに出すアイテムの描画位置(高さだけ)
    bool MoveUp[4];
    bool MoveDown[4];
    bool MoveEnd[4];
    float MotonoPosition;
    int MoveState;
    int MoveNum;

    enum ItemMoveState
    {
        Up = 0,
        Down,
        Finish
    };

    int ItemSize = 250;
    int HarfItemSize = ItemSize / 2;

    // アイテムの位置
    float ItemPositionX = 1920 / 2;
    float ItemPositionY = 1080 / 2 - ItemSize / 2;

    float angle = 0.0f; // 右下で回転するやつの角度
    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;
    float LoadTimer = 4000;
    Sprite* sprFadeRect = nullptr;

    // カメラ
    CameraController* cameraController = nullptr;

    //int ItemNum = {};

    // 3Dモデル
    //Model* mdlApple = nullptr;
    //Model* Loadmdl = nullptr;

    // モデル用行列
    //LoadingTarget LoadTofu;



    //TargetManager* targetManager = nullptr;
    //std::vector<LoadingTarget> loadingTargets;


    // ローディングスレッド
    static void LoadingThread(SceneLoading* scene);
};
