precision highp float;

in vec2 vUv;

uniform sampler2D mainTexture;

out vec4 fragColor;

void main(){
    // fragColor = vec4(1.0f , 0.0f , 0.0f , 1.0f);
    // fragColor = texture(mainTexture , vUv).rgba;
    float color = texture(mainTexture , vUv).r;
    fragColor = vec4(color , 0.0f , 0.0f , 1.0f);
}