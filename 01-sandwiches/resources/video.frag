#version 150

in vec2 varyingtexcoord;
out vec4 outputColor;

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform float alpha;

void main() {
  vec2 pos = gl_FragCoord.xy;
  vec4 color = texture(tex0, varyingtexcoord);
  vec4 color2 = texture(tex1, varyingtexcoord);

  float r = color2.r;
  float b = color2.b;

  color2.r = alpha * r + (1-alpha) * b;
  color2.b = (1-alpha) * r + alpha * b;

  outputColor = 0.5*color + 0.5*color2;
}
