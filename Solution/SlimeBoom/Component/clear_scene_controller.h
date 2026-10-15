#pragma once
#include "clear_scene_data.h"
#include "result_rank.h"
#include "scene_transition_controller.h"
#include "scene_transition_executor.h"
#include "../UI/ui_button.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/image.h"
#include "Components/rect_transform.h"
#include "Components/text_renderer.h"
#include "Coroutine/task.h"
#include "game_object.h"

namespace SlimeBoom
{
// クリア/ゲームオーバー画面を制御するComponent。
// 画面の演出と、リスタート・終了ボタンによるシーン遷移を担当する。
class ClearSceneController : public engine::Component
{
    engine::AssetPtr<engine::Image> m_background_;
    engine::AssetPtr<engine::RectTransform> m_score_board_;
    engine::AssetPtr<engine::TextRenderer> m_game_over_text_;
    engine::AssetPtr<engine::TextRenderer> m_clear_text_;
    engine::AssetPtr<engine::TextRenderer> m_eliminated_slime_count_;
    engine::AssetPtr<engine::TextRenderer> m_level_;
    engine::AssetPtr<engine::GameObject> m_text_ui_;
    engine::AssetPtr<SceneTransitionController> m_restart_scene_transition_controller_;
    engine::AssetPtr<SceneTransitionController> m_quit_scene_transition_controller_;
    engine::AssetPtr<ui::UIButton> m_restart_button_;
    engine::AssetPtr<ui::UIButton> m_quit_button_;

    /// カウントアップの最終値(SetClearSceneDataで受け取る)
    int m_target_eliminate_count_ = 0;
    uint32_t m_target_level_ = 0;
    /// 撃破数・レベルを0から最終値まで数え上げるのにかける時間(秒)
    float m_count_up_duration_sec_ = 1.0f;

    /// 見出し(クリア/ゲームオーバー)をゆっくり拡縮させる余韻の動き
    float m_idle_pulse_amplitude_ = 0.05f;  ///< 拡縮の幅(基準の大きさに対する割合)
    float m_idle_pulse_period_sec_ = 2.0f;  ///< 1往復にかかる時間(秒)
    bool m_is_idle_motion_active_ = false;
    float m_idle_elapsed_sec_ = 0.0f;
    float m_idle_base_scale_ = 1.0f;

    /// 余韻の動きを開始する。開始時点の見出しの大きさを基準にする。
    void StartIdleMotion();

    /// 評価ランクの判定と表示。カウントアップのあとに表示する
    engine::AssetPtr<ResultRank> m_result_rank_;

    /// 背景を徐々に不透明にしながら表示するコルーチン。
    engine::Task ShowClearSceneBackground() const;
    /// スコアボードを少し大きく拡大してから元の大きさに戻しつつ表示するコルーチン。
    engine::Task ScoreBoardScaleChangeMotion() const;
    /// 撃破数とレベルを0から最終値まで数え上げて表示するコルーチン。
    /// TimeScaleが0の間も動くよう、UnscaledDeltaTimeで進める。
    engine::Task CountUpResultValues() const;
    /// 背景の表示、スコアボードの演出、数値のカウントアップの順に実行するコルーチン。
    /// 最後に余韻の動きを開始する。
    engine::Task ShowClearScene();

public:
    void OnInspectorGui() override;
    void OnStart() override;
    /// 有効化されたときにクリア画面の演出を開始する。
    void OnEnabled() override;
    /// 余韻の動き(見出しの拡縮)を進める。TimeScaleが0でも動くようUnscaledDeltaTimeを使う。
    void OnUpdate() override;

    /// 変更したマテリアルの状態を初期化する
    void OnDestroy() override;
    /// 撃破数とレベルの最終値を受け取り、クリア/ゲームオーバーのどちらの文言を出すか切り替える。
    /// 数値の表示はShowClearSceneのカウントアップで行う。
    /// @param clear_scene_data 画面に表示する結果データ
    void SetClearSceneData(const ClearSceneData& clear_scene_data);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            
            CEREAL_NVP(m_eliminated_slime_count_),
            CEREAL_NVP(m_level_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_background_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_score_board_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_game_over_text_),
                CEREAL_NVP(m_clear_text_)
            );
        }

        if (version >= 6)
        {
            ar(
                CEREAL_NVP(m_text_ui_)
            );
        }

        if (version >= 7)
        {
            ar(
                CEREAL_NVP(m_restart_scene_transition_controller_),
                CEREAL_NVP(m_quit_scene_transition_controller_),
                CEREAL_NVP(m_restart_button_),
                CEREAL_NVP(m_quit_button_)
            );
        }

        if (version >= 8)
        {
            ar(
                CEREAL_NVP(m_count_up_duration_sec_)
            );
        }

        if (version >= 9)
        {
            ar(
                CEREAL_NVP(m_idle_pulse_amplitude_),
                CEREAL_NVP(m_idle_pulse_period_sec_)
            );
        }

        // v10ではランクの設定をこのクラスで持っていた。ResultRankに移したので、v10のシーンからは読み捨てる
        if (version == 10)
        {
            engine::AssetPtr<engine::TextRenderer> unused_rank_text;
            int unused_threshold = 0;
            float unused_float = 0.0f;
            ar(
                cereal::make_nvp("m_rank_text_", unused_rank_text),
                cereal::make_nvp("m_rank_s_threshold_", unused_threshold),
                cereal::make_nvp("m_rank_a_threshold_", unused_threshold),
                cereal::make_nvp("m_rank_b_threshold_", unused_threshold),
                cereal::make_nvp("m_rank_c_threshold_", unused_threshold),
                cereal::make_nvp("m_rank_pop_scale_", unused_float),
                cereal::make_nvp("m_rank_pop_duration_sec_", unused_float)
            );
        }

        if (version >= 11)
        {
            ar(
                CEREAL_NVP(m_result_rank_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ClearSceneController, 11)