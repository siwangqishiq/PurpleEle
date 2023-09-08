precision highp float;

uniform vec4 uRect;
uniform vec4 uColor;
uniform vec2 uAngleRange;
uniform int uFillStyle;
uniform float uStrokenWidth;
uniform bool uReverse;

const int mode_filled = 0;
const int mode_stoken = 1;

out vec4 fragColor;

float fillCircle(vec2 pos , vec2 center , float radius){
    vec2 posVec = normalize(pos - center);
    vec2 horVec = vec2(1.0f , 0.0f);
    float angle = degrees(acos(dot(posVec, horVec)));

    mat2 vecMat = mat2(posVec , horVec); //构造2x2矩阵
    float calFlag = determinant(vecMat); //求行列式 确定转动方向
    if(calFlag > 0.0f){
        angle = 360.0f - angle;
    }

    float len = distance(pos , center);

    if(len > radius){
        return 0.0f;
    }

    bool isPaint = false;
    if(angle > mod(uAngleRange.x , 360.01f)
        && angle < mod(uAngleRange.y , 360.01f)){ 
        isPaint = true;
    }else{
        isPaint = false;
    }

    if(uReverse){
        isPaint = !isPaint;
    }

    if(isPaint && uFillStyle == mode_stoken && len < radius - uStrokenWidth){
        isPaint = false;
    }
    return isPaint?1.0f:0.0f;
}

void main(){
    vec2 pos = gl_FragCoord.xy;
    
    float radius = uRect.z / 2.0f;
    vec2 center = vec2(uRect.x + radius , uRect.y - radius);
    fragColor = uColor * fillCircle(pos , center , radius);
}