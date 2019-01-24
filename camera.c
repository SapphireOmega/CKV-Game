#include "camera.h"

#include <stdlib.h>

Camera *create_camera(int x, int y, int w, int h)
{
	Camera *camera = (Camera*)malloc(sizeof(Camera));
	camera->x = x;
	camera->y = y;
	camera->w = w;
	camera->h = h;

	return camera;
}

void destroy_camera(Camera *camera)
{
	free(camera);
}
