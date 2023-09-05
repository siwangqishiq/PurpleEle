precision highp float;

uniform vec4 uRect;
uniform vec4 uColor;
uniform vec2 uAngleRange;

out vec4 fragColor;

float fillCircle(vec2 pos , vec2 center , float radius){
    // return 1.0f - step(radius , distance(pos , center));
    vec2 posVec = normalize(pos - center);
    vec2 horVec = vec2(1.0f , 0.0f);
    float angle = degrees(acos(dot(posVec, horVec)));
    float calFlag = posVec.x * horVec.y - posVec.y * horVec.x;//确定转动方向
    if(calFlag > 0){
        angle = 360 - angle;
    }

    if(distance(pos , center) <= radius 
        && angle > mod(uAngleRange.x , 360.0f)
        && angle < mod(uAngleRange.y , 360.0f)){
        return 1.0f;
    }else{
        return 0.0f;
    }
}

void main(){
    vec2 pos = gl_FragCoord.xy;
    
    float radius = uRect.z / 2.0f;
    vec2 center = vec2(uRect.x + radius , uRect.y - radius);
    fragColor = uColor * fillCircle(pos , center , radius);
}