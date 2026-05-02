#version 140

in vec2 vTexCoord;
in vec3 vColor;
in vec3 vNormal;
in vec3 vPos;

uniform sampler2D texSampler;
uniform bool hasTexture;
uniform mat4 Vmatrix;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

uniform Light moonLight;

out vec4 fragmentColor;


void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(mat3(Vmatrix) * -moonLight.direction);   // light direction
    vec3 V = normalize(-vPos);                 // camera is at origin
   
    vec3 baseColor = hasTexture ? texture(texSampler, vTexCoord).rgb: vColor;

    // AMBIENT
    vec3 ambient = moonLight.ambient * material.ambient;

    // DIFFUSE
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = moonLight.diffuse * diff * material.diffuse;

    // SPECULAR
    vec3 specular = vec3(0.0);

    if (diff > 0.0) { //only calculate it if its hitting the front of the face
        vec3 R = reflect(-L, N);
        float shininess = max(material.shininess, 0.01); //safety check, because 0.0 shininess causes problems
        float spec = pow(max(dot(V, R), 0.0), shininess);

        specular = moonLight.specular * spec * material.specular;
    }

    vec3 result = (ambient + diffuse + specular) * baseColor;

    fragmentColor = vec4(result, 1.0);
}