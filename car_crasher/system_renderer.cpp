#include "system_renderer.h"
#include <queue> // For std::queue to manage a rendering queue

using namespace std;
using namespace sf;

// Static variables for managing the rendering system
static queue<const Drawable *> sprites; // Queue to hold Drawable objects for rendering
static RenderWindow *rw;                // Pointer to the current SFML RenderWindow

// Initialize the renderer with a reference to the RenderWindow
void Renderer::initialise(sf::RenderWindow &r) {
    rw = &r; // Store the reference to the RenderWindow
}

// Retrieve the current RenderWindow
RenderWindow &Renderer::getWindow() {
    return *rw; // Return the stored RenderWindow reference
}

// Shutdown the renderer and clear the queue
void Renderer::shutdown() {
    // Empty the queue to ensure no Drawable objects remain
    while (!sprites.empty())
        sprites.pop();
}

// Update method for consistency, not used in this implementation
void Renderer::update(const double &) {
    // Placeholder for future functionality, such as animations or updates
}

// Render all queued Drawable objects
void Renderer::render() {
    if (rw == nullptr) { // Check if the RenderWindow has been set
        throw("No render window set! "); // Throw an error if no RenderWindow exists
    }

    // Process and draw all queued Drawable objects
    while (!sprites.empty()) {
        rw->draw(*sprites.front()); // Draw the front Drawable in the queue
        sprites.pop();              // Remove the Drawable from the queue
    }

}


// Add a Drawable object to the rendering queue
void Renderer::queue(const Drawable *s) {
    sprites.push(s); // Push the Drawable object onto the queue
}
