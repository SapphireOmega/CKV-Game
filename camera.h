#ifndef CAMERA_H_
#define CAMERA_H_

typedef struct {
	int x;
	int y;
	int w;
	int h;
} Camera;

Camera *create_camera(int x, int y, int w, int h);
void destroy_camera(Camera *camera);

#endif // CAMERA_H_
