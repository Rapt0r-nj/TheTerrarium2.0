#define BUTTON_WIDTH 32
#define BUTTON_HEIGHT 32
#define BUTTON_MARGIN 4

void handleButtonPress(int* a, int buttonType) {
    *a = buttonType;
}

void resetToggleState(int toggleState[101][101], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            toggleState[i][j] = 0;
        }
    }
}
int handleMouseClick(WORLD* w, int world[101][101], int x, int y, int toggleState[101][101], int nt) {
    int col = x / SQUARE_SIZE;
    int row = y / SQUARE_SIZE;
    if (col >= 0 && col < w->size_ && row >= 0 && row < w->size_) {
        if (toggleState[row][col] == 0) {
            world[row][col] = nt;
            toggleState[row][col] = 1;
        }
        return 1;
    }
    return 0;
}

int isMouseInsideRect(int mouseX, int mouseY, SDL_Rect rect) {
    return mouseX >= rect.x && mouseY >= rect.y && mouseX < (rect.x + rect.w) && mouseY < (rect.y + rect.h);
}

void drawButton(SDL_Renderer* renderer, SDL_Rect rect, const char* label, int hover, color btnColor) {
    int cornerRadius = 10; // Not used, but you can implement rounded corners if needed
    SDL_Color color = {btnColor.R, btnColor.G, btnColor.B, 255};
    SDL_Color borderColor = {255, 255, 255, 255};
    SDL_Color hoverColor = {btnColor.R + 30, btnColor.G + 30, btnColor.B + 30, 255};

    if (hover) {
        color = hoverColor;
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, 255);
    SDL_RenderDrawRect(renderer, &rect);

    int cx = rect.x + rect.w / 2;
    int cy = rect.y + rect.h / 2;

    if (strcmp(label, "+") == 0) {
        SDL_RenderDrawLine(renderer, cx - 10, cy, cx + 10, cy);
        SDL_RenderDrawLine(renderer, cx, cy - 10, cx, cy + 10);
    } else if (strcmp(label, "-") == 0) {
        SDL_RenderDrawLine(renderer, cx - 10, cy, cx + 10, cy);
    } else if (strcmp(label, "S") == 0) {
        SDL_Rect startRect = {cx - 10, cy - 10, 20, 20};
        SDL_RenderFillRect(renderer, &startRect);
    }
}





void drawSpeed(SDL_Renderer* renderer, float speed, int x, int y) {
    int numRectangles = (int)(speed * 2);
    if (numRectangles > 10) {
        numRectangles = 10;
    }
    int rectWidth = 10;
    int rectHeight = 20;
    int spacing = 5;

    for (int i = 0; i < numRectangles; ++i) {
        int red = 0, green = 0, blue = 0;
        float ratio = (float)i / 9.0;

        if (ratio < 0.5) {
            red = (int)(510 * ratio);
            green = 255;
        } else {
            red = 255;
            green = (int)(255 * (2.0 - 2.0 * ratio));
        }

        SDL_SetRenderDrawColor(renderer, red, green, blue, 255);

        SDL_Rect rect = {x + i * (rectWidth + spacing), y, rectWidth, rectHeight};
        SDL_RenderFillRect(renderer, &rect);
    }
}
