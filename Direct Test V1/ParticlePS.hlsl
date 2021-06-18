Texture2D shaderTexture;
SamplerState sampleType;

struct PixelInputType
{
	float4 Pos: SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return shaderTexture.Sample(sampleType, input.Tex)*input.Color;
}