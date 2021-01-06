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
#define XSCALE (1080.0f / 4.0f / 2.0f / PI / 4)
#define YSCALE (1920.0f / 4.0f / 2.0f / PI / 4)

// Spearfish
vec4 spearfish(sampler2DRect target, sampler2DRect mask, vec2 pos) {

  // Move to [0, 1]
  float x = pos.x / XSCALE;
  float y = pos.y / YSCALE;

  // Compute normal vector
  vec3 h = vec3(0, 0, f(x, y));
  vec3 df = vec3(df(x, y), 1.0f);
  vec3 n = normalize(vec3(-df.x, -df.y, 1));

  // Compute where it hits
  vec3 d = h + 1.3f*-n;

  // Move to [w, h]
  vec2 coord = vec2(0, 0);;
  coord.x = (x + d.x) * XSCALE;
  coord.y = (y + d.y) * YSCALE;

  vec4 col = texture(target, coord);

  return col;
}

// Whiff
vec4 whiff(sampler2DRect target, sampler2DRect mask, vec2 pos) {
  float z = 1.0f/6.0f/(1.0f+lum(texture(mask, 0.5f*pos)));
  return expanded(target, pos, z);
}

void main() {
  // outputColor = whiff(tex0, tex1, varyingtexcoord);
  outputColor = spearfish(tex0, tex1, varyingtexcoord);
}
