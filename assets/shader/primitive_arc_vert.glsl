layout(location = 0) in vec3 aPos;

uniform mat3 transMat;

void main(){
    vec3 pos = transMat * vec3(aPos.xy , 1.0f);
    gl_Position = vec4(pos.xy , aPos.z ,1.0f);
}
