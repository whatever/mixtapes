#version 120

void main () {
  vec2 pos = gl_FragCoord.xy;
  vec2 xy = vec2(0.2f, 0.5f);
  gl_FragColor = vec4(xy, 1.0f, 1.0f);
}
