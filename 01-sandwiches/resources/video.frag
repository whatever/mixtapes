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

void main() {
  vec2 pos = varyingtexcoord;
  vec4 color2 = texture(tex1, pos);

  // float z = 1.0f/(1.0f - lum(color2)); // 0.1f/(10.f*lum(color2));
  float z = 1.0f/6.0f/lum(color2); // 0.1f/(10.f*lum(color2));

  /*
  vec2 p = pos;
  p.x /= 1080*0.3 * 1.0f;
  p.y /= 1920*0.3 * 1.0f;
  z = sqrt(p.x*p.x + p.y+p.y + 0.00000001f);
  */

  vec4 color = expanded(tex0, pos, z);
  // vec4 color = vec4(z*texture(tex0, pos).rgb, 1.0f);


  float r = color2.r;
  float b = color2.b;

  color2.r = alpha * r + (1-alpha) * b;
  color2.b = (1-alpha) * r + alpha * b;

  // outputColor = (0.5*color + 0.5*color2);
  outputColor = color;
}
