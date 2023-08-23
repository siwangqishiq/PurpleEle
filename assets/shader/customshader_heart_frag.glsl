precision highp float;

uniform float uLeft;
uniform float uTop;
uniform float uWidth;
uniform float uHeight;
uniform vec4 uColor;

out vec4 FragColor;

const float PI = 3.14159267;

vec4 renderHeart(vec2 pos){
    vec2 uv = 2.0f * pos - vec2(1.0f , 1.0f);
    uv.y -= 0.3f;  
    float r = length(uv);  
    float d = atan(uv.x,uv.y)/PI; //[-1.0,1.0] 
    float s = abs(d);
    float e = (13.0*s - 22.0*s*s + 10.0*s*s*s)/(6.0-5.0*s);
    float col = smoothstep(-0.02f, 0.02f, e-r);
    vec4 color = vec4(col) * uColor;
    return color;
}

void main(){
    vec2 pos = gl_FragCoord.xy;
    pos.x = (gl_FragCoord.x - uLeft) / uWidth;
    pos.y = (uHeight - (uTop - gl_FragCoord.y)) / uHeight;
    FragColor = renderHeart(pos);
}