cbuffer LightCBuffer
{
    float3 lightPos;
};

static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.045;
static const float attQuau = 0.0075;

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