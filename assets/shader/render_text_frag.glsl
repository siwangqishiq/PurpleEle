precision mediump float;

in vec2 vUv;
out vec4 outColor;

uniform sampler2D fontTexture;
uniform vec4 textColor;

void main(){
    vec4 originColor = texture(fontTexture , vUv);
    float colorMask = originColor.r;
    outColor = colorMask * textColor;
    // outColor = vec4(1.0f , 0.0f , 0.0f , 1.0f);
}
