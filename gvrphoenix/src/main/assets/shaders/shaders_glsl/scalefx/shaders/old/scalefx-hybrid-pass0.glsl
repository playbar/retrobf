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
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord1;
COMPAT_VARYING     vec4 _position1;
COMPAT_VARYING     vec2 _tex_coord;
struct prev {
    vec2 _texture_size;
    vec2 _tex_coord;
float _placeholder28;
};
struct out_vertex {
    vec4 _position1;
    vec2 _texCoord1;
    vec4 _t1;
    vec4 _t2;
};
out_vertex _ret_0;
prev _PASSPREV21;
vec4 _r0006;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
 
uniform COMPAT_PRECISION vec2 PassPrev2TextureSize;
uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
COMPAT_ATTRIBUTE vec2 PassPrev2TexCoord;
void main()
{
    out_vertex _OUT;
    vec2 _ps;
    _r0006 = VertexCoord.x*MVPMatrix[0];
    _r0006 = _r0006 + VertexCoord.y*MVPMatrix[1];
    _r0006 = _r0006 + VertexCoord.z*MVPMatrix[2];
    _r0006 = _r0006 + VertexCoord.w*MVPMatrix[3];
    _ps = 1.00000000E+00/PassPrev2TextureSize;
    _OUT._t1 = PassPrev2TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, -_ps.y);
    _OUT._t2 = PassPrev2TexCoord.xxxy + vec4(-_ps.x, 0.00000000E+00, _ps.x, 0.00000000E+00);
    _ret_0._position1 = _r0006;
    _ret_0._texCoord1 = PassPrev2TexCoord;
    _ret_0._t1 = _OUT._t1;
    _ret_0._t2 = _OUT._t2;
    gl_Position = _r0006;
    TEX0.xy = PassPrev2TexCoord;
    TEX1 = _OUT._t1;
    TEX2 = _OUT._t2;
    return;
    TEX0.xy = _ret_0._texCoord1;
    TEX1 = _ret_0._t1;
    TEX2 = _ret_0._t2;
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
COMPAT_VARYING     vec4 _t2;
COMPAT_VARYING     vec4 _t1;
COMPAT_VARYING     vec2 _texCoord;
COMPAT_VARYING     vec2 _tex_coord;
struct prev {
    vec2 _texture_size;
    vec2 _tex_coord;
float _placeholder24;
};
struct out_vertex {
    vec2 _texCoord;
    vec4 _t1;
    vec4 _t2;
};
vec4 _ret_0;
float _TMP8;
float _TMP7;
float _TMP6;
float _TMP5;
float _TMP10;
float _TMP9;
float _TMP11;
vec4 _TMP4;
vec4 _TMP3;
vec4 _TMP2;
vec4 _TMP1;
vec4 _TMP0;
prev _PASSPREV21;
float _r0025;
vec3 _d0025;
vec3 _c0025;
vec3 _a0027;
float _r0033;
vec3 _d0033;
vec3 _c0033;
vec3 _a0035;
float _r0041;
vec3 _d0041;
vec3 _c0041;
vec3 _a0043;
float _r0049;
vec3 _d0049;
vec3 _c0049;
vec3 _a0051;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
 
uniform sampler2D PassPrev2Texture;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    _TMP0 = COMPAT_TEXTURE(PassPrev2Texture, TEX1.xw);
    _TMP1 = COMPAT_TEXTURE(PassPrev2Texture, TEX1.yw);
    _TMP2 = COMPAT_TEXTURE(PassPrev2Texture, TEX1.zw);
    _TMP3 = COMPAT_TEXTURE(PassPrev2Texture, TEX2.yw);
    _TMP4 = COMPAT_TEXTURE(PassPrev2Texture, TEX2.zw);
    _r0025 = 5.00000000E-01*(_TMP3.x + _TMP0.x);
    _d0025 = _TMP3.xyz - _TMP0.xyz;
    _c0025 = vec3(2.00000000E+00 + _r0025, 4.00000000E+00, 3.00000000E+00 - _r0025);
    _a0027 = _c0025*_d0025;
    _TMP9 = dot(_a0027, _d0025);
    _TMP11 = inversesqrt(_TMP9);
    _TMP10 = 1.00000000E+00/_TMP11;
    _TMP5 = 1.00000000E+00 - _TMP10/3.00000000E+00;
    _r0033 = 5.00000000E-01*(_TMP3.x + _TMP1.x);
    _d0033 = _TMP3.xyz - _TMP1.xyz;
    _c0033 = vec3(2.00000000E+00 + _r0033, 4.00000000E+00, 3.00000000E+00 - _r0033);
    _a0035 = _c0033*_d0033;
    _TMP9 = dot(_a0035, _d0033);
    _TMP11 = inversesqrt(_TMP9);
    _TMP10 = 1.00000000E+00/_TMP11;
    _TMP6 = 1.00000000E+00 - _TMP10/3.00000000E+00;
    _r0041 = 5.00000000E-01*(_TMP3.x + _TMP2.x);
    _d0041 = _TMP3.xyz - _TMP2.xyz;
    _c0041 = vec3(2.00000000E+00 + _r0041, 4.00000000E+00, 3.00000000E+00 - _r0041);
    _a0043 = _c0041*_d0041;
    _TMP9 = dot(_a0043, _d0041);
    _TMP11 = inversesqrt(_TMP9);
    _TMP10 = 1.00000000E+00/_TMP11;
    _TMP7 = 1.00000000E+00 - _TMP10/3.00000000E+00;
    _r0049 = 5.00000000E-01*(_TMP3.x + _TMP4.x);
    _d0049 = _TMP3.xyz - _TMP4.xyz;
    _c0049 = vec3(2.00000000E+00 + _r0049, 4.00000000E+00, 3.00000000E+00 - _r0049);
    _a0051 = _c0049*_d0049;
    _TMP9 = dot(_a0051, _d0049);
    _TMP11 = inversesqrt(_TMP9);
    _TMP10 = 1.00000000E+00/_TMP11;
    _TMP8 = 1.00000000E+00 - _TMP10/3.00000000E+00;
    _ret_0 = vec4(_TMP5, _TMP6, _TMP7, _TMP8);
    FragColor = _ret_0;
    return;
} 
#endif