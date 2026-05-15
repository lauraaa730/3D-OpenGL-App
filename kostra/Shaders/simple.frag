/**
 * \file    simple.frag
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Main fragment shader implementing directional, point, and spot lights, plus exponential fog.
 */
#version 140

in vec2 vTexCoord;           ///< \brief Interpolated texture coordinates.
in vec3 vColor;              ///< \brief Interpolated vertex color.
in vec3 vNormal;             ///< \brief Interpolated view-space normal.
in vec3 vPos;                ///< \brief Interpolated view-space position.
in float distanceFromCamera; ///< \brief Distance from camera for fog application.

uniform sampler2D texSampler;///< \brief Texture sampler.
uniform bool hasTexture;     ///< \brief Flag indicating if the object is textured.
uniform mat4 Vmatrix;        ///< \brief View matrix for light transformations.
uniform float alpha;         ///< \brief Global transparency value for the object.

/**
 * \brief Structure defining surface material properties.
 */
struct Material {
    vec3 ambient;    ///< \brief Ambient reflection coefficient.
    vec3 diffuse;    ///< \brief Diffuse reflection coefficient.
    vec3 specular;   ///< \brief Specular reflection coefficient.
    float shininess; ///< \brief Specular highlight exponent.
};

uniform Material material; ///< \brief Material properties of the current object.

/**
 * \brief Structure defining generic light properties used for all light types.
 */
struct Light {
    vec3 ambient;        ///< \brief Ambient intensity.
    vec3 diffuse;        ///< \brief Diffuse intensity.
    vec3 specular;       ///< \brief Specular intensity.
    vec3 direction;      ///< \brief Direction (for directional and spot lights).
    vec3 position;       ///< \brief Position (for point and spot lights).
    float spotCosCutOff; ///< \brief Cosine of the spotlight's half angle.
    float spotExponent;  ///< \brief Distribution of light energy within the spotlight cone.
    float constant;      ///< \brief Constant attenuation factor.
    float linear;        ///< \brief Linear attenuation factor.
    float quadratic;     ///< \brief Quadratic attenuation factor.
};

uniform Light moonLight; ///< \brief Directional light simulating the moon.
uniform Light firefly;   ///< \brief Point light simulating a firefly.
uniform Light lamp_1;    ///< \brief Spotlight simulating a street lamp.
uniform Light lantern;   ///< \brief Point light simulating a lantern.

out vec4 fragmentColor;  ///< \brief Final output color of the fragment.

/**
 * \brief Computes lighting contribution from a directional light.
 * \param light The Light structure containing directional light properties.
 * \return Resulting RGB color contribution from this light.
 */
vec3 computeDirectionalLight(Light light) {
    vec3 result = vec3(1.0);

    vec3 N = normalize(vNormal);
    vec3 L = normalize(mat3(Vmatrix) * -light.direction);   //light direction in view space
    vec3 V = normalize(-vPos);                 //camera is at origin in view space

    //ambient
    vec3 ambient = light.ambient * material.ambient;

     //diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    //specular
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

/**
 * \brief Computes lighting contribution and attenuation from a point light.
 * \param light The Light structure containing point light properties.
 * \return Resulting RGB color contribution from this light.
 */
vec3 computePointLight(Light light) {

    vec3 result = vec3(0.0);

    //n vector
    vec3 N = normalize(vNormal);

    //l vector
    vec3 lightVPos = vec3(Vmatrix * vec4(light.position, 1.0)); //we cannot remove the translation in point light!!!
    vec3 lightVector = lightVPos - vPos; //vector pointing from the light to the fragment
    float distanceToLight = length(lightVector);
    vec3 L = normalize(lightVPos - vPos);

    //v vector
    vec3 V = normalize(-vPos);

    //ambient
    vec3 ambient = light.ambient * material.ambient;

     //diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    //specular
    vec3 specular = vec3(0.0);

    //only calculate it if its hitting the front of the face
    //todo is this neccessary???
    if (true) { 
        vec3 R = reflect(-L, N);
        float shininess = max(material.shininess, 0.01); //safety check, because 0.0 shininess causes problems
        float spec = pow(max(dot(V, R), 0.0), shininess);

        specular = light.specular * spec * material.specular;
    }

	//calculate attenuation based on distance
    float attenuation = 1.0 / (light.quadratic*(distanceToLight*distanceToLight) + light.linear*(distanceToLight) + light.constant);

    result = attenuation*(ambient + diffuse + specular);

    return result;
}

/**
 * \brief Computes lighting contribution, attenuation, and cone restrictions from a spotlight.
 * \param light The Light structure containing spotlight properties.
 * \return Resulting RGB color contribution from this light.
 */
vec3 computeSpotLight(Light light) {
    vec3 result = vec3(0.0);

    //n vector
    vec3 N = normalize(vNormal);

    //l vector
    vec3 lightVPos = vec3(Vmatrix * vec4(light.position, 1.0)); 
    vec3 lightVector = lightVPos - vPos; 
    float distanceToLight = length(lightVector);
    vec3 L = normalize(lightVPos - vPos);

    //v vector
    vec3 V = normalize(-vPos);

    vec3 viewLightDir = normalize(mat3(Vmatrix) * light.direction);
    float spotCos = dot(-L, viewLightDir);
    float spotEffect = pow(max(spotCos,0.0), light.spotExponent);

    //ambient
    vec3 ambient = vec3(0.0);

     //diffuse
    vec3 diffuse = vec3(0.0);

    //specular
    vec3 specular = vec3(0.0);

    //only calculate it if its inside the spotlight cone
    if (spotCos > light.spotCosCutOff) { 
        ambient = spotEffect* light.ambient * material.ambient;

        float diff = max(dot(N, L), 0.0);
        diffuse = spotEffect *light.diffuse * diff * material.diffuse;

        vec3 R = reflect(-L, N);
        float shininess = max(material.shininess, 0.01); //safety check, because 0.0 shininess causes problems
        float spec = pow(max(dot(V, R), 0.0), shininess);

        specular = spotEffect* light.specular * spec * material.specular;
    }

	//calculate attenuation based on distance
    float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + light.quadratic * (distanceToLight * distanceToLight));

    result = attenuation*(ambient + diffuse + specular);

    return result;
}

/**
 * \brief Main fragment shader entry point.
 * \details Aggregates texture/color data, evaluates all scene lights, and applies exponential fog.
 */
void main() {
    
    //choose texture or plain color
    vec4 baseColor = hasTexture ? texture(texSampler, vTexCoord): vec4(vColor, alpha); 
    
    //lighting ==========================================================
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
    flashlight.diffuse = vec3(0.9, 0.9, 1.0);       //cold LED white/blue
    flashlight.specular = vec3(1.0, 1.0, 1.0);      //high glare
    flashlight.position = vec3(0.0, 2.0, 0.0);      //hardcoded world position
    flashlight.direction = vec3(0.0, -1.0, 0.0);    //hardcoded pointing forward (-Z)
    flashlight.constant = 1.0;
    flashlight.linear = 0.045;                      //good for ~50 meters
    flashlight.quadratic = 0.0075;
    flashlight.spotCosCutOff = 0.965;               //~15 degree cone
    flashlight.spotExponent = 40.0;                 //soft edge

    //vec3 spotLight_2 = computeSpotLight(flashlight);

    //add all lights to output light vector
    lightsResult += ambientLight;
    lightsResult += directionalLight_1;
    lightsResult += pointLight_1;
    lightsResult += spotLight_1;
    //lightsResult += spotLight_2; //test hardcoded light
    lightsResult += pointLight_2;
    //===================================================================

    //fog ===============================================================
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