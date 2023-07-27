layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_texture;

uniform mat3 transMat;
out vec3 vUvw;

void main(){
    vec3 pos = transMat * vec3(a_position.xy , 1.0f);
    gl_Position = vec4(pos.xy , a_position.z ,1.0f);
    vUvw = a_texture;
}