// Include graphic headers from SFML library, so we can use those functions.
#include <sstream>
#include <SFML/Graphics.hpp>
	
// This is where my game starts.
int main()
{
	// Creating a video game mode object.
	sf::VideoMode vm(1920, 1080);

	// Create and open a window for the game.
	sf::RenderWindow window(vm, "Timer!!!", sf::Style::Fullscreen);

	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

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

	// Bee
	// Create the bee texture.
	sf::Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	sf::Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	bool beeActive = false;
	float beeSpeed = 0.0f;

	// Cloud
	sf::Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	// 3 Clouds
	// Clouds sprites.
	sf::Sprite spriteCloud1;
	sf::Sprite spriteCloud2;
	sf::Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	// Positions
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
	// Clouds on screen
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// Clouds speed
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// Variable to control time itself.
	sf::Clock clock;

	// Time bar.
	sf::RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	sf::Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track whether the game is running.
	bool paused = true;

	// HUD
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;

	// Setting the font.
	messageText.setFont(font);
	scoreText.setFont(font);
	// Setting the text.
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	// Setting the size of our Texts.
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	// Setting the color.
	messageText.setFillColor(sf::Color::White);
	scoreText.setFillColor(sf::Color::White);

	// Position the text.
	sf::FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left + 
		textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);
	

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		If the player hits 'Escape', will close the window(and the game).
		****************************************
		*/
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		// Start the game.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			paused = false;

			// Reset the time and the score.
			score = 0;
			timeRemaining = 6;
		}


		/*
		****************************************
		Update the scene
		****************************************
		*/
		if (!paused)
		{
			// Measuring the time that each PC takes to loop again.
			// With this measure we solve the frame rate on any PC.
			sf::Time dt = clock.restart();

			// Subtract from the amount of time remaining;
			timeRemaining -= dt.asSeconds();
			// Size up the time bar.
			timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, 
				timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				// Pause the game.
				paused = true;

				// Change the message shown to the player.
				messageText.setString("Out of time!!");

				// Reposition the text based on its new size.
				sf::FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			// Bee's setup.
			if (!beeActive)
			{
				// How fast is the bee.
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				//How high is the bee.
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				// Move the bee.
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

				// Left boundries.
				if (spriteBee.getPosition().x < -100)
				{
					// With this we can make the ilusion of making a brand new bee.
					beeActive = false;
				}
			}

			// Clouds' setup.
			// Cloud 1
			if (!cloud1Active)
			{
				// How fast is cloud 1.
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// How high is the cloud.
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);

				// Right boundries.
				if (spriteCloud1.getPosition().x > 1920)
				{
					// With this we can make the ilusion of making a brand new cloud.
					cloud1Active = false;
				}
			}
			// Cloud 2
			if (!cloud2Active)
			{
				// How fast is cloud 2.
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// How high is the cloud.
				srand((int)time(0) * 20);
				float height = (rand() % 150);
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

				// Right boundries.
				if (spriteCloud2.getPosition().x > 1920)
				{
					// With this we can make the ilusion of making a brand new cloud.
					cloud2Active = false;
				}
			}
			// Cloud 3
			if (!cloud3Active)
			{
				// How fast is cloud 3.
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// How high is the cloud.
				srand((int)time(0) * 30);
				float height = (rand() % 150);
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

				// Right boundries.
				if (spriteCloud3.getPosition().x > 1920)
				{
					// With this we can make the ilusion of making a brand new cloud.
					cloud3Active = false;
				}
			}

			// Update the score text.
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());
		}

		/*
		****************************************
		Draw the scene
		****************************************
		*/
		// Clear everything from the last frame.
		window.clear();

		// Draw everything with updates.
		// Draw the background.
		window.draw(spriteBackground);
		// Draw the clouds.
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		// Draw the tree.
		window.draw(spriteTree);
		// Draw the bee.
		window.draw(spriteBee);
		// Draw the score.
		window.draw(scoreText);
		if (paused)
		{
			// Draw the message.
			window.draw(messageText);
		}

		// Updating the frame with everything I draw.
		window.display();
	}

	return 0;
}