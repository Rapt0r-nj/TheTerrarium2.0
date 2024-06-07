#include <stdio.h>
#include <SDL.h>
#include "utils.h"
#include "gg.h"

int main(int argc, char* argv[]) {
    int SIZE;
    int world_buf[101][101];
    int temp_buf[101][101];
    int rules_buf[13];
    int DELTA_T = 1000; // ms
    float speedMultiplier = 1.0;

    WORLD w;

    intro(&w);

    init(&w, &SIZE, world_buf, rules_buf);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("World",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SQUARE_SIZE * SIZE, SQUARE_SIZE * SIZE + 150,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int quit = 0;
    int mouseDown = 0; // track mouse press
    int toggleState[101][101] = {0}; // drag check
    int isSettingUp = 1;
    int newType = 0;

    SDL_Rect plusButtonRect = {10, SQUARE_SIZE * SIZE + 10, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect minusButtonRect = {70, SQUARE_SIZE * SIZE + 10, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect startButtonRect = {130, SQUARE_SIZE * SIZE + 10, BUTTON_WIDTH, BUTTON_HEIGHT};

    SDL_Rect liveButtonRect = {10, SQUARE_SIZE * SIZE + 50, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect deadButtonRect = {70, SQUARE_SIZE * SIZE + 50, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect spaceButtonRect = {130, SQUARE_SIZE * SIZE + 50, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect angelButtonRect = {190, SQUARE_SIZE * SIZE + 50, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect devilButtonRect = {250, SQUARE_SIZE * SIZE + 50, BUTTON_WIDTH, BUTTON_HEIGHT};

    while (!quit) {
        SDL_Event event;
        int x, y;
        int plusHover = 0, minusHover = 0, startHover = 0;
        int liveHover = 0, deadHover = 0, spaceHover = 0, angelHover = 0, devilHover = 0;

        SDL_GetMouseState(&x, &y);
        plusHover = isMouseInsideRect(x, y, plusButtonRect);
        minusHover = isMouseInsideRect(x, y, minusButtonRect);
        startHover = isMouseInsideRect(x, y, startButtonRect);
        liveHover = isMouseInsideRect(x, y, liveButtonRect);
        deadHover = isMouseInsideRect(x, y, deadButtonRect);
        spaceHover = isMouseInsideRect(x, y, spaceButtonRect);
        angelHover = isMouseInsideRect(x, y, angelButtonRect);
        devilHover = isMouseInsideRect(x, y, devilButtonRect);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = 1;
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&x, &y);
                if (liveHover) {
                    handleButtonPress(&newType, w.countdown_);
                } else if (deadHover) {
                    handleButtonPress(&newType, 0);
                } else if (spaceHover) {
                    handleButtonPress(&newType, -10);
                } else if (angelHover) {
                    handleButtonPress(&newType, -20);
                } else if (devilHover) {
                    handleButtonPress(&newType, -66);
                } else if (startHover && isSettingUp) {
                    isSettingUp = 0; // End the setup phase
                    memcpy(w.state_, world_buf, sizeof(world_buf));
                } else if (plusHover && !isSettingUp) {
                    if (speedMultiplier < 5.0) {
                        speedMultiplier += 0.5;
                    }
                } else if (minusHover && !isSettingUp) {
                    speedMultiplier -= 0.5;
                    if (speedMultiplier < 0.5) speedMultiplier = 0.5;
                } else {
                    handleMouseClick(&w, world_buf, x, y, toggleState, newType);
                    mouseDown = 1;
                    resetToggleState(toggleState, w.size_);
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                mouseDown = 0;
            } else if (event.type == SDL_MOUSEMOTION && mouseDown) {
                SDL_GetMouseState(&x, &y);
                handleMouseClick(&w, world_buf, x, y, toggleState, newType);
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_PLUS:
                    case SDLK_EQUALS:
                    case SDLK_KP_PLUS:
                        if (!isSettingUp) {
                            if (speedMultiplier < 5.0) {
                                speedMultiplier += 0.5;
                            }
                        }
                        break;
                    case SDLK_MINUS:
                    case SDLK_KP_MINUS:
                        if (!isSettingUp) {
                            speedMultiplier -= 0.5;
                            if (speedMultiplier < 0.5) speedMultiplier = 0.5;
                        }
                        break;
                    case SDLK_s:
                        if (isSettingUp) {
                            isSettingUp = 0;
                            memcpy(w.state_, world_buf, sizeof(world_buf));
                        }
                        break;
                    case SDLK_1:
                        handleButtonPress(&newType, w.countdown_);
                        break;
                    case SDLK_2:
                        handleButtonPress(&newType, 0);
                        break;
                    case SDLK_3:
                        handleButtonPress(&newType, -10);
                        break;
                    case SDLK_4:
                        handleButtonPress(&newType, -20);
                        break;
                    case SDLK_5:
                        handleButtonPress(&newType, -66);
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (isSettingUp) {
            drawButton(renderer, startButtonRect, "S", startHover, themes[w.theme_].ng);
        } else {
            drawButton(renderer, plusButtonRect, "+", plusHover, (color){0, 255, 0});  // Green
            drawButton(renderer, minusButtonRect, "-", minusHover, (color){255, 0, 0}); // Red
            drawSpeed(renderer, speedMultiplier, 130, SQUARE_SIZE * SIZE + 10); // Moved speed indicator to avoid overlap
        }

        drawButton(renderer, liveButtonRect, "Live", liveHover, themes[w.theme_].lv);
        drawButton(renderer, deadButtonRect, "Dead", deadHover, themes[w.theme_].dd);
        drawButton(renderer, spaceButtonRect, "Space", spaceHover, themes[w.theme_].sp);
        drawButton(renderer, angelButtonRect, "Angel", angelHover, themes[w.theme_].ng);
        drawButton(renderer, devilButtonRect, "Devil", devilHover, themes[w.theme_].dv);

        show(&w, world_buf, renderer);

        if (!isSettingUp) {
            countneighbours(&w, world_buf, temp_buf);
            update(&w, world_buf, temp_buf, rules_buf);
            SDL_Delay(DELTA_T / speedMultiplier);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Do you want to save current world?(0/1): ");
    int chs = 0;
    scanf(" %d", &chs);
    if (chs) {
        writeWorld(&w);
    }
    printf("Ciao!");
    return 0;
}
