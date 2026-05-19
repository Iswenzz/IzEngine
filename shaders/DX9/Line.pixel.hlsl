struct PSInput
{
    float4 Color : COLOR;
};

float4 main(PSInput input) : COLOR
{
    return input.Color;
}
