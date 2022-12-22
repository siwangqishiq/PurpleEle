precision highp float;

in vec4 vColor;
flat in vec4 vShape;
flat in vec4 vRect;

out vec4 fragColor;

const float eps = 0.0001;

const float shape_circle = 0.0f;
const float shape_rect = 1.0f;
const float shape_round_rect = 2.0f;
const float shape_oval = 3.0f;

const float mode_filled = 0.0f;
const float mode_stoken = 1.0f;

float stokeRect(vec2 pos){
    float stokenWidth = vShape.z;

    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;

    if(pos.x >= vRect.x && pos.x <= vRect.x + stokenWidth){
        left = 1.0f;
    }
    if(pos.x <= vRect.x + vRect.z && pos.x >= vRect.x + vRect.z - stokenWidth){
        right = 1.0f;
    }
    if(pos.y <= vRect.y && pos.y >= vRect.y - stokenWidth){
        top = 1.0f;
    }
    if(pos.y >= vRect.y - vRect.w && pos.y <= vRect.y - vRect.w + stokenWidth){
        bottom = 1.0f;
    }
    return min(1.0f , left + right + top + bottom);
}

float renderRect(vec2 pos){
    if(abs(vShape.y - mode_stoken) <= eps){
        return stokeRect(pos);
    }
    return 1.0f;
}

float stokenCircle(vec2 pos , vec2 center , float radius){
    float stokenWidth = vShape.z;
    float innerRadius = radius - stokenWidth;
    float value = (1.0f - step(radius , distance(pos , center))) *(step(innerRadius , distance(pos , center)));
    return min(1.0f , value);
}

float renderCircle(vec2 pos){
    float radius = vRect.w / 2.0f;
    vec2 center = vec2(vRect.x + radius , vRect.y - radius);
    if(abs(vShape.y - mode_stoken) <= eps){
        return stokenCircle(pos , center , radius);
    }
    return 1.0f - step(radius , distance(pos , center));
}

float renderOval(vec2 pos){
    float ra = vRect.z / 2.0f;
    float rb = vRect.w / 2.0f;
    vec2 center = vec2(vRect.x + ra , vRect.y - rb);

    float xa = pos.x - center.x;
    float xb = pos.y - center.y;
    // (pos.x - center.x) * (pos.x - center.x)
    return 1.0f - step(1.0f , (pow(xa , 2) / pow(ra , 2) + (pow(xb , 2)) / pow(rb , 2)));
}

void main(){
    float value = 0.0f;
    vec2 pos = gl_FragCoord.xy;
    if(abs(vShape.x - shape_rect) <= eps){//draw rect
        value = renderRect(pos);
    }else if(abs(vShape.x - shape_circle) <= eps){
        value = renderCircle(pos);
    }else if(abs(vShape.x - shape_oval) <= eps){
        value = renderOval(pos);
    }
    fragColor = vColor * value;
}