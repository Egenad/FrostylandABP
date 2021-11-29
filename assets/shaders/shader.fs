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
uniform bool silhouette;
uniform bool redraw;

const float levels = 14.0;

vec4  Phong(vec4 pos, vec3 intens){
    
    vec4 n = normalize(vec4(vNormal,1.0));
    vec4 s = normalize(vPosition - pos);
    vec4 v = normalize(vPosition - cameraPos);
    vec4 r = reflect(-s, n);

    vec4 Ambient = vec4(intens * material.kambient,1.0);
    float dif = max(dot(n,s),0.0);
    vec4 Diffuse = vec4(intens * (dif * material.kdiffuse),1.0);
    
    float spec = pow(max(dot(v,r),0.0),32);
    vec4 Specular = vec4(intens * (spec * material.kspecular),1.0);

    return Ambient + Diffuse + Specular;
}

void main(){
	if(silhouette){
        if(redraw == false){
            discard;
        }
		vec4 textureColor = texture(ourTexture,TexCoord);
		FragColor = vec4(0.0,0.0,0.0,1.0);
	}else{
	    vec4 color = vec4(0.0,0.0,0.0,1.0);

	    for(int i = 0;i < numLights; i++){

	        vec4 textureColor = texture(ourTexture,TexCoord);

	        if(textureColor.a < 0.1){
	            discard;
	        }

	        color += textureColor * Phong(ViewMatrix * light[i].position, light[i].intensity);
	    }

	    color.x = floor(color.x * levels) / levels;
	    color.y = floor(color.y * levels) / levels;
	    color.z = floor(color.z * levels) / levels;

	    FragColor = color;
    }
}

/*
#CALCULAR LOS DIFERENTES VECTORES    
    vec3 n = normalize(Normal);
    vec3 s = normalize(Light.Position - Position);
    vec3 v = normalize(-Position);
    vec3 r = reflect(-s, n);

    #COMPONENTE AMBIENTAL
    vec3 Ambient = Light.Ambient * vec3(texture(Material.Diffuse, TexCoords));
    
    #COMPONENTE DIFUSA 
    vec3 Diffuse = Light.Diffuse * max(dot(s, n), 0.0) * vec3(texture(Material.Diffuse, TexCoords));  
    
    #COMPONENTE ESPECULAR  
    vec3 Specular = Light.Specular * pow(max(dot(r, v), 0.0), Material.Shininess) * vec3(texture(Material.Specular, TexCoords));
        
    return Ambient + Diffuse + Specular;  
*/