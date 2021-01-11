#version 150

in vec2 varyingtexcoord;
out vec4 outputColor;

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform float alpha;
uniform float t;

vec4 expanded(sampler2DRect t, vec2 pos, float a) {
  mat2 s = a * mat2(1, 0, 0, 1);
  vec4 c = texture(t, s*pos);
  return c;


  /*
  vec4 n = texture(t, pos + a*vec2(+0.0, +1.0));
  vec4 e = texture(t, pos + a*vec2(+1.0, +0.0));
  vec4 s = texture(t, pos + a*vec2(-0.0, -1.0));
  vec4 w = texture(t, pos + a*vec2(-1.0, -0.0));

  vec4 ne = texture(t, pos + a*vec2(+1.0, +1.0));
  vec4 se = texture(t, pos + a*vec2(+1.0, -1.0));
  vec4 sw = texture(t, pos + a*vec2(-1.0, -1.0));
  vec4 nw = texture(t, pos + a*vec2(-1.0, +1.0));

  vec4 c = texture(t, pos);

  vec4 res = vec4(0.0f);
  res += (n + e + s + w);
  res += (ne + se + sw + nw);
  res += c;
  res /= 9.0f;
  return res;
  */
}

float lum(vec4 rgba) {
  return dot(
    vec4(0.2126, 0.7152, 0.0722, 0.0f),
    rgba
  );
}

float f(float x, float y) {
  return
    0.5f*(cos(x) + 0.1f)
    +
    0.5f*(sin(y) + 0.1f);
}

vec2 df(float x, float y) {
  float h = 0.005f;
  float dx = (f(x+h, y) - f(x-h, y)) / h / 2.0f;
  float dy = (f(x, y+h) - f(x, y-h)) / h / 2.0f;
  return vec2(dx, dy);
}


#define PI 3.14159265359f
#define XSCALE (1080.0f / 4.0f / 2.0f / PI / 4.0f)
#define YSCALE (1920.0f / 4.0f / 2.0f / PI / 4.0f)


// f
float f_tex(sampler2DRect tex, vec2 pos) {
  return lum(texture(tex, pos));
}

// df
vec3 df_tex(sampler2DRect tex, vec2 pos) {
  const float h = 5.0f;
  float dx = (f_tex(tex, pos+vec2(h, 0)) - f_tex(tex, pos-vec2(h, 0)))/h/2.0f;
  float dy = (f_tex(tex, pos+vec2(0, h)) - f_tex(tex, pos-vec2(0, h)))/h/2.0f;
  float x = pos.x / XSCALE;
  float y = pos.y / YSCALE;
  return vec3(dx, dy, 1);
}

// Spearfish
vec4 spearfish(sampler2DRect target, sampler2DRect mask, vec2 pos) {

  // Compute normal vector
  float z = f_tex(mask, pos);
  vec3 df = df_tex(mask, pos);
  vec3 n = normalize(vec3(-df.x, -df.y, 1));

  // Compute where it hits
  vec3 h = vec3(0, 0, z);
  vec3 d = h + 300.3f*-n;

  vec4 col = texture(target, pos.xy + d.xy);

  return col;
}

// Whiff
vec4 whiff(sampler2DRect target, sampler2DRect mask, vec2 pos) {
  float z = 1.0f/6.0f/(1.0f+lum(texture(mask, 0.5f*pos)));
  return expanded(target, pos, z);
}

void main() {
  outputColor = whiff(tex0, tex1, varyingtexcoord);
  // outputColor = spearfish(tex0, tex1, varyingtexcoord);
}
