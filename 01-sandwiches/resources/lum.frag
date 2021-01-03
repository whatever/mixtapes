#version 120

void main () {
  vec2 pos = gl_TexCoord[0].xy;
  gl_FragColor = vec4(pos, 0.0f, 1.0f);
}
