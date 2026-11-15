#include "Shaders/crs_ui.hlsli"

// UI(Image)用。_MainTexをそのまま描く

Texture2D _MainTex : register(t0);
SamplerState smp : register(s0);

UiVSOutput VSMain(UiVertexInput input)
{
    return UiTransform(input);
}

float4 PSMain(UiVSOutput input) : SV_TARGET
{
    float4 mainColor = _MainTex.Sample(smp, input.uv);
    return mainColor * input.color;
}
