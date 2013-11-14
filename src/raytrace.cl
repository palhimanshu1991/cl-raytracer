
struct RayTracerContext {
	int width, height;
	float4 cameraPos, cameraDir;
};

__kernel void add(__global float *v1, __global float *v2, __global float *res) {

	int i = get_global_id(0);
	res[i] = v1[i] + v2[i];
}

//__kernel void bob(__global *RayTracerContext context, __global float *res) {
//	int x = get_global_id(0), y = get_global_id(1);
//
//	res[x + y * context.width] = x + y;

//	btVector3 from = fromBase + btVector3(x * scale - size2, -y * scale + size2, 0);
//	btVector3 color = trace(from, dir, context);
//
//	buf(x, y) = toColor(color);
