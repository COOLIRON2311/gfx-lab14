#pragma once

const char* VertexShaderSource = R"(
#version 330 core
in vec3 coord;
in vec2 texcoord;
out vec2 uv;
uniform mat4 mvp;
void main() {
    gl_Position = mvp * vec4(coord / 2, 1.0);
	uv = texcoord;
})";
 
const char* FragShaderSource = R"(
#version 330 core
in vec2 uv;
uniform sampler2D tex;

vec3 Texture()
{
	return texture(tex, uv);
}

void main() {
	vec3 outp = Texture();
	gl_FragColor = vec4(outp, 1.0);
})";

//  ======================================================================

const char* PhongVertexSource = R"(
#version 330 core
in vec3 coord;
in vec2 texcoord;
in vec3 normal;

out vec3 pos;
out vec2 uv;
out vec3 norm;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(coord, 1.0);
    pos = coord;
	uv = texcoord;
    norm = normalize(normal);
}
)";

const char* PhongFragSource = R"(
#version 330 core
in vec3 pos;
in vec2 uv;
in vec3 norm;

uniform struct PointLight {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 atten;
} pointl;

uniform struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} dirl;

uniform struct SpotLight {
    vec3 pos;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutoff;
    vec3 atten;
} spotl;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    float shininess;
} material;

uniform sampler2D tex;
uniform vec3 viewPos;

void main()
{
    vec3 viewDir = normalize(viewPos - pos);

    // ===========
    // Point light
    // ===========
    vec3 lightDir = normalize(pointl.pos - pos); // light direction
    vec3 lightReflDir = reflect(-lightDir, norm); // reflection direction
    float d = length(lightDir); // distance to light

    float NdotL = max(dot(norm, lightDir), 0); // diffuse shading factor
    float RdotV = max(dot(lightReflDir, viewDir), 0); // specular shading factor

    float atten = 1.0f / (pointl.atten.x + pointl.atten.y * d + pointl.atten.z * d * d);
    vec3 spec = pow(RdotV, material.shininess) * pointl.specular * material.specular;
    vec3 diff = NdotL * material.diffuse * pointl.diffuse;

    vec3 r1 = spec + diff;
    // vec3 r1 = material.emission;
    // r1 += material.ambient * pointl.ambient * atten; // ambient
    // r1 += spec * atten; // specular
    // r1 += diff * atten; // diffuse


    // =================
    // Directional light
    // =================
    lightDir = normalize(-dirl.direction);
    lightReflDir = reflect(-lightDir, norm);

    NdotL = max(dot(norm, lightDir), 0);
    RdotV = max(dot(lightReflDir, viewDir), 0);

    spec = pow(RdotV, material.shininess) * dirl.specular * material.specular;
    diff = NdotL * material.diffuse * dirl.diffuse;
    // vec3 r2 = spec + diff;
    vec3 r2 = material.emission;
    r2 += material.ambient * dirl.ambient; // ambient
    r2 += spec; // specular
    r2 += diff; // diffuse

    // ==========
    // Spot light
    // ==========
    lightDir = normalize(spotl.pos - pos);
    float theta = dot(lightDir, -normalize(spotl.direction));
    vec3 r3 = vec3(0.0f);

    if(theta > cos(radians(spotl.cutoff))) {
        lightReflDir = reflect(-lightDir, norm);

        NdotL = max(dot(norm, lightDir), 0);
        RdotV = max(dot(lightReflDir, viewDir), 0);

        spec = pow(RdotV, material.shininess) * spotl.specular * material.specular;
        diff = NdotL * material.diffuse * spotl.diffuse;
        r3 = spec + diff;
        // r3 = material.emission;
        // r3 += material.ambient * spotl.ambient; // ambient
        // r3 += spec; // specular
        // r3 += diff; // diffuse
    }


    vec3 res = r2;
    // vec3 res = r1 + r2 + r3;
    // res += dirl.ambient * material.ambient + material.emission;
    res *= vec3(texture(tex, uv));

    gl_FragColor = vec4(min(res, 1.0f), 1.0f);
}
)";


































const char* VertexShaderSource2 = R"(
#version 330 core
in vec3 coord;
in vec2 texcoord;
in vec3 normal;

// ��������� ��������������
uniform struct Transform {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat3 normal;
    vec3 viewPosition;
} transform;

// ��������� ���������� ��������� ���������
uniform struct PointLight {
    vec4 coord;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
} light;

// ��������� ������������� ��������� ���������
uniform struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light2;

// ��������� ����������
uniform struct SpotLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    vec3 direction;
    float cutoff;
    float exponent;
} light3;

// ��������� ��� ������������ �������
out Vertex {
    vec2 texcoord;
    vec3 normal;
    vec3 lightDir;
    vec3 viewDir;
    float distance;
} Vert;

void main(void) {
	// �������� ���������� ������� � ������� ������� ���������
	vec4 vertex = transform.model * vec4(coord, 1.0);
	// ����������� �� ������� �� �������� ���������
	//� ������� ������� ���������
	vec4 lightDir = light.coord - vertex;

	// ��������� ���������� ������� � ����������
	gl_Position = transform.viewProjection * vertex;

	//��������� �� ����������� ������ ��������� ���������
    // ������� ���������� ����������
    Vert.texcoord = texcoord;

    // ������� ������� � ������� ������� ���������
    Vert.normal = transform.normal * normal;

    //������� ����������� �� �������� ���������
    Vert.lightDir = vec3(lightDir);

    //������� ����������� �� ������� K �����������
    //� ������� ������� ���������
    Vert.viewDir = transform.viewPosition - vec3(vertex);
    // ������� ���������� �� ������� �� ��������� ���������
    Vert.distance=length(lightDir);
}
)";

const char* FragShaderSource2 = R"(
#version 330 core
out vec4 color;

in Vertex {
    vec2 texcoord;
    vec3 normal;
    vec3 lightDir;
    vec3 viewDir;
    float distance;
} Vert;

// ��������� ���������� ��������� ���������
uniform struct PointLight {
    vec4 coord;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
} light;

// ��������� ������������� ��������� ���������
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light2;

// ��������� ����������
struct SpotLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    vec3 direction;
    float cutoff;
    float exponent;
} light3;

// ��������� ���������
uniform struct Material {
    sampler2D texture;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shininess;
} material;

void CalcPointLight() {
    // ����������� �������
    vec3 normal = normalize(Vert.normal);
    // ����������� ����������� �� �������� ���������
    vec3 lightDir = normalize(Vert.lightDir);
    // ����������� ����������� �� ������� � �����������
    vec3 viewDir = normalize(Vert.viewDir);

    // ��������� ����������� ���������
    float attenuation = 1.0 / (light.attenuation.x +
    light.attenuation.y * Vert.distance +
    light.attenuation.z * Vert.distance * Vert.distance);

    // ���������� ������������ �������� ���������
    color = material.emission;

    // ���������� �������� ���������
    color += material.ambient * light.ambient * attenuation;

    // ���������� ����������� ���������
    float Ndot = max(dot(normal, lightDir), 0.0);
    color += material.diffuse * light.diffuse * Ndot * attenuation;

    // ���������� ���������� ���������
    float RdotVpow = max(
        pow(dot(reflect(-lightDir, normal), viewDir),
        material.shininess), 0.0);
    color += material.specular * light.specular * RdotVpow * attenuation;
}

void main(void){

    // ��������� ���������
    CalcPointLight();

    // ��������� ��������
    color *= texture(material.texture, Vert.texcoord);
}
)";

// ======================================================================

// ������ ������� ���� ���� ��������

const char* VertexShaderLightSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
})";

const char* FragmentShaderLightSource = R"(
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    result += CalcPointLight(pointLight, norm, FragPos, viewDir);    
    // phase 3: spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
)";