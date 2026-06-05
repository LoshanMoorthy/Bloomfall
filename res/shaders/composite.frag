#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uScene;
uniform sampler2D uBloom;

void main() {
    vec3 scene = texture(uScene, vUV).rgb;
    vec3 bloom = texture(uBloom, vUV).rgb;
    // additive glow
    vec3 color = scene + bloom;
    // tone map (HDR -> 0..1);
    color = color / (color + vec3(1.0));
    FragColor = vec4(color, 1.0);
}
