#include "pch.h"
#include "level_up_status_view.h"

#include "game_object.h"
#include "Asset/asset_database.h"
#include "Components/camera_component.h"
#include "Components/canvas.h"
#include "Rendering/crs_bridge.h"
#include "../Player/ability/aura_ability.h"
#include "../Player/ability/explosion_ability.h"
#include "../Player/ability/slash_ability.h"

namespace SlimeBoom
{
namespace
{
constexpr float kIconWidthInTexture = 0.293f;
constexpr float kIconHeightInTexture = 0.2015f;
constexpr float kIconCenterFromTop = 0.2608f;

constexpr float kSkillColumnX = -975.0f;
constexpr float kSkillHeaderY = 330.0f;
constexpr float kSkillFirstRowY = 220.0f;
constexpr float kSkillIconSize = 120.0f;
constexpr float kSkillRowSpacing = 145.0f;

constexpr float kStatusIconX = 865.0f;
constexpr float kStatusTextX = 918.0f;
constexpr float kStatusHeaderY = 360.0f;
constexpr float kStatusFirstRowY = 285.0f;
constexpr float kStatusIconSize = 72.0f;
constexpr float kStatusRowSpacing = 84.0f;
constexpr float kStatusHeaderCenterX = 980.0f;

constexpr float kHeaderTextHeight = 44.0f;
constexpr float kValueTextHeight = 38.0f;

constexpr uint16_t kIconRenderQueue = 8101;

const Color kHeaderColor = Color(1.0f, 0.78f, 0.3f, 1.0f);
const Color kValueColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color kEmptyColor = Color(0.7f, 0.7f, 0.7f, 1.0f);

const Vector2 kDefaultCanvasSize = Vector2(2304.0f, 1296.0f);
}

void LevelUpStatusView::Setup(const std::shared_ptr<UiDataProvider>& ui_data_provider)
{
    m_ui_data_provider_ = ui_data_provider;

    // 描画中に作らないよう、使うアイコンのMaterialを先に作っておく
    for (const auto card_type : {kCardType_Slash, kCardType_AuraAbility, kCardType_ExplosionAbility, kCardType_MaxHp,
                                 kCardType_AttackPower, kCardType_AttackSpeed, kCardType_AttackRange, kCardType_AttackNum,
                                 kCardType_Speed, kCardType_AvoidPercent, kCardType_ExpMultiplier,
                                 kCardType_AcquisitionRange})
        GetIconMaterial(card_type);
}

Matrix LevelUpStatusView::BoundsOrigin()
{
    const auto camera = engine::CameraComponent::Main();
    return camera ? camera->GameObject()->Transform()->WorldMatrix() : Matrix::Identity;
}

std::shared_ptr<engine::Material> LevelUpStatusView::GetIconMaterial(const kCardType card_type)
{
    if (const auto it = m_icon_materials_.find(card_type); it != m_icon_materials_.end())
        return it->second;

    const auto provider = m_ui_data_provider_.lock();
    const auto pool = provider ? provider->GetCardPool() : nullptr;
    const auto card = pool ? pool->GetCard(card_type) : nullptr;
    if (card == nullptr)
        return nullptr;

    // カードのImageと同じShaderで、アイコン画像を貼るだけのMaterialを作る
    const auto material = engine::Material::Create("2DBasic.hlsl", std::string("LevelUpStatusIcon_") + ToString(card_type));
    material->render_queue = kIconRenderQueue;
    material->SetTexture("_MainTex", card->GetCardData().icon);
    m_icon_materials_.emplace(card_type, material);
    return material;
}

std::vector<LevelUpStatusView::Row> LevelUpStatusView::CollectSkillRows() const
{
    std::vector<Row> rows;
    const auto provider = m_ui_data_provider_.lock();
    const auto abilities = provider ? provider->GetAbilityProcesser() : nullptr;
    if (abilities == nullptr)
        return rows;

    // 取得した順は残っていないので、決まった順に並べる
    if (abilities->HasAbility<SlashAbility>())
        rows.push_back({kCardType_Slash, ""});
    if (abilities->HasAbility<AuraAbility>())
        rows.push_back({kCardType_AuraAbility, ""});
    if (abilities->HasAbility<ExplosionAbility>())
        rows.push_back({kCardType_ExplosionAbility, ""});
    return rows;
}

std::vector<LevelUpStatusView::Row> LevelUpStatusView::CollectStatusRows() const
{
    const auto provider = m_ui_data_provider_.lock();
    if (provider == nullptr)
        return {};

    const auto& data = provider->GetPlayerData();
    // カードの効果の文字(GetEffectText)と同じ書き方にする
    return {
        {kCardType_MaxHp, CardBase::FormatNumber(data.hp) + " / " + CardBase::FormatNumber(data.max_hp)},
        {kCardType_AttackPower, CardBase::FormatNumber(data.attack_power)},
        {kCardType_AttackSpeed, "x" + CardBase::FormatNumber(data.attack_speed)},
        {kCardType_AttackRange, CardBase::FormatNumber(data.attack_range)},
        {kCardType_AttackNum, std::to_string(data.attack_num)},
        {kCardType_Speed, CardBase::FormatNumber(data.speed)},
        {kCardType_AvoidPercent, CardBase::FormatNumber(data.avoid_percent) + "%"},
        {kCardType_ExpMultiplier, "x" + CardBase::FormatNumber(data.exp_multiplier)},
        {kCardType_AcquisitionRange, CardBase::FormatNumber(provider->GetAcquisitionRange())},
    };
}

Vector2 LevelUpStatusView::CanvasSize() const
{
    const auto canvas = GameObject()->GetComponentInParent<engine::Canvas>();
    if (canvas == nullptr)
        return kDefaultCanvasSize;

    const auto size = canvas->CanvasSize();
    return size.x > 0.0f && size.y > 0.0f ? size : kDefaultCanvasSize;
}

void LevelUpStatusView::DrawIcon(const kCardType card_type, const Vector2 center, const float size)
{
    const auto material = GetIconMaterial(card_type);
    if (material == nullptr)
        return;

    const float quad_width = size / kIconWidthInTexture;
    const float quad_height = size / kIconHeightInTexture;

    const auto quad_center = Vector2(center.x, center.y - (0.5f - kIconCenterFromTop) * quad_height);

    const auto half_canvas = CanvasSize() * 0.5f;
    const auto scale = Matrix::CreateScale(quad_width * 0.5f / half_canvas.x, quad_height * 0.5f / half_canvas.y, 1.0f);
    const auto translation = Matrix::CreateTranslation(quad_center.x / half_canvas.x, quad_center.y / half_canvas.y, 0.0f);
    engine::CrsBridge::AddUiImage(scale * translation, material);
}

void LevelUpStatusView::DrawText(const std::string& text, const Vector2 position, const float height, const float pivot_x,
                                 const Color& color) const
{
    if (m_font_ == nullptr || text.empty())
        return;

    const auto canvas_size = CanvasSize();
    const auto window_size = Vector2(static_cast<float>(engine::Application::WindowWidth()),
                                     static_cast<float>(engine::Application::WindowHeight()));
    const auto pixel_position = Vector2((position.x / canvas_size.x + 0.5f) * window_size.x,
                                        (0.5f - position.y / canvas_size.y) * window_size.y);

    const auto line_size = m_font_->MeasureString("0");
    if (line_size.y <= 0.0f)
        return;
    const float scale = height * (window_size.y / canvas_size.y) / line_size.y;
    const auto text_size = m_font_->MeasureString(text);
    engine::CrsBridge::AddUiText(m_font_, pixel_position, text, color, Vector2(text_size.x * pivot_x, text_size.y * 0.5f),
                                 scale);
}

void LevelUpStatusView::Render()
{
    if (m_font_ == nullptr)
        m_font_ = engine::AssetDatabase::GetAsset<engine::FontData>("Font/Noto Serif JP.spritefont").CastedLock();

    DrawText("SKILL", Vector2(kSkillColumnX, kSkillHeaderY), kHeaderTextHeight, 0.5f, kHeaderColor);
    const auto skill_rows = CollectSkillRows();
    if (skill_rows.empty())
        DrawText("-", Vector2(kSkillColumnX, kSkillFirstRowY), kValueTextHeight, 0.5f, kEmptyColor);
    for (size_t i = 0; i < skill_rows.size(); ++i)
    {
        const float y = kSkillFirstRowY - kSkillRowSpacing * static_cast<float>(i);
        DrawIcon(skill_rows[i].card_type, Vector2(kSkillColumnX, y), kSkillIconSize);
    }

    DrawText("STATUS", Vector2(kStatusHeaderCenterX, kStatusHeaderY), kHeaderTextHeight, 0.5f, kHeaderColor);
    const auto status_rows = CollectStatusRows();
    for (size_t i = 0; i < status_rows.size(); ++i)
    {
        const float y = kStatusFirstRowY - kStatusRowSpacing * static_cast<float>(i);
        DrawIcon(status_rows[i].card_type, Vector2(kStatusIconX, y), kStatusIconSize);
        DrawText(status_rows[i].value, Vector2(kStatusTextX, y), kValueTextHeight, 0.0f, kValueColor);
    }
}
}
