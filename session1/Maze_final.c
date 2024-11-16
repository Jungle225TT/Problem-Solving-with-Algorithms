#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

// Définition des constantes pour l'affichage
#define CELL_SIZE 30
#define WALL_WIDTH 3

// Définition des structures de base pour le labyrinthe
typedef struct Cell {
    int left, top, right, bottom;
    int visitedFromD, visitedFromE;
    int door, exit;
    struct Cell *parentFromD, *parentFromE;
    int pathFromD, pathFromE;
} Cell;

typedef struct Maze {
    Cell **cells;
    int width, height;
} Maze;

typedef struct QueueNode {
    int x, y;
} QueueNode;

// Prototypes de fonctions
void freeMaze(Maze *maze);
void setBoundaryWalls(Maze *maze);
void setExit(Maze *maze);
void initializeMaze(Maze *maze, int width, int height);
void generate_random_wall(Maze *maze, int x_start, int y_start, int width, int height);
void displayMaze(Maze *maze, SDL_Renderer *renderer);
int findPathBidirectionalBFS(Maze *maze, int start_x, int start_y);
void markCompletePath(Maze *maze, Cell *meetingCell);

// Fonction pour configurer les murs extérieurs
void setBoundaryWalls(Maze *maze) {
    for (int x = 0; x < maze->width; x++) {
        maze->cells[x][0].top = 1;
        maze->cells[x][maze->height - 1].bottom = 1;
    }
    for (int y = 0; y < maze->height; y++) {
        maze->cells[0][y].left = 1;
        maze->cells[maze->width - 1][y].right = 1;
    }
}

// Fonction pour définir la sortie du labyrinthe
void setExit(Maze *maze) {
    maze->cells[maze->width - 1][maze->height - 1].exit = 1;
}

// Fonction pour initialiser le labyrinthe
void initializeMaze(Maze *maze, int width, int height) {
    if (!maze || width <= 0 || height <= 0) {
        fprintf(stderr, "Erreur: Dimensions invalides lors de l'initialisation du labyrinthe (%dx%d).\n", width, height);
        exit(EXIT_FAILURE);
    }

    maze->width = width;
    maze->height = height;
    maze->cells = malloc(width * sizeof(Cell *));
    if (!maze->cells) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les cellules principales\n");
        exit(EXIT_FAILURE);
    }

    for (int x = 0; x < width; x++) {
        maze->cells[x] = malloc(height * sizeof(Cell));
        if (!maze->cells[x]) {
            fprintf(stderr, "Erreur d'allocation mémoire pour les cellules en colonne\n");
            for (int i = 0; i < x; i++) {
                free(maze->cells[i]);
            }
            free(maze->cells);
            exit(EXIT_FAILURE);
        }

        for (int y = 0; y < height; y++) {
            maze->cells[x][y] = (Cell){0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0};
        }
    }

    setBoundaryWalls(maze);
    setExit(maze);
}


// Fonction pour générer des murs aléatoires
void generate_random_wall(Maze *maze, int x_start, int y_start, int width, int height) {
    if (width < 2 || height < 2) return;
    int horizontal = width > height;

    if (horizontal) {
        int split = x_start + rand() % (width - 1);
        int door = y_start + rand() % height;
        for (int y = y_start; y < y_start + height; y++) {
            if (y != door) {
                maze->cells[split][y].right = 1;
                maze->cells[split + 1][y].left = 1;
            }
        }
        generate_random_wall(maze, x_start, y_start, split - x_start + 1, height);
        generate_random_wall(maze, split + 1, y_start, width - (split - x_start + 1), height);
    } else {
        int split = y_start + rand() % (height - 1);
        int door = x_start + rand() % width;
        for (int x = x_start; x < x_start + width; x++) {
            if (x != door) {
                maze->cells[x][split].bottom = 1;
                maze->cells[x][split + 1].top = 1;
            }
        }
        generate_random_wall(maze, x_start, y_start, width, split - y_start + 1);
        generate_random_wall(maze, x_start, split + 1, width, height - (split - y_start + 1));
    }
}

// Fonction pour marquer le chemin complet
void markCompletePath(Maze *maze, Cell *meetingCell) {
    // Marquage du chemin depuis le départ (D)
    Cell *current = meetingCell;
    while (current) {
        if (!current->door && !current->exit) {  // Ignorer les portes et sorties
            current->pathFromD = 1;
        }
        current = current->parentFromD;
    }

    // Marquage du chemin depuis l'arrivée (E)
    current = meetingCell;
    while (current) {
        if (!current->door && !current->exit) {  // Ignorer les portes et sorties
            current->pathFromE = 1;
        }
        current = current->parentFromE;
    }
}

// Fonction pour trouver le chemin bidirectionnel
int findPathBidirectionalBFS(Maze *maze, int start_x, int start_y) {
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    QueueNode *queueD = malloc(maze->width * maze->height * sizeof(QueueNode));
    QueueNode *queueE = malloc(maze->width * maze->height * sizeof(QueueNode));
    if (!queueD || !queueE) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les files d'attente.\n");
        free(queueD);
        free(queueE);
        return 0;
    }

    int frontD = 0, rearD = 0;
    int frontE = 0, rearE = 0;

    queueD[rearD++] = (QueueNode){start_x, start_y};
    maze->cells[start_x][start_y].visitedFromD = 1;

    int exit_x = maze->width - 1;
    int exit_y = maze->height - 1;
    queueE[rearE++] = (QueueNode){exit_x, exit_y};
    maze->cells[exit_x][exit_y].visitedFromE = 1;

    // Vérification immédiate
    if (maze->cells[start_y][start_x].exit) {
        printf("Start is the exit. Path found immediately.\n");
        goto cleanup; // Libérer la mémoire et retourner 1
    }

    while (frontD < rearD && frontE < rearE) {
        QueueNode currentD = queueD[frontD++];
        int xD = currentD.x, yD = currentD.y;

        for (int i = 0; i < 4; i++) {
            int nxD = xD + dx[i];
            int nyD = yD + dy[i];

            if (nxD >= 0 && nxD < maze->width && nyD >= 0 && nyD < maze->height &&
                !maze->cells[nxD][nyD].visitedFromD &&
                ((i == 0 && !maze->cells[xD][yD].top) || (i == 1 && !maze->cells[xD][yD].bottom) ||
                 (i == 2 && !maze->cells[xD][yD].left) || (i == 3 && !maze->cells[xD][yD].right))) {

                maze->cells[nxD][nyD].visitedFromD = 1;
                maze->cells[nxD][nyD].parentFromD = &maze->cells[xD][yD];
                queueD[rearD++] = (QueueNode){nxD, nyD};

                if (maze->cells[nxD][nyD].visitedFromE) {
                    markCompletePath(maze, &maze->cells[nxD][nyD]);
                    goto cleanup; // Libérer la mémoire et retourner 1
                }
            }
        }

        QueueNode currentE = queueE[frontE++];
        int xE = currentE.x, yE = currentE.y;

        for (int i = 0; i < 4; i++) {
            int nxE = xE + dx[i];
            int nyE = yE + dy[i];

            if (nxE >= 0 && nxE < maze->width && nyE >= 0 && nyE < maze->height &&
                !maze->cells[nxE][nyE].visitedFromE &&
                ((i == 0 && !maze->cells[xE][yE].top) || (i == 1 && !maze->cells[xE][yE].bottom) ||
                 (i == 2 && !maze->cells[xE][yE].left) || (i == 3 && !maze->cells[xE][yE].right))) {

                maze->cells[nxE][nyE].visitedFromE = 1;
                maze->cells[nxE][nyE].parentFromE = &maze->cells[xE][yE];
                queueE[rearE++] = (QueueNode){nxE, nyE};

                if (maze->cells[nxE][nyE].visitedFromD) {
                    markCompletePath(maze, &maze->cells[nxE][nyE]);
                    goto cleanup; // Libérer la mémoire et retourner 1
                }
            }
        }
    }

    // Aucun chemin trouvé
    free(queueD);
    free(queueE);
    return 0;

cleanup:
    free(queueD);
    free(queueE);
    return 1;
}



// Fonction pour afficher le labyrinthe avec SDL2
// Fonction pour afficher le labyrinthe avec SDL2
// Fonction pour afficher le labyrinthe avec SDL2
// Fonction pour afficher le labyrinthe avec SDL2
void displayMaze(Maze *maze, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Fond blanc
    SDL_RenderClear(renderer);

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            int xPos = x * CELL_SIZE;
            int yPos = y * CELL_SIZE;

            // Dessiner les murs
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Couleur noire pour les murs
            if (maze->cells[x][y].top) {
                SDL_Rect topWall = {xPos, yPos, CELL_SIZE, WALL_WIDTH};
                SDL_RenderFillRect(renderer, &topWall);
            }
            if (maze->cells[x][y].bottom) {
                SDL_Rect bottomWall = {xPos, yPos + CELL_SIZE - WALL_WIDTH, CELL_SIZE, WALL_WIDTH};
                SDL_RenderFillRect(renderer, &bottomWall);
            }
            if (maze->cells[x][y].left) {
                SDL_Rect leftWall = {xPos, yPos, WALL_WIDTH, CELL_SIZE};
                SDL_RenderFillRect(renderer, &leftWall);
            }
            if (maze->cells[x][y].right) {
                SDL_Rect rightWall = {xPos + CELL_SIZE - WALL_WIDTH, yPos, WALL_WIDTH, CELL_SIZE};
                SDL_RenderFillRect(renderer, &rightWall);
            }

            // Dessiner la porte (en vert clair)
            if (maze->cells[x][y].door) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Porte en vert clair
                SDL_Rect doorRect = {xPos + WALL_WIDTH, yPos + WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH};
                SDL_RenderFillRect(renderer, &doorRect);
            }

            // Dessiner la sortie (en rouge clair)
            if (maze->cells[x][y].exit) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Sortie en rouge clair
                SDL_Rect exitRect = {xPos + WALL_WIDTH, yPos + WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH};
                SDL_RenderFillRect(renderer, &exitRect);
            }

            // Dessiner les chemins (différenciés par des couleurs)
            if (maze->cells[x][y].pathFromD) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Chemin de D en jaune
                SDL_Rect pathRect = {xPos + WALL_WIDTH, yPos + WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH};
                SDL_RenderFillRect(renderer, &pathRect);
            } else if (maze->cells[x][y].pathFromE) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Chemin de E en bleu
                SDL_Rect pathRect = {xPos + WALL_WIDTH, yPos + WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH, CELL_SIZE - 2 * WALL_WIDTH};
                SDL_RenderFillRect(renderer, &pathRect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}


void freeMaze(Maze *maze) {
    if (!maze || !maze->cells) return;

    for (int x = 0; x < maze->width; x++) {
        if (maze->cells[x]) {
            free(maze->cells[x]);
        }
    }
    free(maze->cells);
    maze->cells = NULL;
    maze->width = 0;
    maze->height = 0;
}


// Fonction principale pour créer et afficher le labyrinthe
#ifndef TESTING // Cette partie est ignorée si TESTING est défini

int main() {
    srand(time(NULL));

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur d'initialisation SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Demander la taille du labyrinthe
    int width = 0, height = 0;
    printf("Entrez la largeur du labyrinthe: ");
    if (scanf("%d", &width) != 1 || width <= 0) {
        fprintf(stderr, "Erreur: Dimension invalide pour la largeur. Elle doit être un entier strictement positif.\n");
        SDL_Quit();
        return 1;
    }

    printf("Entrez la hauteur du labyrinthe: ");
    if (scanf("%d", &height) != 1 || height <= 0) {
        fprintf(stderr, "Erreur: Dimension invalide pour la hauteur. Elle doit être un entier strictement positif.\n");
        SDL_Quit();
        return 1;
    }

    printf("Dimensions saisies : largeur = %d, hauteur = %d\n", width, height);

    // Créer la fenêtre et le renderer SDL
    SDL_Window *window = SDL_CreateWindow("Labyrinthe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * CELL_SIZE, height * CELL_SIZE, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialiser le labyrinthe
    Maze *maze = malloc(sizeof(Maze));
    if (!maze) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le labyrinthe.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    initializeMaze(maze, width, height);
    generate_random_wall(maze, 0, 0, width, height);

    int entry_x = rand() % width;
    int entry_y = rand() % height;
    maze->cells[entry_x][entry_y].door = 1;

    findPathBidirectionalBFS(maze, entry_x, entry_y);

    // Afficher le labyrinthe
    displayMaze(maze, renderer);

    // Attendre que l'utilisateur ferme la fenêtre
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // Libérer la mémoire et quitter SDL
    freeMaze(maze);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#endif
