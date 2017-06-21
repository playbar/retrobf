// GLSL shader autogenerated by cg2glsl.py.
#if defined(VERTEX)

#if __VERSION__ >= 130
#define COMPAT_VARYING out
#define COMPAT_ATTRIBUTE in
#define COMPAT_TEXTURE texture
#else
#define COMPAT_VARYING varying
#define COMPAT_ATTRIBUTE attribute
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif
COMPAT_VARYING     float _frame_rotation;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
vec4 _oPosition1;
vec4 _r0005;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 COLOR;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;
 
uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    vec4 _oColor;
    vec2 _otexCoord;
    _r0005 = VertexCoord.x*MVPMatrix[0];
    _r0005 = _r0005 + VertexCoord.y*MVPMatrix[1];
    _r0005 = _r0005 + VertexCoord.z*MVPMatrix[2];
    _r0005 = _r0005 + VertexCoord.w*MVPMatrix[3];
    _oPosition1 = _r0005;
    _oColor = COLOR;
    _otexCoord = TexCoord.xy;
    gl_Position = _r0005;
    COL0 = COLOR;
    TEX0.xy = TexCoord.xy;
} 
#elif defined(FRAGMENT)

#if __VERSION__ >= 130
#define COMPAT_VARYING in
#define COMPAT_TEXTURE texture
out vec4 FragColor;
#else
#define COMPAT_VARYING varying
#define FragColor gl_FragColor
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif
COMPAT_VARYING     float _frame_rotation;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
vec4 _ret_0;
float _TMP24;
float _TMP23;
float _TMP22;
float _TMP29;
float _TMP28;
float _TMP27;
float _TMP21;
float _TMP20;
float _TMP19;
float _TMP18;
float _TMP34;
float _TMP33;
float _TMP30;
float _TMP32;
float _TMP31;
vec2 _TMP16;
vec2 _TMP15;
float _TMP26;
float _TMP25;
vec2 _TMP14;
float _TMP13;
vec2 _TMP11;
vec2 _TMP10;
vec2 _TMP9;
float _TMP8;
vec2 _TMP6;
vec2 _TMP5;
vec2 _TMP4;
float _TMP3;
input_dummy _IN1;
vec2 _x0041;
vec2 _x0047;
vec2 _x0053;
vec2 _a0059;
vec2 _b0059;
float _y0061;
float _x0061;
float _t30063;
float _t10063;
float _t00063;
float _t40063;
vec2 _x0079;
vec2 _x0085;
vec2 _x0091;
vec2 _a0097;
vec2 _b0097;
float _y0099;
float _x0099;
float _t30101;
float _t10101;
float _t00101;
float _t40101;
vec2 _x0117;
vec2 _x0123;
vec2 _x0129;
vec2 _a0135;
vec2 _b0135;
float _y0137;
float _x0137;
float _t30139;
float _t10139;
float _t00139;
float _t40139;
vec3 _TMP154;
float _a0159;
float _x0163;
float _TMP164;
vec3 _TMP170;
vec3 _v0177;
float _TMP184;
float _x0185;
COMPAT_VARYING vec4 TEX0;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    vec2 _pc;
    vec2 _pa;
    vec2 _pb;
    vec2 _zc;
    vec3 _fc;
    int _i1;
    float _d1;
    vec2 _za;
    vec3 _fa;
    int _i2;
    float _d2;
    vec2 _zb;
    vec3 _fb;
    int _i3;
    float _d3;
    vec3 _col;
    vec3 _nor;
    _TMP3 = min(TextureSize.y, TextureSize.x);
    _pc = (1.00000000E+04*(TEX0.xy*(TextureSize.xy/InputSize.xy)).xy - TextureSize.xy)/_TMP3;
    _pa = _pc + vec2( 3.99999991E-02, 0.00000000E+00);
    _pb = _pc + vec2( 0.00000000E+00, 3.99999991E-02);
    _zc = _pc;
    _fc = vec3( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00);
    _i1 = 0;
    for (; _i1 < 8; _i1 = _i1 + 1) { 
        _x0041 = _zc.yx + float(FrameCount)*9.99999978E-03*5.00000000E+00;
        _TMP25 = cos(_x0041.x);
        _TMP26 = cos(_x0041.y);
        _TMP4 = vec2(_TMP25, _TMP26);
        _x0047 = _zc.yx + _TMP4;
        _TMP25 = cos(_x0047.x);
        _TMP26 = cos(_x0047.y);
        _TMP5 = vec2(_TMP25, _TMP26);
        _x0053 = _zc.yx + _TMP5;
        _TMP25 = cos(_x0053.x);
        _TMP26 = cos(_x0053.y);
        _TMP6 = vec2(_TMP25, _TMP26);
        _zc = _zc + _TMP6;
        _a0059 = _zc - _pc;
        _b0059 = _zc - _pc;
        _d1 = dot(_a0059, _b0059);
        _fc.x = _fc.x + 1.00000000E+00/(1.00000000E+00 + _d1);
        _fc.y = _fc.y + _d1;
        _y0061 = _zc.y - _pc.y;
        _x0061 = _zc.x - _pc.x;
        _t30063 = abs(_x0061);
        _t10063 = abs(_y0061);
        _t00063 = max(_t30063, _t10063);
        _t10063 = min(_t30063, _t10063);
        _t30063 = 1.00000000E+00/_t00063;
        _t30063 = _t10063*_t30063;
        _t40063 = _t30063*_t30063;
        _t00063 = -1.34804696E-02*_t40063 + 5.74773103E-02;
        _t00063 = _t00063*_t40063 - 1.21239103E-01;
        _t00063 = _t00063*_t40063 + 1.95635900E-01;
        _t00063 = _t00063*_t40063 - 3.32994610E-01;
        _t00063 = _t00063*_t40063 + 9.99995589E-01;
        _t30063 = _t00063*_t30063;
        _TMP31 = abs(_y0061);
        _TMP32 = abs(_x0061);
        if (_TMP31 > _TMP32) { 
            _TMP30 = 1.57079601E+00 - _t30063;
        } else {
            _TMP30 = _t30063;
        } 
        if (_x0061 < 0.00000000E+00) { 
            _TMP33 = 3.14159298E+00 - _TMP30;
        } else {
            _TMP33 = _TMP30;
        } 
        if (_y0061 < 0.00000000E+00) { 
            _TMP34 = -_TMP33;
        } else {
            _TMP34 = _TMP33;
        } 
        _TMP8 = sin(_TMP34);
        _fc.z = _fc.z + _TMP8;
    } 
    _fc = _fc/8.00000000E+00;
    _za = _pa;
    _fa = vec3( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00);
    _i2 = 0;
    for (; _i2 < 8; _i2 = _i2 + 1) { 
        _x0079 = _za.yx + float(FrameCount)*9.99999978E-03*5.00000000E+00;
        _TMP25 = cos(_x0079.x);
        _TMP26 = cos(_x0079.y);
        _TMP9 = vec2(_TMP25, _TMP26);
        _x0085 = _za.yx + _TMP9;
        _TMP25 = cos(_x0085.x);
        _TMP26 = cos(_x0085.y);
        _TMP10 = vec2(_TMP25, _TMP26);
        _x0091 = _za.yx + _TMP10;
        _TMP25 = cos(_x0091.x);
        _TMP26 = cos(_x0091.y);
        _TMP11 = vec2(_TMP25, _TMP26);
        _za = _za + _TMP11;
        _a0097 = _za - _pa;
        _b0097 = _za - _pa;
        _d2 = dot(_a0097, _b0097);
        _fa.x = _fa.x + 1.00000000E+00/(1.00000000E+00 + _d2);
        _fa.y = _fa.y + _d2;
        _y0099 = _za.y - _pa.y;
        _x0099 = _za.x - _pa.x;
        _t30101 = abs(_x0099);
        _t10101 = abs(_y0099);
        _t00101 = max(_t30101, _t10101);
        _t10101 = min(_t30101, _t10101);
        _t30101 = 1.00000000E+00/_t00101;
        _t30101 = _t10101*_t30101;
        _t40101 = _t30101*_t30101;
        _t00101 = -1.34804696E-02*_t40101 + 5.74773103E-02;
        _t00101 = _t00101*_t40101 - 1.21239103E-01;
        _t00101 = _t00101*_t40101 + 1.95635900E-01;
        _t00101 = _t00101*_t40101 - 3.32994610E-01;
        _t00101 = _t00101*_t40101 + 9.99995589E-01;
        _t30101 = _t00101*_t30101;
        _TMP31 = abs(_y0099);
        _TMP32 = abs(_x0099);
        if (_TMP31 > _TMP32) { 
            _TMP30 = 1.57079601E+00 - _t30101;
        } else {
            _TMP30 = _t30101;
        } 
        if (_x0099 < 0.00000000E+00) { 
            _TMP33 = 3.14159298E+00 - _TMP30;
        } else {
            _TMP33 = _TMP30;
        } 
        if (_y0099 < 0.00000000E+00) { 
            _TMP34 = -_TMP33;
        } else {
            _TMP34 = _TMP33;
        } 
        _TMP13 = sin(_TMP34);
        _fa.z = _fa.z + _TMP13;
    } 
    _fa = _fa/8.00000000E+00;
    _zb = _pb;
    _fb = vec3( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00);
    _i3 = 0;
    for (; _i3 < 8; _i3 = _i3 + 1) { 
        _x0117 = _zb.yx + float(FrameCount)*9.99999978E-03*5.00000000E+00;
        _TMP25 = cos(_x0117.x);
        _TMP26 = cos(_x0117.y);
        _TMP14 = vec2(_TMP25, _TMP26);
        _x0123 = _zb.yx + _TMP14;
        _TMP25 = cos(_x0123.x);
        _TMP26 = cos(_x0123.y);
        _TMP15 = vec2(_TMP25, _TMP26);
        _x0129 = _zb.yx + _TMP15;
        _TMP25 = cos(_x0129.x);
        _TMP26 = cos(_x0129.y);
        _TMP16 = vec2(_TMP25, _TMP26);
        _zb = _zb + _TMP16;
        _a0135 = _zb - _pb;
        _b0135 = _zb - _pb;
        _d3 = dot(_a0135, _b0135);
        _fb.x = _fb.x + 1.00000000E+00/(1.00000000E+00 + _d3);
        _fb.y = _fb.y + _d3;
        _y0137 = _zb.y - _pb.y;
        _x0137 = _zb.x - _pb.x;
        _t30139 = abs(_x0137);
        _t10139 = abs(_y0137);
        _t00139 = max(_t30139, _t10139);
        _t10139 = min(_t30139, _t10139);
        _t30139 = 1.00000000E+00/_t00139;
        _t30139 = _t10139*_t30139;
        _t40139 = _t30139*_t30139;
        _t00139 = -1.34804696E-02*_t40139 + 5.74773103E-02;
        _t00139 = _t00139*_t40139 - 1.21239103E-01;
        _t00139 = _t00139*_t40139 + 1.95635900E-01;
        _t00139 = _t00139*_t40139 - 3.32994610E-01;
        _t00139 = _t00139*_t40139 + 9.99995589E-01;
        _t30139 = _t00139*_t30139;
        _TMP31 = abs(_y0137);
        _TMP32 = abs(_x0137);
        if (_TMP31 > _TMP32) { 
            _TMP30 = 1.57079601E+00 - _t30139;
        } else {
            _TMP30 = _t30139;
        } 
        if (_x0137 < 0.00000000E+00) { 
            _TMP33 = 3.14159298E+00 - _TMP30;
        } else {
            _TMP33 = _TMP30;
        } 
        if (_y0137 < 0.00000000E+00) { 
            _TMP34 = -_TMP33;
        } else {
            _TMP34 = _TMP33;
        } 
        _TMP18 = sin(_TMP34);
        _fb.z = _fb.z + _TMP18;
    } 
    _fb = _fb/8.00000000E+00;
    _TMP154 = vec3( 7.99999982E-02, 1.99999996E-02, 1.50000006E-01) + _fc.x*vec3( 5.20000041E-01, 1.08000004E+00, 1.45000005E+00);
    _a0159 = 9.99999978E-03*float(FrameCount)*5.00000000E+00;
    _TMP19 = cos(_a0159);
    _x0163 = _TMP19 - -5.00000000E-01;
    _TMP29 = min(1.00000000E+00, _x0163);
    _TMP164 = max(0.00000000E+00, _TMP29);
    _TMP20 = _TMP164*_TMP164*(3.00000000E+00 - 2.00000000E+00*_TMP164);
    _TMP170 = _TMP154 + _TMP20*(_TMP154.zxy - _TMP154);
    _col = _TMP170*(1.50000006E-01*_fc.y);
    _TMP21 = abs(_fc.z);
    _col = _col + (4.00000006E-01*_TMP21 - 1.00000001E-01);
    _v0177 = vec3(_fa.x - _fc.x, 9.99999978E-03, _fb.x - _fc.x);
    _TMP27 = dot(_v0177, _v0177);
    _TMP28 = inversesqrt(_TMP27);
    _nor = _TMP28*_v0177;
    _TMP22 = dot(_nor, vec3( 5.77300012E-01, 5.77300012E-01, 5.77300012E-01));
    _x0185 = 5.00000000E-01 + 5.00000000E-01*_TMP22;
    _TMP29 = min(1.00000000E+00, _x0185);
    _TMP184 = max(0.00000000E+00, _TMP29);
    _col = _col*(1.00000000E+00 + (6.99999988E-01*_TMP184)*_col);
    _TMP23 = pow(_nor.y, 1.28000000E+02);
    _col = _col + 3.00000012E-01*_TMP23;
    _TMP24 = length(_pc);
    _col = _col*(1.00000000E+00 - 1.00000001E-01*_TMP24);
    _ret_0 = vec4(_col.x, _col.y, _col.z, 1.00000000E+00);
    FragColor = _ret_0;
    return;
} 
#endif