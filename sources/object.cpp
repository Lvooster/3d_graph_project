#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "..\headers\object.h"

#define PI 3.14159265358979323846264338327950288419716939937510
#define E 2.718

namespace eg {
	

	Object::Object(const double density, const int points_amount, const std::string name) {
		m_density = density;
		m_marked_points = new int[points_amount];
		for (int k = 0; k < points_amount; k++)
			m_marked_points[k] = 0;

		m_marked_order = new int[points_amount];
		for (int k = 0; k < points_amount; k++)
			m_marked_order[k] = -1;
	}

	Object::~Object() {
		if (m_points != nullptr)
			delete[] m_points;
	}

	int Object::GetAmount() { return m_amount;  }

	Point Object::GetPoint(int index) { return m_points[index]; }

	double Object::GetDensity() { return m_density; }

	void Object::ChangePointColor(const int index, Color color) {
		m_points[index].color = color;
	}

	int Object::GetMarkedPoint(const int index) {
		return m_marked_points[index];
	}

	void Object::MarkPoint(const int index) {
		if (m_marked_points[index] == 0) {
			ChangePointColor(index, { 20, 150, 250, 255 });
			m_marked_points[index] = 1;
			m_marked_amount++;
			m_marked_order[m_marked_amount - 1] = index;
		}
	}

	void Object::DismarkPoint(const int index) {
		if (m_marked_points[index] == 1){
			ChangePointColor(index, { 255, 255, 255, 255 });
			m_marked_points[index] = 0;
			m_marked_amount--;
		}	
	}

	void Object::ClearMarkedPoints() {
		for (int k = 0; k < m_amount; k++)
			DismarkPoint(k);
	}

	int Object::GetMarkedAmount() {
		return m_marked_amount;
	}

	int Object::GetMarkedIndex(const int index) {
		return m_marked_order[index];
	}

	std::string Object::ReturnCurrObject(const int index) {
		return objects_list[index];
	}

	int Object::ObjectsAmount() { return objects_amount; }

	Object& Object::operator=(const Object& new_obj) {
		delete[] m_points;
		m_amount = new_obj.m_amount;
		delete[] m_marked_points; 
		m_marked_amount = 0; m_marked_amount = new_obj.m_marked_amount;
		delete[] m_marked_order;
		std::string object_name = new_obj.object_name;

		m_density = new_obj.m_density;
		m_marked_points = new int[m_amount];
		for (int k = 0; k < m_amount; k++)
			m_marked_points[k] = new_obj.m_marked_points[k];

		m_marked_order = new int[m_amount];
		for (int k = 0; k < m_amount; k++)
			m_marked_order[k] = new_obj.m_marked_order[k];

		m_points = new Point[m_amount];
		for (int k = 0; k < m_amount; k++)
			m_points[k] = new_obj.m_points[k];

		return *this;
	}

	Sphere::Sphere(const double radius, const double density) : Object::Object(density, (3.2 * density) * (6.4 * density), "Sphere") {
		m_radius = radius;
		m_points = new Point[(3.2 * m_density) * (6.4 * m_density)];
		for (double a = 0; a < 2 * PI; a += (1 / m_density))
			for (double b = 0; b < PI; b += (1 / m_density)) {
				m_points[m_amount].coordinate.x = radius * sin(b) * cos(a);
				m_points[m_amount].coordinate.y = radius * sin(b) * sin(a);
				m_points[m_amount].coordinate.z = radius * cos(b);
				m_points[m_amount].color = object_color;
				m_amount++;
			}
	}

	Sphere::~Sphere() {

	}


	CustomForm::CustomForm(std::string file_name) : Object::Object(m_amount, m_amount, "Custom") {
		std::ifstream input_file;
		input_file.open(file_name);
		std::string line;
		m_amount = -1;
		if (input_file.is_open())
			while (getline(input_file, line)) {
				if (m_amount == -1) {
					m_amount = std::stoi(line);
					m_points = new Point[m_amount];
					m_amount = 0;
				}
				else {
					std::string number;
					number = "";
					int letter = 0;
					while (line[letter] != ' ') { number += line[letter]; letter++; }
					m_points[m_amount].coordinate.x = std::stod(number); letter++; std::cout << "X: " << m_points[m_amount].coordinate.x << '\n';

					number = "";
					while (line[letter] != ' ') { number += line[letter]; letter++; }
					m_points[m_amount].coordinate.y = std::stod(number); letter++; std::cout << "Y: " << m_points[m_amount].coordinate.y << '\n';

					number = "";
					while (line[letter] != ' ') { number += line[letter]; letter++; }
					m_points[m_amount].coordinate.z = std::stod(number); letter++; std::cout << "Z: " << m_points[m_amount].coordinate.z << '\n';

					number = "";
					while (line[letter] != ' ') { number += line[letter]; letter++; }
					m_points[m_amount].color.R = std::stod(number); letter++;

					number = "";
					while (line[letter] != ' ') { number += line[letter]; letter++; }
					m_points[m_amount].color.G = std::stod(number); letter++;

					number = "";
					while (line[letter] != ' ') { number += line[letter]; letter++; }
					m_points[m_amount].color.B = std::stod(number); letter++;

					number = "";
					while (line[letter] != NULL) { number += line[letter], letter++; }
					m_points[m_amount].color.A = std::stod(number);
					
					m_amount++;
				}
			}
	}

	CustomForm::~CustomForm() {

	}


	Ellipse::Ellipse(Oxyz parameters, const double density) : Object::Object(density, (3.2 * density)* (6.4 * density),
		"Ellipse") {
		m_a = parameters.x;
		m_b = parameters.y;
		m_c = parameters.z;
		m_points = new Point[(3.2 * m_density) * (6.4 * m_density)];
		for (double a = 0; a < 2 * PI; a += (1 / m_density))
			for (double b = 0; b < PI; b += (1 / m_density)) {
				m_points[m_amount].coordinate.x = m_a * sin(b) * cos(a);
				m_points[m_amount].coordinate.y = m_b * sin(b) * sin(a);
				m_points[m_amount].coordinate.z = m_c * cos(b);
				m_points[m_amount].color = object_color;
				m_amount++;
			}
	}

	Ellipse::~Ellipse(){
	}


	EllipticalCylinder::EllipticalCylinder(Oxyz parameters, const double density) : Object::Object(density, (6.4 * density)* (2 * parameters.z * density),
		"Elliptical Cylinder") {
		m_a = parameters.x;
		m_b = parameters.y;
		m_points = new Point[(6.4 * m_density) * (2*parameters.z * m_density)];
		for (double j = -parameters.z; j <= parameters.z; j += (1 / m_density))
			for (double i = 0; i < 2 * PI; i += (1 / m_density)) {
				m_points[m_amount].coordinate.x = m_a * cos(i);
				m_points[m_amount].coordinate.y = m_b * sin(i);
				m_points[m_amount].coordinate.z = j;
				m_points[m_amount].color = object_color;
				m_amount++;
			}
	}

	EllipticalCylinder::~EllipticalCylinder() {
	}


	HyperbolicCylinder::HyperbolicCylinder(Oxyz parameters, const double density) : Object::Object(density, 2 * ((6.4 * density) * (2 * parameters.z * density)),
		"Hyperbolic Cylinder") {
		m_a = parameters.x;
		m_b = parameters.y;
		m_points = new Point[2 * ((6.4 * m_density) * (2 * parameters.z * m_density))];
		for (double j = -parameters.z; j <= parameters.z; j += (1 / m_density))
			for (double i = -PI; i < PI; i += (1 / m_density)) {
				m_points[m_amount].coordinate.x = m_a * (pow(E, i) + pow(E, -1 * i)) / 2.0;
				m_points[m_amount].coordinate.y = m_b * (pow(E, i) - pow(E, -1 * i)) / 2.0;
				m_points[m_amount].coordinate.z = j;
				m_points[m_amount].color = object_color;

				m_points[m_amount + 1].coordinate.x = -1 * m_points[m_amount].coordinate.x;
				m_points[m_amount + 1].coordinate.y = m_points[m_amount].coordinate.y;
				m_points[m_amount + 1].coordinate.z = j;
				m_points[m_amount + 1].color = object_color;

				m_amount += 2;
			}
	}

	HyperbolicCylinder::~HyperbolicCylinder() {
	}


	ParabolicCylinder::ParabolicCylinder(Oxy parameters, const double density) : Object::Object(density, (int)(6.28 * density)* (2 * parameters.y * density),
		"Parabolic Cylinder") {
		m_p = parameters.x;
		m_points = new Point[(6.28*m_density)* (2*parameters.y*m_density)];
		for (double j = -parameters.y; j <= parameters.y; j += (1 / m_density))
			for (double i = -PI; i < PI; i += (1 / m_density)) {
				m_points[m_amount].coordinate.x = pow(i, 2);
				m_points[m_amount].coordinate.y = pow(2*m_p, 0.5) * i;
				m_points[m_amount].coordinate.z = j;
				m_points[m_amount].color = object_color;

				m_amount++;
			}
	}

	ParabolicCylinder::~ParabolicCylinder() {
	}


	SingleCavityHyperboloid::SingleCavityHyperboloid(Oxyz parameters, const double density) : Object::Object(density, (6.4 * density)* (6.4 * density),
		"Single Cavity Hyperboloid") {
		m_a = parameters.x;
		m_b = parameters.y;
		m_c = parameters.z;
		m_points = new Point[(6.4 * m_density) * (6.4 * m_density)];
		for (double a = -1 * PI; a < PI; a += (1 / m_density))
			for (double b = -1 * PI; b < PI; b += (1 / m_density)) {
				m_points[m_amount].coordinate.x = m_a * cos(a) * (pow(E, b) + pow(E, -1 * b)) / 2.0;
				m_points[m_amount].coordinate.y = m_b * sin(a) * (pow(E, b) + pow(E, -1 * b)) / 2.0;
				m_points[m_amount].coordinate.z = m_c * (pow(E, b) - pow(E, -1 * b)) / 2.0;
				m_points[m_amount].color = object_color;
				m_amount++;
			}
	}

	SingleCavityHyperboloid::~SingleCavityHyperboloid() {
	}


	DoubleCavityHyperboloid::DoubleCavityHyperboloid(Oxyz parameters, const double density) : Object::Object(density, 2 * (6.4 * density) * (6.4 * density),
		"Double Cavity Hyperboloid") {
		m_a = parameters.x;
		m_b = parameters.y;
		m_c = parameters.z;
		m_points = new Point[2 * (6.4 * m_density) * (6.4 * m_density)];
		for (double a = -1 * PI; a < PI; a += (1 / m_density))
			for (double b = -1 * PI; b < PI; b += (1 / m_density)) {
				m_points[m_amount].coordinate.x = m_a * (pow(E, b) + pow(E, -1 * b)) / 2.0;
				m_points[m_amount].coordinate.y = m_b * sin(a) * (pow(E, b) - pow(E, -1 * b)) / 2.0;
				m_points[m_amount].coordinate.z = m_c * cos(a) * (pow(E, b) - pow(E, -1 * b)) / 2.0;
				m_points[m_amount].color = object_color;

				m_points[m_amount + 1].coordinate.x = -1 * m_points[m_amount].coordinate.x;
				m_points[m_amount + 1].coordinate.y = -1 * m_points[m_amount].coordinate.y;
				m_points[m_amount + 1].coordinate.z = m_points[m_amount].coordinate.z;
				m_points[m_amount + 1].color = object_color;

				m_amount += 2;
			}
	}

	DoubleCavityHyperboloid::~DoubleCavityHyperboloid() {
	}


	EllipticalParaboloid::EllipticalParaboloid(Oxy parameters, const double density) : Object::Object(density, (3.2 * density)* (6.4 * density),
		"Elliptical Paraboloid") {
		m_a = parameters.x;
		m_b = parameters.y;
		m_points = new Point[(3.2 * m_density) * (6.4 * m_density)];
		for (double a = 0; a < PI; a += (1 / m_density))
			for (double b = -1 * PI; b < PI; b += (1 / m_density)) {
				m_points[m_amount].coordinate.x = m_a * a * cos(b);
				m_points[m_amount].coordinate.y = m_b * a * sin(b);
				m_points[m_amount].coordinate.z = 0.5 * a * a;
				m_points[m_amount].color = object_color;
				m_amount++;
			}
	}

	EllipticalParaboloid::~EllipticalParaboloid() {
	}


	HyperbolicParaboloid::HyperbolicParaboloid(Oxy parameters, const double density) : Object::Object(density, (3.2 * density)* (6.4 * density),
		"Hyperbolic Paraboloid") {
		m_a = parameters.x;
		m_b = parameters.y;
		m_points = new Point[(3.2 * m_density) * (6.4 * m_density)];
		for (double a = 0; a < PI; a += (1 / m_density))
			for (double b = -1 * PI; b < PI; b += (1 / m_density)) {
				m_points[m_amount].coordinate.x = m_a * a * (pow(E, b) + pow(E, -1 * b)) / 2.0;
				m_points[m_amount].coordinate.y = m_b * a * (pow(E, b) - pow(E, -1 * b)) / 2.0;
				m_points[m_amount].coordinate.z = 0.5 * a * a;
				m_points[m_amount].color = object_color;
				m_amount++;
			}
	}

	HyperbolicParaboloid::~HyperbolicParaboloid() {
	}


	SecondOrderCone::SecondOrderCone(Oxyz parameters, const double density) : Object::Object(density, 2 * (6.4 * density) * (6.4 * density),
		"Second Order Cone") {
		m_a = parameters.x;
		m_b = parameters.y;
		m_c = parameters.z;
		m_points = new Point[2 * (6.4 * m_density) * (6.4 * m_density)];
		for (double a = -1 * PI; a < PI; a += (1 / m_density))
			for (double b = -1; b < 1; b += (1 / m_density)) {
				m_points[m_amount].coordinate.x = m_a * a * ((2 * b) / (1 + b * b));
				m_points[m_amount].coordinate.y = -1 * m_b * a * ((1 - b * b) / (1 + b * b));
				m_points[m_amount].coordinate.z = m_c * a;
				m_points[m_amount].color = object_color;

				m_points[m_amount + 1].coordinate.x = -1 * m_points[m_amount].coordinate.x;
				m_points[m_amount + 1].coordinate.y = -1 * m_points[m_amount].coordinate.y;
				m_points[m_amount + 1].coordinate.z = m_points[m_amount].coordinate.z;
				m_points[m_amount + 1].color = object_color;

				m_amount += 2;
			}
	}

	SecondOrderCone::~SecondOrderCone() {
	}

	
}