#version 330 core

in vec3 vNormal;
in vec3 vWorldPos;
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uSunDir;        // direction sunlight travels

#define MAX_LIGHTS 16
uniform int  uLightCount;
uniform vec3 uLightPos[MAX_LIGHTS];      // point light world position
uniform vec3 uLightColor[MAX_LIGHTS];    // point light color

void main() {
    vec3 N = normalize(vNormal);

    // directional sun
    vec3 sunL = normalize(-uSunDir);
    float sunDiff = max(dot(N, sunL), 0.0);

    // accumulate all point lights
    vec3 pointTotal = vec3(0.0);
    for (int i = 0; i < uLightCount; i++) {
        vec3 toLight = uLightPos[i] - vWorldPos;
        float dist = length(toLight);
        vec3 L = normalize(toLight);
        float diff = max(dot(N, L), 0.0);
        float atten = 1.0 / (1.0 + 0.1 * dist * dist);
        pointTotal += uLightColor[i] * diff * atten;
    }

    float ambient = 0.20;
    vec3 lighting = vec3(ambient + sunDiff * 0.55) + pointTotal;

    vec4 tex = texture(uTexture, vUV);
    FragColor = vec4(tex.rgb * lighting, tex.a);
}
