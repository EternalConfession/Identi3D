//
// File: Ray.h
// ===========
// Ray definition.
//

#ifndef IDENTI3D_SRC_MATH_RAY_H
#define IDENTI3D_SRC_MATH_RAY_H

#include <src/identi3d/General.h>
#include <src/math/Vector.h>

namespace Identi3D
{

	class __declspec(dllexport) Ray
	{
		friend class Plane;
		friend class AxisAlignedBoundingBox;

	private:
		Vector3 _origin, _direction;

		// for fast intersection calculation.
		int _sign[3];
		Vector3 _inverse;

	public:

		Ray(void) {} ;
		Ray(const Vector3 &origin, const Vector3 &direction);
		~Ray(void) {} ;

		/*
		 * Create a ray using its origin and direction.
		 */
		void set(const Vector3 &origin, const Vector3 &direction);

		/*
		 * Detransform the ray by the specified matrix.
		 */
		void detransform(const Matrix &m);

		/*
		 * Test intersection with a triangle.
		 */
		bool intersect(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, 
					   bool cull_backside = false, float *distance = NULL, Vector3 *hit = NULL) const;
		/*
		 * Test intersection with a plane.
		 */
		bool intersect(const Plane &p, bool cull_backside = false,
					   float *distance = NULL, Vector3 *hit = NULL) const;

		/*
		 * Test intersection with an Axis Aligned Bounding Box.
		 */
		bool intersect(const AxisAlignedBoundingBox &aabb, 
					   float *distance = NULL, Vector3 *hit = NULL) const;
		
		/*
		 * Test intersection with an Oriented Bounding Box.
		 */
		bool intersect(const OrientedBoundingBox &obb,
					   float *distance = NULL, Vector3 *hit = NULL) const;

		/*
		 * Get the origin of the ray.
		 */
		inline const Vector3 &getOrigin(void) const { return _origin; }

		/*
		 * Get the direction of the ray.
		 */
		inline const Vector3 &getDirection(void) const { return _direction; }
	};

};

#endif