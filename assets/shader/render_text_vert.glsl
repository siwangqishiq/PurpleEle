layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_texture;

uniform mat3 transMat;
out vec3 vUvw;

void main(){
    gl_Position = vec4(transMat * a_position ,1.0f);
    vUvw = a_texture;
}