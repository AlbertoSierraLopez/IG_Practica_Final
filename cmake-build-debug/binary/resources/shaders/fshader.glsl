#version 330 core

struct Light {
    vec3  position;
    vec3  direction;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float innerCutOff;
    float outerCutOff;
    float c0;
    float c1;
    float c2;
};

struct Material {
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    vec4  emissive;
    float shininess;
};

#define NLP 1
#define NCOCHES 2

uniform vec3     ucpos;
uniform Light    ulightG;
uniform Light    ulightP;
uniform Light    ulightF[NCOCHES*2];
uniform Material umaterial;

in  vec3 vnor;
in  vec3 vpos;
out vec4 outColor;

vec3 funDirectional(Light light, Material material, vec3 N, vec3 V);
vec3 funPositional (Light light, Material material, vec3 N, vec3 V);
vec3 funFocal      (Light light, Material material, vec3 N, vec3 V);

void main() {

    vec3 N = normalize(vnor);
    vec3 V = normalize(ucpos - vpos);

    vec3 color = umaterial.emissive.rgb + ulightG.ambient * umaterial.ambient.rgb;
    color += funPositional(ulightP,umaterial,N,V);
    for(int i=0; i<NCOCHES*2; i++) color += funFocal      (ulightF[i],umaterial,N,V);

    outColor = vec4(color, umaterial.diffuse.a);

}

vec3 funDirectional(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(-light.direction);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = max(dot(L,N), 0.0);
    float dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    vec3 color = ambient + diffuse + specular;

    return color;

}

vec3 funPositional(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(light.position - vpos);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = max(dot(L,N), 0.0);
    float dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    float distance    = length(light.position - vpos);
    float attenuation = 1.0/(light.c0 + light.c1*distance + light.c2*pow(distance,2));

    vec3 color = attenuation*(ambient + diffuse + specular);

    return color;

}

vec3 funFocal(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(light.position - vpos);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = max(dot(L,N), 0.0);
    float dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    float distance    = length(light.position - vpos);
    float attenuation = 1.0/(light.c0 + light.c1*distance + light.c2*pow(distance,2));

    vec3  direction   = normalize(-light.direction);
    float intensity   = clamp((light.outerCutOff - dot(L,direction))/(light.outerCutOff - light.innerCutOff), 0.0, 1.0);

    vec3 color = attenuation*(ambient + intensity*(diffuse + specular));

    return color;

}
