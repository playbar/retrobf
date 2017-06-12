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
COMPAT_VARYING     vec4 _t6;
COMPAT_VARYING     vec4 _t5;
COMPAT_VARYING     vec4 _t4;
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord1;
COMPAT_VARYING     vec4 _position1;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
struct out_vertex {
    vec4 _position1;
    vec2 _texCoord1;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
    vec4 _t4;
    vec4 _t5;
    vec4 _t6;
};
out_vertex _ret_0;
input_dummy _IN1;
vec4 _r0006;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
COMPAT_VARYING vec4 TEX4;
COMPAT_VARYING vec4 TEX5;
COMPAT_VARYING vec4 TEX6;
 
uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    out_vertex _OUT;
    vec2 _ps;
    _r0006 = VertexCoord.x*MVPMatrix[0];
    _r0006 = _r0006 + VertexCoord.y*MVPMatrix[1];
    _r0006 = _r0006 + VertexCoord.z*MVPMatrix[2];
    _r0006 = _r0006 + VertexCoord.w*MVPMatrix[3];
    _ps = 1.00000000E+00/TextureSize;
    _OUT._t1 = TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, -_ps.y);
    _OUT._t2 = TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, 0.00000000E+00);
    _OUT._t3 = TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, _ps.y);
    _OUT._t4 = TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, 2.00000000E+00*_ps.y);
    _OUT._t5 = TexCoord.xyyy + vec4(-2.00000000E+00*_ps.x, -_ps.y, 0.00000000E+00, _ps.y);
    _OUT._t6 = TexCoord.xyyy + vec4(2.00000000E+00*_ps.x, -_ps.y, 0.00000000E+00, _ps.y);
    _ret_0._position1 = _r0006;
    _ret_0._texCoord1 = TexCoord.xy;
    _ret_0._t1 = _OUT._t1;
    _ret_0._t2 = _OUT._t2;
    _ret_0._t3 = _OUT._t3;
    _ret_0._t4 = _OUT._t4;
    _ret_0._t5 = _OUT._t5;
    _ret_0._t6 = _OUT._t6;
    gl_Position = _r0006;
    TEX0.xy = TexCoord.xy;
    TEX1 = _OUT._t1;
    TEX2 = _OUT._t2;
    TEX3 = _OUT._t3;
    TEX4 = _OUT._t4;
    TEX5 = _OUT._t5;
    TEX6 = _OUT._t6;
    return;
    TEX0.xy = _ret_0._texCoord1;
    TEX1 = _ret_0._t1;
    TEX2 = _ret_0._t2;
    TEX3 = _ret_0._t3;
    TEX4 = _ret_0._t4;
    TEX5 = _ret_0._t5;
    TEX6 = _ret_0._t6;
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
COMPAT_VARYING     vec4 _t6;
COMPAT_VARYING     vec4 _t5;
COMPAT_VARYING     vec4 _t4;
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
struct out_vertex {
    vec2 _texCoord;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
    vec4 _t4;
    vec4 _t5;
    vec4 _t6;
};
vec4 _ret_0;
float _TMP7;
float _TMP6;
float _TMP5;
float _TMP4;
float _TMP3;
float _TMP2;
float _TMP1;
float _TMP0;
uniform sampler2D Texture;
vec4 _crn0072;
vec4 _ort0072;
vec4 _TMP73;
vec4 _a0074;
vec4 _crn0078;
vec4 _ort0078;
vec4 _TMP79;
vec4 _a0080;
vec4 _crn0084;
vec4 _ort0084;
vec4 _TMP85;
vec4 _a0086;
vec4 _crn0090;
vec4 _ort0090;
vec4 _TMP91;
vec4 _a0092;
vec4 _crn0096;
vec4 _ort0096;
vec4 _TMP97;
vec4 _a0098;
vec4 _crn0102;
vec4 _ort0102;
vec4 _TMP103;
vec4 _a0104;
vec4 _crn0108;
vec4 _ort0108;
vec4 _TMP109;
vec4 _a0110;
vec4 _crn0114;
vec4 _ort0114;
vec4 _TMP115;
vec4 _a0116;
vec4 _crn0120;
vec4 _ort0120;
vec4 _TMP121;
vec4 _a0122;
vec4 _res0126;
float _a0128;
float _a0130;
float _a0132;
float _a0134;
vec4 _res0136;
float _a0138;
float _a0140;
float _a0142;
float _a0144;
vec4 _res0146;
float _a0148;
float _a0150;
float _a0152;
float _a0154;
vec4 _res0156;
float _a0158;
float _a0160;
float _a0162;
float _a0164;
bool _TMP165;
vec2 _crn0166;
vec4 _ort0166;
bvec4 _a0168;
bool _TMP169;
vec2 _crn0170;
vec4 _ort0170;
bvec4 _a0172;
bool _TMP173;
vec2 _crn0174;
vec4 _ort0174;
bvec4 _a0176;
bool _TMP177;
vec2 _crn0178;
vec4 _ort0178;
bvec4 _a0180;
vec4 _TMP181;
vec4 _a0182;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
COMPAT_VARYING vec4 TEX4;
COMPAT_VARYING vec4 TEX5;
COMPAT_VARYING vec4 TEX6;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    vec4 _A;
    vec4 _B;
    vec4 _C;
    vec4 _D;
    vec4 _E;
    vec4 _F;
    vec4 _G;
    vec4 _H;
    vec4 _I;
    vec4 _J;
    vec4 _K;
    vec4 _L;
    vec4 _M;
    vec4 _N;
    vec4 _O;
    vec4 _P;
    vec4 _Q;
    vec4 _R;
    vec4 _As;
    vec4 _Bs;
    vec4 _Cs;
    vec4 _Ds;
    vec4 _Es;
    vec4 _Fs;
    vec4 _Gs;
    vec4 _Hs;
    vec4 _Is;
    vec4 _jSx;
    vec4 _jSy;
    vec4 _jSz;
    vec4 _jSw;
    bvec4 _jx;
    bvec4 _jy;
    bvec4 _jz;
    bvec4 _jw;
    bvec4 _res1;
    bvec4 _hori;
    bvec4 _vert;
    bvec4 _or;
    _A = COMPAT_TEXTURE(Texture, TEX1.xw);
    _B = COMPAT_TEXTURE(Texture, TEX1.yw);
    _C = COMPAT_TEXTURE(Texture, TEX1.zw);
    _D = COMPAT_TEXTURE(Texture, TEX2.xw);
    _E = COMPAT_TEXTURE(Texture, TEX2.yw);
    _F = COMPAT_TEXTURE(Texture, TEX2.zw);
    _G = COMPAT_TEXTURE(Texture, TEX3.xw);
    _H = COMPAT_TEXTURE(Texture, TEX3.yw);
    _I = COMPAT_TEXTURE(Texture, TEX3.zw);
    _J = COMPAT_TEXTURE(Texture, TEX4.xw);
    _K = COMPAT_TEXTURE(Texture, TEX4.yw);
    _L = COMPAT_TEXTURE(Texture, TEX4.zw);
    _M = COMPAT_TEXTURE(Texture, TEX5.xy);
    _N = COMPAT_TEXTURE(Texture, TEX5.xz);
    _O = COMPAT_TEXTURE(Texture, TEX5.xw);
    _P = COMPAT_TEXTURE(Texture, TEX6.xy);
    _Q = COMPAT_TEXTURE(Texture, TEX6.xz);
    _R = COMPAT_TEXTURE(Texture, TEX6.xw);
    _crn0072 = vec4(_M.z, _B.x, _D.z, _D.x);
    _ort0072 = vec4(_A.y, _A.w, _D.y, _M.w);
    _a0074 = 2.00000000E+00*_crn0072 - (_ort0072 + _ort0072.wxyz);
    _TMP73 = max(_a0074, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _As = vec4(_crn0072.x > 6.49999976E-01 ? _TMP73.x : 0.00000000E+00, _crn0072.y > 6.49999976E-01 ? _TMP73.y : 0.00000000E+00, _crn0072.z > 6.49999976E-01 ? _TMP73.z : 0.00000000E+00, _crn0072.w > 6.49999976E-01 ? _TMP73.w : 0.00000000E+00);
    _crn0078 = vec4(_A.z, _C.x, _E.z, _E.x);
    _ort0078 = vec4(_B.y, _B.w, _E.y, _A.w);
    _a0080 = 2.00000000E+00*_crn0078 - (_ort0078 + _ort0078.wxyz);
    _TMP79 = max(_a0080, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _Bs = vec4(_crn0078.x > 6.49999976E-01 ? _TMP79.x : 0.00000000E+00, _crn0078.y > 6.49999976E-01 ? _TMP79.y : 0.00000000E+00, _crn0078.z > 6.49999976E-01 ? _TMP79.z : 0.00000000E+00, _crn0078.w > 6.49999976E-01 ? _TMP79.w : 0.00000000E+00);
    _crn0084 = vec4(_B.z, _P.x, _F.z, _F.x);
    _ort0084 = vec4(_C.y, _C.w, _F.y, _B.w);
    _a0086 = 2.00000000E+00*_crn0084 - (_ort0084 + _ort0084.wxyz);
    _TMP85 = max(_a0086, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _Cs = vec4(_crn0084.x > 6.49999976E-01 ? _TMP85.x : 0.00000000E+00, _crn0084.y > 6.49999976E-01 ? _TMP85.y : 0.00000000E+00, _crn0084.z > 6.49999976E-01 ? _TMP85.z : 0.00000000E+00, _crn0084.w > 6.49999976E-01 ? _TMP85.w : 0.00000000E+00);
    _crn0090 = vec4(_N.z, _E.x, _G.z, _G.x);
    _ort0090 = vec4(_D.y, _D.w, _G.y, _N.w);
    _a0092 = 2.00000000E+00*_crn0090 - (_ort0090 + _ort0090.wxyz);
    _TMP91 = max(_a0092, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _Ds = vec4(_crn0090.x > 6.49999976E-01 ? _TMP91.x : 0.00000000E+00, _crn0090.y > 6.49999976E-01 ? _TMP91.y : 0.00000000E+00, _crn0090.z > 6.49999976E-01 ? _TMP91.z : 0.00000000E+00, _crn0090.w > 6.49999976E-01 ? _TMP91.w : 0.00000000E+00);
    _crn0096 = vec4(_D.z, _F.x, _H.z, _H.x);
    _ort0096 = vec4(_E.y, _E.w, _H.y, _D.w);
    _a0098 = 2.00000000E+00*_crn0096 - (_ort0096 + _ort0096.wxyz);
    _TMP97 = max(_a0098, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _Es = vec4(_crn0096.x > 6.49999976E-01 ? _TMP97.x : 0.00000000E+00, _crn0096.y > 6.49999976E-01 ? _TMP97.y : 0.00000000E+00, _crn0096.z > 6.49999976E-01 ? _TMP97.z : 0.00000000E+00, _crn0096.w > 6.49999976E-01 ? _TMP97.w : 0.00000000E+00);
    _crn0102 = vec4(_E.z, _Q.x, _I.z, _I.x);
    _ort0102 = vec4(_F.y, _F.w, _I.y, _E.w);
    _a0104 = 2.00000000E+00*_crn0102 - (_ort0102 + _ort0102.wxyz);
    _TMP103 = max(_a0104, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _Fs = vec4(_crn0102.x > 6.49999976E-01 ? _TMP103.x : 0.00000000E+00, _crn0102.y > 6.49999976E-01 ? _TMP103.y : 0.00000000E+00, _crn0102.z > 6.49999976E-01 ? _TMP103.z : 0.00000000E+00, _crn0102.w > 6.49999976E-01 ? _TMP103.w : 0.00000000E+00);
    _crn0108 = vec4(_O.z, _H.x, _J.z, _J.x);
    _ort0108 = vec4(_G.y, _G.w, _J.y, _O.w);
    _a0110 = 2.00000000E+00*_crn0108 - (_ort0108 + _ort0108.wxyz);
    _TMP109 = max(_a0110, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _Gs = vec4(_crn0108.x > 6.49999976E-01 ? _TMP109.x : 0.00000000E+00, _crn0108.y > 6.49999976E-01 ? _TMP109.y : 0.00000000E+00, _crn0108.z > 6.49999976E-01 ? _TMP109.z : 0.00000000E+00, _crn0108.w > 6.49999976E-01 ? _TMP109.w : 0.00000000E+00);
    _crn0114 = vec4(_G.z, _I.x, _K.z, _K.x);
    _ort0114 = vec4(_H.y, _H.w, _K.y, _G.w);
    _a0116 = 2.00000000E+00*_crn0114 - (_ort0114 + _ort0114.wxyz);
    _TMP115 = max(_a0116, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _Hs = vec4(_crn0114.x > 6.49999976E-01 ? _TMP115.x : 0.00000000E+00, _crn0114.y > 6.49999976E-01 ? _TMP115.y : 0.00000000E+00, _crn0114.z > 6.49999976E-01 ? _TMP115.z : 0.00000000E+00, _crn0114.w > 6.49999976E-01 ? _TMP115.w : 0.00000000E+00);
    _crn0120 = vec4(_H.z, _R.x, _L.z, _L.x);
    _ort0120 = vec4(_I.y, _I.w, _L.y, _H.w);
    _a0122 = 2.00000000E+00*_crn0120 - (_ort0120 + _ort0120.wxyz);
    _TMP121 = max(_a0122, vec4( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00, 0.00000000E+00));
    _Is = vec4(_crn0120.x > 6.49999976E-01 ? _TMP121.x : 0.00000000E+00, _crn0120.y > 6.49999976E-01 ? _TMP121.y : 0.00000000E+00, _crn0120.z > 6.49999976E-01 ? _TMP121.z : 0.00000000E+00, _crn0120.w > 6.49999976E-01 ? _TMP121.w : 0.00000000E+00);
    _jSx = vec4(_As.z, _Bs.w, _Es.x, _Ds.y);
    _a0128 = 2.00000000E+00*_As.z - (_As.y + _As.w);
    _res0126.x = max(_a0128, 0.00000000E+00);
    _a0130 = 2.00000000E+00*_Bs.w - (_Bs.z + _Bs.x);
    _res0126.y = max(_a0130, 0.00000000E+00);
    _a0132 = 2.00000000E+00*_Es.x - (_Es.w + _Es.y);
    _res0126.z = max(_a0132, 0.00000000E+00);
    _a0134 = 2.00000000E+00*_Ds.y - (_Ds.x + _Ds.z);
    _res0126.w = max(_a0134, 0.00000000E+00);
    _jSy = vec4(_Bs.z, _Cs.w, _Fs.x, _Es.y);
    _a0138 = 2.00000000E+00*_Bs.z - (_Bs.y + _Bs.w);
    _res0136.x = max(_a0138, 0.00000000E+00);
    _a0140 = 2.00000000E+00*_Cs.w - (_Cs.z + _Cs.x);
    _res0136.y = max(_a0140, 0.00000000E+00);
    _a0142 = 2.00000000E+00*_Fs.x - (_Fs.w + _Fs.y);
    _res0136.z = max(_a0142, 0.00000000E+00);
    _a0144 = 2.00000000E+00*_Es.y - (_Es.x + _Es.z);
    _res0136.w = max(_a0144, 0.00000000E+00);
    _jSz = vec4(_Es.z, _Fs.w, _Is.x, _Hs.y);
    _a0148 = 2.00000000E+00*_Es.z - (_Es.y + _Es.w);
    _res0146.x = max(_a0148, 0.00000000E+00);
    _a0150 = 2.00000000E+00*_Fs.w - (_Fs.z + _Fs.x);
    _res0146.y = max(_a0150, 0.00000000E+00);
    _a0152 = 2.00000000E+00*_Is.x - (_Is.w + _Is.y);
    _res0146.z = max(_a0152, 0.00000000E+00);
    _a0154 = 2.00000000E+00*_Hs.y - (_Hs.x + _Hs.z);
    _res0146.w = max(_a0154, 0.00000000E+00);
    _jSw = vec4(_Ds.z, _Es.w, _Hs.x, _Gs.y);
    _a0158 = 2.00000000E+00*_Ds.z - (_Ds.y + _Ds.w);
    _res0156.x = max(_a0158, 0.00000000E+00);
    _a0160 = 2.00000000E+00*_Es.w - (_Es.z + _Es.x);
    _res0156.y = max(_a0160, 0.00000000E+00);
    _a0162 = 2.00000000E+00*_Hs.x - (_Hs.w + _Hs.y);
    _res0156.z = max(_a0162, 0.00000000E+00);
    _a0164 = 2.00000000E+00*_Gs.y - (_Gs.x + _Gs.z);
    _res0156.w = max(_a0164, 0.00000000E+00);
    _jx = bvec4(_res0126.x != 0.00000000E+00 && (_res0126 + _res0126.zwxy).x > (_res0126.yzwx + _res0126.wxyz).x, _res0126.y != 0.00000000E+00 && (_res0126 + _res0126.zwxy).y > (_res0126.yzwx + _res0126.wxyz).y, _res0126.z != 0.00000000E+00 && (_res0126 + _res0126.zwxy).z > (_res0126.yzwx + _res0126.wxyz).z, _res0126.w != 0.00000000E+00 && (_res0126 + _res0126.zwxy).w > (_res0126.yzwx + _res0126.wxyz).w);
    _jy = bvec4(_res0136.x != 0.00000000E+00 && (_res0136 + _res0136.zwxy).x > (_res0136.yzwx + _res0136.wxyz).x, _res0136.y != 0.00000000E+00 && (_res0136 + _res0136.zwxy).y > (_res0136.yzwx + _res0136.wxyz).y, _res0136.z != 0.00000000E+00 && (_res0136 + _res0136.zwxy).z > (_res0136.yzwx + _res0136.wxyz).z, _res0136.w != 0.00000000E+00 && (_res0136 + _res0136.zwxy).w > (_res0136.yzwx + _res0136.wxyz).w);
    _jz = bvec4(_res0146.x != 0.00000000E+00 && (_res0146 + _res0146.zwxy).x > (_res0146.yzwx + _res0146.wxyz).x, _res0146.y != 0.00000000E+00 && (_res0146 + _res0146.zwxy).y > (_res0146.yzwx + _res0146.wxyz).y, _res0146.z != 0.00000000E+00 && (_res0146 + _res0146.zwxy).z > (_res0146.yzwx + _res0146.wxyz).z, _res0146.w != 0.00000000E+00 && (_res0146 + _res0146.zwxy).w > (_res0146.yzwx + _res0146.wxyz).w);
    _jw = bvec4(_res0156.x != 0.00000000E+00 && (_res0156 + _res0156.zwxy).x > (_res0156.yzwx + _res0156.wxyz).x, _res0156.y != 0.00000000E+00 && (_res0156 + _res0156.zwxy).y > (_res0156.yzwx + _res0156.wxyz).y, _res0156.z != 0.00000000E+00 && (_res0156 + _res0156.zwxy).z > (_res0156.yzwx + _res0156.wxyz).z, _res0156.w != 0.00000000E+00 && (_res0156 + _res0156.zwxy).w > (_res0156.yzwx + _res0156.wxyz).w);
    _res1.x = _jx.z || !(_jx.y || _jx.w) && _jSx.z != 0.00000000E+00 && (_jx.x || _jSx.x + _jSx.z > _jSx.y + _jSx.w);
    _res1.y = _jy.w || !(_jy.z || _jy.x) && _jSy.w != 0.00000000E+00 && (_jy.y || _jSy.y + _jSy.w > _jSy.x + _jSy.z);
    _res1.z = _jz.x || !(_jz.w || _jz.y) && _jSz.x != 0.00000000E+00 && (_jz.z || _jSz.x + _jSz.z > _jSz.y + _jSz.w);
    _res1.w = _jw.y || !(_jw.x || _jw.z) && _jSw.y != 0.00000000E+00 && (_jw.w || _jSw.y + _jSw.w > _jSw.x + _jSw.z);
    _res1 = bvec4(_res1.x && (_jx.z || !(_res1.w && _res1.y)), _res1.y && (_jy.w || !(_res1.x && _res1.z)), _res1.z && (_jz.x || !(_res1.y && _res1.w)), _res1.w && (_jw.y || !(_res1.z && _res1.x)));
    _crn0166 = vec2(_D.z, _E.x);
    _ort0166 = vec4(_A.w, _E.y, _D.w, _D.y);
    _a0168 = bvec4(_crn0166.x <= 6.49999976E-01 || _crn0166.x <= _ort0166.x || _crn0166.x <= _ort0166.w, _crn0166.y <= 6.49999976E-01 || _crn0166.y <= _ort0166.y || _crn0166.y <= _ort0166.x, _crn0166.x <= 6.49999976E-01 || _crn0166.x <= _ort0166.z || _crn0166.x <= _ort0166.y, _crn0166.y <= 6.49999976E-01 || _crn0166.y <= _ort0166.w || _crn0166.y <= _ort0166.z);
    _TMP165 = _a0168.x && _a0168.y && _a0168.z && _a0168.w;
    _crn0170 = vec2(_E.z, _F.x);
    _ort0170 = vec4(_B.w, _F.y, _E.w, _E.y);
    _a0172 = bvec4(_crn0170.x <= 6.49999976E-01 || _crn0170.x <= _ort0170.x || _crn0170.x <= _ort0170.w, _crn0170.y <= 6.49999976E-01 || _crn0170.y <= _ort0170.y || _crn0170.y <= _ort0170.x, _crn0170.x <= 6.49999976E-01 || _crn0170.x <= _ort0170.z || _crn0170.x <= _ort0170.y, _crn0170.y <= 6.49999976E-01 || _crn0170.y <= _ort0170.w || _crn0170.y <= _ort0170.z);
    _TMP169 = _a0172.x && _a0172.y && _a0172.z && _a0172.w;
    _crn0174 = vec2(_H.z, _I.x);
    _ort0174 = vec4(_E.w, _I.y, _H.w, _H.y);
    _a0176 = bvec4(_crn0174.x <= 6.49999976E-01 || _crn0174.x <= _ort0174.x || _crn0174.x <= _ort0174.w, _crn0174.y <= 6.49999976E-01 || _crn0174.y <= _ort0174.y || _crn0174.y <= _ort0174.x, _crn0174.x <= 6.49999976E-01 || _crn0174.x <= _ort0174.z || _crn0174.x <= _ort0174.y, _crn0174.y <= 6.49999976E-01 || _crn0174.y <= _ort0174.w || _crn0174.y <= _ort0174.z);
    _TMP173 = _a0176.x && _a0176.y && _a0176.z && _a0176.w;
    _crn0178 = vec2(_G.z, _H.x);
    _ort0178 = vec4(_D.w, _H.y, _G.w, _G.y);
    _a0180 = bvec4(_crn0178.x <= 6.49999976E-01 || _crn0178.x <= _ort0178.x || _crn0178.x <= _ort0178.w, _crn0178.y <= 6.49999976E-01 || _crn0178.y <= _ort0178.y || _crn0178.y <= _ort0178.x, _crn0178.x <= 6.49999976E-01 || _crn0178.x <= _ort0178.z || _crn0178.x <= _ort0178.y, _crn0178.y <= 6.49999976E-01 || _crn0178.y <= _ort0178.w || _crn0178.y <= _ort0178.z);
    _TMP177 = _a0180.x && _a0180.y && _a0180.z && _a0180.w;
    _a0182 = vec4(_E.y, _E.w, _H.y, _D.w);
    _TMP181 = max(_a0182, vec4( 6.49999976E-01, 6.49999976E-01, 6.49999976E-01, 6.49999976E-01));
    _TMP0 = max(_D.w, _A.w);
    _TMP1 = max(_E.w, _B.w);
    _TMP2 = max(_E.w, _H.w);
    _TMP3 = max(_D.w, _G.w);
    _hori = bvec4(_TMP181.x < _TMP0 && _TMP165, _TMP181.x < _TMP1 && _TMP169, _TMP181.z < _TMP2 && _TMP173, _TMP181.z < _TMP3 && _TMP177);
    _TMP4 = max(_E.y, _D.y);
    _TMP5 = max(_E.y, _F.y);
    _TMP6 = max(_H.y, _I.y);
    _TMP7 = max(_H.y, _G.y);
    _vert = bvec4(_TMP181.w < _TMP4 && _TMP165, _TMP181.y < _TMP5 && _TMP169, _TMP181.y < _TMP6 && _TMP173, _TMP181.w < _TMP7 && _TMP177);
    _or = bvec4(_A.w < _D.y, _B.w <= _F.y, _H.w < _I.y, _G.w <= _G.y);
    _ret_0 = (vec4(float(_res1.x), float(_res1.y), float(_res1.z), float(_res1.w)) + 2.00000000E+00*vec4(float(_hori.x), float(_hori.y), float(_hori.z), float(_hori.w)) + 4.00000000E+00*vec4(float(_vert.x), float(_vert.y), float(_vert.z), float(_vert.w)) + 8.00000000E+00*vec4(float(_or.x), float(_or.y), float(_or.z), float(_or.w)))/1.50000000E+01;
    FragColor = _ret_0;
    return;
} 
#endif
