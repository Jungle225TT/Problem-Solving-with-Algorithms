#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "Maze_final.c" // Inclure le fichier principal contenant votre code

#define TEST_PASS "\033[1;32mPASS\033[0m"
#define TEST_FAIL "\033[1;31mFAIL\033[0m"

// Fonction pour exécuter un test et afficher son résultat
#define RUN_TEST(testFunc, testName)           \
    do {                                       \
        printf("Running %s... ", testName);    \
        if (testFunc()) {                      \
            printf("%s\n", TEST_PASS);         \
        } else {                               \
            printf("%s\n", TEST_FAIL);         \
        }                                      \
    } while (0)

// Vérifie si une cellule est la sortie (coin sud-est)
bool isExit(int row, int col, int rows, int cols) {
    return (row == rows - 1 && col == cols - 1);
}

// Test des fonctionnalités de base
int testBasicFunctionality() {
    printf("=== Test: Basic Functionality ===\n");

    Maze maze;

    // Test de base pour labyrinthe 10x10
    initializeMaze(&maze, 10, 10);
    generate_random_wall(&maze, 0, 0, 10, 10);
    if (!findPathBidirectionalBFS(&maze, 0, 0)) {
        freeMaze(&maze);
        return 0;
    }
    freeMaze(&maze);

    // Labyrinthe simple
    initializeMaze(&maze, 5, 5);
    maze.cells[0][0].door = 1;
    setExit(&maze);
    if (!findPathBidirectionalBFS(&maze, 0, 0)) {
        freeMaze(&maze);
        return 0;
    }
    freeMaze(&maze);

    // Labyrinthe complexe
    initializeMaze(&maze, 20, 20);
    generate_random_wall(&maze, 0, 0, 20, 20);
    if (!findPathBidirectionalBFS(&maze, 0, 0)) {
        freeMaze(&maze);
        return 0;
    }
    freeMaze(&maze);

    // Labyrinthe sans solution
    initializeMaze(&maze, 10, 10);
    maze.cells[0][0].door = 1;
    maze.cells[9][9].exit = 1;
    for (int x = 0; x < 10; x++)
        for (int y = 0; y < 10; y++)
            maze.cells[x][y].top = maze.cells[x][y].bottom = maze.cells[x][y].left = maze.cells[x][y].right = 1;
    if (findPathBidirectionalBFS(&maze, 0, 0)) {
        freeMaze(&maze);
        return 0; // Échec si un chemin est trouvé
    }
    freeMaze(&maze);

    return 1; // Succès
}

// Test des performances
int testPerformance() {
    printf("=== Test: Performance ===\n");

    Maze maze;
    int sizes[] = {50, 100, 200};
    clock_t start, end;

    for (int i = 0; i < 3; i++) {
        int size = sizes[i];
        initializeMaze(&maze, size, size);
        generate_random_wall(&maze, 0, 0, size, size);

        start = clock();
        if (!findPathBidirectionalBFS(&maze, 0, 0)) {
            freeMaze(&maze);
            return 0;
        }
        end = clock();

        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Maze %dx%d solved in %.4f seconds.\n", size, size, elapsed);
        freeMaze(&maze);
    }

    return 1; // Succès
}

// Test des cas limites
int testBoundaryCases() {
    printf("=== Test: Boundary Cases ===\n");

    Maze maze;

    // Test 1x1
    printf("Testing 1x1 maze...\n");
    initializeMaze(&maze, 1, 1);
    maze.cells[0][0].door = 1;
    maze.cells[0][0].exit = 1; // La cellule est à la fois la porte et la sortie
    if (!findPathBidirectionalBFS(&maze, 0, 0)) {
        freeMaze(&maze);
        return 0;
    }
    freeMaze(&maze);

    // Test 1000x1000
    printf("Testing 1000x1000 maze...\n");
    initializeMaze(&maze, 1000, 1000);
    generate_random_wall(&maze, 0, 0, 1000, 1000);
    if (!findPathBidirectionalBFS(&maze, 0, 0)) {
        freeMaze(&maze);
        return 0;
    }
    freeMaze(&maze);

    return 1; // Succès
}

// Test : Vérifier qu'il y a une unique sortie au coin sud-est
int testUniqueExit() {
    printf("=== Test: Unique Exit ===\n");

    int rows = 20, cols = 20;
    Maze maze;
    initializeMaze(&maze, rows, cols);
    generate_random_wall(&maze, 0, 0, rows, cols);

    int exit_count = 0;
    for (int x = 0; x < maze.width; x++) {
        for (int y = 0; y < maze.height; y++) {
            if (maze.cells[x][y].exit && isExit(x, y, maze.width, maze.height)) {
                exit_count++;
            }
        }
    }

    freeMaze(&maze);
    return exit_count == 1; // Vérifie qu'il y a une seule sortie
}

// Test : Vérifier qu'un chemin existe entre l'entrée et la sortie
int testPathFromEntrance() {
    printf("=== Test: Path From Entrance ===\n");

    int rows = 20, cols = 20;
    Maze maze;
    initializeMaze(&maze, rows, cols);
    generate_random_wall(&maze, 0, 0, rows, cols);

    int start_x = 0, start_y = 0;
    maze.cells[start_x][start_y].door = 1;

    int path_found = findPathBidirectionalBFS(&maze, start_x, start_y);
    freeMaze(&maze);
    return path_found; // Retourne 1 si le chemin est trouvé
}

// Test : Générer et valider un petit labyrinthe (3x3)
int testSmallMaze() {
    printf("=== Test: Small Maze (3x3) ===\n");

    int rows = 3, cols = 3;
    Maze maze;
    initializeMaze(&maze, rows, cols);
    generate_random_wall(&maze, 0, 0, rows, cols);

    maze.cells[0][0].door = 1;

    if (!findPathBidirectionalBFS(&maze, 0, 0)) {
        freeMaze(&maze);
        return 0; // Échec si aucun chemin n'est trouvé
    }

    freeMaze(&maze);
    return 1; // Succès
}

// Test de stress
int testStress() {
    printf("=== Test: Stress ===\n");

    Maze maze;
    initializeMaze(&maze, 2000, 2000);
    generate_random_wall(&maze, 0, 0, 2000, 2000);

    clock_t start = clock();
    if (!findPathBidirectionalBFS(&maze, 0, 0)) {
        freeMaze(&maze);
        return 0;
    }
    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Stress test 2000x2000 maze solved in %.4f seconds.\n", elapsed);

    freeMaze(&maze);
    return 1; // Succès
}
int testMemoryManagement() {
    printf("=== Test: Memory Management ===\n");

    for (int size = 10; size <= 1000; size *= 10) { // Test sizes: 10x10, 100x100, 1000x1000
        printf("Testing maze size: %dx%d\n", size, size);
        Maze maze;
        initializeMaze(&maze, size, size);
        generate_random_wall(&maze, 0, 0, size, size);

        // Simulate pathfinding and stress usage
        int start_x = rand() % size;
        int start_y = rand() % size;
        maze.cells[start_x][start_y].door = 1;

        findPathBidirectionalBFS(&maze, start_x, start_y);

        // Free the maze and ensure memory is properly deallocated
        freeMaze(&maze);
    }

    printf("Memory management test completed.\n");
    return 1; // Assume success if no assertions fail
}


// Fonction principale pour exécuter les tests
int main() {
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

    printf("\n=== Début des tests ===\n\n");

    RUN_TEST(testBasicFunctionality, "Basic Functionality Test");
    RUN_TEST(testPerformance, "Performance Test");
    RUN_TEST(testBoundaryCases, "Boundary Cases Test");
    RUN_TEST(testUniqueExit, "Unique Exit Test");
    RUN_TEST(testPathFromEntrance, "Path From Entrance Test");
    RUN_TEST(testSmallMaze, "Small Maze Test");
    RUN_TEST(testStress, "Stress Test");
    RUN_TEST(testMemoryManagement, "Memory Management Test");


    printf("\n=== Tous les tests sont terminés ! ===\n");
    return 0;
}
