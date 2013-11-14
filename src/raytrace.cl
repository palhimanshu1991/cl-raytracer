
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



__constant float castDistance = 1000.0;
__constant float cameraSize = 5.0;
__constant float cameraSize2 = 5.0 / 2.0;

typedef struct RenderState {
	__global SceneInfo *sceneInfo;
	__global SceneItem *sceneItems;
} RenderState;

/** Converts a [0, 1] rgba vector to a 32 bit ARGB int. */
inline int packInt(float4 color) {
	color *= 255.0;
	return (
		((int)color.x << 16) |
		((int)color.y << 8) |
		((int)color.z) |
		((int)color.w << 24)
	);
}

//float4 sphereIntersection(float4 start, float4 direction, float4 center, float radius) {
float4 sphereIntersection(float4 start, float4 end, float4 center, float radius) {
	// http://www.dreamincode.net/forums/topic/124203-ray-sphere-intersection/
	float4 q = center - start;
	float c = length(q);
	float v = dot(q, end);
	float d = radius * radius - (c * c - v * v);

	if (d > 0) {
		//no hit
		return (float4)(-1);
	} else {
		return (float4)(0);
	}


}

typedef struct RayHit {
	//pos.w < 0 is used to flag "no hit"
	float4 pos, color, normal;
} RayHit;

RayHit castLine(RenderState *state, float4 start, float4 end) {
	int hitIndex = -1;
	float4 hitPos;
	//float nearestHit = INF;

	for (int i = 0, numItems = state->sceneInfo->numItems; i < numItems; ++i) {
		float4 itemHit = sphereIntersection(start, end, state->sceneItems[i].position, state->sceneItems[i].radius);

		if (itemHit.w >= 0) {
			hitPos = itemHit;
			hitIndex = i;
			break;
			//todo: nearest stuff
		}
	}

	if (hitIndex < 0) {
		//no hit
		return (RayHit){
			(float4)(-1),
			(float4)(0),
			(float4)(0),
		};
	} else {
		return (RayHit){
			hitPos,
			state->sceneItems[hitIndex].color,
			(float4)(1),//todo
		};
	}
}



/**
 * Main kernel for raytracing a scene.
 */
__kernel void renderScene(
	__global SceneInfo *sceneInfo,
	__global SceneItem *sceneItems,
	__global int *results
) {
	RenderState renderState = {sceneInfo, sceneItems};

	int x = get_global_id(0), y = get_global_id(1);
	int width = sceneInfo->width;//, height = sceneInfo->height;
	float scale = cameraSize / (float)width;

	float4 from = sceneInfo->cameraPos + (float4)(x * scale - cameraSize, -y * scale + cameraSize2, 0, 0);
	float4 to = from + sceneInfo->cameraDir * castDistance;
	float4 color = (float4)(0, .0, 0, 1);


	RayHit hit = castLine(&renderState, from, to);
	if (hit.pos.w < 0) {
		color.x = 0;
	} else {
		color.x = 1;
	}





	//float4 color = (float4)(1, x / (float)width, y / (float)height, 1);
	results[x + y * sceneInfo->width] = packInt(color);
}

//__kernel void add(__global float *v1, __global float *v2, __global float *res) {
//
//	int i = get_global_id(0);
//	res[i] = v1[i] + v2[i];
//}


