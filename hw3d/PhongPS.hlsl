cbuffer LightCBuffer
{
    float3 lightPos;
    static const float3 materialColor;
    static const float3 ambient;
    static const float3 diffuseColor;
    static const float diffuseIntensity;
    static const float attConst;
    static const float attLin;
    static const float attQuau;
};

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
    // fragment to light vector data
    const float3 vTol = lightPos - worldPos;
    const float distTol = length(vTol);
    const float3 dirTol = vTol / distTol;
    // diffuse attenuation
    const float att = 1 / (attConst + attLin * distTol + attQuau * (distTol * distTol));
    // diffuse intensity
    const float diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(n, dirTol));
    // final color
    return float4(saturate(diffuse + ambient) * materialColor, 1.0f);
}