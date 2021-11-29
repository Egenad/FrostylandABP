#version 400
out vec4 FragColor;

in vec2 TexCoord;
in vec3 vNormal;
in vec4 vPosition;

struct Material {
    vec3 kdiffuse;
    vec3 kambient;
    vec3 kspecular;
    float kglossiness;
};

struct Light {
    vec4 position;
    vec3 intensity;
};

uniform int numLights;
uniform Light light[10];
uniform sampler2D ourTexture;
uniform Material material;
uniform mat4 ViewMatrix;
uniform vec4 cameraPos;

vec4  Phong(vec4 pos, vec3 intens){
    
    vec4 n = normalize(vec4(vNormal,1.0));
    vec4 s = normalize(vPosition - pos);
    vec4 v = normalize(vPosition - cameraPos);
    vec4 r = reflect(-s, n);

    vec4 Ambient = vec4(intens * material.kambient,1.0);
    
    float dif = max(dot(n,s),0.0); 
    vec4 Diffuse = vec4(intens * (dif * material.kdiffuse),1.0);
    
    float spec = pow(max(dot(v,r),0.0),material.kglossiness);
    vec4 Specular = vec4(intens * (spec * material.kspecular),1.0);

    return Ambient + Diffuse + Specular;
}

void main(){
    vec4 color = vec4(1.0,1.0,1.0,1.0);

    for(int i = 0;i < numLights; i++){

        color += texture(ourTexture,TexCoord) * Phong(ViewMatrix * light[i].position, light[i].intensity);
    }

    FragColor = color ;
}
