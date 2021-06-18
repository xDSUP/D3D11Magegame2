#include "Basic.hlsli"

struct VertexInputType
{
	float3 pos: POSITION;
	float3 normal: NORMAL;
	float2 tex: TEXCOORD;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
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
	vOut.tangent = mul(input.tangent, (float3x3)worldMatrix);
	vOut.tangent = normalize(output.tangent);

	// Calculate the binormal vector against the world matrix only and then normalize the final value.
	vOut.binormal = mul(input.binormal, (float3x3)worldMatrix);
	vOut.binormal = normalize(output.binormal);
	return vOut;
}