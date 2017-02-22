//
//  Aabb.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 02.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Aabb.h"

AABB::AABB() {}

AABB::AABB(const Vector& min, const Vector& max) :
	Min(min),
	Max(max) {}

AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) :
	Min(minX, minY, minZ),
	Max(maxX, maxY, maxZ) {}

Vector AABB::size() const
{
	return Max - Min;
}

// Prüft Überschneidung mit anderer AABB
// http://www.miguelcasillas.com/?mcportfolio=collision-detection-c
bool AABB::intersects(AABB & box) const
{
	return(
		Max.X > box.Min.X &&
		Min.X < box.Max.X &&
		Max.Y > box.Min.Y &&
		Min.Y < box.Max.Y &&
		Max.Z > box.Min.Z &&
		Min.Z < box.Max.Z);
}

// Wendet Matrix auf die AABB Vectoren an
AABB AABB::operator*(const Matrix & m) const
{
	return AABB(m * Min, m * Max);
}

