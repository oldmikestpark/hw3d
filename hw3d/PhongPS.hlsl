cbuffer LightCBuffer
{
    float3 lightPos;
    float3 materialColor;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
    // fragment to light vector data
    const float3 vTol = lightPos - worldPos;
    const float distTol = length(vTol);
    const float3 dirTol = vTol / distTol;
    // diffuse attenuation
    const float att = 1 / (attConst + attLin * distTol + attQuad * (distTol * distTol));
    // diffuse intensity
    const float diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(n, dirTol));
    // final color
    return float4(saturate(diffuse + ambient) * materialColor, 1.0f);
}