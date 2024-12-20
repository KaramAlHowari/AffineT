#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
using namespace std;

//initializing global variables scale of the shape, and PI
int shape_scale = 1;
const float PI = 3.14159265358979323846;


// Function to draw the Cartesian coordinate system
void drawCoordinateSystem(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    sf::VertexArray lines(sf::Lines);

    // Draw vertical lines
    for (unsigned int x = 0; x <= windowSize.x; x += 100 / shape_scale) {
        lines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(200, 200, 200)));
        lines.append(sf::Vertex(sf::Vector2f(x, windowSize.y), sf::Color(200, 200, 200)));
    }

    // Draw horizontal lines
    for (unsigned int y = 0; y <= windowSize.y; y += 100 / shape_scale) {
        lines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(200, 200, 200)));
        lines.append(sf::Vertex(sf::Vector2f(windowSize.x, y), sf::Color(200, 200, 200)));
    }

    // Draw x-axis
    lines.append(sf::Vertex(sf::Vector2f(0, windowSize.y / 2), sf::Color::Blue));
    lines.append(sf::Vertex(sf::Vector2f(windowSize.x, windowSize.y / 2), sf::Color::Blue));

    // Draw y-axis
    lines.append(sf::Vertex(sf::Vector2f(windowSize.x / 2, 0), sf::Color::Blue));
    lines.append(sf::Vertex(sf::Vector2f(windowSize.x / 2, windowSize.y), sf::Color::Blue));

    window.draw(lines);
}


// Functions to get correct input from the user
int getIntegerInput(string message, int min_value, int max_value)
{
    string input;
    int number;
    bool validInput = false;
    while (!validInput) {
        cout << message;
        cin >> input;
        try {
            number = stoi(input);
            if (number >= min_value && number <= max_value)
            {
                validInput = true;
            }
            else
            {
                cout << "Invalid input. Please enter a number between " << min_value << " and " << max_value << endl;
            }
        }
        catch (invalid_argument& e) {
            cout << "Invalid input. Please enter a valid number." << endl;
        }
        catch (out_of_range& e) {
            cout << "Invalid input. Please enter a number within the valid range." << endl;
        }
    }
    return number;
}
float getFloatInput(string message, int min_value, int max_value)
{
    string input;
    float number;
    bool validInput = false;
    while (!validInput)
    {
        cout << message;
        cin >> input;
        try {
            number = stod(input);
            if (number > min_value && number < max_value)
            {
                validInput = true;
            }
            else
            {
                cout << "Invalid input. Please enter a number between " << min_value << " and " << max_value << endl;
            }
        }
        catch (invalid_argument& e) {
            cout << "Invalid input. Please enter a valid number." << endl;
        }
        catch (out_of_range& e) {
            cout << "Invalid input. Please enter a number within the valid range." << endl;
        }
    }
    return number;
}


// Function to get the coordinates of the vertices from the user
vector<sf::Vector2f> getVertices(size_t numVertices) 
{
    vector<sf::Vector2f> vertices(numVertices);
    float max_value = 0;
    for (size_t i = 0; i < numVertices; ++i)
    {
		// X and Y coordinates of the vertex
        float x, y;
		x = getFloatInput("Enter x coordinate for vertex " + to_string(i + 1) + ": ", -100, 100);
		y = getFloatInput("Enter y coordinate for vertex " + to_string(i + 1) + ": ", -100, 100);
        vertices[i] = sf::Vector2f(x, y);
		// Find the maximum absolute value of x or y
        if (abs(x) > max_value)
        {
            max_value = abs(x);
        }
        if (abs(y) > max_value)
        {
            max_value = abs(y);
        }
    }
	// Set the shape_scale based on the maximum absolute value of x or y
    if (max_value > 2)
    {
        shape_scale = max_value / 2;
    }
    return vertices;
}

// Function to create a shape for visualization
sf::ConvexShape createShape(const vector<sf::Vector2f>& vertices)
{
    sf::ConvexShape shape;
    shape.setPointCount(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        shape.setPoint(i, vertices[i]);
    }
    shape.setFillColor(sf::Color::Green);

    // Scale and translate the shape to fit within the window
    float scale = 100.0f / shape_scale; // Adjust scale factor based on shape_scale
    sf::Vector2f offset(400, 400); // Offset to center the shape in the window
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        sf::Vector2f point = shape.getPoint(i);
        point.x = point.x * scale + offset.x;
        point.y = -point.y * scale + offset.y; // Invert y-axis to match Cartesian coordinates
        shape.setPoint(i, point);
    }

    return shape;
}

// The screen has it's own coordinate system, so we need to convert the coordinates of the shape to the screen coordinates
// with the top left corner being (0, 0) and the bottom right corner being (800, 800)
// and (0, 0) being the center of the screen as (400, 400)
// Get actual coordinates of the x axis
float yCoordinate(const float& y)
{
    return (y / -100) + 4;
}
// Get actual coordinates of the y axis
float xCoordinate(const float& x)
{
    return (x / 100) - 4;
}

// Get on window placement of the x axis
float yPlacement(const float& y)
{
    return ((4 - y) * 100);
}
// Get on window placement of the y axis
float xPlacement(const float& x)
{
    return ((4 + x) * 100);
}


// Function to apply translation to a shape
void applyTranslation(sf::ConvexShape& shape, float dx, float dy)
{
    for (size_t i = 0; i < shape.getPointCount(); ++i) {
        sf::Vector2f point = shape.getPoint(i);
        point.x += (dx*100);
        point.y -= (dy*100); // Invert y-axis to match Cartesian coordinates
        shape.setPoint(i, point);
    }
}

// Function to apply scaling to a shape
void applyScaling(sf::ConvexShape& shape, float sx, float sy)
{
    for (size_t i = 0; i < shape.getPointCount(); ++i) {
        sf::Vector2f point = shape.getPoint(i);
        point.x = xPlacement(sx*xCoordinate(point.x));
        point.y = yPlacement(sy*yCoordinate(point.y));
        shape.setPoint(i, point);
    }
}

// Function to apply rotation to a shape
void applyRotation(sf::ConvexShape& shape, float angle)
{
    float radians = angle * PI / 180.0f;
    for (size_t i = 0; i < shape.getPointCount(); ++i)
    {
        sf::Vector2f point = shape.getPoint(i);
		float x = xCoordinate(point.x);
		float y = yCoordinate(point.y);
        float x_new = xPlacement(x * cos(radians) - y * sin(radians));
        float y_new = yPlacement(x * sin(radians) + y * cos(radians));
        shape.setPoint(i, sf::Vector2f(x_new, y_new));
    }
}

// Function to apply shaering to a shape
void applyShaering(sf::ConvexShape& shape, float shx, float shy)
{
	for (size_t i = 0; i < shape.getPointCount(); ++i)
	{
		sf::Vector2f point = shape.getPoint(i);
		float x = xCoordinate(point.x);
		float y = yCoordinate(point.y);
		float x_new = xPlacement(x + shx * y);
		float y_new = yPlacement(y + shy * x);
		shape.setPoint(i, sf::Vector2f(x_new, y_new));
	}
}

// Function to print the vertices of a shape
void printShapeVertices(const sf::ConvexShape& shape)
{
    cout << "Shape Vertices:" << endl;
    for (size_t i = 0; i < shape.getPointCount(); ++i)
    {
        sf::Vector2f point = shape.getPoint(i);
        cout << "Vertex " << i + 1 << ": (" << xCoordinate(point.x) << ", " << yCoordinate(point.y) << ")" << endl;
    }
}

// Main function
int main() 
{
    // Window settings ( changing resolution breaks the code :) )
    sf::RenderWindow window(sf::VideoMode(800, 800), "Karam's code");

    // Get number of vertices from user
    size_t numVertices = getIntegerInput("Enter the number of vertices (min 3, max 10): ", 3, 10);

    // Get the coordinates of the vertices from the user
    vector<sf::Vector2f> vertices = getVertices(numVertices);

    // Create the original shape for visualization
    sf::ConvexShape originalShape = createShape(vertices);

    // Create a copy of the shape for transformations
    sf::ConvexShape transformedShape = originalShape;
    transformedShape.setFillColor(sf::Color::Red);


    // Main loop
    while (window.isOpen()) 
    {
		// Event handling for closing the window
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Render the coordinate system and the shapes
        window.clear();
        drawCoordinateSystem(window);
        window.draw(originalShape);
        window.draw(transformedShape);
        window.display();

		// Print the vertices of the transformed shape
		printShapeVertices(transformedShape);

        // Ask the user for the transformation type and amount
        int transformationType;
        
		transformationType = getIntegerInput("Enter the transformation type (1: translation, 2: scaling, 3: rotation, 4: shaering, 5: exit): ", 1, 5);

		// Apply the transformation based on the user input
        if (transformationType == 5)
        {
            break;
        }
        else if (transformationType == 1)
        {
            float dx, dy;
			dx = getFloatInput("Enter translation amount dx: ", -4, 4);
			dy = getFloatInput("Enter translation amount dy: ", -4, 4);
            applyTranslation(transformedShape, dx, dy);
        }
        else if (transformationType == 2)
        {
            float sx, sy;
			sx = getFloatInput("Enter scaling factors (sx): ", 0, 4);
			sy = getFloatInput("Enter scaling factors (sy): ", 0, 4);
            applyScaling(transformedShape, sx, sy);
        }
        else if (transformationType == 3)
        {
            float angle;
			angle = getFloatInput("Enter rotation angle (degrees): ", -360, 360);
            applyRotation(transformedShape, angle);
        }
        else if (transformationType == 4)
        {
            float shx, shy;
			shx = getFloatInput("Enter shaering factors (shx): ", -4, 4);
			shy = getFloatInput("Enter shaering factors (shy): ", -4, 4);
			applyShaering(transformedShape, shx, shy);
        }
        else {
            cout << "Invalid transformation type. Please try again." << endl;
        }

    }

    return 0;
}
