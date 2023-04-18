#pragma once
#include "..\headers\camera.h"
#include <vector>

namespace eg {

	class Object {
	protected:
		double m_density;
		Point* m_points;	//Object points array
		int m_amount = 0;	//Valid amount of object points array elements
		int* m_marked_points;
		int m_marked_amount = 0;
		int* m_marked_order;
		Color object_color = { 255, 255, 255, 255 };
		std::string object_name;
		std::vector<std::string> objects_list = { "Sphere",
												"Ellipse",
												"Elliptical Cylinder",
												"Hyperbolic Cylinder",
												"Parabolic Cylinder",
												"Single Cavity Hyperboloid",
												"Double Cavity Hyperboloid",
												"Elliptical Paraboloid",
												"Hyperbolic Paraboloid",
												"Second Order Cone" };
		int objects_amount = objects_list.size();
	public:
		Object(const double density, const int points_amount, const std::string name);
		virtual ~Object();

		int GetAmount();			//Get amount of object points array elements

		Point GetPoint(int index);	//Get point of object with index

		double GetDensity();		//Get points density

		void ChangePointColor(const int index, Color color); //Change color of object point

		int GetMarkedPoint(const int index); //Get marked point of object with index

		void MarkPoint(const int index); //Mark point with index

		void DismarkPoint(const int index); //Dismark point with index

		void ClearMarkedPoints(); //Clear all marked points of object

		int GetMarkedAmount(); //Get amount of marked point

		int GetMarkedIndex(const int index); //Get index of marked point

		std::string ReturnCurrObject(const int index); //Return current type object

		int ObjectsAmount(); //Return amount of all available objects to use

		Object& operator=(const Object& new_obj);
	};

	class Sphere : public Object {
	private:
		double m_radius;	//Radius of sphere
	public:
		Sphere(const double radius, const double density = 50.0);
		virtual ~Sphere();
	};

	class CustomForm : public Object {
	public:
		CustomForm(std::string file_name);
		virtual ~CustomForm();
	};

	class Ellipse : public Object {
	private:
		double m_a, m_b, m_c;
	public:
		Ellipse(Oxyz parameters, const double density = 50.0);
		virtual ~Ellipse();
	};

	class EllipticalCylinder : public Object {
	private:
		double m_a, m_b;
	public:
		EllipticalCylinder(Oxyz parameters, const double density = 50.0);
		virtual ~EllipticalCylinder();
	};

	class HyperbolicCylinder : public Object {
	private:
		double m_a, m_b;
	public:
		HyperbolicCylinder(Oxyz parameters, const double density = 50.0);
		virtual ~HyperbolicCylinder();
	};

	class ParabolicCylinder : public Object {
	private:
		double m_p;
	public:
		ParabolicCylinder(Oxy parameters, const double density = 50.0);
		virtual ~ParabolicCylinder();
	};

	class SingleCavityHyperboloid : public Object {
	private:
		double m_a, m_b, m_c;
	public:
		SingleCavityHyperboloid(Oxyz parameters, const double density = 50.0);
		virtual ~SingleCavityHyperboloid();
	};

	class DoubleCavityHyperboloid : public Object {
	private:
		double m_a, m_b, m_c;
	public:
		DoubleCavityHyperboloid(Oxyz parameters, const double density = 50.0);
		virtual ~DoubleCavityHyperboloid();
	};

	class EllipticalParaboloid : public Object {
	private:
		double m_a, m_b;
	public:
		EllipticalParaboloid(Oxy parameters, const double density = 50.0);
		virtual ~EllipticalParaboloid();
	};

	class HyperbolicParaboloid : public Object {
	private:
		double m_a, m_b;
	public:
		HyperbolicParaboloid(Oxy parameters, const double density = 50.0);
		virtual ~HyperbolicParaboloid();
	};

	class SecondOrderCone : public Object {
	private:
		double m_a, m_b, m_c;
	public:
		SecondOrderCone(Oxyz parameters, const double density = 50.0);
		virtual ~SecondOrderCone();
	};
}