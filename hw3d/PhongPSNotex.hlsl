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
    float4 materialColor;
    float4 specularColor;
    float specularPower;
};

float4 main(float3 viewPos : Position, float3 viewNormal : Normal) : SV_Target
{
    const float3 vTol = lightPos - viewPos;
    const float distTol = length(vTol);
    const float3 dirTol = vTol / distTol;
    
    const float att = 1.0f / (attConst + attLin * distTol + attQuad * (distTol * distTol));
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(vTol, viewNormal));

    const float3 w = viewNormal * dot(vTol, viewNormal);
    const float3 r = w * 2.0f - vTol;

    const float4 specular = att * (float4(diffuseColor, 1.0f) * diffuseIntensity) * specularColor * pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);
    return saturate(float4(diffuse + ambient, 1.0f) * materialColor + specular);
}