
//Scene structs, copied from Scene.h
typedef struct SceneInfo {
	float4 cameraPos, cameraDir;
	float4 lightPos;//simple for now
	int width, height, numItems;
} SceneInfo;

typedef struct SceneItem {
	float4 color, position;
	float radius;
} SceneItem;


inline int packInt(float4 color) {
	color *= 255.0;
	return (
		((int)color.x << 16) |
		((int)color.y << 8) |
		((int)color.z) |
		((int)color.w << 24)
	);
}


__kernel void renderScene(
	__global SceneInfo *sceneInfo,
	__global SceneItem *sceneItems,
	__global int *results
) {
	int x = get_global_id(0), y = get_global_id(1);

	float4 color = (float4)(1, x / (float)sceneInfo->width, y / (float)sceneInfo->height, 1);
	results[x + y * sceneInfo->width] = packInt(color);

//	btVector3 from = fromBase + btVector3(x * scale - size2, -y * scale + size2, 0);
//	btVector3 color = trace(from, dir, context);
//
//	buf(x, y) = toColor(color);
}

//__kernel void add(__global float *v1, __global float *v2, __global float *res) {
//
//	int i = get_global_id(0);
//	res[i] = v1[i] + v2[i];
//}

