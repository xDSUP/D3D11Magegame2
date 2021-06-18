cbuffer cbPerObj
{
	float4x4 ortho;
}

struct VertexInputType
{
	float4 pos:POSITION;
	float2 tex:TEXCOORD;
};

struct PixelInputType
{
	float4 Pos:SV_POSITION;
	float2 tex:TEXCOORD;
};

PixelInputType main( VertexInputType input )
{
	PixelInputType output;

	output.Pos = mul(input.pos, ortho);
	output.tex = input.tex;
	
	return output;
}