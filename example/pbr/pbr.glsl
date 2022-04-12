#vertex shader
#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2 ) in vec2 a_uv;


uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_view;

out vec3 o_normal;
out vec3 o_pos;
void main()
{
	gl_Position=u_projection*u_view*u_model*vec4(a_pos,1.0f);
	o_pos=vec3(u_model*vec4(a_pos,1.0f));
	o_normal=mat3(u_model)*a_normal;
}


#fragment shader
#version 330 core

in vec3 o_pos;
in vec3 o_normal;

out vec4 FragColor;

vec3 albedo=vec3(0.5f,0.0f,0.0f);
float ao=1.0f;


uniform vec3 u_light_pos[4];
uniform vec3 u_light_col[4];
uniform vec3 u_camera_pos;
uniform float metallic;
uniform float roughness;
const float PI=3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
void main()
{
	vec3 normal=normalize(o_normal);
	vec3 view_dir=normalize(u_camera_pos-o_pos);
	
	vec3 color=vec3(0.0f,0.0f,0.0f);

	vec3 F0=vec3(0.04);
	F0=mix(F0,albedo,metallic);
	vec3 lo=vec3(0.0);
	for(int i=0;i<4;i++)
	{
		vec3 light_dir=normalize(u_light_pos[i]-o_pos);
		vec3 halfvec=normalize(view_dir+light_dir);
		float dist=length(u_light_pos[i]-o_pos);
		float attenuation=1/(dist*dist);
		vec3 radiance=u_light_col[i]*attenuation;

		//Cook-Torrance BRDF
		float NDF=DistributionGGX(normal,halfvec,roughness);
		float G=GeometrySmith(normal,view_dir,light_dir,roughness);
		vec3 F=fresnelSchlick(clamp(dot(halfvec,view_dir),0.0f,1.0f),F0);

		vec3 numerator=NDF*G*F;
		float denominator=4*max(dot(normal,view_dir),0.0f)*max(dot(normal,light_dir),0.0f)+0.0001;
		vec3 specular=numerator/denominator;

		vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(normal, light_dir), 0.0);        

        // add to outgoing radiance Lo
        lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note
		
      
	}

	vec3 ambient=vec3(0.03)*albedo*ao;
	color=ambient+lo;
	color=color/(color+vec3(1.0f));

	color=pow(color,vec3(1.0/2.2));
	
	FragColor=vec4(color,1.0f);
}