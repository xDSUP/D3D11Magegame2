#include "Basic.hlsli"

struct VertexInputType
{
	float3 pos: POSITION;
	float3 normal: NORMAL;
	float2 tex: TEXCOORD;
};


VertexPosHWNormalTex main(VertexInputType input)
{
	VertexPosHWNormalTex vOut;
	//matrix viewProj = mul(g_View, g_Proj);
	//float4 posW = mul(float4(input.pos, 1.0f), g_World);

	vOut.PosH = mul(float4(input.pos, 1.0), WVP);
	vOut.PosW = mul(float4(input.pos, 1.0), g_World).xyz;
	vOut.NormalW = mul(input.normal, (float3x3) g_WorldInvTranspose);
	vOut.Tex = input.tex;
	// Calculate the tangent vector against the world matrix only and then normalize the final value.
	return vOut;
}