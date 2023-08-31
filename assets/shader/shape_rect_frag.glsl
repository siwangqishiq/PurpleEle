precision mediump float;

uniform vec4 uColor;
uniform int uFillStyle;
uniform float uStrokenWidth;

uniform float uViewWidth;
uniform float uViewHeight;
uniform vec4 uRect;

out vec4 fragColor;

float stokeRect(vec2 pos){
    
    float left = step(uRect.x , pos.x) * (1.0f - step(uRect.x + uStrokenWidth , pos.x));
    float right = (1.0f - step(uRect.x + uRect.z , pos.x)) * step(uRect.x + uRect.z - uStrokenWidth , pos.x);
    float top = (1.0f - step(uRect.y , pos.y)) * step(uRect.y - uStrokenWidth , pos.y);
    float bottom = step(uRect.y - uRect.w , pos.y) * (1.0f - step(uRect.y - uRect.w + uStrokenWidth ,pos.y));
    
    return min(1.0f , left + right + top + bottom);
}

void main(){
    // FragColor = vec4(1.0f , 0.0f , 0.0f , 1.0f);
    if(uFillStyle == 0){
        fragColor = uColor;
        // float valueR = gl_FragCoord.x / uViewWidth;
        // float valueG = gl_FragCoord.y / uViewHeight;
        // fragColor = vec4(valueR , valueG , 0.0f , 1.0f);
    }else{
        vec2 pos = gl_FragCoord.xy;
        vec2 rectLeftBottom = vec2(uRect.x , uRect.y - uRect.w);
        fragColor = uColor * stokeRect(pos);
    }
}