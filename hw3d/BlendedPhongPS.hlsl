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
    float padding[2];
};

float4 main(float3 worldPos : Position, float3 n : Normal, float3 color : Color) : SV_Target
{
    // fragment to light vector data
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    // attenuation
    const float att = 1.0f / (attConst + distToL * attLin + (distToL * distToL) * attQuad);
    // diffuse intensity
    const float3 diffuse = att * diffuseIntensity * diffuseColor * max(0.0f, dot(n, dirToL));
    // reflected light vector
    const float3 w = n * dot(n, vToL);
    const float3 r = w * 2.0f - vToL;
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (diffuseIntensity * diffuseColor) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(vToL))), specularPower);
    // final result
    return float4(saturate(diffuse + specular + ambient), 1.0f);
}