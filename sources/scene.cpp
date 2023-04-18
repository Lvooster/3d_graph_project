#include <iostream>
#include <cmath>
#include "..\headers\scene.h"

#define STEP_SPEED 0.0005

#define BASE 1

namespace eg {

	struct size {
			double width, height;
		};

	Scene::Scene(int width, int height) {
		m_width = width;
		m_height = height;

		m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_width, m_height), "3D Scene");

		m_texture = std::make_unique<sf::Texture>();
		m_texture->create(m_width, m_height);
		m_sprite = std::make_unique < sf::Sprite>(*m_texture);

		Oxyz cam_pos = { 0.0, 0.0, 0.0 };
		Angle cam_angles = { 0.0, 0.0, 0.0 };
		m_camera = std::make_unique<Camera>(m_width, m_height, cam_pos, cam_angles);
	}

	Scene::~Scene() {
	}

	Object* ObjGenerator(const std::string& obj_name, double density = 50.0) {
		Object* new_object;
		if (obj_name == "Sphere")
			new_object = new Sphere(BASE);
		else if (obj_name == "Ellipse")
			new_object = new Ellipse({ BASE, BASE, BASE });
		else if (obj_name == "Elliptical Cylinder")
			new_object = new EllipticalCylinder({ BASE, BASE, BASE });
		else if (obj_name == "Hyperbolic Cylinder")
			new_object = new HyperbolicCylinder({ BASE, BASE, BASE });
		else if (obj_name == "Single Cavity Hyperboloid")
			new_object = new SingleCavityHyperboloid({ BASE, BASE, BASE });
		else if (obj_name == "Double Cavity Hyperboloid")
			new_object = new DoubleCavityHyperboloid({ BASE, BASE, BASE });
		else if (obj_name == "Elliptical Paraboloid")
			new_object = new EllipticalParaboloid({ BASE, BASE });
		else if (obj_name == "Hyperbolic Paraboloid")
			new_object = new HyperbolicParaboloid({ BASE, BASE });
		else if (obj_name == "Second Order Cone")
			new_object = new SecondOrderCone({ BASE, BASE, BASE });
		else
			new_object = new Sphere(BASE);

		return new_object;
	}

	void Scene::ProcessCycle(Object& object) {
		//Setup before cycle
		bool key_pressed = true;
		unsigned int fps_time_start = clock();
		int operations_count = 0;
		short int wheel_scrolled = 0;
		bool lmouse_pressed = false;
		bool rbutton_pressed = false;
		Oxy old_mouse_position = { 0.0, 0.0 };
		bool project_line = false;
		bool key_pressed_p = false;
		bool key_pressed_c = false;
		bool key_pressed_m = false;
		bool key_pressed_o = false;
		bool key_pressed_n = false;
		bool dist_display = false;
		bool key_pressed_f = false;
		bool show_fps = false;
		int curr_obj_index = 0;
		sf::Vector2u window_old_size = { 0, 0 };
		sf::Vector2u window_new_size = m_window->getSize();

		//Interface
		size s_upper_box = { m_width, m_height * 0.076 };
		Oxy pos_upper_box = { 0.0, 0.0 };
		sf::RectangleShape upper_box(sf::Vector2f(s_upper_box.width, s_upper_box.height));
		upper_box.setPosition(0.0, 0.0);
		upper_box.setFillColor(sf::Color(155, 155, 155));

		size s_lower_box = { m_width, m_height * 0.076 };
		sf::RectangleShape lower_box(sf::Vector2f(s_lower_box.width, s_lower_box.height));
		lower_box.setPosition(0.0, m_height - s_lower_box.height);
		lower_box.setFillColor(sf::Color(155, 155, 155));

		size s_object_field = { s_upper_box.width * 0.25, s_upper_box.height * 0.5 };
		Oxy pos_object_field = { s_upper_box.width * 0.025, s_upper_box.height * 0.25 };
		sf::RectangleShape object_field(sf::Vector2f(s_object_field.width, s_object_field.height));
		object_field.setPosition(pos_object_field.x, pos_object_field.y);
		object_field.setFillColor(sf::Color(246, 246, 246));

		sf::Font font;
		font.loadFromFile("resource/calibri.ttf");
		sf::Text object_text("Object", font);
		object_text.setCharacterSize((int)(s_object_field.height));
		object_text.setFillColor(sf::Color::Black);
		object_text.setPosition(s_upper_box.width * 0.025, s_upper_box.height * 0.25 - 5);

		size s_parameters_field = { s_upper_box.width * 0.3125, s_upper_box.height * 0.5 };
		Oxy pos_parameters_field = { s_upper_box.width * 0.5625, s_upper_box.height * 0.25 };
		sf::RectangleShape parameters_filed(sf::Vector2f(s_parameters_field.width, s_parameters_field.height));
		parameters_filed.setPosition(pos_parameters_field.x, pos_parameters_field.y);
		parameters_filed.setFillColor(sf::Color(246, 246, 246));

		size s_butt_input = { s_upper_box.width * 0.1, s_upper_box.height * 0.5 };
		sf::RectangleShape butt_input(sf::Vector2f(s_butt_input.width, s_butt_input.height));
		butt_input.setPosition(s_upper_box.width * 0.8875, s_upper_box.height * 0.25);
		butt_input.setFillColor(sf::Color(200, 200, 200));

		size s_butt_ok = { s_upper_box.width * 0.05, s_upper_box.height * 0.5 };
		Oxy pos_butt_ok = { s_upper_box.width * 0.28, s_upper_box.height * 0.25 };
		sf::RectangleShape butt_ok(sf::Vector2f(s_butt_ok.width, s_butt_ok.height));
		butt_ok.setPosition(pos_butt_ok.x, pos_butt_ok.y);
		butt_ok.setFillColor(sf::Color(200, 200, 200));

		sf::Text middle_text("Middle", font);
		middle_text.setCharacterSize((int)(m_height * 0.03));
		middle_text.setFillColor({ 20, 150, 250, 255 });

		sf::Text ok_text("OK", font);
		ok_text.setCharacterSize((int)(s_butt_ok.height));
		ok_text.setFillColor(sf::Color::Black);
		double ok_width = ok_text.getLocalBounds().width;
		ok_text.setOrigin(ok_width * 0.5, 0);
		ok_text.setPosition(pos_butt_ok.x + s_butt_ok.width * 0.5, pos_butt_ok.y - 5);

		sf::Text input_text("ENTER", font);
		input_text.setCharacterSize((int)(s_butt_input.height));
		input_text.setFillColor(sf::Color::Black);
		double input_width = input_text.getLocalBounds().width;
		input_text.setOrigin(input_width * 0.5, 0);
		input_text.setPosition(s_upper_box.width * 0.8875 + s_butt_input.width * 0.5, s_upper_box.height * 0.25 - 5);

		sf::Text square_text("Square", font);
		square_text.setCharacterSize((int)(s_butt_input.height));
		square_text.setFillColor(sf::Color::Black);
		square_text.setPosition(s_lower_box.width * 0.01, m_height - s_lower_box.height + s_lower_box.height * 0.2);

		sf::Text info_text_1("| F - rotation mode, J - movement mode; P - points mode on/off; M - change scale; N - show points distance", font);
		info_text_1.setCharacterSize((int)(s_butt_input.height * 0.6));
		info_text_1.setFillColor(sf::Color::Black);
		info_text_1.setPosition(s_lower_box.width * 0.3, (m_height - s_lower_box.height) + s_lower_box.height * 0.05);

		sf::Text info_text_2("| C + O - clear marked points; C + M - return sqale to 1; V - show FPS", font);
		info_text_2.setCharacterSize((int)(s_butt_input.height * 0.6));
		info_text_2.setFillColor(sf::Color::Black);
		info_text_2.setPosition(s_lower_box.width * 0.3, (m_height - s_lower_box.height) + s_butt_input.height * 0.7 + s_lower_box.height * 0.05);

		sf::Text fps_text(" ", font);
		fps_text.setCharacterSize((int)(s_butt_input.height * 0.6));
		fps_text.setFillColor(sf::Color::White);
		fps_text.setPosition(5, s_upper_box.height + 5);

		bool mode = false;
		while (m_window->isOpen()) {

			/*--------OUTPUT--------*/
			
			//If something changed, update screen
			if (key_pressed) {
				m_camera->ClearPicture();
			}

			//Fps display
			if (show_fps) {
				unsigned int fps_time_current = (clock() - fps_time_start) / 1000;
				if (fps_time_current != 0) {
					fps_text.setString(std::to_string(operations_count));
					key_pressed = true;
					fps_time_start = clock();
					operations_count = 0;
				}
				operations_count++;
			}
			else fps_text.setString(" ");


			sf::Event event;

			//Checking events for window closing
			while (m_window->pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					m_window->close();
				else if (event.type == sf::Event::MouseWheelMoved)
					wheel_scrolled = event.mouseWheel.delta;
			}

			//Checking if window size changed
			window_new_size = m_window->getSize();
			if (window_new_size != window_old_size) {
				window_old_size = window_new_size;
				key_pressed = true;
			}

			
			//If we updated screen
			if (key_pressed) {
				//Screen refresh
				m_window->clear();
				m_window->draw(*m_sprite);
			//If button P activated and screen is refreshing
			if (project_line) {
				//If marked amount bigger than 1 and smaller of all point
				if ((object.GetMarkedAmount() > 1) && (object.GetMarkedAmount() < object.GetAmount() - 1)) {
					//--------Line drawing--------
						for (int i = 0; i < object.GetMarkedAmount() - 1; i++) {
						Point point_1;
						Point point_2;
						
						if (object.GetPoint(i).coordinate.x > object.GetPoint(i + 1).coordinate.x) {
							point_1 = object.GetPoint(object.GetMarkedIndex(i + 1));
							point_2 = object.GetPoint(object.GetMarkedIndex(i));
						}
						else {
							point_1 = object.GetPoint(object.GetMarkedIndex(i));
							point_2 = object.GetPoint(object.GetMarkedIndex(i + 1));
						}

						double a = point_2.coordinate.x - point_1.coordinate.x;
						double b = point_2.coordinate.y - point_1.coordinate.y;
						double c = point_2.coordinate.z - point_1.coordinate.z;
						for (float t = 0.0; t < 1.0; t += 0.001) {
							Point line_point;
							line_point.color = { 20, 150, 250, 255 };
							line_point.coordinate.x = a * t + point_1.coordinate.x;
							line_point.coordinate.y = b * t + point_1.coordinate.y;
							line_point.coordinate.z = c * t + point_1.coordinate.z;

							m_camera->ProjectPoint(line_point);
						}
					//Square finding
						if (object.GetMarkedAmount() == 3) {
							double square = 0.0;
								Point p_1 = object.GetPoint(object.GetMarkedIndex(2));
								Point p_2 = object.GetPoint(object.GetMarkedIndex(1));
								Point p_3 = object.GetPoint(object.GetMarkedIndex(0));

								Oxyz vec_1 = { (p_2.coordinate.x - p_1.coordinate.x) / m_scale,
											(p_2.coordinate.y - p_1.coordinate.y) / m_scale,
											(p_2.coordinate.z - p_1.coordinate.z) / m_scale };

								Oxyz vec_2 = { (p_3.coordinate.x - p_1.coordinate.x) / m_scale,
											(p_3.coordinate.y - p_1.coordinate.y) / m_scale,
											(p_3.coordinate.z - p_1.coordinate.z) / m_scale };

								Oxyz vec_multiple = { vec_1.y * vec_2.z - vec_1.z * vec_2.y,
													-1 * (vec_1.x * vec_2.z - vec_1.z * vec_2.x),
													vec_1.x * vec_2.y - vec_1.y * vec_2.x };

								square += 0.5 * pow(vec_multiple.x * vec_multiple.x +
									vec_multiple.y * vec_multiple.y +
									vec_multiple.z * vec_multiple.z, 0.5);
							square_text.setString("Square: " + std::to_string(square));
						}
						else
							square_text.setString("Square: not 3 points!");

						//Scale update (M button)
						if (object.GetMarkedAmount() >= 2) {
							if (key_pressed_m) {
								if (key_pressed_c) {
									m_scale = 1;
								}
								else if (object.GetMarkedAmount() == 2){
									double distance = pow(pow(point_1.coordinate.x - point_2.coordinate.x, 2) +
										+pow(point_1.coordinate.y - point_2.coordinate.y, 2) +
										+pow(point_1.coordinate.z - point_2.coordinate.z, 2), 0.5);
									m_scale = distance;
								}
							}
						}
						//Diistance displaying
						if (dist_display) {
							Point middle_point;
							middle_point.color = { 0, 0, 255, 255 };
							middle_point.coordinate.x = (point_1.coordinate.x + point_2.coordinate.x) * 0.5;
							middle_point.coordinate.y = (point_1.coordinate.y + point_2.coordinate.y) * 0.5;
							middle_point.coordinate.z = (point_1.coordinate.z + point_2.coordinate.z) * 0.5;
							
							int a = m_camera->ReturnProjectPoint(middle_point).x;
							int b = m_camera->ReturnProjectPoint(middle_point).y;
							//std::cout << a << ", " << b << '\n';

							float distance = pow(pow(point_1.coordinate.x - point_2.coordinate.x, 2) +
								+pow(point_1.coordinate.y - point_2.coordinate.y, 2) +
								+pow(point_1.coordinate.z - point_2.coordinate.z, 2), 0.5) / m_scale;

							distance = round(distance * 1000) / 1000;
							std::string dist_str = std::to_string(distance);
							dist_str = dist_str.substr(0, dist_str.size() - 3);

							middle_text.setString(dist_str);

							double middle_width = middle_text.getLocalBounds().width;
							middle_text.setOrigin(middle_width * 0.5, 0);

							middle_text.setPosition(a, b);
							m_window->draw(middle_text);
						}
					}
				}
			}
				//Points projection onto camera plane
				for (int i = 0; i < object.GetAmount(); i++)
					m_camera->ProjectPoint(object.GetPoint(i), i, object.GetMarkedPoint(i));

				//Texture updating
				m_texture->update((uint8_t*)m_camera->Picture(), m_width, m_height, 0, 0);

#define INTERFACE

				//Interface refresh
#ifdef INTERFACE
				m_window->draw(upper_box);
				m_window->draw(lower_box);
				m_window->draw(object_field);
				m_window->draw(object_text);
				//m_window->draw(parameters_filed);
				//m_window->draw(butt_input);
				m_window->draw(butt_ok);
				m_window->draw(ok_text);
				//m_window->draw(input_text);
				m_window->draw(square_text);
				m_window->draw(info_text_1);
				m_window->draw(info_text_2);
				m_window->draw(fps_text);
#endif
				m_window->display();
				
			}

			/*--------INPUT--------*/

			//Mode selector
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
				mode = true;
				m_camera->ChangePosition({ 1.0, 0.0, 0.0 });
				m_camera->FindCenter();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
				mode = false;
			}

			//Mode 1
			if (mode) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					m_camera->MovePolarPosition({ STEP_SPEED * object.GetDensity(), 0.0 , 0.0 }); key_pressed = true;
				}
				else key_pressed = false;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					m_camera->MovePolarPosition({ -1 * STEP_SPEED * object.GetDensity(), 0.0 , 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					m_camera->MovePolarPosition({ 0.0, -1 * STEP_SPEED * object.GetDensity() , 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					m_camera->MovePolarPosition({ 0.0, STEP_SPEED * object.GetDensity() , 0.0 }); key_pressed = true;
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (!lmouse_pressed) {
						old_mouse_position = { (double)sf::Mouse::getPosition().x, (double)sf::Mouse::getPosition().y };
						lmouse_pressed = true;
					}
					else {
						Oxy new_mouse_position = { sf::Mouse::getPosition().x, sf::Mouse::getPosition().y };
						if ((new_mouse_position.x != old_mouse_position.x) || (new_mouse_position.y != old_mouse_position.y)) {
							Oxy mouse_vector = { new_mouse_position.x - old_mouse_position.x, new_mouse_position.y - old_mouse_position.y };
							m_camera->MovePolarPosition({ 0.0, -1 * 0.05 * mouse_vector.x * STEP_SPEED * object.GetDensity() , 0.0 });
							m_camera->MovePolarPosition({ 0.05 * mouse_vector.y * STEP_SPEED * object.GetDensity(),  0.0, 0.0 });
							key_pressed = true;
							old_mouse_position = new_mouse_position;
						}
					}
				}
				else lmouse_pressed = false;

				double R = m_camera->GetPolarPosition().z;
				double teta = m_camera->GetPolarPosition().x;
				double fi = m_camera->GetPolarPosition().y;
				m_camera->ChangePosition({ R * sin(teta) * cos(fi), R * cos(teta), R * sin(teta) * sin(fi) });
				m_camera->FindCenter();
			}


			//Mode 2
			if (!mode) {
				//Buttons control
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					m_camera->MoveAngle({ -(STEP_SPEED * object.GetDensity()), 0.0, 0.0 }); key_pressed = true;
				}
				else key_pressed = false;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					m_camera->MoveAngle({ (STEP_SPEED * object.GetDensity()), 0.0, 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					m_camera->MoveAngle({ 0.0, -(STEP_SPEED * object.GetDensity()), 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					m_camera->MoveAngle({ 0.0, (STEP_SPEED * object.GetDensity()), 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
					m_camera->MovePosition({ (STEP_SPEED * 0.8 * object.GetDensity()), 0.0, 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
					m_camera->MovePosition({ -(STEP_SPEED * object.GetDensity()), 0.0, 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
					m_camera->MovePosition({ 0.00, (STEP_SPEED * 0.8 * object.GetDensity()), 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
					m_camera->MovePosition({ 0.00, -(STEP_SPEED * 0.8 * object.GetDensity()), 0.0 }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
					m_camera->MovePosition({ 0.00, 0.00, (STEP_SPEED * 0.8 * object.GetDensity()) }); key_pressed = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
					m_camera->MovePosition({ 0.00, 0.00, -(STEP_SPEED * 0.8 * object.GetDensity()) }); key_pressed = true;
				}
			}

			Oxy mouse_position = {(double)sf::Mouse::getPosition(*m_window).x, (double)sf::Mouse::getPosition(*m_window).y};

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				key_pressed = true;
				if (rbutton_pressed) {
					mouse_position = { (double)sf::Mouse::getPosition(*m_window).x, (double)sf::Mouse::getPosition(*m_window).y };
					int index = m_camera->GetPointIndex(mouse_position);
					if (index != -1) {
						object.MarkPoint(index);
					}
				}
				rbutton_pressed = false;
			}
			else {
				rbutton_pressed = true;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
				if (!key_pressed_c) {
					key_pressed = true;
					key_pressed_c = true;
				}
			}
			else key_pressed_c = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
				if (!key_pressed_o && key_pressed_c) {
					key_pressed_o = true;
					key_pressed = true;
					object.ClearMarkedPoints();
				}
			}
			else key_pressed_o = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
				if (!key_pressed_m) {
					key_pressed = true;
					key_pressed_m = true;
				}
			}
			else key_pressed_m = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
				if (!key_pressed_n) {
					key_pressed = true;
					key_pressed_n = true;
					if (!dist_display)
						dist_display = true;
					else
						dist_display = false;
				}
			}
			else key_pressed_n = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
				if (!key_pressed_f) {
					key_pressed = true;
					key_pressed_f = true;
					if (!show_fps)
						show_fps = true;
					else
						show_fps = false;
				}
			}
			else key_pressed_f = false;

			//Line drawing
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
				if (!key_pressed_p) {
					key_pressed = true;
					key_pressed_p = true;
					if (!project_line)
						project_line = true;
					else
						project_line = false;
				}
			}
			else key_pressed_p = false;

			if (((mouse_position.x >= 0) && (mouse_position.x <= pos_upper_box.x + s_upper_box.width)) &&
				((mouse_position.y >= 0) && (mouse_position.y <= pos_upper_box.y + s_upper_box.height))) {
				if (((mouse_position.x >= pos_object_field.x) && (mouse_position.x <= pos_object_field.x + s_object_field.width)) &&
					((mouse_position.y >= pos_object_field.y) && (mouse_position.y <= pos_object_field.y + s_object_field.height))) {
					if (wheel_scrolled != 0) {

						if (curr_obj_index + wheel_scrolled < 0)
							curr_obj_index = object.ObjectsAmount() - 1;
						else if (curr_obj_index + wheel_scrolled >= object.ObjectsAmount())
							curr_obj_index = 0;
						else
							curr_obj_index += wheel_scrolled;

						object_text.setString(object.ReturnCurrObject(curr_obj_index));
						wheel_scrolled = 0;
						key_pressed = true;
					}
				}
				else if (((mouse_position.x >= pos_butt_ok.x) && (mouse_position.x <= pos_butt_ok.x + s_butt_ok.width)) &&
					((mouse_position.y >= pos_butt_ok.y) && (mouse_position.y <= pos_butt_ok.y + s_butt_ok.height))) {
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
						if (!lmouse_pressed) {
							key_pressed = true;
							lmouse_pressed = true;
							Object* new_obj = ObjGenerator(object.ReturnCurrObject(curr_obj_index));
							object = *new_obj;
						}
					}
					else lmouse_pressed = false;
				}
			}
			else if (mode) {
				if (wheel_scrolled != 0) {
					m_camera->MovePolarPosition({ 0.0, 0.0 , -12.0 * (double)wheel_scrolled * STEP_SPEED * object.GetDensity() }); key_pressed = true;
					wheel_scrolled = 0;
				}
			}
		}
			
	}
}