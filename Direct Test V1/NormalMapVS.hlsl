#include "Basic.hlsli"

VertexPosHWNormalTangentTex main(VertexPosNormalTangentTex vIn)
{
    VertexPosHWNormalTangentTex vOut;


    vOut.PosH = mul(float4(vIn.PosL, 1.0), WVP);
    vOut.PosW = mul(float4(vIn.PosL, 1.0), g_World).xyz;
    vOut.NormalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);
    vOut.TangentW = mul(vIn.TangentL, g_World);
    vOut.Tex = vIn.Tex;
    return vOut;
}