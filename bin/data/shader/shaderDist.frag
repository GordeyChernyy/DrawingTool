// horisontal blur
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
//    gl_FragColor = texture2DRect(u_tex_unit0,gt);
//gl_FragColor += texture2DRect(u_tex_unit0,gt*0.2);
}

float Tile1D(float p, float a){
    p -= 4.0 * a * floor(p/4.0 * a);
    p -= 2.* max(p - 2.0 * a , 0.0);
    return p;
}

// ---------------------------------------

//vec4 wc(float mult, sampler2DRect tex, vec2 st, float period, float speed){
//    float x = cos( st.t * cos(time) * period + 0.5 - gl_FragCoord.y * period ) * speed;
//    float y = cos( st.s * period ) * speed ;
//    vec4 c = texture2DRect(tex, st + vec2(x, y));
//    return c*mult;
//}
//void main( void )
//
//{
//    
//    vec2 uv = mousePos/windowSize;
//    vec2 gt = gl_TexCoord[0].st;
//    float amt = 0.3;
//    vec4 color;
//    color.r += wc(1.0, u_tex_unit0, gt, waveR, speedR).r;
//    color.g += wc(1.0, u_tex_unit0, gt, waveG, speedG).g;
//    color.b += wc(1.0, u_tex_unit0, gt, waveB, speedB).b;
//    color.a += wc(1.0, u_tex_unit0, gt, waveA, speedA).a;
//    //    color.b  -= clamp(color.b*0.002, 0.0, 0.0006);
//    
//    gl_FragColor = vec4(color.r, color.g, color.b, color.a);
//    
//}
//

// ---------------------------------------

//vec2 st = gl_TexCoord[0].st;
//float amt = 0.3;
//vec4 color;
//color += 1.0 * texture2DRect(u_tex_unit0, st + vec2(cos(st.t*cos(time)*0.02+0.5-gl_FragCoord.y*0.22)*5.2, cos(st.s*0.02)*2.2));
//color.r *= 1.0 * texture2DRect(u_tex_unit0, st + vec2(sin(gl_FragCoord.y*0.2-gl_FragCoord.x*0.2)*12.2, tan(time*12)*1.2)).a;
//color.g *= 1.0 * texture2DRect(u_tex_unit0, st + vec2(sin(gl_FragCoord.y*0.2-gl_FragCoord.x*0.2)*12.2, tan(time*12)*1.2)).b;
//
//color /= 1;
//gl_FragColor = vec4(color.r, color.g, color.b, color.a);