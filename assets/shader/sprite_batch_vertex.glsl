layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUv;

out vec2 vUv;

uniform mat3 transMat;

void main(){
    vec3 pos = transMat * vec3(aPos.xy , 1.0f);
    gl_Position = vec4(pos.xy , aPos.z , 1.0f);
    vUv = aUv;
}
