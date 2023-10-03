#include "torus.cpp"
#include <stdlib.h>
#include <time.h>

/*
 * Creates an agent in 2D with an ID, a random initial position,
 * a running total of area covered on an associated torus, 
 * a strategy, and some memory in case the strategy is viki. 
 */
class agent_2D {
    public:
        int x, y;
        uint8_t id = 1;
        unsigned long long area_covered = 0;
        int memory[VIKI_MEMORY];
        int strategy = RANDOM_WALK;
        torus_2D * t; 

        agent_2D(torus_2D * _t, int _strategy, uint8_t _id) {
            srand(time(NULL));
            this->x = rand() % TORUS_SIZE;
            this->y = rand() % TORUS_SIZE;
            this->t = _t;
            this->id = _id;
            this->area_covered = 0;
            this->strategy = _strategy; 
            this->t->grid[this->x][this->y] = this->id;
            this->memory[0] = RIGHT;
            this->memory[1] = 2;  
            for (int i = 2; i < VIKI_MEMORY; i++) {
                this->memory[i] = -1;
            }
        }

        void reset_agent() {
            this->x = rand() % TORUS_SIZE;
            this->y = rand() % TORUS_SIZE;
            this->area_covered = 0;
            this->memory[0] = RIGHT;
            this->memory[1] = 2;  
            for (int i = 2; i < VIKI_MEMORY; i++) {
                this->memory[i] = -1;
            }
        }

        void reset_agent_to_origin() {
            this->x = 0;
            this->y = 0;
            this->area_covered = 0;
            this->memory[0] = RIGHT;
            this->memory[1] = 2;  
            for (int i = 2; i < VIKI_MEMORY; i++) {
                this->memory[i] = -1;
            }
        }

        void reset_agent_to_distance_from_origin(int distance) { // distance should be odd, represents Manhattan distance from origin
            this->x = 1 + distance / 2;
            this->y = 0 + distance / 2;
            this->area_covered = 0;
            this->memory[0] = RIGHT;
            this->memory[1] = 2;  
            for (int i = 2; i < VIKI_MEMORY; i++) {
                this->memory[i] = -1;
            }
        }

        void update_torus() {
            if (this->t->grid[this->x][this->y] == 0) {
                this->t->grid[this->x][this->y] = this->id;
                this->area_covered++;
            }
        }

        void move() {
            int direction = 0;
            if (strategy == VIKI) direction = viki();
            if (strategy == VIKI_COLORBLIND) direction = viki_colorblind();
            if (strategy == RANDOM_WALK) direction = random_walk();
            if (strategy == GREEDY_BIASED) direction = greedy_biased();
            if (strategy == GREEDY_UNBIASED) direction = greedy_unbiased();
            if (strategy == RANDOM_WALK_NB) direction = random_walk_non_backtracking();
            if (peek(direction) == MINE) return;
            this->x = (this->x + dirx[direction] + TORUS_SIZE) % TORUS_SIZE;
            this->y = (this->y + diry[direction] + TORUS_SIZE) % TORUS_SIZE;
            update_torus();
        }

        uint8_t peek(int dir) {
            if (dir == RIGHT) return this->t->grid[(this->x + 1) % TORUS_SIZE][this->y];
            if (dir == UP) return this->t->grid[this->x][(this->y + 1) % TORUS_SIZE];
            if (dir == LEFT) return this->t->grid[(this->x - 1 + TORUS_SIZE) % TORUS_SIZE][this->y];
            if (dir == DOWN) return this->t->grid[this->x][(this->y - 1 + TORUS_SIZE) % TORUS_SIZE];
            return -1; 
        }

        int random_walk() {
            int directions[4];
            int count = 0;
            if (peek(RIGHT) != MINE) directions[count++] = RIGHT;
            if (peek(UP) != MINE) directions[count++] = UP;
            if (peek(LEFT) != MINE) directions[count++] = LEFT;
            if (peek(DOWN) != MINE) directions[count++] = DOWN;
            if (count == 0) return RIGHT;
            return directions[rand() % count]; 
        }

        int random_walk_non_backtracking() {
            int directions[4];
            int count = 0;
            int last_direction = this->memory[0];
            int non_mine_count = 0;
            for (const int &direction : {RIGHT, LEFT, DOWN, UP}) {
                if (peek(direction) != MINE) {
                    non_mine_count++;
                    if (direction != last_direction) {
                        directions[count] = direction;
                        count++;
                    }
                }
            }
            int direction = RIGHT;
            if (count > 0) {
                direction = directions[rand() % count];
                this->memory[0] = this->opposite(direction);
                return direction;
            } else if (count == 0 && non_mine_count == 1) {
                this->memory[0] = this->opposite(last_direction);
                return last_direction;
            } else {
                direction = this->random_walk();
                this->memory[0] = this->opposite(direction);
                return direction;
            }
        }

        int opposite(int direction) {
            if (direction == RIGHT) return LEFT;
            if (direction == LEFT) return RIGHT;
            if (direction == UP) return DOWN;
            if (direction == DOWN) return UP;
            if (direction == ZUP) return ZDOWN;
            if (direction == ZDOWN) return ZUP;
            return RIGHT;
        }

        int greedy_biased() {
            if (peek(RIGHT) == BLANK) return RIGHT; 
            if (peek(UP) == BLANK) return UP; 
            if (peek(LEFT) == BLANK) return LEFT; 
            if (peek(DOWN) == BLANK) return DOWN; 
            return random_walk(); 
        }

        int greedy_unbiased() {
            int directions[4];
            int count = 0;
            for (int direction = 0; direction < 5; direction++) {
                if (direction == 2) continue; 
                if (direction < 2) {
                    if (peek(direction) == BLANK) directions[count++] = direction; 
                } else {
                    if (peek(direction - 1) == BLANK) directions[count++] = direction - 1; 
                }
            }
            if (count == 0) return random_walk();
            return directions[rand() % count]; 
        }

        int viki_colorblind() {
            if (peek(RIGHT) != BLANK && peek(UP) != BLANK && peek(LEFT) != BLANK && peek(DOWN) != BLANK) return random_walk();
            int last_direction = this->memory[0];
            if (last_direction == RIGHT) {
                if (peek(UP) == BLANK) {
                    this->memory[0] = UP;
                    return UP;
                }
                if (peek(RIGHT) == BLANK) {
                    this->memory[0] = RIGHT;
                    return RIGHT; 
                }
                if (peek(DOWN) == BLANK) {
                    this->memory[0] = DOWN;
                    return DOWN; 
                }
            }
            if (last_direction == UP) {
                if (peek(LEFT) == BLANK) {
                    this->memory[0] = LEFT;
                    return LEFT; 
                }
                if (peek(UP) == BLANK) {
                    this->memory[0] = UP;
                    return UP;
                }
                if (peek(RIGHT) == BLANK) {
                    this->memory[0] = RIGHT;
                    return RIGHT; 
                }
            }
            if (last_direction == LEFT) {
                if (peek(DOWN) == BLANK) {
                    this->memory[0] = DOWN;
                    return DOWN; 
                }
                if (peek(LEFT) == BLANK) {
                    this->memory[0] = LEFT;
                    return LEFT; 
                }
                if (peek(UP) == BLANK) {
                    this->memory[0] = UP;
                    return UP;
                }
            }
            if (last_direction == DOWN) {
                if (peek(RIGHT) == BLANK) {
                    this->memory[0] = RIGHT;
                    return RIGHT;
                }
                if (peek(DOWN) == BLANK) {
                    this->memory[0] = DOWN;
                    return DOWN; 
                }
                if (peek(LEFT) == BLANK) {
                    this->memory[0] = LEFT;
                    return LEFT; 
                }
            }
            return random_walk();
        }

        // VIKI HELPER METHODS

        int get_encoding(int x, int y) {
            if (x < 0) x += TORUS_SIZE;
            if (x >= TORUS_SIZE) x -= TORUS_SIZE;
            if (y < 0) x += TORUS_SIZE;
            if (y >= TORUS_SIZE) x -= TORUS_SIZE;            
            return x * TORUS_SIZE + y;
        }

        int get_x(int encoding) {
            return encoding / TORUS_SIZE;
        }

        int get_y(int encoding) {
            return encoding % TORUS_SIZE;
        }

        bool is_in_memory(int encoding) {
            for (int i = 2; i < this->memory[1] && i < VIKI_MEMORY; i++) {
                if (this->memory[i] == encoding) return true;
            }
            return false;
        }

        int viki() {
            int last_direction = this->memory[0];
            bool right_open = !is_in_memory(get_encoding(this->x + 1, this->y)) && peek(RIGHT) != this->id && peek(RIGHT) != MINE;
            bool left_open = !is_in_memory(get_encoding(this->x - 1, this->y)) && peek(LEFT) != this->id && peek(LEFT) != MINE;
            bool up_open = !is_in_memory(get_encoding(this->x, this->y + 1)) && peek(UP) != this->id && peek(UP) != MINE;
            bool down_open = !is_in_memory(get_encoding(this->x, this->y - 1)) && peek(DOWN) != this->id && peek(DOWN) != MINE;
            if (!right_open && !left_open && !up_open && !down_open) return random_walk();
            if (last_direction == RIGHT) {
                if (up_open) return UP;
                if (right_open) return RIGHT;
                if (down_open) return DOWN;
                return LEFT;
            }
            if (last_direction == UP) {
                if (left_open) return LEFT;
                if (up_open) return UP;
                if (right_open) return RIGHT;
                return DOWN;
            }
            if (last_direction == LEFT) {
                if (down_open) return DOWN;
                if (left_open) return LEFT;
                if (up_open) return UP;
                return RIGHT;
            }
            if (last_direction == DOWN) {
                if (right_open) return RIGHT;
                if (down_open) return DOWN;
                if (left_open) return LEFT;
                return UP;
            }
            return random_walk();
        }
};

/*
 * Creates an agent in 3D with an ID, a random initial position,
 * a running total of area covered on an associated torus, 
 * a strategy, and some memory in case the strategy is viki. 
 */
class agent_3D {
    public:
        int x, y, z;
        uint8_t id = 1;
        unsigned long long area_covered = 0;
        int memory[MEMORY];
        int strategy = RANDOM_WALK;
        torus_3D * t; 

        agent_3D(torus_3D * _t, int _strategy, uint8_t _id) {
            srand(time(NULL));
            this->x = rand() % TORUS_SIZE;
            this->y = rand() % TORUS_SIZE;
            this->z = rand() % TORUS_SIZE;
            this->t = _t;
            this->id = _id;
            this->area_covered = 0;
            this->strategy = _strategy; 
            this->t->grid[this->x][this->y][this->z] = this->id;
            for (int i = 0; i < MEMORY; i++) {
                this->memory[i] = 0;
            }
        }

        void reset_agent() {
            this->x = rand() % TORUS_SIZE;
            this->y = rand() % TORUS_SIZE;
            this->area_covered = 0;
            for (int i = 0; i < MEMORY; i++) {
                this->memory[i] = 0;
            }
        }

        void update_torus() {
            if (this->t->grid[this->x][this->y][this->z] == BLANK) {
                this->t->grid[this->x][this->y][this->z] = this->id;
                this->area_covered++;
            }
        }

        void move(int iteration) {
            int direction = 0;
            if (iteration % 300 < 100) this->strategy = GREEDY_BIASED_XY;
            else if (iteration % 300 < 200) this->strategy = GREEDY_BIASED_YZ;
            else if (iteration % 300 < 300) this->strategy = GREEDY_BIASED_ZX;
            if (this->strategy == GREEDY_BIASED_XY) direction = greedy_biased_xy();
            if (this->strategy == GREEDY_BIASED_YZ) direction = greedy_biased_yz();
            if (this->strategy == GREEDY_BIASED_ZX) direction = greedy_biased_zx();
            int new_x = (this->x + dirx[direction] + TORUS_SIZE) % TORUS_SIZE;
            int new_y = (this->y + diry[direction] + TORUS_SIZE) % TORUS_SIZE;
            int new_z = (this->z + dirz[direction] + TORUS_SIZE) % TORUS_SIZE;
            if (this->t->grid[new_x][new_y][new_z] == MINE) {
                return;
            }
            this->x = new_x;
            this->y = new_y;
            this->z = new_z; 
            update_torus();
        }

        void move() {
            int direction = 0;
            if (strategy == VIKI) direction = viki();
            if (strategy == RANDOM_WALK) direction = random_walk();
            if (strategy == GREEDY_BIASED) direction = greedy_biased();
            if (strategy == GREEDY_UNBIASED) direction = greedy_unbiased();
            if (strategy == GREEDY_BIASED_XY) direction = greedy_biased_xy();
            if (strategy == GREEDY_BIASED_YZ) direction = greedy_biased_yz();
            if (strategy == GREEDY_BIASED_ZX) direction = greedy_biased_zx();
            if (strategy == RANDOM_WALK_NB) direction = random_walk_non_backtracking();
            int new_x = (this->x + dirx[direction] + TORUS_SIZE) % TORUS_SIZE;
            int new_y = (this->y + diry[direction] + TORUS_SIZE) % TORUS_SIZE;
            int new_z = (this->z + dirz[direction] + TORUS_SIZE) % TORUS_SIZE;
            if (this->t->grid[new_x][new_y][new_z] == MINE) {
                return;
            }
            this->x = new_x;
            this->y = new_y;
            this->z = new_z; 
            update_torus();
        }

        uint8_t peek(int dir) {
            if (dir == RIGHT) return this->t->grid[(this->x + 1) % TORUS_SIZE][this->y][this->z];
            if (dir == UP) return this->t->grid[this->x][(this->y + 1) % TORUS_SIZE][this->z];
            if (dir == ZUP) return this->t->grid[this->x][this->y][(this->z + 1) % TORUS_SIZE];
            if (dir == LEFT) return this->t->grid[(this->x - 1 + TORUS_SIZE) % TORUS_SIZE][this->y][this->z];
            if (dir == DOWN) return this->t->grid[this->x][(this->y - 1 + TORUS_SIZE) % TORUS_SIZE][this->z];
            if (dir == ZDOWN) return this->t->grid[this->x][this->y][(this->z - 1 + TORUS_SIZE) % TORUS_SIZE];
            return -1; 
        }

        int random_walk() {
            int dir = rand() % 6;
            return dir;  
        }

        int random_walk_non_backtracking() {
            int directions[6];
            int count = 0;
            int last_direction = this->memory[0];
            if (peek(RIGHT) != MINE && peek(RIGHT) != last_direction) directions[count++] = RIGHT;
            if (peek(UP) != MINE && peek(UP) != last_direction) directions[count++] = UP;
            if (peek(LEFT) != MINE && peek(LEFT) != last_direction) directions[count++] = LEFT;
            if (peek(DOWN) != MINE && peek(DOWN) != last_direction) directions[count++] = DOWN;
            if (peek(ZUP) != MINE && peek(ZUP) != last_direction) directions[count++] = ZUP;
            if (peek(ZDOWN) != MINE && peek(ZDOWN) != last_direction) directions[count++] = ZDOWN;
            int direction = RIGHT;
            if (count > 0) {
                direction = directions[rand() % count];
            }
            this->memory[0] = direction;
            return direction;
        }

        int greedy_biased() {
            if (peek(RIGHT) == BLANK) return RIGHT; 
            if (peek(UP) == BLANK) return UP; 
            if (peek(ZUP) == BLANK) return ZUP; 
            if (peek(LEFT) == BLANK) return LEFT; 
            if (peek(DOWN) == BLANK) return DOWN; 
            if (peek(ZDOWN) == BLANK) return ZDOWN; 
            return random_walk(); 
        }

        int greedy_biased_xy() {
            if (peek(RIGHT) == BLANK) return RIGHT;
            if (peek(UP) == BLANK) return UP;
            if (peek(LEFT) == BLANK) return LEFT;
            if (peek(DOWN) == BLANK) return DOWN; 
            if (peek(ZUP) == BLANK) return ZUP;
            if (peek(ZDOWN) == BLANK) return ZDOWN;
            return random_walk();
        }

        int greedy_biased_yz() {
            if (peek(UP) == BLANK) return UP;
            if (peek(ZUP) == BLANK) return ZUP;
            if (peek(DOWN) == BLANK) return DOWN; 
            if (peek(ZDOWN) == BLANK) return ZDOWN;
            if (peek(RIGHT) == BLANK) return RIGHT;
            if (peek(LEFT) == BLANK) return LEFT;  
            return random_walk();
        }

        int greedy_biased_zx() {
            
            if (peek(ZUP) == BLANK) return ZUP;
            if (peek(RIGHT) == BLANK) return RIGHT;
            if (peek(ZDOWN) == BLANK) return ZDOWN;
            if (peek(LEFT) == BLANK) return LEFT;
            if (peek(UP) == BLANK) return UP;
            if (peek(DOWN) == BLANK) return DOWN; 
            return random_walk();
        }

        int greedy_unbiased() {
            int directions[6];
            int count = 0;
            for (int direction = 0; direction < 6; direction++) {
                if (peek(direction) == BLANK) directions[count++] = direction; 
            }
            if (count == 0) return random_walk();
            return directions[rand() % count]; 
        }

        // VIKI METHODS

        int get_encoding(int x, int y, int z) {
            return x * TORUS_SIZE * TORUS_SIZE + y * TORUS_SIZE + z;
        }

        int get_x(int encoding) {
            return encoding / (TORUS_SIZE * TORUS_SIZE);
        }

        int get_y(int encoding) {
            encoding -= get_x(encoding) * TORUS_SIZE * TORUS_SIZE;
            return encoding / TORUS_SIZE;
        }

        int get_z(int encoding) {
            encoding -= get_x(encoding) * TORUS_SIZE * TORUS_SIZE;
            return encoding % TORUS_SIZE;
        }

        void assign_viki_memory(int zero, int one, int two, int three, int four, int five, int six) {
            if (zero != -1) this->memory[0] = zero;
            if (one != -1) this->memory[1] = one;
            if (two != -1) this->memory[2] = two;
            if (three != -1) this->memory[3] = three;
            if (four != -1) this->memory[4] = four;
            if (five != -1) this->memory[5] = five;
            if (six != -1) this->memory[6] = six;
        }

        int corner_case_helper(bool x_dist, bool y_dist, bool z_dist) {
            int start_x = get_x(this->memory[2]);
            int start_y = get_y(this->memory[2]);
            int start_z = get_z(this->memory[2]);
            int x = x_dist ? ((start_x + this->memory[3]) % TORUS_SIZE) : ((start_x + this->memory[5]) % TORUS_SIZE);
            int y = y_dist ? ((start_y + this->memory[3]) % TORUS_SIZE) : ((start_y + this->memory[5]) % TORUS_SIZE);
            int z = z_dist ? ((start_z + this->memory[3]) % TORUS_SIZE) : ((start_z + this->memory[5]) % TORUS_SIZE);
            return get_encoding(x, y, z);
        }

        int get_initial_spiral_out_pattern() {
            int res = 0;
            if (this->memory[6] == 0) res = UP;
            if (this->memory[6] == 1) res = RIGHT;
            if (this->memory[6] == 2) res = ZUP;
            if (this->memory[6] == 3) res = LEFT;
            if (this->memory[6] == 4) res = DOWN;
            if (this->memory[6] == 5) res = RIGHT;
            if (this->memory[6] == 6) res = ZDOWN;
            if (this->memory[6] == 7) res = DOWN;
            this->memory[6]++;
            return res;
        }

        int viki() {
            int cur = get_encoding(this->x, this->y, this->z);
            if (this->memory[0] == 0) {
                assign_viki_memory(1, -1, cur, 1, -1, -1, -1);
                return RIGHT; 
            }
            int start_x = get_x(this->memory[2]);
            int start_y = get_y(this->memory[2]);
            int start_z = get_z(this->memory[2]);
            if (this->memory[1] == 0) { // AGENT IS SPIRALING IN
                if (this->memory[4] == 0) {
                    if (cur == corner_case_helper(true, false, true)) {
                        this->memory[4] = 1;
                        return LEFT; 
                    } else return ZUP;
                }
                if (this->memory[4] == 1) {
                    if (cur == corner_case_helper(false, false, true)) {
                        this->memory[4] = 2;
                        return UP;
                    } else return LEFT;
                }
                if (this->memory[4] == 2) {
                    if (cur == corner_case_helper(false, true, true)) {
                        this->memory[4] = 3;
                        return ZDOWN;
                    } else return UP;
                }
                if (this->memory[4] == 3) {
                    if (cur == corner_case_helper(false, true, false)) {
                        this->memory[4] = 4;
                        return RIGHT;
                    } else return ZDOWN; 
                }
                if (this->memory[4] == 4) {
                    if (cur == corner_case_helper(true, true, false)) {
                        this->memory[4] = 5;
                        if (this->memory[3] - this->memory[5] != 1) return DOWN;
                    } else return RIGHT;
                }
                if (this->memory[4] == 5) {
                    if (cur == get_encoding((start_x + this->memory[3]) % TORUS_SIZE, (start_y + this->memory[5] + 1) % TORUS_SIZE, (start_z + this->memory[5]) % TORUS_SIZE)) {
                        if (cur == get_encoding((start_x + this->memory[3]) % TORUS_SIZE, (start_y + this->memory[3]) % TORUS_SIZE, (start_z + this->memory[3] - 1) % TORUS_SIZE)) {
                            int new_start_encoding = get_encoding((start_x + this->memory[3] + 1) % TORUS_SIZE, (start_y + this->memory[3] + 1) % TORUS_SIZE, (start_z + this->memory[3] + 1) % TORUS_SIZE);
                            assign_viki_memory(-1, 1, new_start_encoding, this->memory[3] + 1, 0, 2, 0);
                        } else {
                            assign_viki_memory(-1, -1, -1, -1, 0, this->memory[5] + 1, -1);
                        }
                        return ZUP;
                    } else return DOWN; 
                }
            } else { // AGENT IS SPIRALING OUT
                if (this->memory[6] < 8) {
                    return get_initial_spiral_out_pattern();
                }
                cur = get_encoding(this->x, this->y, this->z);
                start_x = get_x(this->memory[2]);
                start_y = get_y(this->memory[2]);
                start_z = get_z(this->memory[2]);
                if (this->memory[4] == 0) {
                    if (cur == get_encoding(start_x, (start_y - this->memory[5] + TORUS_SIZE) % TORUS_SIZE, start_z)) {
                        this->memory[4] = 1;
                        return LEFT;
                    } else return ZUP;
                }
                if (this->memory[4] == 1) {
                    if (cur == get_encoding((start_x - this->memory[5] + TORUS_SIZE) % TORUS_SIZE, (start_y - this->memory[5] + TORUS_SIZE) % TORUS_SIZE, start_z)) {
                        this->memory[4] = 2;
                        return UP;
                    } else return LEFT;
                }
                if (this->memory[4] == 2) {
                    if (cur == get_encoding((start_x - this->memory[5] + TORUS_SIZE) % TORUS_SIZE, start_y, start_z)) {
                        this->memory[4] = 3;
                        return ZDOWN;
                    } else return UP;
                }
                if (this->memory[4] == 3) {
                    if (cur == get_encoding((start_x - this->memory[5] + TORUS_SIZE) % TORUS_SIZE, start_y, (start_z - this->memory[5] + TORUS_SIZE) % TORUS_SIZE)) {
                        this->memory[4] = 4;
                        return RIGHT;
                    } else return ZDOWN;
                }
                if (this->memory[4] == 4) {
                    if (cur == get_encoding(start_x, start_y, (start_z - this->memory[5] + TORUS_SIZE) % TORUS_SIZE)) {
                        this->memory[4] = 5;
                        if (this->memory[3] - this->memory[5] != 1) return DOWN;
                    } else return RIGHT;
                }
                if (this->memory[4] == 5) {
                    if (cur == get_encoding(start_x, (start_y - this->memory[5] + TORUS_SIZE) % TORUS_SIZE, (start_z - this->memory[5] + TORUS_SIZE) % TORUS_SIZE)) {
                        if (this->memory[5] == this->memory[3]) {
                            int new_start_encoding = get_encoding((this->x - this->memory[3] + TORUS_SIZE) % TORUS_SIZE, this->y, this->z);
                            assign_viki_memory(-1, 0, new_start_encoding, this->memory[3] + 1, 0, 0, 0);
                            return RIGHT;
                        } else {
                            assign_viki_memory(-1, -1, -1, -1, 0, this->memory[5] + 1, -1);
                            return DOWN; 
                        }
                    } else return DOWN;
                }

            }
            return 0;
        }
};

