#pragma once
#include "Coroutine/task.h"

namespace SlimeBoom
{
// 起動時に最初に表示するシーン(タイトルシーン)を読み込むクラス。
class DefaultSceneCreator
{
    /// 100フレーム待ってからタイトルシーンを読み込むコルーチン。
    static engine::Task DelayLoadSceneTask();
    
public:
    /// デフォルトシーンの読み込みを開始する。
    static void CreateDefaultScene();
};
}
