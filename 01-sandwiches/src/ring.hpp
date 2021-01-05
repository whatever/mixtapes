#pragma once

#define RING_MESH_COUNT 10

#define TWOPI 3.1415*2

class Ring {

  public:

    void update(unsigned int t) {

      float ts = ((float) t) / 1000.0f;

      for (int i=0; i < RING_MESH_COUNT; i++) {
        float u = ts + ((float) i)/((float) RING_MESH_COUNT)*TWOPI;
        float r = 100.0f;
        float x = r*cos(u);
        float y = r*sin(u);
        float z = 0.0f;
        meshes[i].resetTransform();
        meshes[i].setScale(0.25f);
        meshes[i].setPosition(x, y, z);
        meshes[i].rotateDeg(+1.0f * 8 * u + i, ofVec3f(1.0f, 0.0f, 0.0f));
        meshes[i].rotateDeg(+5.0f * 8 * u + i, ofVec3f(0.0f, 1.0f, 0.0f));
        meshes[i].rotateDeg(-3.0f * 8 * u + i, ofVec3f(0.0f, 0.0f, 1.0f));
      }
    };

    void draw() {
      for (int i=0; i < RING_MESH_COUNT; i++) {
        meshes[i].draw();
      }
    };

  protected:
    ofBoxPrimitive meshes[RING_MESH_COUNT];
};
