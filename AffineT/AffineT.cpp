#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

int shape_scale = 1;

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

// Function to get the number of vertices from the user
size_t getNumberOfVertices() {
    size_t numVertices;
	string input;
    
	bool validInput = false;

    while (!validInput) {
        cout << "Enter the number of vertices (min 3, max 10): ";
		cin >> input;
        try {
            numVertices = stoi(input);
            validInput = true;
		}
		catch (invalid_argument& e) {
			cout << "Invalid input. Please enter a valid number." << endl;
		}
        catch (out_of_range& e) {
            cout << "Invalid input. Please enter a number within the valid range." << endl;
        }
    }
   

    // Ensure the number of vertices is between 3 and 10
    if (numVertices < 3) {
        cout << "A shape requires at least 3 vertices. Setting default to 3 vertices." << endl;
        numVertices = 3;
    }
    else if (numVertices > 10) {
        cout << "A shape can have a maximum of 10 vertices. Setting to 10 vertices." << endl;
        numVertices = 10;
    }

    return numVertices;
}

// Function to get the coordinates of the vertices from the user
vector<sf::Vector2f> getVertices(size_t numVertices) 
{
    vector<sf::Vector2f> vertices(numVertices);
    float max_value = 0;
    for (size_t i = 0; i < numVertices; ++i) {
        float x, y;
        cout << "Enter coordinates for vertex " << i + 1 << " (x y): ";
        cin >> x >> y;
        if (abs(x) > max_value)
        {
            max_value = abs(x);
        }
        if (abs(y) > max_value)
        {
            max_value = abs(y);
        }
        vertices[i] = sf::Vector2f(x, y);
    }
    if (max_value > 2)
    {
        shape_scale = abs(max_value) / 2;
    }
    return vertices;
}

// Function to create a shape for visualization
sf::ConvexShape createShape(const vector<sf::Vector2f>& vertices, bool applyScale = true) {
    sf::ConvexShape shape;
    shape.setPointCount(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        shape.setPoint(i, vertices[i]);
    }
    shape.setFillColor(sf::Color::Green);

    if (applyScale) {
        // Scale and translate the shape to fit within the window
        float scale = 100.0f / shape_scale; // Adjust scale factor based on shape_scale
        sf::Vector2f offset(400, 400); // Offset to center the shape in the window
        for (size_t i = 0; i < vertices.size(); ++i) {
            sf::Vector2f point = shape.getPoint(i);
            point.x = point.x * scale + offset.x;
            point.y = -point.y * scale + offset.y; // Invert y-axis to match Cartesian coordinates
            shape.setPoint(i, point);
        }
    }

    return shape;
}

// Function to apply translation to a shape
void applyTranslation(sf::ConvexShape& shape, float dx, float dy) {
    for (size_t i = 0; i < shape.getPointCount(); ++i) {
        sf::Vector2f point = shape.getPoint(i);
        point.x += dx;
        point.y -= dy; // Invert y-axis to match Cartesian coordinates
        shape.setPoint(i, point);
    }
}

// Function to apply scaling to a shape
void applyScaling(sf::ConvexShape& shape, float sx, float sy) {
    for (size_t i = 0; i < shape.getPointCount(); ++i) {
        sf::Vector2f point = shape.getPoint(i);
        point.x *= sx;
        point.y *= sy;
        shape.setPoint(i, point);
    }
}

// Function to apply rotation to a shape
void applyRotation(sf::ConvexShape& shape, float angle) {
    float radians = angle * 3.1416 / 180.0f;
    for (size_t i = 0; i < shape.getPointCount(); ++i) {
        sf::Vector2f point = shape.getPoint(i);
        float x_new = point.x * cos(radians) - point.y * sin(radians);
        float y_new = point.x * sin(radians) + point.y * cos(radians);
        shape.setPoint(i, sf::Vector2f(x_new, y_new));
    }
}

int main() {
    // Window settings
    sf::RenderWindow window(sf::VideoMode(800, 800), "Karam's code");

    // Get number of vertices from user
    size_t numVertices = getNumberOfVertices();

    // Get the coordinates of the vertices from the user
    vector<sf::Vector2f> vertices = getVertices(numVertices);

    // Create the original shape for visualization
    sf::ConvexShape originalShape = createShape(vertices);

    // Create a copy of the shape for transformations
    sf::ConvexShape transformedShape = originalShape;
    transformedShape.setFillColor(sf::Color::Red);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Render the coordinate system and the shapes
        window.clear();
        drawCoordinateSystem(window);
        window.draw(transformedShape);
        window.draw(originalShape);
        window.display();

        // Ask the user for the transformation type and amount
        char transformationType;
        cout << "Enter transformation type (t for translation, s for scaling, r for rotation, e for exit): ";
        cin >> transformationType;

        if (transformationType == 'e') {
            break;
        }

        if (transformationType == 't') {
            float dx, dy;
            cout << "Enter translation amounts (dx dy): ";
            cin >> dx >> dy;
            applyTranslation(transformedShape, dx, dy);
        }
        else if (transformationType == 's') {
            float sx, sy;
            cout << "Enter scaling factors (sx sy): ";
            cin >> sx >> sy;
            applyScaling(transformedShape, sx, sy);
        }
        else if (transformationType == 'r') {
            float angle;
            cout << "Enter rotation angle (degrees): ";
            cin >> angle;
            applyRotation(transformedShape, angle);
        }
        else {
            cout << "Invalid transformation type. Please try again." << endl;
        }




    }

    return 0;
}

/*
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
double getDoubleInput(string message, int min_value, int max_value)
{
    string input;
    int number;
    bool validInput = false;
    while (!validInput) {
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
*/