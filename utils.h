
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "colors.h"
#include <unistd.h>

#define SQUARE_SIZE 20


typedef struct WORLD{
  int size_;
  int state_[101][101];
  int rules_[13];
  int neighbourhood_;
  int countdown_;
  int theme_;
} WORLD;

void intro(WORLD* w){
    printf("You wish...\n0. Create new World\n1. Load saved World\n  ");
    int ch = 0; scanf(" %d", &ch);
    if(ch) readWorld(w);
    else newWorld(w);
}

void newWorld(WORLD* w){
    srand(time(NULL));
    do {
        printf("Enter World size:\n  ");
        scanf(" %d", &(w->size_));
        if (w->size_ >= 20) {
            break;
        } else {
            printf("Error: World size must be at least 20.\n");
        }
    } while (1);

    printf("Enter neighbourhood profile:\n");
    printf("0. von Neumann_1\n");
    printf("1. Moore\n");
    printf("2. von Neumann_2\n");
    printf("Your pick: ");
    int p;
    while (scanf(" %d", &p) != 1 || (p != 0 && p != 1 && p != 2)) {
        printf("Please enter a valid option (0, 1, or 2): ");
        while (getchar() != '\n'); // Clear input buffer
    }
    w->neighbourhood_ = 8;
    if(p==0) w->neighbourhood_ = 4;
    if(p==2) w->neighbourhood_ = 12;

    memset(w->rules_, 0, sizeof w->rules_);
    getchar();
    printf("regen?\n  ");
    take(w->rules_, 1);

    printf("stay?\n  ");
    take(w->rules_, -1);

    printf("countdown?\n  ");
    scanf(" %d", &(w->countdown_));

    printf("colorScheme?\n");
    printf("0. cattpuccin\n");
    printf("1. tokyo night\n");
    printf("2. dracula\n");
    printf("Your pick: ");
    int theme;
    while (scanf("%d", &theme) != 1 || (theme != 0 && theme != 1 && theme != 2)) {
        printf("Please enter a valid option (0, 1, or 2): ");
        while (getchar() != '\n'); // Clear input buffer
    }
    w->theme_ = theme;

    for(int i = 0; i < w->size_; i++){
        for(int j = 0; j < w->size_; j++){
            w->state_[i][j] = 0;
        }
    }
}

int writeWorld(WORLD* w){
    char filename[100];
     while (1) {
        printf("Enter the filename (must end with .wr):\n  ");
        scanf("%s", filename);
        if (access(filename, F_OK) == -1) {
            // File does not exis..
            break;
        } else {
            printf("File with the same name already exists. Please use a new name.\n");
        }
    }
    FILE *file = fopen(filename, "wb");
    size_t sizeW = sizeof(WORLD);
    if (fwrite(w, sizeW, 1, file)!= 1) {
        perror("Failed to write to the file");
        fclose(file);
    return 1;
    }
}

int readWorld(WORLD* w){
    char filename[100];
    printf("Enter the filename (must end with .wr):\n  ");
    scanf("%s", filename);
    FILE *file = fopen(filename, "rb");
    if(file == NULL) {
        printf("File not found or could not be opened.\n");
        return 1;
    }
    if (fread(w, sizeof(WORLD), 1, file)!= 1) {
        perror("Failed to read from the file");
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}

void init(WORLD* w, int* s, int world[101][101], int rules[13]){ //done
    *s = w->size_;
    for(int i = 0; i < w->size_; i++){
        for(int j = 0; j < w->size_; j++){
            world[i][j] = w->state_[i][j];
        }
    }
    for(int  i=0; i<13; i++) rules[i]=w->rules_[i];
}



void countneighbours(WORLD* w, int world[101][101], int temp[101][101]){ //done
    int dy[]={-1, 1, 0, 0, 1, 1, -1, -1, -2, 2, 0, 0};
    int dx[]={0, 0, -1, 1, 1, -1, 1, -1, 0, 0, 2, -2};

    int sz=w->size_;
    for(int i=0; i<sz; i++){
        for(int j=0; j<sz; j++){
            int cc=0; int df=0;
            for(int k=0; k<(w->neighbourhood_); k++){
                int ni=(i+dy[k]+sz)%sz;
                int nj=(j+dx[k]+sz)%sz;
                if(world[ni][nj]==-20){
                    cc = -20;
                    break;
                }
                if(world[ni][nj] == -66){
                    df=1;
                }
                if(world[ni][nj]>=1 && world[ni][ni]<=(w->countdown_)) cc++;
            }
            if(df && w->rules_[cc]==1) temp[i][j]=-69;
            else if(df) temp[i][j]=-66;
            else temp[i][j]=cc;
        }
    }
}

void take(int inputs[13], int delta){ //bug: empty line gives zero
    char buffer[100];
    int count = 0;
    fgets(buffer, sizeof(buffer), stdin);
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        inputs[atoi(token)] = delta;
        count++;
        token = strtok(NULL, " ");
    }
}


void update(WORLD* w, int world[101][101], int temp[101][101], int rules[13]) { //1=regen, 0=ded, -1=stay
    for (int i = 0; i < w->size_; ++i){
        for (int j = 0; j < w->size_; ++j){
            int alive = (world[i][j]==w->countdown_);
            int ghost = (world[i][j]>=1 && world[i][j]<w->countdown_);

            if(world[i][j]<0) continue; //continue if space, angel, devil;
            if(temp[i][j]==-69) world[i][j]=(w->countdown_-1);
            else if(alive && temp[i][j]==-66) world[i][j]=(w->countdown_-1); //devil in neighbour
            else if(temp[i][j]==-20) world[i][j]=(w->countdown_); //angel in neighbour
            else if(ghost) world[i][j]--;
            else if(rules[temp[i][j]]==1) world[i][j]=(w->countdown_); //regen
            else if(rules[temp[i][j]]==-1) continue; //stay;
            else if(alive && rules[temp[i][j]]==0) world[i][j]--;
        }
    }
}


void show(WORLD* w, int world[101][101], SDL_Renderer* renderer){ //done
  for (int row = 0; row < w->size_; ++row) {
        for (int col = 0; col < w->size_; ++col) {
            SDL_Rect squareRect = { col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
            if (world[row][col] == 0) {
                SDL_SetRenderDrawColor(renderer, themes[w->theme_].dd.R, themes[w->theme_].dd.G, themes[w->theme_].dd.B, 255);
            }
            else  if (world[row][col] == w->countdown_){
                SDL_SetRenderDrawColor(renderer, themes[w->theme_].lv.R, themes[w->theme_].lv.G, themes[w->theme_].lv.B, 255);
            }
            else  if (world[row][col]>=1 && world[row][col]<=w->countdown_){
                SDL_SetRenderDrawColor(renderer, themes[w->theme_].gh.R, themes[w->theme_].gh.G, themes[w->theme_].gh.B, 255);
            }
            else  if (world[row][col] == -10){
                SDL_SetRenderDrawColor(renderer, themes[w->theme_].sp.R, themes[w->theme_].sp.G, themes[w->theme_].sp.B, 255);
            }
            else  if (world[row][col] == -20){
                SDL_SetRenderDrawColor(renderer, themes[w->theme_].ng.R, themes[w->theme_].ng.G, themes[w->theme_].ng.B, 255);
            }
            else  if (world[row][col] == -66){
                SDL_SetRenderDrawColor(renderer, themes[w->theme_].dv.R, themes[w->theme_].dv.G, themes[w->theme_].dv.B, 255);
            }

            SDL_RenderFillRect(renderer, &squareRect);
        }
    }
    SDL_RenderPresent(renderer);
}
