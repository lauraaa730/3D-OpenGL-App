#version 140

in vec2 vTexCoord;
in vec3 vColor;
in vec3 vNormal;
in vec3 vPos;

uniform sampler2D texSampler;
uniform bool hasTexture;
uniform mat4 Vmatrix;
uniform float alpha;

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
    vec3 direction;     //direction for directionalLight and spotLight direction
    vec3 position;      //position for pointLight
    float spotCosCutOff; //cosine of the spotlight's half angle
    float spotExponent;  // distribution of the light energy within the spotlight's cone (center -> cone's edge)
};

uniform Light moonLight;
uniform Light firefly;

out vec4 fragmentColor;

vec3 computeDirectionalLight(Light light) {
    vec3 result = vec3(1.0);

    vec3 N = normalize(vNormal);
    vec3 L = normalize(mat3(Vmatrix) * -light.direction);   // light direction
    vec3 V = normalize(-vPos);                 // camera is at origin

    // AMBIENT
    vec3 ambient = light.ambient * material.ambient;

     // DIFFUSE
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // SPECULAR
    vec3 specular = vec3(0.0);

    if (diff > 0.0) { //only calculate it if its hitting the front of the face
        vec3 R = reflect(-L, N);
        float shininess = max(material.shininess, 0.01); //safety check, because 0.0 shininess causes problems
        float spec = pow(max(dot(V, R), 0.0), shininess);

        specular = light.specular * spec * material.specular;
    }

    result = ambient + diffuse + specular;

    return result;
}

vec3 computePointLight(Light light) {
    vec3 result = vec3(0.0);

    vec3 N = normalize(vNormal);
    vec3 lightVPos = mat3(Vmatrix) * light.position;
    vec3 L = normalize(light.position - vPos);
    vec3 V = normalize(-vPos);

    // AMBIENT
    vec3 ambient = light.ambient * material.ambient;

     // DIFFUSE
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // SPECULAR
    vec3 specular = vec3(0.0);

    if (diff > 0.0) { //only calculate it if its hitting the front of the face
        vec3 R = reflect(-L, N);
        float shininess = max(material.shininess, 0.01); //safety check, because 0.0 shininess causes problems
        float spec = pow(max(dot(V, R), 0.0), shininess);

        specular = light.specular * spec * material.specular;
    }

    result = ambient + diffuse + specular;


    return result;
}


void main() {
    
    //texture or plain color?
    vec4 baseColor = hasTexture ? texture(texSampler, vTexCoord): vec4(vColor, alpha); 
    

    vec3 lightsResult = vec3(0.0);

    //global ambient lighting
    vec3 globalAmbientLight = vec3(0.1, 0.1, 0.05);
    vec3 ambientLight = vec3(material.ambient * globalAmbientLight);

    //compute all lights
    vec3 directionalLight_1 = computeDirectionalLight(moonLight);
    vec3 pointLight_1       = computePointLight(firefly);

    //add all lights to output light vector
    lightsResult += ambientLight;
    lightsResult += directionalLight_1;
    lightsResult += pointLight_1;

    //multiply base color with result light
    fragmentColor = vec4((lightsResult * baseColor.rgb), baseColor.a);
}