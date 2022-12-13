precision mediump float;

uniform vec4 uColor;
uniform int uFillStyle;
uniform float uStrokenWidth;

uniform float uViewWidth;
uniform float uViewHeight;
uniform vec4 uRect;

out vec4 fragColor;

float stokeRect(vec2 pos){
    if(pos.x >= uRect.x && pos.x <= uRect.x + uStrokenWidth 
        || pos.x <= uRect.x + uRect.z && pos.x >= uRect.x + uRect.z - uStrokenWidth
        || pos.y <= uRect.y && pos.y >= uRect.y - uStrokenWidth
        || pos.y >= uRect.y - uRect.w && pos.y <= uRect.y - uRect.w + uStrokenWidth){
        return 1.0f;
    }
    return 0.0f;
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