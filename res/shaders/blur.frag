#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uImage;
uniform bool uHorizontal;
uniform vec2 uTexelSize;

const float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
    vec3 result = texture(uImage, vUV).rgb * weights[0];
    vec2 dir = uHorizontal ? vec2(uTexelSize.x, 0.0) : vec2(0.0, uTexelSize.y);
    for (int i = 1; i < 5; i++) {
        result += texture(uImage, vUV + dir * float(i)).rgb * weights[i];
        result += texture(uImage, vUV - dir * float(i)).rgb * weights[i];
    }
    FragColor = vec4(result, 1.0);
}
