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

Texture2D tex : register(t0);
Texture2D nmap : register(t2);

SamplerState splr;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float3 tan : Tangent, float3 bitan : Bitangent,float2 tc : Texcoord) : SV_TARGET
{
    if (normalMappingEnabled)
    {
        const float3x3 tanToView = float3x3(
            normalize(tan),
            normalize(bitan),
            normalize(viewNormal)
        );
        
        const float3 normalSample = nmap.Sample(splr, tc).xyz;
        float3 tanNormal;
        tanNormal = normalSample * 2.0f - 1.0f;
        tanNormal.y *= -1.0f;
        tanNormal.z *= -1.0f;
        
        viewNormal = normalize(mul(tanNormal, tanToView));
    }
    const float3 vToL = lightPos - viewPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;

    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));

    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, viewNormal));

    const float3 w = viewNormal * dot(vToL, viewNormal);
    const float3 r = 2.0f * w - vToL;
    
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);
    
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}