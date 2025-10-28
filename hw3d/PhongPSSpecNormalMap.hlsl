#include "PointLight.hlsli"
#include "ShaderOps.hlsli"

cbuffer ObjectCBuf
{
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};

Texture2D tex;
Texture2D spec;
Texture2D nmap;

SamplerState splr;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float3 viewTan : Tangent, float3 viewBitan : Bitangent, float2 tc : Texcoord) : SV_Target
{
    viewNormal = normalize(viewNormal);
    if (normalMapEnabled)
    {
        viewNormal = MapNormal(normalize(viewTan), normalize(viewBitan), viewNormal, tc, nmap, splr);
    }
	// fragment to light vector data
    const float3 viewFragToL = viewLightPos - viewPos;
    const float distFragToL = length(viewFragToL);
    const float3 viewDirFragToL = viewFragToL / distFragToL;
	
	// reflected light vector
    const float3 w = viewNormal * dot(viewFragToL, viewNormal);
    const float3 r = w * 2.0f - viewFragToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if (specularMapEnabled)
    {
        const float4 specularSample = spec.Sample(splr, tc);
        specularReflectionColor = specularSample.rgb * specularMapWeight;
        if (hasGloss)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }

    }
    else
    {
        specularReflectionColor = specularColor;
    }
    
    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, distFragToL);
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirFragToL, viewNormal));
    
    const float3 specularReflected = Speculate(
        specularReflectionColor,
        1.0f, viewNormal,
        viewFragToL, viewPos, att, specularPower
    );
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specularReflected), 1.0f);
}