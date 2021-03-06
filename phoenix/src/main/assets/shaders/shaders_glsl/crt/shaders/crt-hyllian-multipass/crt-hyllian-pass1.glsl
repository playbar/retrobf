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
COMPAT_VARYING     vec2 _texCoord1;
COMPAT_VARYING     float _frame_rotation;
struct prev {
    vec2 _video_size1;
    vec2 _texture_size1;
float _placeholder20;
};
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
struct out_vertex {
    vec2 _texCoord1;
};
vec4 _oPosition1;
out_vertex _ret_0;
vec4 _r0007;
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
    _r0007 = VertexCoord.x*MVPMatrix[0];
    _r0007 = _r0007 + VertexCoord.y*MVPMatrix[1];
    _r0007 = _r0007 + VertexCoord.z*MVPMatrix[2];
    _r0007 = _r0007 + VertexCoord.w*MVPMatrix[3];
    _oPosition1 = _r0007;
    _ret_0._texCoord1 = TexCoord.xy;
    gl_Position = _r0007;
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
COMPAT_VARYING     vec2 _texCoord;
COMPAT_VARYING     float _frame_rotation;
struct prev {
    vec2 _video_size;
    vec2 _texture_size;
float _placeholder28;
};
struct input_dummy {
    vec2 _video_size1;
    vec2 _texture_size1;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
struct out_vertex {
    vec2 _texCoord;
};
vec4 _ret_0;
float _TMP13;
float _TMP12;
float _TMP11;
vec3 _TMP5;
float _TMP4;
float _TMP10;
float _TMP9;
float _TMP8;
float _TMP7;
vec3 _TMP6;
vec4 _TMP2;
vec4 _TMP1;
vec2 _TMP0;
uniform sampler2D Texture;
prev _PASSPREV11;
input_dummy _IN1;
vec2 _c0024;
vec3 _TMP31;
vec3 _x0032;
vec3 _TMP37;
vec3 _x0038;
vec3 _TMP43;
vec3 _x0044;
vec3 _TMP53;
vec3 _x0054;
vec3 _TMP63;
vec3 _x0064;
float _c0070;
float _a0072;
COMPAT_VARYING vec4 TEX0;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    vec2 _TextureSize;
    vec3 _color;
    vec2 _dy;
    vec2 _pix_coord;
    vec2 _tc;
    vec2 _fp;
    float _pos1;
    vec3 _lum0;
    vec3 _lum1;
    float _mod_factor;
    vec3 _dotMaskWeights;
    _TextureSize = vec2(_PASSPREV11._texture_size.x, TextureSize.y);
    _dy = vec2(0.00000000E+00, 1.00000000E+00/_TextureSize.y);
    _pix_coord = TEX0.xy*_TextureSize + vec2( 0.00000000E+00, 5.00000000E-01);
    _TMP0 = floor(_pix_coord);
    _tc = (_TMP0 + vec2( 0.00000000E+00, 5.00000000E-01))/_TextureSize;
    _fp = fract(_pix_coord);
    _c0024 = _tc - _dy;
    _TMP1 = COMPAT_TEXTURE(Texture, _c0024);
    _TMP2 = COMPAT_TEXTURE(Texture, _tc);
    _pos1 = 1.00000000E+00 - _fp.y;
    _lum0 = vec3( 8.60000014E-01, 8.60000014E-01, 8.60000014E-01) + _TMP1.xyz*vec3( 1.39999986E-01, 1.39999986E-01, 1.39999986E-01);
    _lum1 = vec3( 8.60000014E-01, 8.60000014E-01, 8.60000014E-01) + _TMP2.xyz*vec3( 1.39999986E-01, 1.39999986E-01, 1.39999986E-01);
    _x0032 = _fp.y/(_lum0 + 1.00000001E-07);
    _TMP6 = min(vec3( 1.00000000E+00, 1.00000000E+00, 1.00000000E+00), _x0032);
    _TMP31 = max(vec3( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00), _TMP6);
    _x0038 = _pos1/(_lum1 + 1.00000001E-07);
    _TMP6 = min(vec3( 1.00000000E+00, 1.00000000E+00, 1.00000000E+00), _x0038);
    _TMP37 = max(vec3( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00), _TMP6);
    _x0044 = (-7.20000029E+00*_TMP31)*_TMP31;
    _TMP11 = pow(2.71828198E+00, _x0044.x);
    _TMP12 = pow(2.71828198E+00, _x0044.y);
    _TMP13 = pow(2.71828198E+00, _x0044.z);
    _TMP43 = vec3(_TMP11, _TMP12, _TMP13);
    _x0054 = (-7.20000029E+00*_TMP37)*_TMP37;
    _TMP11 = pow(2.71828198E+00, _x0054.x);
    _TMP12 = pow(2.71828198E+00, _x0054.y);
    _TMP13 = pow(2.71828198E+00, _x0054.z);
    _TMP53 = vec3(_TMP11, _TMP12, _TMP13);
    _x0064 = _TMP1.xyz*_TMP43 + _TMP2.xyz*_TMP53;
    _TMP6 = min(vec3( 1.00000000E+00, 1.00000000E+00, 1.00000000E+00), _x0064);
    _TMP63 = max(vec3( 0.00000000E+00, 0.00000000E+00, 0.00000000E+00), _TMP6);
    _color = _TMP63*vec3( 1.50000000E+00, 1.50000000E+00, 1.50000000E+00);
    _mod_factor = (TEX0.x*OutputSize.x*_PASSPREV11._texture_size.x)/_PASSPREV11._video_size.x;
    _a0072 = _mod_factor/2.00000000E+00;
    _TMP7 = abs(_a0072);
    _TMP8 = fract(_TMP7);
    _TMP9 = abs(2.00000000E+00);
    _c0070 = _TMP8*_TMP9;
    if (_mod_factor < 0.00000000E+00) { 
        _TMP10 = -_c0070;
    } else {
        _TMP10 = _c0070;
    } 
    _TMP4 = floor(_TMP10);
    _dotMaskWeights = vec3( 1.00000000E+00, 6.99999988E-01, 1.00000000E+00) + _TMP4*vec3( -3.00000012E-01, 3.00000012E-01, -3.00000012E-01);
    _TMP5 = vec3( 1.00000000E+00, 1.00000000E+00, 1.00000000E+00) + (_dotMaskWeights - vec3( 1.00000000E+00, 1.00000000E+00, 1.00000000E+00));
    _color.xyz = _color.xyz*_TMP5;
    _TMP11 = pow(_color.x, 4.54545438E-01);
    _TMP12 = pow(_color.y, 4.54545438E-01);
    _TMP13 = pow(_color.z, 4.54545438E-01);
    _color = vec3(_TMP11, _TMP12, _TMP13);
    _ret_0 = vec4(_color.x, _color.y, _color.z, 1.00000000E+00);
    FragColor = _ret_0;
    return;
} 
#endif
