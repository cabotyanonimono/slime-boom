#pragma once
#include "Components/component.h"

// FBXファイルのパスを指定してモデルを読み込む、テスト用のComponent。
// インスペクターのGenerateボタンで実行する。
class MeshRendererGenerator : public engine::Component
{
    std::string m_path_;
public:
    void OnInspectorGui() override;
};
