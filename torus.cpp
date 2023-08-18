#include "parameters.h"
#include <cstdint>
#include <iostream>

/*
 * A 2 dimensional torus
 */
class torus_2D {
    public:
        uint8_t grid[TORUS_SIZE][TORUS_SIZE];

        torus_2D() {
            for (int i = 0; i < TORUS_SIZE; i++) {
                for (int j = 0; j < TORUS_SIZE; j++) {
                    this->grid[i][j] = 0;
                }
            }
        }

        void reset_torus() {
            for (int i = 0; i < TORUS_SIZE; i++) {
                for (int j = 0; j < TORUS_SIZE; j++) {
                    this->grid[i][j] = 0;
                }
            }
        }

        void reset_torus_with_mines(double m) {
            for (int i = 0; i < TORUS_SIZE; i++) {
                for (int j = 0; j < TORUS_SIZE; j++) {
                    this->grid[i][j] = 0;
                    if ((((double) rand()) / RAND_MAX ) < m) this->grid[i][j] = MINE;
                }
            }
        }

        void print_torus() {
            for (int x = 0; x < TORUS_SIZE; x++) {
                for (int y = 0; y < TORUS_SIZE; y++) {
                    std::cout << this->grid[x][y] << " "; 
                }
                std::cout << std::endl;
            }
        }
};

/*
 * A 3 dimensional torus
 */
class torus_3D {
    public:
        int size;
        uint8_t grid[TORUS_SIZE][TORUS_SIZE][TORUS_SIZE];

        torus_3D() {
            this->size = TORUS_SIZE;
            for (int i = 0; i < TORUS_SIZE; i++) {
                for (int j = 0; j < TORUS_SIZE; j++) {
                    for (int k = 0; k < TORUS_SIZE; k++) {
                        this->grid[i][j][k] = 0;
                    }
                }
            }
        }

        void reset_torus() {
            for (int i = 0; i < TORUS_SIZE; i++) {
                for (int j = 0; j < TORUS_SIZE; j++) {
                    for (int k = 0; k < TORUS_SIZE; k++) {
                        this->grid[i][j][k] = 0;
                    }
                }
            }
        }

        void print_torus() {
            for (int z = TORUS_SIZE - 1; z >= 0; z--) {
                for (int y = TORUS_SIZE - 1; y >= 0; y--) {
                    for (int x = 0; x < TORUS_SIZE; x++) {
                        std::cout << this->grid[x][y][z] << " "; 
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
};

