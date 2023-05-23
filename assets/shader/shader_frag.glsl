precision highp float;

uniform float uViewWidth;
uniform float uViewHeight;

out vec4 FragColor;

vec4 renderCircle(vec2 pos) {
    float radius = uViewHeight / 4.0f;
    vec2 center = vec2(uViewWidth / 2.0f , uViewHeight / 2.0f);

    if(distance(pos , center) >= radius){
        discard;
    }

    return vec4(1.0f , 0.0 , 0.0 , smoothstep(distance(pos , center) , radius , radius - radius/4.0f));
}

void main(){
    vec2 pos = gl_FragCoord.xy;
    FragColor = renderCircle(pos);
}