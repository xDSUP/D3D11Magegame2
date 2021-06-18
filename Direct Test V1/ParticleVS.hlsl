cbuffer buf
{
	float4x4 WVP;
	float4 color;
	float4 offset;
}


struct VertexInputType
{
	float3 Pos:POSITION;
	float2 Tex:TEXCOORD;
};

struct PixelInputType
{
	float4 Pos: SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	output.Pos = mul(float4(input.Pos,1), WVP) + offset;
	output.Tex = input.Tex;
	output.Color = color;

	return output;
}