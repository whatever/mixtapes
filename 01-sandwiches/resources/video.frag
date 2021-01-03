#version 150

in vec2 varyingtexcoord;
out vec4 outputColor;

uniform sampler2DRect tex;
uniform float alpha;

void main() {
  vec2 pos = gl_FragCoord.xy;
  vec4 color = texture(tex, pos);
  color.r = alpha;
  outputColor = color;
}
