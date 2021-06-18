#include "LightHelper.hlsli"

Texture2D g_TexDiffuse : register(t0);
Texture2D g_TexNormal : register(t1);
SamplerState g_SamLinear : register(s0);


cbuffer VSConstantBuffer : register(b0)
{
    float4x4 WVP;
    float4x4 g_World;
    float4x4 g_WorldInvTranspose;
}

cbuffer PSConstantBuffer : register(b1)
{
    DirectionalLight g_DirLight[4] ;
    PointLight g_PointLight[10];
    SpotLight g_SpotLight[4];
    Material g_Material;
    float3 g_EyePosW;
    float g_NumDirLight;
    float g_NumPointLight;
    float g_NumSpotLight;
    float g_Pad1;
    float g_Pad2;
	
}

struct VertexPosNormalTex
{
	float3 PosL : POSITION;
    float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexPosHWNormalTex
{
	float4 PosH : SV_POSITION;
    float3 PosW : POSITION;     // 在世界中的位置
    float3 NormalW : NORMAL;    // 法向量在世界中的方向
	float2 Tex : TEXCOORD;
};

struct VertexPosHTex
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

struct VertexPosNormalTangentTex
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float4 TangentL : TANGENT;
    float2 Tex : TEXCOORD;
};


struct VertexPosHWNormalTangentTex
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION; // 在世界中的位置
    float3 NormalW : NORMAL; // 法向量在世界中的方向
    float4 TangentW : TANGENT; // 切线在世界中的方向
    float2 Tex : TEXCOORD;
};










