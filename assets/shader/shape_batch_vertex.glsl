layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec4 aShape;
layout(location = 3) in vec4 aRect;

out vec4 vColor;
flat out vec4 vShape;
flat out vec4 vRect;

uniform mat3 transMat;

void main(){
    vec3 pos = transMat * vec3(aPos.xy , 1.0f);
    gl_Position = vec4(pos.xy , aPos.z , 1.0);
    vColor = aColor;
    vShape = aShape;
    vRect = aRect;
}
