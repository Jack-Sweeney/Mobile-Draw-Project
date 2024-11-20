//Jack Sweeney
//C00262459

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define MAX_RECTS 1000

typedef struct Rect {
    Vector2 position;
    float width;
    float height;
    Color color;
} Rect;

int main() {
    // Initialization
    int screenWidth = 1000;
    int screenHeight = 800;
    InitWindow(screenWidth, screenHeight , "Simple Drawing Package - Rectangle Mode");

    // Variables
    Rect rectangles[MAX_RECTS];
    int rectCount = 0;
    Color currentColor = BLACK;  // Default color for drawing rectangles
    Vector2 rectStart = { 0 };
    Vector2 rectEnd = { 0 };
    bool drawing = true;
    bool selectionMode = false;

    // Define a simple color palette
    Color palette[] = { BLACK, RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, MAROON, BROWN, DARKBLUE};
    int paletteSize = sizeof(palette) / sizeof(palette[0]);
    int selectedColor = 0;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
       
        //Eraser code
        Rectangle eraser = { 200, screenHeight - 80, 150, 30 };
        if (GuiButton(eraser, "Eraser"))
        {

            currentColor = WHITE;
        }
        //Selection mode and Drawing mode swap
        Rectangle toggleButton = { 10, screenHeight - 80, 150, 30 };
        if (GuiButton(toggleButton, selectionMode ? "Selection Mode" : "Drawing Mode")) 
        {
            selectionMode = !selectionMode;
        }
        if (selectionMode == true)
        {
            drawing = false;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                
                rectStart = GetMousePosition();
            }

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
               
                rectEnd = GetMousePosition();

               
                Rectangle currentRect = {
                    fminf(rectStart.x, rectEnd.x),
                    fminf(rectStart.y, rectEnd.y),
                    fabsf(rectEnd.x - rectStart.x),
                    fabsf(rectEnd.y - rectStart.y)
                };

            
                DrawRectangleLinesEx(currentRect, 2, BLUE);
            }
        }
        else
        {
            drawing = true;
        }

        if (drawing == true)
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                // Start a new rectangle
                rectStart = GetMousePosition();
                if (rectCount < MAX_RECTS) {
                    Vector2 rectEnd = GetMousePosition();

                    rectangles[rectCount].position.x = rectStart.x;
                    rectangles[rectCount].position.y = rectStart.y;
                    rectangles[rectCount].width = 10;
                    rectangles[rectCount].height = 10;
                    rectangles[rectCount].color = currentColor;
                    rectCount++;
                }
            }


            
           
           
            

            // Change color based on palette selection
            for (int i = 0; i < paletteSize; i++) {
                Rectangle colorButton = { 10 + 40 * i, screenHeight - 40, 30, 30 };
                if (GuiButton(colorButton, "")) {
                    currentColor = palette[i];
                    selectedColor = i;
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(WHITE);

        
        // Draw the palette buttons
        for (int i = 0; i < paletteSize; i++) {
            Rectangle colorButton = { 10 + 40 * i, screenHeight - 40, 30, 30 };
            DrawRectangleRec(colorButton, palette[i]);
            if (i == selectedColor) {
                DrawRectangleLinesEx(colorButton, 2, BLACK);  // Highlight selected color
            }
        }

        //CheckCollisionPointRec(mousePos, boundingBox);
        // bool insideDrawArea

        DrawRectangle(0, 0, screenWidth, 150, RAYWHITE);

        // Draw all stored rectangles
        for (int i = 0; i < rectCount; i++) {
            DrawRectangleRec({ rectangles[i].position.x, rectangles[i].position.y, rectangles[i].width, rectangles[i].height }, rectangles[i].color);
        }

        // Draw the current rectangle if drawing


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}