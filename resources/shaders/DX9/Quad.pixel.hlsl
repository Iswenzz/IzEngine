SamplerState Textures[16] : register(s0);

struct PSInput
{
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
    float TexIndex : TEXCOORD1;
};

float4 main(PSInput input) : COLOR
{
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // DX9 HLSL doesn't support dynamic array indexing so we branch manually
    int index = (int)input.TexIndex;
    if (index == 0) texColor = tex2D(Textures[0], input.TexCoord);
    else if (index == 1) texColor = tex2D(Textures[1], input.TexCoord);
    else if (index == 2) texColor = tex2D(Textures[2], input.TexCoord);
    else if (index == 3) texColor = tex2D(Textures[3], input.TexCoord);
    else if (index == 4) texColor = tex2D(Textures[4], input.TexCoord);
    else if (index == 5) texColor = tex2D(Textures[5], input.TexCoord);
    else if (index == 6) texColor = tex2D(Textures[6], input.TexCoord);
    else if (index == 7) texColor = tex2D(Textures[7], input.TexCoord);
    else if (index == 8) texColor = tex2D(Textures[8], input.TexCoord);
    else if (index == 9) texColor = tex2D(Textures[9], input.TexCoord);
    else if (index == 10) texColor = tex2D(Textures[10], input.TexCoord);
    else if (index == 11) texColor = tex2D(Textures[11], input.TexCoord);
    else if (index == 12) texColor = tex2D(Textures[12], input.TexCoord);
    else if (index == 13) texColor = tex2D(Textures[13], input.TexCoord);
    else if (index == 14) texColor = tex2D(Textures[14], input.TexCoord);
    else if (index == 15) texColor = tex2D(Textures[15], input.TexCoord);

    return texColor * input.Color;
}
