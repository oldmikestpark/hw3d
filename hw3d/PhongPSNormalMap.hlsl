cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    bool normalMappingEnabled;
    float padding[1];
};

Texture2D tex;
Texture2D nmap;

SamplerState splr;

float4 main(float3 worldPos : Position, float3 n : Normal, float2 tc : Texcoord) : SV_TARGET
{
    if (normalMappingEnabled)
    {
        n = -nmap.Sample(splr, tc).xyz;
    }
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;

    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));

    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

    const float3 w = n * dot(vToL, n);
    const float3 r = 2.0f * w - vToL;
    
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
    
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}