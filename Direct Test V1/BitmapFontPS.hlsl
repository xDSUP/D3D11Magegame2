
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PixelBuffer
{
    float4 pixelColor;
};

struct PixelInputType
{
    float4 Pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, input.tex) * pixelColor;
}
