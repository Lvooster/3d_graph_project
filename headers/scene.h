#pragma once
#include <SFML/Graphics.hpp>
#include "..\headers\camera.h"
#include "..\headers\object.h"

namespace eg {

	class Scene {
	private:
		int m_width;								//Window width
		int m_height;								//Window height
		std::unique_ptr<sf::RenderWindow> m_window;	//Window of scene
		std::unique_ptr<sf::Texture> m_texture;		//Texture of window background
		std::unique_ptr<sf::Sprite> m_sprite;		//Window background
		std::unique_ptr<Camera> m_camera;			//Camera of scene
		double m_scale = 1;							//scale for measuring distances

	public:
		Scene(int width, int height);
		~Scene();

		void ProcessCycle(Object& object); //Main cycle of programm
	};

}