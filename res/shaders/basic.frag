#version 330 core
in vec3 vNormal;
in vec3 vWorldPos;
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTexture;

#define MAX_LIGHTS 16
uniform int  uLightCount;
uniform vec3 uLightPos[MAX_LIGHTS];
uniform vec3 uLightColor[MAX_LIGHTS];

uniform vec3 uTint;

void main() {
    vec3 N = normalize(vNormal);

    vec3 lightTotal = vec3(0.0);
    for (int i = 0; i < uLightCount; i++) {
        vec3 toLight = uLightPos[i] - vWorldPos;
        float dist = length(toLight);
        vec3 L = normalize(toLight);
        float diff = max(dot(N, L), 0.0);
        float atten = 1.0 / (1.0 + 0.35 * dist * dist);
        lightTotal += uLightColor[i] * diff * atten;
    }

    float ambient = 0.02;
    vec3 lighting = vec3(ambient) + lightTotal;

    vec4 tex = texture(uTexture, vUV);
    FragColor = vec4(tex.rgb * lighting * uTint, tex.a);
}
