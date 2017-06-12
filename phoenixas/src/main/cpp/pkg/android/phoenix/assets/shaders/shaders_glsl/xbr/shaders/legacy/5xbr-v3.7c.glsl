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
COMPAT_VARYING     vec4 _t7;
COMPAT_VARYING     vec4 _t6;
COMPAT_VARYING     vec4 _t5;
COMPAT_VARYING     vec4 _t4;
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord2;
COMPAT_VARYING     vec4 _color1;
COMPAT_VARYING     vec4 _position1;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
struct out_vertex {
    vec4 _position1;
    vec4 _color1;
    vec2 _texCoord2;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
    vec4 _t4;
    vec4 _t5;
    vec4 _t6;
    vec4 _t7;
};
out_vertex _ret_0;
input_dummy _IN1;
vec4 _r0008;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 COLOR;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
COMPAT_VARYING vec4 TEX4;
COMPAT_VARYING vec4 TEX5;
COMPAT_VARYING vec4 TEX6;
COMPAT_VARYING vec4 TEX7;
 
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
    vec2 _texCoord;
    _r0008 = VertexCoord.x*MVPMatrix[0];
    _r0008 = _r0008 + VertexCoord.y*MVPMatrix[1];
    _r0008 = _r0008 + VertexCoord.z*MVPMatrix[2];
    _r0008 = _r0008 + VertexCoord.w*MVPMatrix[3];
    _ps = vec2(1.00000000E+00/TextureSize.x, 1.00000000E+00/TextureSize.y);
    _texCoord = TexCoord.xy + vec2( 1.00000001E-07, 1.00000001E-07);
    _OUT._t1 = _texCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, -2.00000000E+00*_ps.y);
    _OUT._t2 = _texCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, -_ps.y);
    _OUT._t3 = _texCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, 0.00000000E+00);
    _OUT._t4 = _texCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, _ps.y);
    _OUT._t5 = _texCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, 2.00000000E+00*_ps.y);
    _OUT._t6 = _texCoord.xyyy + vec4(-2.00000000E+00*_ps.x, -_ps.y, 0.00000000E+00, _ps.y);
    _OUT._t7 = _texCoord.xyyy + vec4(2.00000000E+00*_ps.x, -_ps.y, 0.00000000E+00, _ps.y);
    _ret_0._position1 = _r0008;
    _ret_0._color1 = COLOR;
    _ret_0._texCoord2 = _texCoord;
    _ret_0._t1 = _OUT._t1;
    _ret_0._t2 = _OUT._t2;
    _ret_0._t3 = _OUT._t3;
    _ret_0._t4 = _OUT._t4;
    _ret_0._t5 = _OUT._t5;
    _ret_0._t6 = _OUT._t6;
    _ret_0._t7 = _OUT._t7;
    gl_Position = _r0008;
    COL0 = COLOR;
    TEX0.xy = _texCoord;
    TEX1 = _OUT._t1;
    TEX2 = _OUT._t2;
    TEX3 = _OUT._t3;
    TEX4 = _OUT._t4;
    TEX5 = _OUT._t5;
    TEX6 = _OUT._t6;
    TEX7 = _OUT._t7;
    return;
    COL0 = _ret_0._color1;
    TEX0.xy = _ret_0._texCoord2;
    TEX1 = _ret_0._t1;
    TEX2 = _ret_0._t2;
    TEX3 = _ret_0._t3;
    TEX4 = _ret_0._t4;
    TEX5 = _ret_0._t5;
    TEX6 = _ret_0._t6;
    TEX7 = _ret_0._t7;
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
COMPAT_VARYING     vec4 _t7;
COMPAT_VARYING     vec4 _t6;
COMPAT_VARYING     vec4 _t5;
COMPAT_VARYING     vec4 _t4;
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord;
COMPAT_VARYING     vec4 _color;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
struct out_vertex {
    vec4 _color;
    vec2 _texCoord;
    vec4 _t1;
    vec4 _t2;
    vec4 _t3;
    vec4 _t4;
    vec4 _t5;
    vec4 _t6;
    vec4 _t7;
};
vec4 _ret_0;
float _TMP53;
vec3 _TMP46;
vec3 _TMP48;
vec3 _TMP50;
vec3 _TMP51;
vec3 _TMP49;
vec3 _TMP47;
vec3 _TMP40;
vec3 _TMP42;
vec3 _TMP44;
vec3 _TMP45;
vec3 _TMP43;
vec3 _TMP41;
vec4 _TMP33;
vec4 _TMP32;
bvec4 _TMP31;
bvec4 _TMP30;
bvec4 _TMP29;
bvec4 _TMP28;
bvec4 _TMP27;
bvec4 _TMP26;
bvec4 _TMP25;
bvec4 _TMP24;
bvec4 _TMP23;
bvec4 _TMP22;
bvec4 _TMP21;
vec4 _TMP20;
vec4 _TMP19;
vec4 _TMP18;
vec4 _TMP17;
vec4 _TMP16;
vec4 _TMP15;
vec4 _TMP14;
vec4 _TMP13;
vec4 _TMP12;
vec4 _TMP11;
vec4 _TMP10;
vec4 _TMP9;
vec4 _TMP8;
vec4 _TMP7;
vec4 _TMP6;
vec4 _TMP5;
vec4 _TMP4;
vec4 _TMP3;
vec4 _TMP2;
vec4 _TMP1;
vec4 _TMP0;
uniform sampler2D Texture;
input_dummy _IN1;
vec2 _x0074;
vec4 _r0118;
vec4 _r0128;
vec4 _r0138;
vec4 _r0148;
vec4 _r0158;
vec4 _r0168;
vec4 _TMP179;
vec4 _a0182;
vec4 _TMP185;
vec4 _a0188;
vec4 _TMP191;
vec4 _a0194;
vec4 _TMP197;
vec4 _a0200;
vec4 _TMP203;
vec4 _a0206;
vec4 _TMP209;
vec4 _a0212;
vec4 _TMP215;
vec4 _a0218;
vec4 _TMP221;
vec4 _a0224;
vec4 _TMP227;
vec4 _a0230;
vec4 _TMP233;
vec4 _a0236;
vec4 _TMP239;
vec4 _a0242;
vec4 _TMP245;
vec4 _a0248;
vec4 _TMP249;
vec4 _a0252;
vec4 _TMP253;
vec4 _a0256;
vec4 _TMP257;
vec4 _a0260;
vec4 _TMP261;
vec4 _a0264;
vec4 _TMP267;
vec4 _a0270;
vec4 _TMP271;
vec4 _a0274;
vec4 _TMP275;
vec4 _a0278;
vec4 _TMP279;
vec4 _a0282;
vec4 _TMP283;
vec4 _a0286;
vec4 _TMP287;
vec4 _a0290;
vec4 _TMP291;
vec4 _a0294;
vec4 _TMP295;
vec4 _a0298;
vec4 _TMP299;
vec4 _a0302;
vec4 _TMP303;
vec4 _a0306;
vec4 _TMP307;
vec4 _a0310;
vec2 _r0312;
vec2 _a0318;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
COMPAT_VARYING vec4 TEX4;
COMPAT_VARYING vec4 TEX5;
COMPAT_VARYING vec4 TEX6;
COMPAT_VARYING vec4 TEX7;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    bvec4 _edr;
    bvec4 _edr_left;
    bvec4 _edr_up;
    bvec4 _px;
    bvec4 _interp_restriction_lv1;
    bvec4 _interp_restriction_lv2_left;
    bvec4 _interp_restriction_lv2_up;
    bvec4 _nc;
    bvec4 _fx;
    bvec4 _fx_left;
    bvec4 _fx_up;
    vec2 _fp;
    vec2 _df12;
    vec3 _res;
    _x0074 = TEX0.xy*TextureSize;
    _fp = fract(_x0074);
    _TMP0 = COMPAT_TEXTURE(Texture, TEX1.xw);
    _TMP1 = COMPAT_TEXTURE(Texture, TEX1.yw);
    _TMP2 = COMPAT_TEXTURE(Texture, TEX1.zw);
    _TMP3 = COMPAT_TEXTURE(Texture, TEX2.xw);
    _TMP4 = COMPAT_TEXTURE(Texture, TEX2.yw);
    _TMP5 = COMPAT_TEXTURE(Texture, TEX2.zw);
    _TMP6 = COMPAT_TEXTURE(Texture, TEX3.xw);
    _TMP7 = COMPAT_TEXTURE(Texture, TEX3.yw);
    _TMP8 = COMPAT_TEXTURE(Texture, TEX3.zw);
    _TMP9 = COMPAT_TEXTURE(Texture, TEX4.xw);
    _TMP10 = COMPAT_TEXTURE(Texture, TEX4.yw);
    _TMP11 = COMPAT_TEXTURE(Texture, TEX4.zw);
    _TMP12 = COMPAT_TEXTURE(Texture, TEX5.xw);
    _TMP13 = COMPAT_TEXTURE(Texture, TEX5.yw);
    _TMP14 = COMPAT_TEXTURE(Texture, TEX5.zw);
    _TMP15 = COMPAT_TEXTURE(Texture, TEX6.xy);
    _TMP16 = COMPAT_TEXTURE(Texture, TEX6.xz);
    _TMP17 = COMPAT_TEXTURE(Texture, TEX6.xw);
    _TMP18 = COMPAT_TEXTURE(Texture, TEX7.xy);
    _TMP19 = COMPAT_TEXTURE(Texture, TEX7.xz);
    _TMP20 = COMPAT_TEXTURE(Texture, TEX7.xw);
    _r0118.x = dot(_TMP4.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0118.y = dot(_TMP6.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0118.z = dot(_TMP10.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0118.w = dot(_TMP8.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0128.x = dot(_TMP5.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0128.y = dot(_TMP3.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0128.z = dot(_TMP9.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0128.w = dot(_TMP11.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0138.x = dot(_TMP7.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0138.y = dot(_TMP7.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0138.z = dot(_TMP7.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0138.w = dot(_TMP7.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0148.x = dot(_TMP20.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0148.y = dot(_TMP2.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0148.z = dot(_TMP15.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0148.w = dot(_TMP12.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0158.x = dot(_TMP14.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0158.y = dot(_TMP18.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0158.z = dot(_TMP0.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0158.w = dot(_TMP17.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0168.x = dot(_TMP13.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0168.y = dot(_TMP19.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0168.z = dot(_TMP1.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0168.w = dot(_TMP16.xyz, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _fx = bvec4((vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 1.00000000E+00, 1.00000000E+00, -1.00000000E+00, -1.00000000E+00)*_fp.x).x > 1.50000000E+00, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 1.00000000E+00, 1.00000000E+00, -1.00000000E+00, -1.00000000E+00)*_fp.x).y > 5.00000000E-01, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 1.00000000E+00, 1.00000000E+00, -1.00000000E+00, -1.00000000E+00)*_fp.x).z > -5.00000000E-01, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 1.00000000E+00, 1.00000000E+00, -1.00000000E+00, -1.00000000E+00)*_fp.x).w > 5.00000000E-01);
    _fx_left = bvec4((vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 5.00000000E-01, 2.00000000E+00, -5.00000000E-01, -2.00000000E+00)*_fp.x).x > 1.00000000E+00, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 5.00000000E-01, 2.00000000E+00, -5.00000000E-01, -2.00000000E+00)*_fp.x).y > 1.00000000E+00, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 5.00000000E-01, 2.00000000E+00, -5.00000000E-01, -2.00000000E+00)*_fp.x).z > -5.00000000E-01, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 5.00000000E-01, 2.00000000E+00, -5.00000000E-01, -2.00000000E+00)*_fp.x).w > 0.00000000E+00);
    _fx_up = bvec4((vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 2.00000000E+00, 5.00000000E-01, -2.00000000E+00, -5.00000000E-01)*_fp.x).x > 2.00000000E+00, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 2.00000000E+00, 5.00000000E-01, -2.00000000E+00, -5.00000000E-01)*_fp.x).y > 0.00000000E+00, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 2.00000000E+00, 5.00000000E-01, -2.00000000E+00, -5.00000000E-01)*_fp.x).z > -1.00000000E+00, (vec4( 1.00000000E+00, -1.00000000E+00, -1.00000000E+00, 1.00000000E+00)*_fp.y + vec4( 2.00000000E+00, 5.00000000E-01, -2.00000000E+00, -5.00000000E-01)*_fp.x).w > 5.00000000E-01);
    _a0182 = _r0118.wxyz - _r0118;
    _TMP179 = abs(_a0182);
    _TMP21 = bvec4(_TMP179.x < 1.50000000E+01, _TMP179.y < 1.50000000E+01, _TMP179.z < 1.50000000E+01, _TMP179.w < 1.50000000E+01);
    _a0188 = _r0118.wxyz - _r0128;
    _TMP185 = abs(_a0188);
    _TMP22 = bvec4(_TMP185.x < 1.50000000E+01, _TMP185.y < 1.50000000E+01, _TMP185.z < 1.50000000E+01, _TMP185.w < 1.50000000E+01);
    _a0194 = _r0118.zwxy - _r0118.yzwx;
    _TMP191 = abs(_a0194);
    _TMP23 = bvec4(_TMP191.x < 1.50000000E+01, _TMP191.y < 1.50000000E+01, _TMP191.z < 1.50000000E+01, _TMP191.w < 1.50000000E+01);
    _a0200 = _r0118.zwxy - _r0128.zwxy;
    _TMP197 = abs(_a0200);
    _TMP24 = bvec4(_TMP197.x < 1.50000000E+01, _TMP197.y < 1.50000000E+01, _TMP197.z < 1.50000000E+01, _TMP197.w < 1.50000000E+01);
    _a0206 = _r0138 - _r0128.wxyz;
    _TMP203 = abs(_a0206);
    _TMP25 = bvec4(_TMP203.x < 1.50000000E+01, _TMP203.y < 1.50000000E+01, _TMP203.z < 1.50000000E+01, _TMP203.w < 1.50000000E+01);
    _a0212 = _r0118.wxyz - _r0168.yzwx;
    _TMP209 = abs(_a0212);
    _TMP26 = bvec4(_TMP209.x < 1.50000000E+01, _TMP209.y < 1.50000000E+01, _TMP209.z < 1.50000000E+01, _TMP209.w < 1.50000000E+01);
    _a0218 = _r0118.wxyz - _r0148;
    _TMP215 = abs(_a0218);
    _TMP27 = bvec4(_TMP215.x < 1.50000000E+01, _TMP215.y < 1.50000000E+01, _TMP215.z < 1.50000000E+01, _TMP215.w < 1.50000000E+01);
    _a0224 = _r0118.zwxy - _r0168;
    _TMP221 = abs(_a0224);
    _TMP28 = bvec4(_TMP221.x < 1.50000000E+01, _TMP221.y < 1.50000000E+01, _TMP221.z < 1.50000000E+01, _TMP221.w < 1.50000000E+01);
    _a0230 = _r0118.zwxy - _r0158;
    _TMP227 = abs(_a0230);
    _TMP29 = bvec4(_TMP227.x < 1.50000000E+01, _TMP227.y < 1.50000000E+01, _TMP227.z < 1.50000000E+01, _TMP227.w < 1.50000000E+01);
    _a0236 = _r0138 - _r0128.zwxy;
    _TMP233 = abs(_a0236);
    _TMP30 = bvec4(_TMP233.x < 1.50000000E+01, _TMP233.y < 1.50000000E+01, _TMP233.z < 1.50000000E+01, _TMP233.w < 1.50000000E+01);
    _a0242 = _r0138 - _r0128;
    _TMP239 = abs(_a0242);
    _TMP31 = bvec4(_TMP239.x < 1.50000000E+01, _TMP239.y < 1.50000000E+01, _TMP239.z < 1.50000000E+01, _TMP239.w < 1.50000000E+01);
    _interp_restriction_lv1 = bvec4(_r0138.x != _r0118.w && _r0138.x != _r0118.z && (!_TMP21.x && !_TMP22.x || !_TMP23.x && !_TMP24.x || _TMP25.x && (!_TMP26.x && !_TMP27.x || !_TMP28.x && !_TMP29.x) || _TMP30.x || _TMP31.x), _r0138.y != _r0118.x && _r0138.y != _r0118.w && (!_TMP21.y && !_TMP22.y || !_TMP23.y && !_TMP24.y || _TMP25.y && (!_TMP26.y && !_TMP27.y || !_TMP28.y && !_TMP29.y) || _TMP30.y || _TMP31.y), _r0138.z != _r0118.y && _r0138.z != _r0118.x && (!_TMP21.z && !_TMP22.z || !_TMP23.z && !_TMP24.z || _TMP25.z && (!_TMP26.z && !_TMP27.z || !_TMP28.z && !_TMP29.z) || _TMP30.z || _TMP31.z), _r0138.w != _r0118.z && _r0138.w != _r0118.y && (!_TMP21.w && !_TMP22.w || !_TMP23.w && !_TMP24.w || _TMP25.w && (!_TMP26.w && !_TMP27.w || !_TMP28.w && !_TMP29.w) || _TMP30.w || _TMP31.w));
    _interp_restriction_lv2_left = bvec4(_r0138.x != _r0128.z && _r0118.y != _r0128.z, _r0138.y != _r0128.w && _r0118.z != _r0128.w, _r0138.z != _r0128.x && _r0118.w != _r0128.x, _r0138.w != _r0128.y && _r0118.x != _r0128.y);
    _interp_restriction_lv2_up = bvec4(_r0138.x != _r0128.x && _r0118.x != _r0128.x, _r0138.y != _r0128.y && _r0118.y != _r0128.y, _r0138.z != _r0128.z && _r0118.z != _r0128.z, _r0138.w != _r0128.w && _r0118.w != _r0128.w);
    _a0248 = _r0138 - _r0128;
    _TMP245 = abs(_a0248);
    _a0252 = _r0138 - _r0128.zwxy;
    _TMP249 = abs(_a0252);
    _a0256 = _r0128.wxyz - _r0168;
    _TMP253 = abs(_a0256);
    _a0260 = _r0128.wxyz - _r0168.yzwx;
    _TMP257 = abs(_a0260);
    _a0264 = _r0118.zwxy - _r0118.wxyz;
    _TMP261 = abs(_a0264);
    _TMP32 = _TMP245 + _TMP249 + _TMP253 + _TMP257 + 4.00000000E+00*_TMP261;
    _a0270 = _r0118.zwxy - _r0118.yzwx;
    _TMP267 = abs(_a0270);
    _a0274 = _r0118.zwxy - _r0158;
    _TMP271 = abs(_a0274);
    _a0278 = _r0118.wxyz - _r0148;
    _TMP275 = abs(_a0278);
    _a0282 = _r0118.wxyz - _r0118;
    _TMP279 = abs(_a0282);
    _a0286 = _r0138 - _r0128.wxyz;
    _TMP283 = abs(_a0286);
    _TMP33 = _TMP267 + _TMP271 + _TMP275 + _TMP279 + 4.00000000E+00*_TMP283;
    _edr = bvec4(_TMP32.x < _TMP33.x && _interp_restriction_lv1.x, _TMP32.y < _TMP33.y && _interp_restriction_lv1.y, _TMP32.z < _TMP33.z && _interp_restriction_lv1.z, _TMP32.w < _TMP33.w && _interp_restriction_lv1.w);
    _a0290 = _r0118.wxyz - _r0128.zwxy;
    _TMP287 = abs(_a0290);
    _a0294 = _r0118.zwxy - _r0128;
    _TMP291 = abs(_a0294);
    _edr_left = bvec4((2.00000000E+00*_TMP287).x <= _TMP291.x && _interp_restriction_lv2_left.x, (2.00000000E+00*_TMP287).y <= _TMP291.y && _interp_restriction_lv2_left.y, (2.00000000E+00*_TMP287).z <= _TMP291.z && _interp_restriction_lv2_left.z, (2.00000000E+00*_TMP287).w <= _TMP291.w && _interp_restriction_lv2_left.w);
    _a0298 = _r0118.wxyz - _r0128.zwxy;
    _TMP295 = abs(_a0298);
    _a0302 = _r0118.zwxy - _r0128;
    _TMP299 = abs(_a0302);
    _edr_up = bvec4(_TMP295.x >= (2.00000000E+00*_TMP299).x && _interp_restriction_lv2_up.x, _TMP295.y >= (2.00000000E+00*_TMP299).y && _interp_restriction_lv2_up.y, _TMP295.z >= (2.00000000E+00*_TMP299).z && _interp_restriction_lv2_up.z, _TMP295.w >= (2.00000000E+00*_TMP299).w && _interp_restriction_lv2_up.w);
    _nc = bvec4(_edr.x && (_fx.x || _edr_left.x && _fx_left.x || _edr_up.x && _fx_up.x), _edr.y && (_fx.y || _edr_left.y && _fx_left.y || _edr_up.y && _fx_up.y), _edr.z && (_fx.z || _edr_left.z && _fx_left.z || _edr_up.z && _fx_up.z), _edr.w && (_fx.w || _edr_left.w && _fx_left.w || _edr_up.w && _fx_up.w));
    _a0306 = _r0138 - _r0118.wxyz;
    _TMP303 = abs(_a0306);
    _a0310 = _r0138 - _r0118.zwxy;
    _TMP307 = abs(_a0310);
    _px = bvec4(_TMP303.x <= _TMP307.x, _TMP303.y <= _TMP307.y, _TMP303.z <= _TMP307.z, _TMP303.w <= _TMP307.w);
    if (_nc.x) { 
        if (_px.x) { 
            _TMP41 = _TMP8.xyz;
        } else {
            _TMP41 = _TMP10.xyz;
        } 
        _TMP40 = _TMP41;
    } else {
        if (_nc.y) { 
            if (_px.y) { 
                _TMP43 = _TMP4.xyz;
            } else {
                _TMP43 = _TMP8.xyz;
            } 
            _TMP42 = _TMP43;
        } else {
            if (_nc.z) { 
                if (_px.z) { 
                    _TMP45 = _TMP6.xyz;
                } else {
                    _TMP45 = _TMP4.xyz;
                } 
                _TMP44 = _TMP45;
            } else {
                _TMP44 = _TMP7.xyz;
            } 
            _TMP42 = _TMP44;
        } 
        _TMP40 = _TMP42;
    } 
    if (_nc.w) { 
        if (_px.w) { 
            _TMP47 = _TMP10.xyz;
        } else {
            _TMP47 = _TMP6.xyz;
        } 
        _TMP46 = _TMP47;
    } else {
        if (_nc.z) { 
            if (_px.z) { 
                _TMP49 = _TMP6.xyz;
            } else {
                _TMP49 = _TMP4.xyz;
            } 
            _TMP48 = _TMP49;
        } else {
            if (_nc.y) { 
                if (_px.y) { 
                    _TMP51 = _TMP4.xyz;
                } else {
                    _TMP51 = _TMP8.xyz;
                } 
                _TMP50 = _TMP51;
            } else {
                _TMP50 = _TMP7.xyz;
            } 
            _TMP48 = _TMP50;
        } 
        _TMP46 = _TMP48;
    } 
    _r0312.x = dot(_TMP40, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _r0312.y = dot(_TMP46, vec3( 1.43519993E+01, 2.81760006E+01, 5.47200012E+00));
    _a0318 = _r0312 - _r0138.xy;
    _df12 = abs(_a0318);
    _TMP53 = float((_df12.y >= _df12.x));
    _res = _TMP40 + _TMP53*(_TMP46 - _TMP40);
    _ret_0 = vec4(_res.x, _res.y, _res.z, 1.00000000E+00);
    FragColor = _ret_0;
    return;
} 
#endif
