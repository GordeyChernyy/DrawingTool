uniform sampler2DRect u_tex_unit0;
uniform float s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
uniform vec2 windowSize;
uniform float time;
uniform vec2 mousePos;
const float PI = 3.141592658;
const float TAU = 2.0 * PI;

void main(void ){
    float sections = floor(10.0*s1);
    vec2 gt = gl_TexCoord[0].st;
    vec2 pos = vec2(vec2(gt.x, gt.y) - vec2(s2 * windowSize.x, s3 * windowSize.y)) / windowSize.x;
    
    float rad = length(pos);
    float angle = atan(pos.x, pos.y);
    
    float ma = mod(angle, TAU/sections);
    ma = abs(ma - PI/sections);
    float scale = windowSize.x/windowSize.y;
    float x = cos(ma) * rad * windowSize.x ;
    float y = sin(ma) * rad * windowSize.y*scale;
    
    float time = time/10.0;
    
    gl_FragColor = texture2DRect(u_tex_unit0, vec2(x,y));
}
