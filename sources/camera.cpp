#include <iostream>
#include "..\headers\camera.h"
#include "..\headers\matrix.h"

#define PI 3.14159265358979323846264338327950288419716939937510

namespace eg {

	Camera::Camera(int width, int height, Oxyz cam_position, Angle cam_angles, double Fa, double Fb) {
		m_width = width;
		m_height = height;

		a_0 = 0.5 * width;
		b_0 = 0.5 * height;

		if (Fa == -1) { f_a = a_0; f_b = a_0; }
		else { f_a = Fa; f_b = Fb; }

		m_position = cam_position;
		m_angles = cam_angles;

		m_picture = new Color[width * height];
		m_projected_points = new int[width * height];
		for (int k = 0; k < width * height; k++)
			m_projected_points[k] = -1;
		Fill();
	}

	Camera::~Camera() {
	}

	Color* Camera::Picture() { return m_picture; }

	void Camera::Fill(const Color color) {
		for (int i = 0; i < m_width; i++)
			for (int j = 0; j < m_height; j++)
				m_picture[(m_height * i) + j] = color;
	}

	void Camera::ClearPicture() {
		Fill();
		for (int k = 0; k < (m_width * m_height); k++)
			m_projected_points[k] = -1;
	}

	void Camera::ProjectPoint(Point point, const int index, const short int marked) {
		//Point coordinates in WCS
		double x_w = point.coordinate.x;
		double y_w = -1 * point.coordinate.z;
		double z_w = point.coordinate.y;

		//Angles values in CCS
		double angle_Ox = m_angles.Ox;
		double angle_Oy = m_angles.Oy;
		double angle_Oz = m_angles.Oz;

		double cos_a = cos(angle_Ox);
		double cos_b = cos(angle_Oy);
		double sin_a = sin(angle_Ox);
		double sin_b = sin(angle_Oy);

		//Rotate matrix of point coordinates
		mx::Mx3dbl RotateMatrix({ { {cos_b,			 0,		-1*sin_b},
									{-1*sin_a*sin_b, cos_a,	-1*sin_a*cos_b},
									{cos_a*sin_b,	 sin_a,	cos_a*cos_b} } });

		//Transition vector from WCS to CCS
		mx::Vec3dbl TransMatrix({{{x_w - m_position.x},
								   {y_w - m_position.y},
								   {z_w - m_position.z} } });

		//Point coordinates in CCS
		mx::Vec3dbl CoordRes = RotateMatrix * TransMatrix;
		double x = CoordRes.get(0, 0);
		double y = CoordRes.get(1, 0);
		double z = CoordRes.get(2, 0);

		if (z <= 0) return;

		//Point coordinates in camera plane coordinate system
		double a = (x / z) * f_a + a_0;
		double b = (y / z) * f_b + b_0;

		if ((a >= 0 && a < m_width) && (b >= 0 && b < m_height)) {
			m_picture[(int)b * m_width + (int)a] = point.color;
			m_projected_points[(int)b * m_width + (int)a] = index;
			if (marked) {
				int l_border = -2;
				int r_border = 2;
				int d_border = 2;
				int u_border = -2;

				if ((int)a == 0) l_border = 0;
				else if ((int)a == (int)(m_width - 1.0)) r_border = 0;
				if ((int)a == 1) l_border = -1;
				else if ((int)a == (int)(m_width - 2.0)) r_border = 1;

				if ((int)b == 0) u_border = 0;
				else if ((int)b == (int)(m_height - 1.0)) d_border = 0;
				if ((int)b == 1) u_border = -1;
				else if ((int)b == (int)(m_height - 2.0)) d_border = 1;

				for (int i = l_border; i <= r_border; i++)
					for (int j = u_border; j <= d_border; j++)
						m_picture[((int)b + j) * m_width + ((int)a + i)] = { 20, 150, 250, 255 };
			}
		}
	}

	Oxy Camera::ReturnProjectPoint(Point point) {
		//Point coordinates in WCS
		double x_w = point.coordinate.x;
		double y_w = -1 * point.coordinate.z;
		double z_w = point.coordinate.y;

		//Angles values in CCS
		double angle_Ox = m_angles.Ox;
		double angle_Oy = m_angles.Oy;
		double angle_Oz = m_angles.Oz;

		double cos_a = cos(angle_Ox);
		double cos_b = cos(angle_Oy);
		double sin_a = sin(angle_Ox);
		double sin_b = sin(angle_Oy);

		//Rotate matrix of point coordinates
		mx::Mx3dbl RotateMatrix({ { {cos_b,			 0,		-1 * sin_b},
									{-1 * sin_a * sin_b, cos_a,	-1 * sin_a * cos_b},
									{cos_a * sin_b,	 sin_a,	cos_a * cos_b} } });

		//Transition vector from WCS to CCS
		mx::Vec3dbl TransMatrix({ {{x_w - m_position.x},
								   {y_w - m_position.y},
								   {z_w - m_position.z} } });

		//Point coordinates in CCS
		mx::Vec3dbl CoordRes = RotateMatrix * TransMatrix;
		double x = CoordRes.get(0, 0);
		double y = CoordRes.get(1, 0);
		double z = CoordRes.get(2, 0);

		if (z <= 0) return { -1, -1 };

		//Point coordinates in camera plane coordinate system
		double a = (x / z) * f_a + a_0;
		double b = (y / z) * f_b + b_0;
		return { a, b };
	}

	void Camera::MoveAngle(Angle new_angles) {
		m_angles.Ox += new_angles.Ox;
		m_angles.Oy += new_angles.Oy;
		m_angles.Oy += new_angles.Oy;
	}

	void Camera::ChangeAngle(Angle new_angles) {
		m_angles = new_angles;
	}

	void Camera::MovePosition(Oxyz new_position) {
		m_position.x += new_position.x;
		m_position.y += new_position.y;
		m_position.z += new_position.z;
	}

	void Camera::ChangePosition(Oxyz new_position) {
		m_position = new_position;
	}

	void Camera::MovePolarPosition(Oxyz new_position) {
		p_position.x += new_position.x;
		p_position.y += new_position.y;
		p_position.z += new_position.z;
	}

	Oxyz Camera::GetPolarPosition() { return p_position; }

	int Camera::GetPointIndex(Oxy point_coord) {
		int a = point_coord.x;
		int b = point_coord.y;

		if ((a < 0 || a >= m_width) || (b < 0 || b >= m_height))
			return -1;

		int l_border = -2;
		int r_border = 2;
		int d_border = 2;
		int u_border = -2;

		if (a == 0) l_border = 0;
		else if (a == m_width - 1.0) r_border = 0;
		if (a == 1) l_border = -1;
		else if (a == m_width - 2.0) r_border = 1;

		if (b == 0) u_border = 0;
		else if (b == m_height - 1.0) d_border = 0;
		if (b == 1) u_border = -1;
		else if (b == m_height - 2.0) d_border = 1;

		int index = -1;
		for (int i = l_border; i <= r_border; i++)
			for (int j = u_border; j <= d_border; j++)
				if (m_projected_points[(b + j) * m_width + (a + i)] != -1)
					index = m_projected_points[(b + j) * m_width + (a + i)];

		return index;
	}

	void Camera::ChangePixelColor(int index, Color color) {
		m_picture[index] = color;
	}

	void Camera::FindCenter() {
		double a = m_position.x;
		double b = m_position.z;
		double angle_Oy = atan(abs(a / b));
		if (a > 0 && b > 0) angle_Oy = angle_Oy + PI;
		else if (a < 0 && b > 0) angle_Oy = -1 * angle_Oy + PI;
		else if (a > 0 && b < 0) angle_Oy *= -1;
		else if (a == 0 && b > 0) angle_Oy = -1 * PI;
		else if (a == 0 && b < 0) angle_Oy = 0;
		else if (a > 0 && b == 0) angle_Oy = -0.5 * PI;
		else if (a < 0 && b == 0) angle_Oy = 0.5 * PI;
		else if (a == 0 && b == 0) angle_Oy = 0;

		double c = m_position.y;
		if (c > 0) {
			if ((angle_Oy > (3 * PI / 4)) && (angle_Oy < (5 * PI / 4))) {
				double d = m_position.z;
				double angle_Ox = atan(abs(d / c));
				angle_Ox = angle_Ox - 0.5 * PI;
				ChangeAngle({ angle_Ox , angle_Oy, m_angles.Oz });
			}
			else if ((angle_Oy > (-1 * PI / 4)) && (angle_Oy < (PI / 4))) {
				double d = m_position.z;
				double angle_Ox = atan(abs(d / c));
				angle_Ox = angle_Ox - 0.5 * PI;
				ChangeAngle({ angle_Ox , angle_Oy, m_angles.Oz });
			}
			else {
				double angle_Ox = atan(abs(m_position.x / c)) - 0.5 * PI;
				ChangeAngle({ angle_Ox , angle_Oy, m_angles.Oz });
			}

		}
		else if (c == 0) {
			double angle_Ox = m_angles.Ox;
			ChangeAngle({ angle_Ox , angle_Oy, m_angles.Oz });
		}
		else {
			if ((angle_Oy > (3 * PI / 4)) && (angle_Oy < (5 * PI / 4))) {
				double d = m_position.z;
				double angle_Ox = atan(abs(d / c));
				angle_Ox = angle_Ox - 0.5 * PI;
				angle_Ox = -1 * angle_Ox;
				ChangeAngle({ angle_Ox , angle_Oy, m_angles.Oz });
			}
			else if ((angle_Oy > (-1 * PI / 4)) && (angle_Oy < (PI / 4))) {
				double d = m_position.z;
				double angle_Ox = atan(abs(d / c));
				angle_Ox = angle_Ox - 0.5 * PI;
				angle_Ox = -1 * angle_Ox;
				ChangeAngle({ angle_Ox , angle_Oy, m_angles.Oz });
			}
			else {
				double angle_Ox = atan(abs(m_position.x / c)) - 0.5 * PI;
				angle_Ox = -1 * angle_Ox;
				ChangeAngle({ angle_Ox , angle_Oy, m_angles.Oz });
			}
			
		}
		
	}
}