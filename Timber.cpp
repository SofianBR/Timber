// Include graphic headers from SFML library, so we can use those functions.
#include <SFML/Graphics.hpp>
	
// This is where my game starts.
int main()
{
	// Creating a video game mode object.
	sf::VideoMode vm(1920, 1080);

	// Create and open a window for the game.
	sf::RenderWindow window(vm, "Timer!!!", sf::Style::Fullscreen);

	// Background
	// Create a texture to hold a graphic on the GPU.
	sf::Texture textureBackground;
	// Load a graphic into the texture;
	textureBackground.loadFromFile("graphics/background.png");
	// Create the background sprite.
	sf::Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	// This is another way to set the position.
	// Sprite.setPosition function takes 2 floats or a Vector2f (which contains 2 floats).
	//sf::Vector2f initialPos = { 0, 0 };
	spriteBackground.setPosition(0, 0);

	// Tree
	// Create the tree texture.
	sf::Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	sf::Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	while (window.isOpen())
	{

		// If the player hits 'Escape', will close the window (and the game).
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}


		// Clear everything from the last frame.
		window.clear();

		window.draw(spriteBackground);
		window.draw(spriteTree);

		// Updating the frame with everything I draw.
		window.display();
	}

	return 0;
}