#version 330 core

out vec4 fragmentColour;

in vec4 fragmentShaderColor;
in vec2 texCoords;
in vec3 fragPos;

in vec3 worldSpaceNormal;
in vec4 pointLightDirection;
in vec4 spotLightDirection;
in vec4 viewerVector;

struct DirectionalLight
{
    vec3 direction;
    vec3 ambientIntensity;
    vec3 diffuseIntensity;
    vec3 specularIntensity;
};

uniform DirectionalLight direct1;

struct PointLight
{
    vec3 position;
    vec3 ambientIntensity;
    vec3 diffuseIntensity;
    vec3 specularIntensity;

    float constant;
    float linear;
    float quadratic;
};
uniform PointLight point1;

struct SpotLight
{
    vec3 position;
    vec3 ambientIntensity;
    vec3 diffuseIntensity;
    vec3 specularIntensity;

    vec3 coneDirection;
    float coneAngle;

    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spot1;

uniform vec3 ambientIntensity;
uniform vec3 diffuseIntensity;


uniform vec3 specularIntensity;
uniform vec3 specularColor;
uniform float shinines;

uniform sampler2D colourTexture;

vec4 lighting;

vec3 N;
vec4 V;
vec4 L;
float D;

//lighting overall not working well at all, not sure where the issue is

//directional
//works, but badly
vec4 CalculateDirectionalAmbient(DirectionalLight d)
{
    return vec4(d.ambientIntensity, 1.0f);
    
}

vec4 CalculateDirectionalDiffuse(DirectionalLight d)
{
    vec4 L = vec4(normalize(-d.direction), 0.0f);
    return max(dot(L, vec4(N, 1.0f)), 0.0f) * vec4(d.diffuseIntensity, 0.0f);

}

vec4 CalculateDirectionalSpecular(DirectionalLight d)
{
    vec4 L = vec4(normalize(-d.direction), 1.0f);
    vec4 reflection = reflect(L, vec4(N, 0.0f));
    float dotVal = max(dot(reflection, V), 0.0f);
    return pow(max(dot(reflection, V), 0.0f), shinines) * vec4(d.specularIntensity, 1.0f);

}

vec4 CalculateDirectional(DirectionalLight d)
{
    return CalculateDirectionalAmbient(d) + CalculateDirectionalDiffuse(d) + CalculateDirectionalSpecular(d);
}




//point
vec4 CalculatePointAmbient(PointLight p)
{
    return vec4(p.ambientIntensity, 1.0f);
}

vec4 CalculatePointDiffuse(PointLight p)
{
    L = normalize(pointLightDirection);
    return max(dot(L, vec4(N, 1.0f)), 0.0f) * vec4(p.diffuseIntensity, 1.0f);
}

vec4 CalculatePointSpecular(PointLight p)
{
    L = normalize(pointLightDirection);
    vec4 reflection = reflect(-L, vec4(N, 1.0f));
    return pow(max(dot(reflection, V), 0.0f), shinines) * vec4(p.specularIntensity, 1.0f);
}

float CalculatePointAttenuation(PointLight p)
{
    D = distance(p.position, fragPos);
    float a = 1 / (p.constant + p.linear * D + p.quadratic * pow(D, 2));
    return a;
}

vec4 CalculatePoint(PointLight p)
{
    return (CalculatePointAmbient(p) + CalculatePointDiffuse(p) + CalculatePointSpecular(p));
}



//spotlight
vec4 CalculateSpotAmbient(SpotLight s)
{
    return vec4(s.ambientIntensity, 1.0f);
}

vec4 CalculateSpotDiffuse(SpotLight s)
{
    L = normalize(spotLightDirection);
    return max(dot(L, vec4(N, 1.0f)), 0.0f) * vec4(s.diffuseIntensity, 1.0f);
}

vec4 CalculateSpotSpecular(SpotLight s)
{
    L = vec4(spotLightDirection);
    vec4 reflection = reflect(-L, vec4(N, 0.0f));
    return pow(max(dot(reflection, V), 0.0f), shinines) * vec4(s.specularIntensity, 1.0f);
}

float CalculateSpotAttenuation(SpotLight s)
{
    D = distance(s.position, fragPos);
    return 1 / (s.constant + s.linear * D + s.quadratic * pow(D, 2));
}

vec4 CalculateSpot(SpotLight s)
{
    L = normalize(spotLightDirection);
    float angle = acos(dot(spotLightDirection, vec4(s.coneDirection, 0.0f)) / (length(spotLightDirection) * length(vec4(s.coneDirection, 0.0f))));

    if(angle < s.coneAngle)
    {
        return (CalculateSpotAmbient(s) + CalculateSpotDiffuse(s) + CalculateSpotSpecular(s)) * CalculateSpotAttenuation(s);
    }
    else
        return CalculateSpotAmbient(s);
}

void main()
{
    
    N = normalize(worldSpaceNormal);
    V = normalize(viewerVector);

    //code for the old lighting
    //vec4 L = normalize(direct1.direction));
    //vec4 reflection = reflect(-L, vec4(N, 1.0f));
    //
    //
    //vec3 N = normalize(worldSpaceNormal);
    //vec4 L = normalize(lightDirection);
    //
    //vec4 V = normalize(viewerVector);
    //vec4 reflection = reflect(-L, vec4(N, 1.0f));
    //
    //// ----object color----
    ////fragmentColour = fragmentShaderColor * vec4(ambientIntensity, 1.0f)
    ////               + fragmentShaderColor * ((max(dot(L, vec4(N, 1.0)), 0.0)) * vec4(diffuseIntensity, 1))
    ////               + vec4(specularColor, 1.0) * pow(max(dot(reflection, V), 0.0), shinines) * vec4(specularIntensity, 1);
    //
    //lighting = vec4(ambientIntensity, 1.0f)
    //         + ((max(dot(L, vec4(N, 1.0)), 0.0)) * vec4(diffuseIntensity, 1))
    //         + vec4(specularColor, 1.0) * pow(max(dot(reflection, V), 0.0), shinines) * vec4(specularIntensity, 1);

    lighting += CalculateDirectional(direct1);
    lighting += CalculatePoint(point1) * CalculatePointAttenuation(point1);
    //lighting += CalculateSpot(spot1);

    fragmentColour = texture(colourTexture, texCoords) * lighting;
}
        