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
  return (sin(x));
}

vec2 df(float x, float y) {
  float h = 0.005f;
  float dx = (f(x+h, y) - f(x-h, y)) / h / 2.0f;
  float dy = (f(x, y+h) - f(x, y-h)) / h / 2.0f;
  return vec2(dx, dy);
}

// Spearfish
vec4 spearfish(sampler2DRect target, sampler2DRect mask, vec2 pos) {

  vec2 p;
  p.x = pos.x / 1080 * 9.0f * 2 * 3.1415;
  p.y = pos.y / 1920 * 9.0f * 2 * 3.1415;

  float z = f(p.x, p.y);

  vec2 dz = normalize(df(p.x, p.y));
  vec2 n = normalize(vec2(-dz.y, dz.x));

  // vec2 h = vec2(0.0f, 
  // z = dot(dz, n);

  vec2 q = pos + vec2(20.f*z, 0);

  return z * texture(mask, q);
}

// Whiff
vec4 whiff(sampler2DRect target, sampler2DRect mask, vec2 pos) {
  float z = 1.0f/6.0f/(1.0f+lum(texture(mask, 0.5f*pos)));
  return expanded(target, pos, z);
}

void main() {
  outputColor = whiff(tex0, tex1, varyingtexcoord);
  outputColor = spearfish(tex0, tex1, varyingtexcoord);
}
