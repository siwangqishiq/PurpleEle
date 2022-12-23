precision highp float;

in vec4 vColor;
flat in vec4 vShape;
flat in vec4 vRect;

out vec4 fragColor;

const float zero = 0.0f;
const float one = 1.0f;
const float eps = 0.0001f;

const float shape_circle = 0.0f;
const float shape_rect = 1.0f;
const float shape_round_rect = 2.0f;
const float shape_oval = 3.0f;

const float mode_filled = 0.0f;
const float mode_stoken = 1.0f;

bool floatEqual(float v1 , float v2){
    return abs(v1 - v2) <= eps;
}

float stokeRect(vec2 pos){
    float stokenWidth = vShape.z;

    // float left = 0.0f;
    // float right = 0.0f;
    // float top = 0.0f;
    // float bottom = 0.0f;
    // if(vRect.x <= pos.x && vRect.x + stokenWidth >= pos.x){
    //     left = 1.0f;
    // }
    // if(vRect.x + vRect.z >= pos.x && vRect.x + vRect.z - stokenWidth <= pos.x){
    //     right = 1.0f;
    // }
    // if(vRect.y >= pos.y && vRect.y - stokenWidth <= pos.y){
    //     top = 1.0f;
    // }
    // if(vRect.y - vRect.w <= pos.y && vRect.y - vRect.w + stokenWidth >= pos.y){
    //     bottom = 1.0f;
    // }

    float left = step(vRect.x , pos.x) * (1.0f - step(vRect.x + stokenWidth , pos.x));
    float right = (1.0f - step(vRect.x + vRect.z , pos.x)) * step(vRect.x + vRect.z - stokenWidth, pos.x);
    float top = (1.0f - step(vRect.y , pos.y)) * step(vRect.y - stokenWidth , pos.y);
    float bottom = step(vRect.y - vRect.w , pos.y) * (1.0f - step(vRect.y - vRect.w + stokenWidth , pos.y));
    return min(1.0f , left + right + top + bottom);
}

float renderRect(vec2 pos){
    if(floatEqual(vShape.y , mode_stoken)){
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
    if(floatEqual(vShape.y , mode_stoken)){
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
    return 1.0f - step(1.0f , (pow(xa , 2.0f) / pow(ra , 2.0f) + (pow(xb , 2.0f)) / pow(rb , 2.0f)));
}

bool isPointInRect(vec4 rect , vec2 p){
    return p.x >= rect.x && p.x <= rect.x + rect.z && p.y <= rect.y && p.y >= rect.y - rect.w;
}

bool isPointInCircle(vec2 center , float r , vec2 p){
    return distance(p , center) <= r;
}

float renderRoundRect(vec2 pos){
    float roundRadius = vShape.w;
    float cube = 1.0f * roundRadius;
    vec4 leftTopRect = vec4(vRect.x , vRect.y , cube , cube);
    if(isPointInRect(leftTopRect , pos)
        && !isPointInCircle(vec2(leftTopRect.x + cube , leftTopRect.y - cube) , roundRadius , pos)){
        return zero;
    }

    vec4 rightTopRect = vec4(vRect.x + vRect.z - cube , vRect.y , cube , cube);
    if(isPointInRect(rightTopRect , pos)
        && !isPointInCircle(vec2(rightTopRect.x + rightTopRect.z - cube , rightTopRect.y - cube) , roundRadius , pos)){
        return zero;
    }

    vec4 leftBottomRect = vec4(vRect.x, vRect.y - vRect.w + cube , cube , cube);
    if(isPointInRect(leftBottomRect , pos)
        && !isPointInCircle(vec2(leftBottomRect.x + cube , leftBottomRect.y - leftBottomRect.w + cube) , roundRadius , pos)){
        return zero;
    }

    vec4 rightBottomRect = vec4(vRect.x + vRect.z - cube, vRect.y - vRect.w + cube , cube , cube);
    if(isPointInRect(rightBottomRect , pos)
        && !isPointInCircle(vec2(rightBottomRect.x + rightBottomRect.z - cube , rightBottomRect.y - rightBottomRect.w + cube) , roundRadius , pos)){
        return zero;
    }
    return one;
}

void main(){
    float value = 0.0f;
    vec2 pos = gl_FragCoord.xy;
    if(floatEqual(vShape.x , shape_rect)){//draw rect
        value = renderRect(pos);
    }else if(floatEqual(vShape.x , shape_circle)){
        value = renderCircle(pos);
    }else if(floatEqual(vShape.x , shape_oval)){
        value = renderOval(pos);
    }else if(floatEqual(vShape.x ,shape_round_rect)){
        value = renderRoundRect(pos);
    }
    fragColor = vColor * value;
}