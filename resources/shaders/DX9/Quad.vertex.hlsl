struct VSInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
    float TexIndex : TEXCOORD1;
};

struct VSOutput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
    float TexIndex : TEXCOORD1;
};

float4x4 ViewProjection : register(c0);

VSOutput main(VSInput input)
{
    VSOutput output;
    output.Position = mul(float4(input.Position, 1.0f), ViewProjection);
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    output.TexIndex = input.TexIndex;
    return output;
}
