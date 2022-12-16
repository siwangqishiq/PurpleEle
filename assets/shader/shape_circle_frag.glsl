precision highp float;

uniform vec4 uColor;
uniform int uFillStyle;
uniform float uStrokenWidth;

uniform float uWidth;
uniform float uHeight;
uniform float uViewWidth;
uniform float uViewHeight;
uniform vec4 uRect;

out vec4 fragColor;

float stokenCircle(vec2 pos , vec2 center , float radius){
    float innerRadius = radius - uStrokenWidth;
    float value = (1.0f - step(radius , distance(pos , center))) *(step(innerRadius , distance(pos , center)));
    return min(1.0f , value);
}

float fillCircle(vec2 pos , vec2 center , float radius){
    return 1.0f - step(radius , distance(pos , center));
}

void main(){
    vec2 pos = gl_FragCoord.xy;
    float radius = uRect.w / 2.0f;
    vec2 center = vec2(uRect.x + radius , uRect.y - radius);

    if(uFillStyle == 0){
        fragColor = uColor * fillCircle(pos, center , radius);
    }else{
         fragColor = uColor * stokenCircle(pos, center , radius);
    }
}