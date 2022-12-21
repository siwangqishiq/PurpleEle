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

float renderCircle(vec2 pos){
    return 1.0f;
}

void main(){
    float value = 0.0f;
    vec2 pos = gl_FragCoord.xy;
    if(abs(vShape.x - shape_rect) <= eps){//draw rect
        value = renderRect(pos);
    }else if(abs(vShape.x - shape_circle) <= eps){
        value = renderCircle(pos);
    }
    fragColor = vColor * value;
}