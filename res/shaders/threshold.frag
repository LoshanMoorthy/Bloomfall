#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uScene;

void main() {
    vec3 c = texture(uScene, vUV).rgb;
    float brightness = dot(c, vec3(0.2126, 0.7152, 0.0722));
    FragColor = (brightness > 1.0) ? vec4(c, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}
