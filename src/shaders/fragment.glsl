#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main() {
    // Ambient
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength * vec3(texture(u_Texture, TexCoord));

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(u_Texture, TexCoord));

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(texture(u_Texture, TexCoord));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}