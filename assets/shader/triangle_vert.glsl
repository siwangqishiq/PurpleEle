layout(location = 0) in vec2 aPos;
uniform mat3 transMat;
void main(){
    gl_Position = vec4(transMat * vec3(aPos , 1.0f) , 1.0);
}
