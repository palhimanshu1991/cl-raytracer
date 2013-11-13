/*
 * File:   ShaderFuncs.h
 * Author: jon
 *
 * Created on November 12, 2013, 8:23 PM
 */

#ifndef SHADERFUNCS_H
#define	SHADERFUNCS_H



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

#endif	/* SHADERFUNCS_H */

