#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex; // Render texture

struct MaterialInfo
{
    vec4 Kd;
    vec4 Ks;
    vec4 Ka;
    float Shininess;
};
uniform MaterialInfo Material;

struct LightInfo
{
    vec4 Position;
    vec4 Intensity;
};
uniform LightInfo Lights[10];

out vec4 FragColor;

vec3 phongModel(int lightIndex, vec3 pos, vec3 norm)
{
    vec3 s = normalize(vec3(Lights[lightIndex].Position) - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect(-s, norm);
    vec3 ambient = Lights[lightIndex].Intensity.xyz * Material.Ka.xyz;
    float sDotN = max(dot(s, norm), 0.0);
    vec3 diffuse = texture(RenderTex, TexCoord).xyz * Lights[lightIndex].Intensity.xyz * sDotN;
    return diffuse;
    vec3 spec = vec3(0.0);
    if(sDotN > 0.0)
    {
        spec = Lights[lightIndex].Intensity.xyz * Material.Ks.xyz * pow(max(dot(r,v), 0.0), Material.Shininess);
    }
    return ambient + diffuse + spec;
}

void main()
{
    vec3 color = vec3(0.0);
    for(int i = 0; i < 10; i++)
    {
        color += phongModel(i, Position, Normal);
    }
    FragColor = vec4(color, 1.0);
}