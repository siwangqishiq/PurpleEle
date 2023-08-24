precision highp float;

uniform float uViewWidth;
uniform float uViewHeight;

out vec4 FragColor;

const vec3 WHITE_COLOR = vec3(1.0 , 1.0 , 1.0);
const vec3 SKYBLUE_COLOR = vec3(0.5, 0.7, 1.0);

struct Ray{
    vec3 origin;
    vec3 dir;
};

struct Camera{
    vec3 pos;
};

struct Object{
    int index;
    vec3 pos;
};

const int LIST_MAX_SIZE = 100;
struct List{
    int size;
    Object data[LIST_MAX_SIZE];
};
bool addList(inout List list , Object item){
    if(list.size + 1 >= LIST_MAX_SIZE){
        return false;
    }

    list.data[list.size] = item;
    list.size += 1;
    return true;
}


//光线追踪着色
vec3 rayColor(Ray ray){
    float t = 0.5 * (ray.dir.y + 1.0);
    return WHITE_COLOR* (1.0 - t) + SKYBLUE_COLOR * (t);
    // return SKYBLUE_COLOR;
}

void main(){
    float aspect = uViewWidth / uViewHeight;
    
    const float viewPortWidth = 2.0; //视口宽度
    float viewPortHeight = viewPortWidth / aspect; //视口高度
    const float focal = 1.0f;//焦距

    const vec3 origin = vec3(0.0 , 0.0 , 0.0); //射线起始点

    vec3 translateX = vec3(viewPortWidth , 0.0 , 0.0);
    vec3 translateY = vec3(0.0 , viewPortHeight , 0.0);
    vec3 translateZ = vec3(0.0 , 0.0 , focal);
    
    vec3 bottomLeftPos = origin - translateX / 2.0f - translateY / 2.0f - translateZ;

    vec2 uvCoord = vec2(gl_FragCoord.x / uViewWidth , gl_FragCoord.y / uViewHeight); 
    vec3 position = bottomLeftPos + uvCoord.x * translateX + uvCoord.y * translateY - origin;

    Ray ray;
    ray.origin = origin;
    ray.dir = normalize(position - origin);
    
    vec3 color;
    color = rayColor(ray);
    FragColor = vec4(color.xyz ,1.0);
}