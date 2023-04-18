#pragma once
#include <iostream>

namespace eg {

	struct Oxyz {	//Coordinates set
		double x, y, z;
	};

	struct Oxy {
		double x, y;
	};

	struct Color {	//Color set
		uint8_t R, G, B, A;
	};

	struct Point {	//Point parameters (coordinate and color) set
		Oxyz coordinate;
		Color color;
	};

	struct Angle {	//Angles set
		double Ox, Oy, Oz;
	};

	class Camera {
	private:
		int m_width;		//Camera plane width
		int m_height;		//Camera plane height
		Color* m_picture;	//Array of camera plane color values
		int* m_projected_points;
		double f_a, f_b;	//Camera focus
		double a_0, b_0;	//Camera plane center
		Oxyz m_position;	//Camera coordinates in world coordinate system (WCS)
		Angle m_angles;		//Camera angles in camera coordinate system (CCS)
		Oxyz p_position = { 0, 0, 3 };
	public:
		Camera(int width, int height, Oxyz cam_position, Angle cam_angles, double Fa = -1, double Fb = -1);
		~Camera();

		Color* Picture(); //Return pixels array of picture

		void Fill(const Color color = { 0, 0, 0, 0 }); //Fill picture with color (black by default)

		void ClearPicture(); //Clear pixels array of camera

		void ProjectPoint(Point point, const int index = 0, const short int mark = 0); //Project point onto camera plane perspective

		Oxy ReturnProjectPoint(Point point);

		void MoveAngle(Angle new_angles); //Rotate camera in WCS

		void ChangeAngle(Angle new_angles); //Change camera angles

		void MovePosition(Oxyz new_position); //Move camera in CCS

		void ChangePosition(Oxyz new_position); //Change camera position in CCS

		void MovePolarPosition(Oxyz new_position); //Move camera around point

		Oxyz GetPolarPosition();

		int GetPointIndex(Oxy point_coord); //Get index of point in points array

		void ChangePixelColor(int index, Color color); //Change color of pixel

		void FindCenter(); //Find center of object and rotate camera to center
	};

}