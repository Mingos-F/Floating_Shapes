#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

int BaseCode(int argv, char* argc[]) {
	// create a new window of size 640 by 480 pixles
	// top-left of the windows is (0,0) and the bottom-rigth is (w,h)
	const int wWidth = 640;
	const int wHight = 480;
	sf::RenderWindow window(sf::VideoMode(wWidth, wHight), "SFML works!");

	// make a shape that will be drawn to the screeen
	sf::CircleShape circle(50);            // create a circle shape with radius 50
	circle.setFillColor(sf::Color::Green); // set the circle color to green
	circle.setPosition(300.0f, 300.0f);    // set the top-left position of the circle
	float circleMoveSpeed = -0.01f;        // set speed of the circle

	// load a font to display the text
	sf::Font myFont;

	// attempt to load the font from a file
	if (!myFont.loadFromFile("arial.ttf")) {

		// if we cant load the font prnt an error to the console and exit
		std::cerr << "Could not laod the font" << std::endl;
		exit(-1);
	}

	// set up the text object that will be drawn on screen
	sf::Text text("Sample text", myFont, 24);

	//position the top-left corner of the text so that the text aligns on the bottom
	//text character size in pixles, so move the text up from the bottom by its hight
	text.setPosition(0, wHight - (float)text.getCharacterSize());

	//main loop - frame by frame
	while (window.isOpen()) {
		// event handling
		sf::Event event;
		while (window.pollEvent(event)) {

			// this event triggers when the window is closed
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// this event is triggerd when a key is pressed
			if (event.type == sf::Event::KeyPressed) {

				// print the key that was pressed to the console
				std::cout << "key pressed with code = " << event.key.code << std::endl;

			}

			// when x key is pressed
			if (event.key.code == sf::Keyboard::X) {

				// reverse the direction of the circle on the screen
				circleMoveSpeed *= -1.0f;
			}

		}

		// basic animation - move the circle each frame if still in frame
		sf::Vector2f previousPosition = circle.getPosition();
		sf::Vector2f moveVector(circleMoveSpeed, circleMoveSpeed);
		sf::Vector2f newPosition = previousPosition + moveVector;
		circle.setPosition(newPosition);

		//basic rendering function calls
		window.clear();      // clear the window from anything previously drawn
		window.draw(circle); // draw the circle
		window.draw(text);   // draw the text
		window.display();    // call the window display function
	}
}