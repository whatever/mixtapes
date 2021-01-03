#version 150

in vec2 varyingtexcoord;
out vec4 outputColor;

uniform sampler2DRect tex;
uniform float alpha;

void main() {
  vec2 pos = gl_FragCoord.xy;
  vec4 color = texture(tex, varyingtexcoord);

  float r = color.r;
  float b = color.b;

  color.r = alpha * r + (1-alpha) * b;
  color.b = (1-alpha) * r + alpha * b;
  outputColor = color;
}
