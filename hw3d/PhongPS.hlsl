cbuffer LightCBuffer
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

Texture2D tex;

SamplerState splr;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float2 tc : Texcood) : SV_Target
{
    viewNormal = normalize(viewNormal);
    
    // fragment to light vector data
    const float3 vTol = lightPos - viewPos;
    const float distTol = length(vTol);
    const float3 dirTol = vTol / distTol;
    // diffuse attenuation
    const float att = 1 / (attConst + attLin * distTol + attQuad * (distTol * distTol));
    // diffuse intensity
    const float diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewNormal, dirTol));
    // reflected light vector
    const float3 w = viewNormal * dot(viewNormal, vTol);
    const float3 r = w * 2.0f - vTol;
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(saturate(dot(normalize(r), normalize(-viewPos))), specularPower);
    // final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f) * tex.Sample(splr, tc);
}