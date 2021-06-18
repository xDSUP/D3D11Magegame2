#include "Basic.hlsli"

float4 main(VertexPosHWNormalTangentTex pIn) : SV_Target
{
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    texColor = g_TexDiffuse.Sample(g_SamLinear, pIn.Tex);
    clip(texColor.a - 0.1f);

    pIn.NormalW = normalize(pIn.NormalW);

    float3 toEyeW = normalize(g_EyePosW - pIn.PosW);
    float distToEye = distance(g_EyePosW, pIn.PosW);

    float3 normalMapSample = g_TexNormal.Sample(g_SamLinear, pIn.Tex).rgb;
    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pIn.NormalW, pIn.TangentW);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i;

    [unroll]
    for (i = 0; i < g_NumDirLight; ++i)
    {
        ComputeDirectionalLight(g_Material, g_DirLight[i], bumpedNormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    [unroll]
    for (i = 0; i < g_NumPointLight; ++i)
    {
        ComputePointLight(g_Material, g_PointLight[i], pIn.PosW, bumpedNormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    [unroll]
    for (i = 0; i < g_NumSpotLight; ++i)
    {
        ComputeSpotLight(g_Material, g_SpotLight[i], pIn.PosW, bumpedNormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    float4 litColor = texColor * (ambient + diffuse) + spec;

    //if (g_ReflectionEnabled)
    //{
    //    float3 incident = -toEyeW;
    //    float3 reflectionVector = reflect(incident, pIn.NormalW);
    //    float4 reflectionColor = g_TexCube.Sample(g_Sam, reflectionVector);
    //
    //    litColor += g_Material.Reflect * reflectionColor;
    //}
    //if (g_RefractionEnabled)
    //{
    //    float3 incident = -toEyeW;
    //    float3 refractionVector = refract(incident, pIn.NormalW, g_Eta);
    //    float4 refractionColor = g_TexCube.Sample(g_Sam, refractionVector);
    //
    //    litColor += g_Material.Reflect * refractionColor;
    //}

    litColor.a = texColor.a * g_Material.Diffuse.a;
    return litColor;
}
