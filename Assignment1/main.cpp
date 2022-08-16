#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

/* Specifiction
/ read description os shapes from a configuration file, and then draw those shapes on the screen
/ each shape will have a name, position, speed, color as proprieties unique to them (e.g. rectangle will have a width and heigth, circle radious etc.)
/ shapes must "bounce" from the sides of the window which contains them (if it its the side of windows x speed reverses if it its the top y speed reverses)
/ name of the shape must be placed in its exact center with the specified font size and color specified in the file
/
/ Config file sintax
/
/ Window W H
/ W - Width int
/ H - Height int
/
/ Font F S R G B
/ F - font file std::string
/ S - size int
/ RGB - color int,int,int
/
/ Rectangle N X Y SX SY R G B W H
/ N - name std::string
/ X,Y - position float,float
/ SX,SY - speed float,float
/ RGB - color int,int,int
/ W,H - size float,float
/
/ Circle N X Y SX SY R G B R
/ N - name std::string
/ X,Y - position float,float
/ SX,SY - speed float,float
/ RGB - color int,int,int
/ R - radious float
/
/ View Hints section of Lecture 04
/
*/

// classes to group data

class Font {
	std::string fileName = "";
	int size = 0, red = 0, green = 0, blue = 0;
public:

	const int getsize() {
		return size;
	}

	const int getred() {
		return red;
	}

	const int getgreen() {
		return green;
	}

	const int getblue() {
		return blue;
	}

	void read(std::ifstream& fin, sf::Font& myFont) {
		fin >> fileName >> size >> red >> green >> blue;

		if (!myFont.loadFromFile(fileName)) {

			// if we cant load the font prnt an error to the console and exit
			std::cerr << "Could not laod the font" << std::endl;
			exit(-1);
		}
	}

};


class Circle {
	sf::CircleShape circleShape;
	float radious = 0.0f;
	std::string name = "";
	float x = 0.0f, y = 0.0f, sx = 0.0f, sy = 0.0f;
	int red = 0, green = 0, blue = 0;
	sf::Text text;

public:

	Circle() {

	}

	~Circle()
	{

	}

	void read(std::ifstream& fin) {
		fin >> name >> x >> y >> sx >> sy >> red >> green >> blue >> radious;
		circleShape.setRadius(radious);
		circleShape.setFillColor(sf::Color(red, green, blue));
		circleShape.setPosition(x, y);
	}

	void textPosition(const sf::Font& myFont, Font& fontStruct) {
		text.setString(name);
		text.setFont(myFont);
		text.setCharacterSize(fontStruct.getsize());
		text.setFillColor(sf::Color(fontStruct.getred(), fontStruct.getgreen(), fontStruct.getblue()));
		text.setPosition(circleShape.getPosition() + sf::Vector2f(radious, radious)
			- sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
	}

	void animate(const sf::VideoMode& vm, sf::RenderWindow& window) {

		// get the circle's current position
		sf::Vector2f previousPosition = circleShape.getPosition();

		// verify if is in touch with the window borders

		if (previousPosition.x < 0 || (previousPosition.x + (radious * 2) > vm.width)) {
			sx *= -1.0f;
		}
		else if (previousPosition.y < 0 || (previousPosition.y + (radious * 2) > vm.height)) {
			sy *= -1.0f;
		}

		// update circle and text position
		sf::Vector2f moveVector(sx, sy);
		sf::Vector2f newPosition = previousPosition + moveVector;
		circleShape.setPosition(newPosition);
		text.setPosition(circleShape.getPosition() + sf::Vector2f(radious, radious) -
			sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
		window.draw(circleShape);	// draw the circle
		window.draw(text); // draw the text
	}
};

class Rectangle {
	sf::RectangleShape rectangleShape;
	float width = 0.0f, heigth = 0.0f;
	int red = 0, green = 0, blue = 0;
	std::string name = "";
	float x = 0.0f, y = 0.0f, sx = 0.0f, sy = 0.0f;
	sf::Text text;

public:

	Rectangle() {

	}

	~Rectangle() {

	}

	void read(std::ifstream& fin) {
		fin >> name >> x >> y >> sx >> sy >> red >> green >> blue >> width >> heigth;
		rectangleShape.setSize(sf::Vector2f(width, heigth));
		rectangleShape.setFillColor(sf::Color(red, green, blue));
		rectangleShape.setPosition(x, y);
	}

	void textPosition(const sf::Font& myFont, Font& fontStruct) {
		text.setString(name);
		text.setFont(myFont);
		text.setCharacterSize(fontStruct.getsize());
		text.setFillColor(sf::Color(fontStruct.getred(), fontStruct.getgreen(), fontStruct.getblue()));
		text.setPosition(rectangleShape.getPosition() + sf::Vector2f(width / 2, heigth / 2)
			- sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
	}


	void animate(const sf::VideoMode& vm, sf::RenderWindow& window) {

		// get the rectangle's current position
		sf::Vector2f previousPosition = rectangleShape.getPosition();

		// verify if is in touch with the window borders
		if (previousPosition.x < 0 || (previousPosition.x + width > vm.width)) {
			sx *= -1.0f;
		}
		else if (previousPosition.y < 0 || (previousPosition.y + heigth > vm.height)) {
			sy *= -1.0f;
		}
		// update rectangle and text position
		sf::Vector2f moveVector(sx, sy);
		sf::Vector2f newPosition = previousPosition + moveVector;
		rectangleShape.setPosition(newPosition);
		text.setPosition(rectangleShape.getPosition() + sf::Vector2f(width / 2, heigth / 2) -
			sf::Vector2f(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2));
		window.draw(rectangleShape);	// draw the rectangle
		window.draw(text); // draw the text
	}
};


int main(int argv, char* argc[]) {

	// instances of structures to load data from file
	Font fontStruct;

	// vectores with the circles and rectangles
	std::vector<Circle*> circles;
	std::vector<Rectangle*> rectangles;

	// video mode object to load window config
	sf::VideoMode vm;

	// input file stream
	std::ifstream fin("Config.txt");

	// auxiliar variable to identify what we are reading from file
	std::string id;

	// font object
	sf::Font myFont;

	// read from file if data is valid
	while (fin >> id) {

		// read the window data
		if (id == "Window") {
			fin >> vm.width >> vm.height;
		}
		// read the font data
		else if (id == "Font") {
			fontStruct.read(fin, myFont);
		}
		// read the circle data
		else if (id == "Circle") {
			Circle* circleStruct = new Circle();
			circleStruct->read(fin);
			circles.push_back(circleStruct);
		}
		// read the rectangle data
		else if (id == "Rectangle") {
			Rectangle* rectangleStruct = new Rectangle();
			rectangleStruct->read(fin);
			rectangles.push_back(rectangleStruct);
		}
	}

	// create the window
	sf::RenderWindow window(vm, "SFML works!");

	// create the text and position it in ceter of circle
	for (auto& c : circles) {
		c->textPosition(myFont, fontStruct);
	}

	// create the text and position it in ceter of rectangle
	for (auto& r : rectangles) {
		r->textPosition(myFont, fontStruct);
	}

	// main loop
	while (window.isOpen()) {
		// event handling
		sf::Event event;
		while (window.pollEvent(event)) {

			// this event triggers when the window is closed
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		//basic rendering function calls
		window.clear();      // clear the window from anything previously drawn

		for (auto& c : circles) {
			c->animate(vm, window);
		}
		for (auto& r : rectangles) {
			r->animate(vm, window);
		}
		window.display();    // call the window display function
	}


	return 0;
}