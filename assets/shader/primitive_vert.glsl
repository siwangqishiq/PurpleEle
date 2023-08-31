layout(location = 0) in vec3 aPos;

uniform mat4 modelMat;
uniform mat3 transMat;

void main(){
    float depth = aPos.z;
    vec4 mPos = modelMat * vec4(aPos.xy , 0.0f , 1.0f);
    vec3 pos = transMat * vec3(mPos.xy , 1.0f);
    gl_Position = vec4(pos.xy , depth ,1.0f);
}
