const char* vertexShader = R"(
#version 440

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Projection;

out vec3 fragPos;
out vec3 normal;

void main() {
    fragPos = vec3(u_Model * vec4(a_Pos, 1.0));
    normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    gl_Position = u_Projection * u_View * vec4(fragPos, 1.0);
}
)";

const char* fragmentShader = R"(
#version 440

in vec3 fragPos;
in vec3 normal;

uniform vec3 u_Color;
uniform vec3 u_lightPos;

out vec4 fragColor;

void main() {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(u_lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_Color;;
    fragColor = vec4(diffuse, 1.0);
}
)";