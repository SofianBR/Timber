// Include graphic headers from SFML library, so we can use those functions.
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];

// Player/Branches position.
// Left or Right.
enum class side
{
	LEFT, RIGHT, NONE
};

side branchPositions[NUM_BRANCHES];

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

	// Player
	sf::Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	sf::Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	// The player starts on the left
	side playerSide = side::LEFT;

	// Gravestone
	sf::Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	sf::Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 800);

	// Axe
	sf::Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	sf::Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line the axe up with the tree.
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Flying log
	sf::Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	sf::Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	// Log on screen.
	bool logActive = false;
	// Log speed.
	float logSpeedX = 1000;
	float logSpeedY = -1500;

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

	// Prepare 6 branches.
	sf::Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// Set the texture for each branch sprite.
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}
	
	// Adding branches with side.
	for (int i = 1; i < 6; i++)
	{
		updateBranches(i);
	}

	// Control the player input.
	bool acceptInput = false;

	// Sound
	// The player chopping sound.
	sf::SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sounds/chop.wav");
	sf::Sound chopSound;
	chopSound.setBuffer(chopBuffer);

	// The player getting squished by a branch.
	sf::SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sounds/death.wav");
	sf::Sound deathSound;
	deathSound.setBuffer(deathBuffer);

	// Out of time.
	sf::SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sounds/out_of_time.wav");
	sf::Sound ootSound;
	ootSound.setBuffer(ootBuffer);

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		If the player hits 'Escape', will close the window(and the game).
		****************************************
		*/

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased && !paused)
			{
				acceptInput = true;

				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

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

			// Make all the branches disappear.
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// Making sure the gravestione is hidden.
			spriteRIP.setPosition(675, 2000);

			// Move the player into position.
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}

		// Wrap the player controls.
		if (acceptInput)
		{
			// Right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				chopSound.play();
			}

			// Left
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				chopSound.play();
			}
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

				// Play the out of time sound
				ootSound.play();
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

			// Update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side.
					branches[i].setPosition(610, height);

					// Flip the sprite round the other way.
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side.
					branches[i].setPosition(1330, height);

					// Set the sprite rotation to normal.
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch.
					branches[i].setPosition(3000, height);
				}
			}

			// Handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Has the log reached the right edge?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Setting this to be a whole new log next frame.
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// Has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				// Draw the gravestone
				spriteRIP.setPosition(525, 760);

				// Hide the player
				spritePlayer.setPosition(2000, 660);

				// Change the message text
				messageText.setString("SQUISHED!!");

				// Center it on the screen
				sf::FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play the death sound
				deathSound.play();
			}
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
		// Draw the branches.
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		// Draw the tree.
		window.draw(spriteTree);
		// Draw the player.
		window.draw(spritePlayer);
		// Draw the axe.
		window.draw(spriteAxe);
		// Draw the flying log.
		window.draw(spriteLog);
		// Draw the gravestone.
		window.draw(spriteRIP);
		// Draw the bee.
		window.draw(spriteBee);
		// Draw the score.
		window.draw(scoreText);
		// Draw the timebar
		window.draw(timeBar);

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

// Function definitions.
void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0.
	// LEFT, RIGHT or NONE.
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}
}