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
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord1;
COMPAT_VARYING     vec4 _position1;
COMPAT_VARYING     vec2 _tex_coord;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
struct prev {
    vec2 _texture_size1;
    vec2 _tex_coord;
float _placeholder36;
};
struct out_vertex {
    vec4 _position1;
    vec2 _texCoord1;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
};
out_vertex _ret_0;
input_dummy _IN1;
vec4 _r0007;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
 
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
    _r0007 = VertexCoord.x*MVPMatrix[0];
    _r0007 = _r0007 + VertexCoord.y*MVPMatrix[1];
    _r0007 = _r0007 + VertexCoord.z*MVPMatrix[2];
    _r0007 = _r0007 + VertexCoord.w*MVPMatrix[3];
    _ps = 1.00000000E+00/TextureSize;
    _OUT._t1 = TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, -_ps.y);
    _OUT._t2 = TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, 0.00000000E+00);
    _OUT._t3 = TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, _ps.y);
    _ret_0._position1 = _r0007;
    _ret_0._texCoord1 = TexCoord.xy;
    _ret_0._t1 = _OUT._t1;
    _ret_0._t2 = _OUT._t2;
    _ret_0._t3 = _OUT._t3;
    gl_Position = _r0007;
    TEX0.xy = TexCoord.xy;
    TEX1 = _OUT._t1;
    TEX2 = _OUT._t2;
    TEX3 = _OUT._t3;
    return;
    TEX0.xy = _ret_0._texCoord1;
    TEX1 = _ret_0._t1;
    TEX2 = _ret_0._t2;
    TEX3 = _ret_0._t3;
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
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord;
COMPAT_VARYING     vec2 _tex_coord;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
struct prev {
    vec2 _texture_size1;
    vec2 _tex_coord;
float _placeholder34;
};
struct out_vertex {
    vec2 _texCoord;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
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
float _TMP13;
float _TMP10;
float _TMP12;
float _TMP11;
float _TMP9;
float _TMP8;
uniform sampler2D Texture;
prev _PASSPREV21;
vec2 _a0061;
vec2 _b0061;
vec2 _crn0061;
vec2 _a0075;
vec2 _b0075;
vec2 _crn0075;
vec2 _a0089;
vec2 _b0089;
vec2 _crn0089;
vec2 _a0103;
vec2 _b0103;
vec2 _crn0103;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
 
uniform sampler2D PassPrev2Texture;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    vec4 _A;
    vec4 _B;
    vec4 _D;
    vec4 _E;
    vec4 _F;
    vec4 _G;
    vec4 _H;
    vec4 _I;
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
    vec4 _jDx;
    vec4 _jSy;
    vec4 _jDy;
    vec4 _jSz;
    vec4 _jDz;
    vec4 _jSw;
    vec4 _jDw;
    bvec4 _jx;
    bvec4 _jy;
    bvec4 _jz;
    bvec4 _jw;
    bvec4 _res;
    bvec4 _clr;
    vec4 _h;
    vec4 _v;
    bvec4 _or;
    bvec4 _hori;
    bvec4 _vert;
    _A = COMPAT_TEXTURE(PassPrev2Texture, TEX1.xw);
    _B = COMPAT_TEXTURE(PassPrev2Texture, TEX1.yw);
    _D = COMPAT_TEXTURE(PassPrev2Texture, TEX2.xw);
    _E = COMPAT_TEXTURE(PassPrev2Texture, TEX2.yw);
    _F = COMPAT_TEXTURE(PassPrev2Texture, TEX2.zw);
    _G = COMPAT_TEXTURE(PassPrev2Texture, TEX3.xw);
    _H = COMPAT_TEXTURE(PassPrev2Texture, TEX3.yw);
    _I = COMPAT_TEXTURE(PassPrev2Texture, TEX3.zw);
    _As = COMPAT_TEXTURE(Texture, TEX1.xw);
    _Bs = COMPAT_TEXTURE(Texture, TEX1.yw);
    _Cs = COMPAT_TEXTURE(Texture, TEX1.zw);
    _Ds = COMPAT_TEXTURE(Texture, TEX2.xw);
    _Es = COMPAT_TEXTURE(Texture, TEX2.yw);
    _Fs = COMPAT_TEXTURE(Texture, TEX2.zw);
    _Gs = COMPAT_TEXTURE(Texture, TEX3.xw);
    _Hs = COMPAT_TEXTURE(Texture, TEX3.yw);
    _Is = COMPAT_TEXTURE(Texture, TEX3.zw);
    _jSx = vec4(_As.z, _Bs.w, _Es.x, _Ds.y);
    _jDx = 2.00000000E+00*vec4(_As.z, _Bs.w, _Es.x, _Ds.y) - (vec4(_As.y, _Bs.z, _Es.w, _Ds.x) + vec4(_As.w, _Bs.x, _Es.y, _Ds.z));
    _jSy = vec4(_Bs.z, _Cs.w, _Fs.x, _Es.y);
    _jDy = 2.00000000E+00*vec4(_Bs.z, _Cs.w, _Fs.x, _Es.y) - (vec4(_Bs.y, _Cs.z, _Fs.w, _Es.x) + vec4(_Bs.w, _Cs.x, _Fs.y, _Es.z));
    _jSz = vec4(_Es.z, _Fs.w, _Is.x, _Hs.y);
    _jDz = 2.00000000E+00*vec4(_Es.z, _Fs.w, _Is.x, _Hs.y) - (vec4(_Es.y, _Fs.z, _Is.w, _Hs.x) + vec4(_Es.w, _Fs.x, _Is.y, _Hs.z));
    _jSw = vec4(_Ds.z, _Es.w, _Hs.x, _Gs.y);
    _jDw = 2.00000000E+00*vec4(_Ds.z, _Es.w, _Hs.x, _Gs.y) - (vec4(_Ds.y, _Es.z, _Hs.w, _Gs.x) + vec4(_Ds.w, _Es.x, _Hs.y, _Gs.z));
    _jx = bvec4(_jDx.x > 0.00000000E+00 && (_jDx.y <= 0.00000000E+00 && _jDx.w <= 0.00000000E+00 || (_jDx + _jDx.zwxy).x > (_jDx.yzwx + _jDx.wxyz).x), _jDx.y > 0.00000000E+00 && (_jDx.z <= 0.00000000E+00 && _jDx.x <= 0.00000000E+00 || (_jDx + _jDx.zwxy).y > (_jDx.yzwx + _jDx.wxyz).y), _jDx.z > 0.00000000E+00 && (_jDx.w <= 0.00000000E+00 && _jDx.y <= 0.00000000E+00 || (_jDx + _jDx.zwxy).z > (_jDx.yzwx + _jDx.wxyz).z), _jDx.w > 0.00000000E+00 && (_jDx.x <= 0.00000000E+00 && _jDx.z <= 0.00000000E+00 || (_jDx + _jDx.zwxy).w > (_jDx.yzwx + _jDx.wxyz).w));
    _jy = bvec4(_jDy.x > 0.00000000E+00 && (_jDy.y <= 0.00000000E+00 && _jDy.w <= 0.00000000E+00 || (_jDy + _jDy.zwxy).x > (_jDy.yzwx + _jDy.wxyz).x), _jDy.y > 0.00000000E+00 && (_jDy.z <= 0.00000000E+00 && _jDy.x <= 0.00000000E+00 || (_jDy + _jDy.zwxy).y > (_jDy.yzwx + _jDy.wxyz).y), _jDy.z > 0.00000000E+00 && (_jDy.w <= 0.00000000E+00 && _jDy.y <= 0.00000000E+00 || (_jDy + _jDy.zwxy).z > (_jDy.yzwx + _jDy.wxyz).z), _jDy.w > 0.00000000E+00 && (_jDy.x <= 0.00000000E+00 && _jDy.z <= 0.00000000E+00 || (_jDy + _jDy.zwxy).w > (_jDy.yzwx + _jDy.wxyz).w));
    _jz = bvec4(_jDz.x > 0.00000000E+00 && (_jDz.y <= 0.00000000E+00 && _jDz.w <= 0.00000000E+00 || (_jDz + _jDz.zwxy).x > (_jDz.yzwx + _jDz.wxyz).x), _jDz.y > 0.00000000E+00 && (_jDz.z <= 0.00000000E+00 && _jDz.x <= 0.00000000E+00 || (_jDz + _jDz.zwxy).y > (_jDz.yzwx + _jDz.wxyz).y), _jDz.z > 0.00000000E+00 && (_jDz.w <= 0.00000000E+00 && _jDz.y <= 0.00000000E+00 || (_jDz + _jDz.zwxy).z > (_jDz.yzwx + _jDz.wxyz).z), _jDz.w > 0.00000000E+00 && (_jDz.x <= 0.00000000E+00 && _jDz.z <= 0.00000000E+00 || (_jDz + _jDz.zwxy).w > (_jDz.yzwx + _jDz.wxyz).w));
    _jw = bvec4(_jDw.x > 0.00000000E+00 && (_jDw.y <= 0.00000000E+00 && _jDw.w <= 0.00000000E+00 || (_jDw + _jDw.zwxy).x > (_jDw.yzwx + _jDw.wxyz).x), _jDw.y > 0.00000000E+00 && (_jDw.z <= 0.00000000E+00 && _jDw.x <= 0.00000000E+00 || (_jDw + _jDw.zwxy).y > (_jDw.yzwx + _jDw.wxyz).y), _jDw.z > 0.00000000E+00 && (_jDw.w <= 0.00000000E+00 && _jDw.y <= 0.00000000E+00 || (_jDw + _jDw.zwxy).z > (_jDw.yzwx + _jDw.wxyz).z), _jDw.w > 0.00000000E+00 && (_jDw.x <= 0.00000000E+00 && _jDw.z <= 0.00000000E+00 || (_jDw + _jDw.zwxy).w > (_jDw.yzwx + _jDw.wxyz).w));
    _res.x = _jx.z || !(_jx.y || _jx.w) && _jSx.z != 0.00000000E+00 && (_jx.x || _jSx.x + _jSx.z > _jSx.y + _jSx.w);
    _res.y = _jy.w || !(_jy.z || _jy.x) && _jSy.w != 0.00000000E+00 && (_jy.y || _jSy.y + _jSy.w > _jSy.x + _jSy.z);
    _res.z = _jz.x || !(_jz.w || _jz.y) && _jSz.x != 0.00000000E+00 && (_jz.z || _jSz.x + _jSz.z > _jSz.y + _jSz.w);
    _res.w = _jw.y || !(_jw.x || _jw.z) && _jSw.y != 0.00000000E+00 && (_jw.w || _jSw.y + _jSw.w > _jSw.x + _jSw.z);
    _res = bvec4(_res.x && (_jx.z || !(_res.w && _res.y)), _res.y && (_jy.w || !(_res.x && _res.z)), _res.z && (_jz.x || !(_res.y && _res.w)), _res.w && (_jw.y || !(_res.z && _res.x)));
    _crn0061 = vec2(_D.z, _E.x);
    _a0061 = vec2(_D.w, _E.y);
    _b0061 = vec2(_A.w, _D.y);
    _TMP8 = min(_a0061.x, _a0061.y);
    _TMP9 = min(_b0061.x, _b0061.y);
    _TMP10 = max(_TMP8, _TMP9);
    _TMP11 = min(_a0061.x, _b0061.y);
    _TMP12 = min(_b0061.x, _a0061.y);
    _TMP13 = max(_TMP11, _TMP12);
    _clr.x = _crn0061.x >= _TMP10 && _crn0061.y >= _TMP13;
    _crn0075 = vec2(_F.x, _E.z);
    _a0075 = vec2(_E.w, _E.y);
    _b0075 = vec2(_B.w, _F.y);
    _TMP8 = min(_a0075.x, _a0075.y);
    _TMP9 = min(_b0075.x, _b0075.y);
    _TMP10 = max(_TMP8, _TMP9);
    _TMP11 = min(_a0075.x, _b0075.y);
    _TMP12 = min(_b0075.x, _a0075.y);
    _TMP13 = max(_TMP11, _TMP12);
    _clr.y = _crn0075.x >= _TMP10 && _crn0075.y >= _TMP13;
    _crn0089 = vec2(_H.z, _I.x);
    _a0089 = vec2(_E.w, _H.y);
    _b0089 = vec2(_H.w, _I.y);
    _TMP8 = min(_a0089.x, _a0089.y);
    _TMP9 = min(_b0089.x, _b0089.y);
    _TMP10 = max(_TMP8, _TMP9);
    _TMP11 = min(_a0089.x, _b0089.y);
    _TMP12 = min(_b0089.x, _a0089.y);
    _TMP13 = max(_TMP11, _TMP12);
    _clr.z = _crn0089.x >= _TMP10 && _crn0089.y >= _TMP13;
    _crn0103 = vec2(_H.x, _G.z);
    _a0103 = vec2(_D.w, _H.y);
    _b0103 = vec2(_G.w, _G.y);
    _TMP8 = min(_a0103.x, _a0103.y);
    _TMP9 = min(_b0103.x, _b0103.y);
    _TMP10 = max(_TMP8, _TMP9);
    _TMP11 = min(_a0103.x, _b0103.y);
    _TMP12 = min(_b0103.x, _a0103.y);
    _TMP13 = max(_TMP11, _TMP12);
    _clr.w = _crn0103.x >= _TMP10 && _crn0103.y >= _TMP13;
    _TMP0 = min(_D.w, _A.w);
    _TMP1 = min(_E.w, _B.w);
    _TMP2 = min(_E.w, _H.w);
    _TMP3 = min(_D.w, _G.w);
    _h = vec4(_TMP0, _TMP1, _TMP2, _TMP3);
    _TMP4 = min(_E.y, _D.y);
    _TMP5 = min(_E.y, _F.y);
    _TMP6 = min(_H.y, _I.y);
    _TMP7 = min(_H.y, _G.y);
    _v = vec4(_TMP4, _TMP5, _TMP6, _TMP7);
    _or = bvec4((_h + vec4(_D.w, _E.w, _E.w, _D.w)).x > (_v + vec4(_E.y, _E.y, _H.y, _H.y)).x, (_h + vec4(_D.w, _E.w, _E.w, _D.w)).y > (_v + vec4(_E.y, _E.y, _H.y, _H.y)).y, (_h + vec4(_D.w, _E.w, _E.w, _D.w)).z > (_v + vec4(_E.y, _E.y, _H.y, _H.y)).z, (_h + vec4(_D.w, _E.w, _E.w, _D.w)).w > (_v + vec4(_E.y, _E.y, _H.y, _H.y)).w);
    _hori = bvec4(_h.x < _v.x && _clr.x, _h.y < _v.y && _clr.y, _h.z < _v.z && _clr.z, _h.w < _v.w && _clr.w);
    _vert = bvec4(_h.x > _v.x && _clr.x, _h.y > _v.y && _clr.y, _h.z > _v.z && _clr.z, _h.w > _v.w && _clr.w);
    _ret_0 = (vec4(float(_res.x), float(_res.y), float(_res.z), float(_res.w)) + 2.00000000E+00*vec4(float(_hori.x), float(_hori.y), float(_hori.z), float(_hori.w)) + 4.00000000E+00*vec4(float(_vert.x), float(_vert.y), float(_vert.z), float(_vert.w)) + 8.00000000E+00*vec4(float(_or.x), float(_or.y), float(_or.z), float(_or.w)))/1.50000000E+01;
    FragColor = _ret_0;
    return;
} 
#endif
