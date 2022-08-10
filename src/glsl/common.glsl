// DEFINES

#define IMAGE_SIZE uvec2(1024, 1024)
#define BINS_IN_IMAGE uvec2(16, 16)
#define TILES_IN_BIN uvec2(8, 8)
#define TILES_IN_IMAGE uvec2(BINS_IN_IMAGE * TILES_IN_BIN)
#define PIXELS_IN_TILE uvec2(IMAGE_SIZE / TILES_IN_IMAGE)

#define BINS_IN_IMAGE_LINEAR int(int(BINS_IN_IMAGE.x) * int(BINS_IN_IMAGE.y))
#define TILES_IN_IMAGE_LINEAR int(int(BINS_IN_IMAGE_LINEAR) * int(TILES_IN_BIN_LINEAR))
#define PIXELS_IN_TILE_LINEAR int(PIXELS_IN_TILE.x * PIXELS_IN_TILE.y)
#define TILES_IN_BIN_LINEAR (TILES_IN_BIN.x * TILES_IN_BIN.y)


// STRUCTS

struct SetupTriangleData
{
	vec4 screenPos[3];
};

struct AABB2D
{
	vec2 minPos, maxPos;
};

struct PointTriangleOrientation
{
	vec3 edge_function_results;
	vec3 barycentrics;
	float current_z;
};


// FUNCTIONS 

vec2 Perpendicular(in vec2 vec)
{
	vec2 res; 
	if(abs(vec.x) < 1e-5 || abs(vec.y) < 1e-5)
		res = vec2(vec.y, vec.x);
	else
	{
		res.y = 1;
		res.x = -vec.y * res.y / vec.x;
	}
	return normalize(res);
}

void MakeClockwise(inout vec2 v1, inout vec2 v2, inout vec2 v3)
{
	vec3 vect1 = vec3(v2 - v1, 0);
	vec3 vect2 = vec3(v3 - v1, 0);
	vec3 c = cross(vect1, vect2);
	if(c.z > 0)
	{
		vec2  temp = v2;
		v2 = v3;
		v3 = temp;
	}
}

void MakeClockwise(inout vec3 v1, inout vec3 v2, inout vec3 v3)
{
	vec3 vect1 = vec3(v2.xy - v1.xy, 0);
	vec3 vect2 = vec3(v3.xy - v1.xy, 0);
	vec3 c = cross(vect1, vect2);
	if(c.z > 0.)
	{
		vec3  temp = v2;
		v2 = v3;
		v3 = temp;
	}
}

float EdgeFunction(in vec2 p1, in vec2 p2, in vec2 test_point)
{
	return (test_point.x - p1.x) * (p2.y - p1.y) - (test_point.y - p1.y) * (p2.x - p1.x);
}

void GetTriangle2DAABB(in vec4 screenPos[3], out AABB2D aabb)
{
	vec4 minV = vec4(min(min(screenPos[0], screenPos[1]), screenPos[2]));
	vec4 maxV = vec4(max(max(screenPos[0], screenPos[1]), screenPos[2]));
	aabb.minPos = minV.xy;
	aabb.maxPos = maxV.xy;
}

bool PointInTriangle(vec2 point, vec2 v1, vec2 v2, vec2 v3)
{
	MakeClockwise(v1, v2, v3);
	float d1 = EdgeFunction(v1, v2, point);
	float d2 = EdgeFunction(v2, v3, point);
	float d3 = EdgeFunction(v3, v1, point);

	return d1 > 0 && d2 > 0 && d3 > 0;
}

bool SegmentsIntersect(vec2 A, vec2 B, vec2 C, vec2 D)
{
	vec2 O1 = A;
	vec2 O2 = C;
	vec2 D1  = B - A;
	vec2 D2 = D - C;
	vec2 D1Perp = Perpendicular(D1);
	vec2 D2Perp = Perpendicular(D2);
	float s = dot(O2 - O1, D2Perp) / dot(D1, D2Perp);
	float t = dot(O1 - O2, D1Perp) / dot(D2, D1Perp);
	return s >= 0 && s <= 1 && t >= 0 && t <= 1;
}

void TestPointInTriangle(in vec3 p1, in vec3 p2, in vec3 p3, vec2 test_point, out PointTriangleOrientation results)
{
	MakeClockwise(p1, p2, p3);
	results.edge_function_results = vec3(EdgeFunction(p2.xy, p3.xy, test_point),
										 EdgeFunction(p3.xy, p1.xy, test_point),
										 EdgeFunction(p1.xy, p2.xy, test_point));
	vec3 triangle_area = vec3(EdgeFunction(p1.xy, p2.xy, p3.xy));
	results.barycentrics = (results.edge_function_results / triangle_area);
	vec3 b = results.barycentrics;
	results.current_z = 1 / p1.z * b[0] + 1 / p2.z * b[1] + 1 / p3.z * b[2]; 

}



// DEBUG FUNCTIONS

uint debugFloat(float v)
{
	return uint(abs(v) * 100 + 100 * int(v > 0));
}
uvec2 getTileIndex(uvec2 pix)
{
	return pix / 8;
}
uvec2 getPixIndex(uvec2 pix)
{
	return pix % 8;
}