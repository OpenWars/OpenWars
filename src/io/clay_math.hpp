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
#ifndef __openwars__io__clay_math__hpp__
#define __openwars__io__clay_math__hpp__

namespace OpenWars {
	namespace Clay {
		typedef struct I_Vector3 {
			float x;
			float y;
			float z;
		} I_Vector3;

		typedef struct I_Matrix {
			float m0, m4, m8, m12;
			float m1, m5, m9, m13;
			float m2, m6, m10, m14;
			float m3, m7, m11, m15;
		} I_Matrix;

		I_Matrix matrix_look_at(I_Vector3 eye, I_Vector3 target, I_Vector3 up);
		I_Matrix matrix_identity(void);
		I_Matrix matrix_perspective(double fovY, double aspect, double nearPlane, double farPlane);
		I_Matrix matrix_ortho(
			double left, double right, double bottom, double top,
			double nearPlane, double farPlane
		);
		I_Vector3 vector3_unproject(I_Vector3 source, I_Matrix projection, I_Matrix view);
		I_Vector3 vector3_normalize(I_Vector3 v);
		I_Vector3 vector3_subtract(I_Vector3 v1, I_Vector3 v2);
	};
};

#endif
