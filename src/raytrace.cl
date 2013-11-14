
typedef int int32_t;

//Scene structs, copied from Scene.h
	#define PACKED __attribute__ ((aligned(32)))
	typedef struct SceneInfo {
		float4 cameraPos, cameraDir;
		float4 lightPos;//simple for now
		int32_t width, height, numItems;
	} PACKED SceneInfo;

	typedef struct SceneItem {
		float4 color, position;
		float radius;
	} PACKED SceneItem;



__constant float cameraSize = 5.0;
__constant float cameraSize2 = 5.0 / 2.0;

__constant int maxReflections = 5;

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
float sphereIntersection(float4 start, float4 direction, float4 center, float radius) {
	// http://www.dreamincode.net/forums/topic/124203-ray-sphere-intersection/
	float4 q = center - start;
	float c = length(q);
	float v = dot(q, direction);
	float d = radius * radius - (c * c - v * v);

	if (d < 0) {
		//no hit
		return -1;
	} else {
		return v - sqrt(d);//distance from start to intersection along direction
	}
}

typedef struct RayHit {
	//pos.w < 0 is used to flag "no hit"
	float4 pos, color, normal;
} RayHit;

RayHit castRay(RenderState *state, float4 start, float4 direction) {
	int hitIndex = -1;
	float nearestHit = INFINITY;

	for (int i = 0, numItems = state->sceneInfo->numItems; i < numItems; ++i) {
		float hitDistance = sphereIntersection(start, direction, state->sceneItems[i].position, state->sceneItems[i].radius);

		if (hitDistance >= 0 && hitDistance < nearestHit) {
			//nearest hit so far
			nearestHit = hitDistance;
			hitIndex = i;
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
		//reconstruct hit location
		float4 hitPos = start + direction * nearestHit;

		//calculate normal of the sphere here
		float4 normal = normalize(hitPos - state->sceneItems[hitIndex].position);

		return (RayHit){
			hitPos,
			state->sceneItems[hitIndex].color,
			normal,
		};
	}
}

float clampToOne(float v) {
	return clamp(v, (float)0, (float)1);
}

float diffuseStrength(float4 lightPos, float4 surfacePos, float4 surfaceNormal) {
	float4 lightNormal = normalize(lightPos - surfacePos);
	return clampToOne(dot(lightNormal, surfaceNormal));
}

float length2(float4 vec) {
	return dot(vec, vec);
}

float4 reflect(float4 inRay, float4 normal) {
	// http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	return inRay - ( dot(2 * inRay, normal) / length2(normal) * normal);
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

	float4 from = sceneInfo->cameraPos + (float4)(x * scale - cameraSize2, -y * scale + cameraSize2, 0, 0);
	float4 direction = sceneInfo->cameraDir;
	float4 color = (float4)(0, 0, 0, 0);

	float currentContrib = 1;
	for (int i = 0; i < maxReflections; ++i) {
		RayHit hit = castRay(&renderState, from, direction);
		if (hit.pos.w < 0)
			break;//no more hits

		if (false) {
			color = hit.normal * .5 + (float4).5;
			break;
		}

		//calculate the ordinary lighting for this surface at this point
		float lightStrength = diffuseStrength(sceneInfo->lightPos, hit.pos, hit.normal) * .5;
		//todo: shadows
		lightStrength = clampToOne(lightStrength + .1);//ambient, clamp

		//our surface color
		float4 surfaceColor = hit.color * lightStrength;

		//accum this color
		color += surfaceColor * currentContrib;

		//setup the next bounce:
		//the light next bounce will affect the color even less
		currentContrib *= .8;
		from = hit.pos;
		direction = reflect(direction, hit.normal);
	}






	color = clamp(color, (float)0, (float)1);
	results[x + y * sceneInfo->width] = packInt(color);
}

//__kernel void add(__global float *v1, __global float *v2, __global float *res) {
//
//	int i = get_global_id(0);
//	res[i] = v1[i] + v2[i];
//}


