#version 140

in vec2 vTexCoord;
in vec3 vColor;
in vec3 vNormal;
in vec3 vPos;
in float distanceFromCamera;

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
    vec3 direction;      //direction for directionalLight and spotLight direction
    vec3 position;       //position for pointLight
    float spotCosCutOff; //cosine of the spotlight's half angle
    float spotExponent;  // distribution of the light energy within the spotlight's cone (center -> cone's edge)
    float constant;      //constant attenuation
    float linear;        //linear attenuation
    float quadratic;     //quadratic attenuation
};

uniform Light moonLight;
uniform Light firefly;
uniform Light lamp_1;
uniform Light lantern;

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

    //only calculate it if its hitting the front of the face
    if (diff > 0.0) {
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

    //N vector
    vec3 N = normalize(vNormal);

    //L vector
    vec3 lightVPos = vec3(Vmatrix * vec4(light.position, 1.0)); //we cannot remove the translation in point light!!!
    vec3 lightVector = lightVPos - vPos; //vector pointing from the light to the fragment
    float distanceToLight = length(lightVector);
    vec3 L = normalize(lightVPos - vPos);

    //V vector
    vec3 V = normalize(-vPos);

    // AMBIENT
    vec3 ambient = light.ambient * material.ambient;

     // DIFFUSE
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // SPECULAR
    vec3 specular = vec3(0.0);

    //only calculate it if its hitting the front of the face
    //TODO is this neccessary???
    if (true) { 
        vec3 R = reflect(-L, N);
        float shininess = max(material.shininess, 0.01); //safety check, because 0.0 shininess causes problems
        float spec = pow(max(dot(V, R), 0.0), shininess);

        specular = light.specular * spec * material.specular;
    }

    float attenuation = 1.0 / (light.quadratic*(distanceToLight*distanceToLight) + light.linear*(distanceToLight) + light.constant);

    result = attenuation*(ambient + diffuse + specular);


    return result;
}

vec3 computeSpotLight(Light light) {
    vec3 result = vec3(0.0);

    //N vector
    vec3 N = normalize(vNormal);

    //L vector
    vec3 lightVPos = vec3(Vmatrix * vec4(light.position, 1.0)); 
    vec3 lightVector = lightVPos - vPos; 
    float distanceToLight = length(lightVector);
    vec3 L = normalize(lightVPos - vPos);

    //V vector
    vec3 V = normalize(-vPos);

    vec3 viewLightDir = normalize(mat3(Vmatrix) * light.direction);
    float spotCos = dot(-L, viewLightDir);
    float spotEffect = pow(max(spotCos,0.0), light.spotExponent);

    // AMBIENT
    vec3 ambient = vec3(0.0);

     // DIFFUSE
    vec3 diffuse = vec3(0.0);

    // SPECULAR
    vec3 specular = vec3(0.0);

    //only calculate it if its hitting the front of the face
    //TODO is this neccessary???
    if (spotCos > light.spotCosCutOff) { 
        ambient = spotEffect* light.ambient * material.ambient;

        float diff = max(dot(N, L), 0.0);
        diffuse = spotEffect *light.diffuse * diff * material.diffuse;

        vec3 R = reflect(-L, N);
        float shininess = max(material.shininess, 0.01); //safety check, because 0.0 shininess causes problems
        float spec = pow(max(dot(V, R), 0.0), shininess);

        specular = spotEffect* light.specular * spec * material.specular;
    }

    float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + light.quadratic * (distanceToLight * distanceToLight));

    result = attenuation*(ambient + diffuse + specular);

    return result;
}


void main() {
    
    //choose texture or plain color
    vec4 baseColor = hasTexture ? texture(texSampler, vTexCoord): vec4(vColor, alpha); 
    
    //LIGHTING ==========================================================
    vec3 lightsResult = vec3(0.0);

    //global ambient lighting
    vec3 globalAmbientLight = vec3(0.1, 0.1, 0.05);
    vec3 ambientLight = vec3(material.ambient * globalAmbientLight);

    //compute all lights
    vec3 directionalLight_1 = computeDirectionalLight(moonLight);
    vec3 pointLight_1       = computePointLight(firefly);
    vec3 spotLight_1        = computeSpotLight(lamp_1);
    vec3 pointLight_2       = computePointLight(lantern);

    //testing ----------------------------------
    Light flashlight;
    flashlight.ambient = vec3(0.0, 0.0, 0.0);
    flashlight.diffuse = vec3(0.9, 0.9, 1.0);       // Cold LED white/blue
    flashlight.specular = vec3(1.0, 1.0, 1.0);      // High glare
    flashlight.position = vec3(0.0, 2.0, 0.0);      // Hardcoded world position
    flashlight.direction = vec3(0.0, -1.0, 0.0);    // Hardcoded pointing forward (-Z)
    flashlight.constant = 1.0;
    flashlight.linear = 0.045;                      // Good for ~50 meters
    flashlight.quadratic = 0.0075;
    flashlight.spotCosCutOff = 0.965;               // ~15 degree cone
    flashlight.spotExponent = 40.0;                 // Soft edge

    //vec3 spotLight_1 = computeSpotLight(flashlight);

    //add all lights to output light vector
    lightsResult += ambientLight;
    lightsResult += directionalLight_1;
    lightsResult += pointLight_1;
    lightsResult += spotLight_1;
    lightsResult += pointLight_2;
    //===================================================================

    //FOG ===============================================================
    vec3 fogColor = vec3(0.0, 0.1953, 0.5039);
    float fogDensity = 0.1;
    float fogFactor = exp(-(pow(fogDensity * distanceFromCamera,2.0)));
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    //===================================================================

    //multiply base color with result light and mix with fog factor
    vec3 finalColor = mix(fogColor, baseColor.rgb * lightsResult, fogFactor);
    //vec3 finalColor = baseColor.rgb * lightsResult;

    fragmentColor = vec4(finalColor, baseColor.a);
}