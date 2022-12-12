layout(location = 0) in vec3 aPos;
uniform mat3 transMat;

void main(){
    gl_Position = vec4(transMat * aPos , 1.0);
}
