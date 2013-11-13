/*
 * File:   ShaderFuncs.h
 * Author: jon
 *
 * Created on November 12, 2013, 8:23 PM
 */

#ifndef SHADERFUNCS_H
#define	SHADERFUNCS_H


btVector4 operator*(const btVector4& v1, const btScalar& s)
{
	return btVector4(
		v1.x() * s,
		v1.y() * s,
		v1.z() * s,
		v1.w() * s
	);
}

btVector4 operator+(const btVector4& v1, const btVector4& v2)
{
	return btVector4(
		v1.x() + v2.x(),
		v1.y() + v2.y(),
		v1.z() + v2.z(),
		v1.w() + v2.w()
	);
}


inline double clamp(double v, double min = 0, double max = 1) {
	if (v < min) return min;
	else if (v > max) return max;
	else
		return v;
}

inline double diffuseStrength(const btVector3 &light, const btVector3 &pos, const btVector3 &normal) {
	btVector3 lightNormal = (light - pos).normalize();
	return clamp(lightNormal.dot(normal));
};

inline btVector3 reflect(const btVector3 &inRay, const btVector3 &normal) {
	// http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	return inRay - ( (2 * inRay).dot(normal) / normal.length2() * normal);
}

#endif	/* SHADERFUNCS_H */

