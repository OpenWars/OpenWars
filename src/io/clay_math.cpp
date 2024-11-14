/**
 *
   ___                __        __
  / _ \ _ __   ___ _ _\ \      / /_ _ _ __ ___
 | | | | '_ \ / _ \ '_ \ \ /\ / / _` | '__/ __|
 | |_| | |_) |  __/ | | \ V  V / (_| | |  \__ \
  \___/| .__/ \___|_| |_|\_/\_/ \__,_|_|  |___/
       |_|

Copyright (C) 2024 OpenWars Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef __openwars__io__clay_math__cpp__
#define __openwars__io__clay_math__cpp__

#include "clay_math.hpp"
#include <raylib.h>
#include <raymath.h>

namespace OpenWars {
	namespace Clay {
		Matrix i_m_matrix_i2r(I_Matrix m) {
			return Matrix {
				m.m0, m.m4, m.m8, m.m12,
				m.m1, m.m5, m.m9, m.m13,
				m.m2, m.m6, m.m10, m.m14,
				m.m3, m.m7, m.m11, m.m15,
			};
		};

		I_Matrix i_m_matrix_r2i(Matrix m) {
			return I_Matrix {
				m.m0, m.m4, m.m8, m.m12,
				m.m1, m.m5, m.m9, m.m13,
				m.m2, m.m6, m.m10, m.m14,
				m.m3, m.m7, m.m11, m.m15,
			};
		};

		Vector3 i_m_vector3_i2r(I_Vector3 v) {
			return Vector3 { v.x, v.y, v.z };
		};

		I_Vector3 i_m_vector3_r2i(Vector3 v) {
			return I_Vector3 { v.x, v.y, v.z };
		};

		I_Matrix matrix_look_at(I_Vector3 eye, I_Vector3 target, I_Vector3 up) {
			return i_m_matrix_r2i(MatrixLookAt(i_m_vector3_i2r(eye), i_m_vector3_i2r(target), i_m_vector3_i2r(up)));
		};

		I_Matrix matrix_identity(void) {
			return i_m_matrix_r2i(MatrixIdentity());
		};

		I_Matrix matrix_perspective(double fovY, double aspect, double nearPlane, double farPlane) {
			return i_m_matrix_r2i(MatrixPerspective(fovY, aspect, nearPlane, farPlane));
		};
		
		I_Matrix matrix_ortho(
			double left, double right, double bottom, double top,
			double nearPlane, double farPlane
		) {
			return i_m_matrix_r2i(MatrixOrtho(left, right, bottom, top, nearPlane, farPlane));
		};

		I_Vector3 vector3_unproject(I_Vector3 source, I_Matrix projection, I_Matrix view) {
			return i_m_vector3_r2i(Vector3Unproject(i_m_vector3_i2r(source), i_m_matrix_i2r(projection), i_m_matrix_i2r(view)));
		};

		I_Vector3 vector3_normalize(I_Vector3 v) {
			return i_m_vector3_r2i(Vector3Normalize(i_m_vector3_i2r(v)));
		};

		I_Vector3 vector3_subtract(I_Vector3 v1, I_Vector3 v2) {
			return i_m_vector3_r2i(Vector3Subtract(i_m_vector3_i2r(v1), i_m_vector3_i2r(v2)));
		};
	};
};

#endif
