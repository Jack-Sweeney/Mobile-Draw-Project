//Jack Sweeney
//C00262459

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>

#define MAX_RECTS 1000

typedef struct Rect {
    Vector2 position;
    float width;
    float height;
    Color color;
} Rect;

#define SAVE_FILE "rectangles.dat"

// Save function
void SaveRectangles(Rect* rectangles, int rectCount) {
    FILE* file = fopen(SAVE_FILE, "wb");
    if (file) {
        fwrite(&rectCount, sizeof(int), 1, file); // Save the number of rectangles
        fwrite(rectangles, sizeof(Rect), rectCount, file); // Save the rectangle data
        fclose(file);
        printf("Saved %d rectangles to file.\n", rectCount);
    }
    else {
        printf("Failed to open file for saving.\n");
    }
}

// Load function
int LoadRectangles(Rect* rectangles) {
    FILE* file = fopen(SAVE_FILE, "rb");
    int rectCount = 0;
    if (file) {
        fread(&rectCount, sizeof(int), 1, file); // Read the number of rectangles
        fread(rectangles, sizeof(Rect), rectCount, file); // Read the rectangle data
        fclose(file);
        printf("Loaded %d rectangles from file.\n", rectCount);
    }
    else {
        printf("Failed to open file for loading.\n");
    }
    return rectCount;
}


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
    Rectangle selectionRect = { 0, 0, 0, 0 };
    bool selectionActive = false;
    Rect copyRect[MAX_RECTS];
    int copyCount = 0;
    Rect saveRect = { 0, 0, 0, 0 };

    int rectPosX = 0;
    int rectPosY = 0;


    // Define a simple color palette
    Color palette[] = { BLACK, RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, MAROON, BROWN, DARKBLUE};
    int paletteSize = sizeof(palette) / sizeof(palette[0]);
    int selectedColor = 0;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
       
        DrawRectangle(0, 700, screenWidth, 150, DARKBROWN);
        //Eraser code
        Rectangle eraser = { 200, screenHeight - 80, 150, 30 };
        if (GuiButton(eraser, "Eraser"))
        {

            currentColor = WHITE;
        }

        Rectangle Clear = { 400, screenHeight - 80, 150, 30 };
        if (GuiButton(Clear, "Clear all"))
        {
            rectCount = 0; 
            ClearBackground(WHITE);
        }


        Rectangle Save = { 600, screenHeight - 80, 150, 30 };
        if (GuiButton(Save, "Save"))
        {
            SaveRectangles(rectangles, rectCount);
        }

        Rectangle Load = { 800, screenHeight - 80, 150, 30 };
        if (GuiButton(Load, "Load")) 
        {
            rectCount = LoadRectangles(rectangles);
        }
        //Selection mode and Drawing mode swap
        Rectangle toggleButton = { 10, screenHeight - 80, 150, 30 };
        if (GuiButton(toggleButton, selectionMode ? "Selection Mode" : "Drawing Mode")) 
        {
            selectionMode = !selectionMode;
        }
        //Draw selection rectangle outline
        if (selectionMode == true)
        {

            DrawText("Press F to fill, C to copy and V to paste", 450, 770, 24, WHITE); 
          
            drawing = false;
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
            {
                
                rectStart = GetMousePosition();
            }

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) 
            {
                if (rectStart.y < 700)
                {
                    rectEnd = GetMousePosition();


                    selectionRect.x = fminf(rectStart.x, rectEnd.x);
                    selectionRect.y = fminf(rectStart.y, rectEnd.y);
                    selectionRect.width = fabsf(rectEnd.x - rectStart.x);
                    selectionRect.height = fabsf(rectEnd.y - rectStart.y);

                    selectionActive = true;
                }
            
                DrawRectangleLinesEx(selectionRect, 2, BLUE);
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
               
                selectionActive = true;
            }
           
            ///Draw Rectangle code
            if (IsKeyDown(KEY_F)) 
            {
               
                     if (selectionActive == true) {
                        if (rectStart.y < 700 && rectCount < MAX_RECTS) {
                            rectangles[rectCount].position.x = selectionRect.x;
                            rectangles[rectCount].position.y = selectionRect.y;
                            rectangles[rectCount].width = selectionRect.width;
                            rectangles[rectCount].height = selectionRect.height;
                            rectangles[rectCount].color = currentColor;
                            rectCount++;
                        }
                        selectionActive = false;  
                    }
                
            }

            // Copy operation
            if (rectStart.y < 700)
            {
                if (IsKeyPressed(KEY_C) && selectionActive == true) 
                {
                    copyCount = 0;
                    for (int i = 0; i < rectCount; i++) {
                        Rect rect = rectangles[i];
                        if (rect.position.x >= selectionRect.x &&
                            rect.position.y >= selectionRect.y &&
                            rect.position.x + rect.width <= selectionRect.x + selectionRect.width &&
                            rect.position.y + rect.height <= selectionRect.y + selectionRect.height) 
                        {

                            if (copyCount < MAX_RECTS) {
                                // Adjust position relative to the selection rectangle
                                Rect copied = rect;
                                copied.position.x -= selectionRect.x;
                                copied.position.y -= selectionRect.y;
                                copyRect[copyCount] = copied;
                                copyCount++;
                            }
                        }
                    }
                    printf("Copied %d rectangles!\n", copyCount);
                }

                // Paste operation
                if (IsKeyPressed(KEY_V) && selectionActive  == true) 
                {
                    for (int i = 0; i < copyCount; i++) 
                    {
                        if (rectCount < MAX_RECTS) {
                            Rect copied = copyRect[i];

                            // Adjust position based on the new selection rectangle
                            copied.position.x += selectionRect.x;
                            copied.position.y += selectionRect.y;

                            rectangles[rectCount] = copied;
                            rectCount++;
                        }
                    }
                    printf("Pasted %d rectangles!\n", copyCount);
                    selectionActive = false;
                }
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
                if (rectStart.y < 700 && rectCount < MAX_RECTS) {
                    Vector2 rectEnd = GetMousePosition();

                    rectangles[rectCount].position.x = rectStart.x;
                    rectangles[rectCount].position.y = rectStart.y;
                    rectangles[rectCount].width = 10;
                    rectangles[rectCount].height = 10;
                    rectangles[rectCount].color = currentColor;
                    rectCount++;
                }
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
        

       
        // Draw
        BeginDrawing();
        ClearBackground(WHITE); 

         
        
        // Draw the palette buttons
        for (int i = 0; i < paletteSize; i++) {
            Rectangle colorButton = { 10 + 40 * i, screenHeight - 40, 30, 30 };
            DrawRectangleRec(colorButton, palette[i]);
            
        }

        if (selectionActive == true)
        {
            DrawRectangleLinesEx(selectionRect, 2, BLACK);  // Highlight selected color
        }
       

      

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