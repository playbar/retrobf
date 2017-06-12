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
COMPAT_VARYING     vec2 VARt1;
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
    vec2 VARt1;
};
out_vertex _ret_0;
input_dummy _IN1;
vec4 _r0006;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 TEX0;
 
uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    out_vertex _OUT;
    _r0006 = VertexCoord.x*MVPMatrix[0];
    _r0006 = _r0006 + VertexCoord.y*MVPMatrix[1];
    _r0006 = _r0006 + VertexCoord.z*MVPMatrix[2];
    _r0006 = _r0006 + VertexCoord.w*MVPMatrix[3];
    _OUT.VARt1 = 1.00000000E+00/TextureSize;
    _ret_0._position1 = _r0006;
    _ret_0._texCoord1 = TexCoord.xy;
    VARt1 = _OUT.VARt1;
    gl_Position = _r0006;
    TEX0.xy = TexCoord.xy;
    return;
    TEX0.xy = _ret_0._texCoord1;
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
COMPAT_VARYING     vec2 VARt1;
COMPAT_VARYING     vec2 _texCoord;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
struct out_vertex {
    vec2 _texCoord;
    vec2 VARt1;
};
vec4 _ret_0;
float _TMP7;
float _TMP11;
float _TMP10;
float _TMP12;
float _TMP13;
float _TMP5;
vec3 _TMP4;
vec3 _TMP3;
float _TMP9;
float _TMP8;
vec4 _TMP2;
vec4 _TMP1;
vec4 _TMP0;
out_vertex _VAR1;
uniform sampler2D Texture;
vec2 _c0021;
vec2 _c0023;
vec3 _v0025;
vec3 _v0031;
float _TMP40;
vec3 _diff0047;
float _ravg0047;
float _a0049;
float _x0055;
float _TMP56;
COMPAT_VARYING vec4 TEX0;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    float _tag;
    _TMP0 = COMPAT_TEXTURE(Texture, TEX0.xy);
    _c0021 = TEX0.xy + vec2( -1.00000000E+00, 0.00000000E+00)*VARt1;
    _TMP1 = COMPAT_TEXTURE(Texture, _c0021);
    _c0023 = TEX0.xy + vec2( 1.00000000E+00, 0.00000000E+00)*VARt1;
    _TMP2 = COMPAT_TEXTURE(Texture, _c0023);
    _v0025 = _TMP0.xyz - _TMP1.xyz;
    _TMP8 = dot(_v0025, _v0025);
    _TMP9 = inversesqrt(_TMP8);
    _TMP3 = _TMP9*_v0025;
    _v0031 = _TMP0.xyz - _TMP2.xyz;
    _TMP8 = dot(_v0031, _v0031);
    _TMP9 = inversesqrt(_TMP8);
    _TMP4 = _TMP9*_v0031;
    _TMP5 = dot(_TMP3, _TMP4);
    _TMP12 = min(1.00000000E+00, _TMP5);
    _TMP40 = max(0.00000000E+00, _TMP12);
    _diff0047 = _TMP1.xyz - _TMP2.xyz;
    _ravg0047 = (_TMP1.x + _TMP2.x)*5.00000000E-01;
    _diff0047 = _diff0047*_diff0047*vec3(2.00000000E+00 + _ravg0047, 4.00000000E+00, 3.00000000E+00 - _ravg0047);
    _a0049 = _diff0047.x + _diff0047.y + _diff0047.z;
    _TMP13 = inversesqrt(_a0049);
    _TMP10 = 1.00000000E+00/_TMP13;
    _x0055 = (_TMP10 - 3.00000000E+00)/-3.00000000E+00;
    _TMP12 = min(1.00000000E+00, _x0055);
    _TMP56 = max(0.00000000E+00, _TMP12);
    _TMP11 = _TMP56*_TMP56*(3.00000000E+00 - 2.00000000E+00*_TMP56);
    _TMP7 = _TMP11*_TMP11;
    _tag = _TMP40*_TMP7;
    _ret_0 = vec4(_TMP0.x, _TMP0.y, _TMP0.z, _tag);
    FragColor = _ret_0;
    return;
} 
#endif
