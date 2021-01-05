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
    vec4(0.2126, 0.7152, 0.0722, 1.0f),
    rgba
  );
}

vec4 whiff(sampler2DRect target, sampler2DRect mask, vec2 pos) {
  float z = 1.0f/6.0f/lum(texture(mask, pos));
  return expanded(target, pos, z);
}

void main() {
  outputColor = whiff(tex0, tex1, varyingtexcoord);
}
