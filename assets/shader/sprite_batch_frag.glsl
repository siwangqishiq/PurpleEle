precision highp float;

in vec2 vUv;

uniform sampler2D uTex;

out vec4 fragColor;

void main(){
    // fragColor = vec4(0.0f , 1.0f , 0.0f , 1.0f);
    fragColor = texture(uTex , vUv).rgba;
}