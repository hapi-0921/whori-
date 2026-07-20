#include <fstream>
#include <algorithm>
#include "save.h"
#include "ScoreManager.h"

struct SaveData
{
    int ranking[4];
};

void Save::SaveGame()
{
    ScoreManager& scoreManager = ScoreManager::Instance();

    SaveData data = {};

    // 以前のランキングを読み込む
    std::ifstream in("save.dat", std::ios::binary);

    if (in.is_open())
    {
        in.read(reinterpret_cast<char*>(&data), sizeof(data));
        in.close();
    }
    else
    {
        // 初回起動
        for (int i = 0; i < 4; i++)
        {
            data.ranking[i] = 0;
        }
    }

    // 5個の配列を作る
    int temp[5];

    // 今までの4件をコピー
    for (int i = 0; i < 4; i++)
    {
        temp[i] = data.ranking[i];
    }
    bool same = false;

    for (int i = 0; i < 4; i++)
    {
        if (data.ranking[i] == scoreManager.allScore)
        {
            same = true;
            break;
        }
    }

    if (same)
    {
        return;    // 同じ点数なので保存しない
    }
    // 今回のスコアを追加
    temp[4] = scoreManager.allScore;

    // 大きい順に並び替え
    std::sort(temp, temp + 5, std::greater<int>());

    // 上位4件だけ保存
    for (int i = 0; i < 4; i++)
    {
        data.ranking[i] = temp[i];
    }

    // 保存
    std::ofstream out("save.dat", std::ios::binary);

    if (out.is_open())
    {
        out.write(reinterpret_cast<char*>(&data), sizeof(data));
        out.close();
    }
}

void Save::LoadGame()
{
    SaveData data = {};

    std::ifstream in("save.dat", std::ios::binary);

    if (!in.is_open())
    {
        for (int i = 0; i < 4; i++)
        {
            data.ranking[i] = 0;
        }
        return;
    }

    in.read(reinterpret_cast<char*>(&data), sizeof(data));
    in.close();

    // 読み込んだランキングを保存クラスのメンバへ
    for (int i = 0; i < 4; i++)
    {
        ranking[i] = data.ranking[i];
    }
}
void Save::Reset()
{
    for (int i = 0; i < 4; i++)
    {
        ranking[i] = 0;
    }

    std::ofstream out("save.dat", std::ios::binary);

    if (out.is_open())
    {
        out.write(reinterpret_cast<char*>(ranking), sizeof(ranking));
        out.close();
    }
}