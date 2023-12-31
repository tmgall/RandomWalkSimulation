#include "agent.cpp"
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <math.h>

/*
 * Simulates a competition between two agents in 1D.
 * 
 * The output file will hold the total areas covered at the specified
 * u values by each agent.  
 */
class simulation_1D_1v1 {
    public:
        agent_1D * agent1 = NULL;
        agent_1D * agent2 = NULL;
        int sample_size = 1;
        std::ofstream torus_file;
        std::ofstream interface_file;
        unsigned long long interface_size;

        simulation_1D_1v1(agent_1D * agent1, agent_1D * agent2, int sample_size, const char * torus_file_name, const char * interface_file_name) {
            this->agent1 = agent1;
            this->agent2 = agent2;
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            srand(time(NULL));
            torus_file.open(torus_file_name);
            interface_file.open(interface_file_name);
            this->interface_size = 0;
        }

        void simulate_sample_size() {
            for (int i = 0; i < sample_size; i++) {
                simulate();
            }
            torus_file.close();
            interface_file.close();
        }

        void simulate() {
            agent1->t->reset_torus();
            this->agent1->reset_agent();
            this->agent2->reset_agent();
            for (long long i = 0; i < TORUS_SIZE * TORUS_SIZE; i++) {
                if (rand() % 2 == 0) {
                    this->agent1->move();
                    this->agent2->move();
                } else {
                    this->agent2->move();
                    this->agent1->move();
                }
            }
            for (int i = 0; i < TORUS_SIZE; i++) {
                if (this->agent1->t->grid[i] != (this->agent1->t->grid[(i + 1) % TORUS_SIZE])) {
                    this->interface_size++;
                    torus_file << i << " ";
                }
            }
            torus_file << "\n";
            interface_file << this->interface_size << "\n";
            this->interface_size = 0;
        }
};

/* 
 * Simulates one agent in 2 dimensions. 
 */ 
class simulation_2D {
    public:
        agent_2D * agent1 = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        std::ofstream output_file; 
        std::ofstream output_file_sums; 
        unsigned long long area_total[U_LIST_LEN];

        simulation_2D(agent_2D * agent1, int sample_size, const char * file, const char * file_sum) {
            this->agent1 = agent1;
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            for (int i = 0; i < U_LIST_LEN; i++) {
                scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * log(TORUS_SIZE));
                area_total[i] = 0;
            }
            srand(time(NULL));
            output_file.open(file);
            output_file_sums.open(file_sum);
        }

        void simulate_sample_size() {
            output_file << "[";
            for (int i = 0; i < sample_size; i++) {
                simulate();
                if (i != sample_size - 1) output_file << ", ";
            }
            output_file << "]";
            output_file.close();

            output_file_sums << "[";
            for (int i = 0; i < U_LIST_LEN; i++) {
                output_file_sums << area_total[i];
                if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
            }
            output_file_sums << "]";
            output_file_sums.close();
        }

        void simulate() {
            agent1->t->reset_torus();
            this->agent1->reset_agent();
            int current_u_list_position = 0;
            output_file << "[";
            for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
                this->agent1->move();
                if (i + 1 == scaled_u_list[current_u_list_position]) {
                    area_total[current_u_list_position] += this->agent1->area_covered;
                    output_file << this->agent1->area_covered; 
                    if (i + 1 != scaled_u_list[U_LIST_LEN - 1]) output_file << ", ";
                    current_u_list_position++;
                }
            }
            output_file << "]";
        }
};

/* 
 * Simulates three agents collaborating in 2 dimensions. 
 */ 
class simulation_3_collab_2D {
    public:
        agent_2D * agent1 = NULL;
        agent_2D * agent2 = NULL;
        agent_2D * agent3 = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        std::ofstream output_file; 
        std::ofstream output_file_sums; 
        unsigned long long area_total[U_LIST_LEN];

        simulation_3_collab_2D(agent_2D * agent1, agent_2D * agent2, agent_2D * agent3, int sample_size, const char * file, const char * file_sum) {
            this->agent1 = agent1;
            this->agent2 = agent2;
            this->agent3 = agent3; 
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            for (int i = 0; i < U_LIST_LEN; i++) {
                scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * log(TORUS_SIZE));
                area_total[i] = 0;
            }
            srand(time(NULL));
            output_file.open(file);
            output_file_sums.open(file_sum);
        }

        void simulate_sample_size() {
            output_file << "[";
            for (int i = 0; i < sample_size; i++) {
                simulate();
                if (i != sample_size - 1) output_file << ", ";
            }
            output_file << "]";
            output_file.close();

            output_file_sums << "[";
            for (int i = 0; i < U_LIST_LEN; i++) {
                output_file_sums << area_total[i];
                if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
            }
            output_file_sums << "]";
            output_file_sums.close();
        }

        void simulate() {
            this->agent1->t->reset_torus();
            this->agent1->reset_agent();
            this->agent2->reset_agent();
            this->agent3->reset_agent();
            int current_u_list_position = 0;
            output_file << "[";
            for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
                this->agent1->move();
                this->agent2->move();
                this->agent3->move();
                if (i + 1 == scaled_u_list[current_u_list_position]) {
                    area_total[current_u_list_position] += this->agent1->area_covered + this->agent2->area_covered + this->agent3->area_covered;
                    output_file << this->agent1->area_covered + this->agent2->area_covered + this->agent3->area_covered; 
                    if (i + 1 != scaled_u_list[U_LIST_LEN - 1]) output_file << ", ";
                    current_u_list_position++;
                }
            }
            output_file << "]";
        }
};

/*
 * Simulates a competition between two agents in 2D.
 * 
 * The output file will hold the total areas covered at the specified
 * u values by each agent.  
 */
class simulation_2D_1v1 {
    public:
        agent_2D * agent1 = NULL;
        agent_2D * agent2 = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        std::ofstream output_file; 
        std::ofstream output_file_sums; 
        unsigned long long team1_area_total[U_LIST_LEN];
        unsigned long long team2_area_total[U_LIST_LEN];

        simulation_2D_1v1(agent_2D * agent1, agent_2D * agent2, int sample_size, const char * file, const char * file_sum) {
            this->agent1 = agent1;
            this->agent2 = agent2;
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            for (int i = 0; i < U_LIST_LEN; i++) {
                scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * log(TORUS_SIZE));
                team1_area_total[i] = 0;
                team2_area_total[i] = 0;
            }
            srand(time(NULL));
            output_file.open(file);
            output_file_sums.open(file_sum);
        }

        void simulate_sample_size() {
            output_file << "[";
            for (int i = 0; i < sample_size; i++) {
                simulate();
                if (i != sample_size - 1) output_file << ", ";
            }
            output_file << "]";
            output_file.close();

            output_file_sums << "[";
            for (int i = 0; i < U_LIST_LEN; i++) {
                output_file_sums << "[" << team1_area_total[i] << ", " << team2_area_total[i] << "]";
                if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
            }
            output_file_sums << "]";
            output_file_sums.close();
        }

        void simulate() {
            agent1->t->reset_torus();
            this->agent1->reset_agent();
            this->agent2->reset_agent();
            int current_u_list_position = 0;
            output_file << "[";
            for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
                if (rand() % 2 == 0) {
                    this->agent1->move();
                    this->agent2->move();
                } else {
                    this->agent2->move();
                    this->agent1->move();
                }
                if (i + 1 == scaled_u_list[current_u_list_position]) {
                    unsigned long long team1_area_covered = this->agent1->area_covered;
                    unsigned long long team2_area_covered = this->agent2->area_covered;
                    team1_area_total[current_u_list_position] += team1_area_covered;
                    team2_area_total[current_u_list_position] += team2_area_covered; 
                    output_file << "[" << this->agent1->area_covered << ", "; 
                    output_file << this->agent2->area_covered << "]";
                    if (i + 1 != this->scaled_u_list[U_LIST_LEN - 1]) output_file << ", ";
                    current_u_list_position++;
                }
            }
            output_file << "]";
        }
};

/*
 * Simulates a competition between two agents in 2D and calculates their interface.
 * 
 * The output file will hold the total areas covered at the specified
 * u values by each agent.  
 */
class simulation_2D_1v1_interface {
    public:
        agent_2D * agent1 = NULL;
        agent_2D * agent2 = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        std::ofstream output_file; 
        std::ofstream output_file_sums; 
        unsigned long long team1_area_total[U_LIST_LEN];
        unsigned long long team2_area_total[U_LIST_LEN];
        unsigned long long total_interface;
        int distance;

        simulation_2D_1v1_interface(agent_2D * agent1, agent_2D * agent2, int sample_size, const char * file, const char * file_sum, int distance) {
            this->agent1 = agent1;
            this->agent2 = agent2;
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            for (int i = 0; i < U_LIST_LEN; i++) {
                scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * log(TORUS_SIZE));
                team1_area_total[i] = 0;
                team2_area_total[i] = 0;
            }
            srand(time(NULL));
            output_file.open(file);
            output_file_sums.open(file_sum);
            this->total_interface = 0;
            this->distance = distance;
        }

        void simulate_sample_size() {
            output_file << "[";
            for (int i = 0; i < sample_size; i++) {
                simulate();
                if (i != sample_size - 1) output_file << ", ";
            }
            output_file << "]";
            output_file.close();

            output_file_sums << "[";
            for (int i = 0; i < U_LIST_LEN; i++) {
                output_file_sums << "[" << team1_area_total[i] << ", " << team2_area_total[i] << "]";
                if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
            }
            output_file_sums << "(total interface: " << this->total_interface << ")";
            output_file_sums << "]";
            output_file_sums.close();
        }

        void simulate() {
            agent1->t->reset_torus();
            this->agent1->reset_agent_to_origin();
            this->agent2->reset_agent_to_distance_from_origin(this->distance);
            int current_u_list_position = 0;
            output_file << "[";
            for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
                if (rand() % 2 == 0) {
                    this->agent1->move();
                    this->agent2->move();
                } else {
                    this->agent2->move();
                    this->agent1->move();
                }
                if (i + 1 == scaled_u_list[current_u_list_position]) {
                    unsigned long long team1_area_covered = this->agent1->area_covered;
                    unsigned long long team2_area_covered = this->agent2->area_covered;
                    team1_area_total[current_u_list_position] += team1_area_covered;
                    team2_area_total[current_u_list_position] += team2_area_covered; 
                    output_file << "[" << this->agent1->area_covered << ", "; 
                    output_file << this->agent2->area_covered << "]";
                    if (i + 1 != this->scaled_u_list[U_LIST_LEN - 1]) output_file << ", ";
                    current_u_list_position++;
                }
            }
            long interface = this->calculate_interface();
            this->total_interface += interface;
            output_file << "(interface: " << interface << ")";
            output_file << "]";
        }

        long calculate_interface() {
            long interface = 0;
            for (int i = 0; i < TORUS_SIZE; i++) {
                for (int j = 0; j < TORUS_SIZE; j++) {
                    if (this->agent1->t->grid[i][j] == 1) { // torus[i, j] claimed by agent 1
                        if (this->agent1->t->grid[(i + 1) % TORUS_SIZE][j] == 2) {
                            interface++;
                        }
                        if (this->agent1->t->grid[i][(j + 1) % TORUS_SIZE] == 2) {
                            interface++;
                        }
                    }
                    if (this->agent1->t->grid[i][j] == 2) { // torus[i, j] claimed by agent 1
                        if (this->agent1->t->grid[(i + 1) % TORUS_SIZE][j] == 1) {
                            interface++;
                        }
                        if (this->agent1->t->grid[i][(j + 1) % TORUS_SIZE] == 1) {
                            interface++;
                        }
                    }
                }
            }
            return interface;
        }
};

/*
 * Simulates a competition between two teams of agents in 2D.
 * 
 * The first team is a solo agent.
 * The second team is 3 different agents.
 * 
 * The solo agent will move three times for every one move of the second team. 
 * 
 * The output file will hold the total areas covered at the specified
 * u values by each agent.  
 */
class simulation_2D_1v3 {
    public:
        agent_2D * solo_agent = NULL;
        agent_2D * agent1 = NULL;
        agent_2D * agent2 = NULL;
        agent_2D * agent3 = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        std::ofstream output_file; 
        std::ofstream output_file_sums; 
        unsigned long long team1_area_total[U_LIST_LEN];
        unsigned long long team2_area_total[U_LIST_LEN];

        simulation_2D_1v3(agent_2D * solo_agent, agent_2D * agent1, agent_2D * agent2, agent_2D * agent3, int sample_size, const char * file, const char * file_sum) {
            this->solo_agent = solo_agent;
            this->agent1 = agent1;
            this->agent2 = agent2;
            this->agent3 = agent3;
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            for (int i = 0; i < U_LIST_LEN; i++) {
                scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * log(TORUS_SIZE));
                team1_area_total[i] = 0;
                team2_area_total[i] = 0;
            }
            srand(time(NULL));
            output_file.open(file);
            output_file_sums.open(file_sum);
        }

        void simulate_sample_size() {
            output_file << "[";
            for (int i = 0; i < sample_size; i++) {
                simulate();
                if (i != sample_size - 1) output_file << ", ";
            }
            output_file << "]";
            output_file.close();

            output_file_sums << "[";
            for (int i = 0; i < U_LIST_LEN; i++) {
                output_file_sums << "[" << team1_area_total[i] << ", " << team2_area_total[i] << "]";
                if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
            }
            output_file_sums << "]";
            output_file_sums.close();
        }

        void simulate() {
            agent1->t->reset_torus();
            this->solo_agent->reset_agent();
            this->agent1->reset_agent();
            this->agent2->reset_agent();
            this->agent3->reset_agent();
            int current_u_list_position = 0;
            output_file << "[";
            for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
                if (rand() % 2 == 0) {
                    this->solo_agent->move();
                    this->solo_agent->move();
                    this->solo_agent->move();
                    this->agent1->move();
                    this->agent2->move();
                    this->agent3->move();
                } else {
                    this->agent1->move();
                    this->agent2->move();
                    this->agent3->move();
                    this->solo_agent->move();
                    this->solo_agent->move();
                    this->solo_agent->move();
                }
                if (i + 1 == scaled_u_list[current_u_list_position]) {
                    unsigned long long team1_area_covered = this->solo_agent->area_covered;
                    unsigned long long team2_area_covered = this->agent1->area_covered + this->agent2->area_covered + this->agent3->area_covered;
                    team1_area_total[current_u_list_position] += team1_area_covered;
                    team2_area_total[current_u_list_position] += team2_area_covered; 
                    output_file << "[" << team1_area_covered << ", "; 
                    output_file << team2_area_covered << "]";
                    if (i + 1 != scaled_u_list[U_LIST_LEN - 1]) output_file << ", ";
                    current_u_list_position++;
                }
            }
            output_file << "]";
        }
};

/* 
 * Simulates a walk of a single agent in 2D on a torus with mines.
 * 
 * The output file will hold the total areas covered at the specified
 * u values by the agent.
 */
class simulation_2D_solo_mines {
    public:
        agent_2D * agent = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        double mine_chance = 0.01; 
        std::ofstream output_file; 
        std::ofstream output_file_sums; 
        unsigned long long area_total[U_LIST_LEN];
        
        simulation_2D_solo_mines(agent_2D * agent, double mine_chance, int sample_size, const char * file, const char * file_sum) {
        this->agent = agent;
        this->mine_chance = mine_chance;
        this->sample_size = sample_size;
        double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
        for (int i = 0; i < U_LIST_LEN; i++) {
            scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * log(TORUS_SIZE));
            this->area_total[i] = 0;
        }
        srand(time(NULL));
        output_file.open(file);
        output_file_sums.open(file_sum);
    }

    void simulate_sample_size() {
        output_file << "[";
        for (int i = 0; i < sample_size; i++) {
            simulate();
            if (i != sample_size - 1) output_file << ", ";
        }
        output_file << "]";
        output_file.close();

        output_file_sums << "[";
        for (int i = 0; i < U_LIST_LEN; i++) {
            output_file_sums << area_total[i];
            if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
        }
        output_file_sums << "]";
        output_file_sums.close();
    }

    void simulate() {
        agent->t->reset_torus_with_mines(this->mine_chance);
        this->agent->reset_agent();
        int current_u_list_position = 0;
        output_file << "[";
        for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
            this->agent->move();
            if (i + 1 == scaled_u_list[current_u_list_position]) {
                this->area_total[current_u_list_position] += this->agent->area_covered;
                output_file << this->agent->area_covered; 
                if (i != scaled_u_list[U_LIST_LEN - 1] - 1) output_file << ", ";
                current_u_list_position++;
            }
        }
        output_file << "]";
    }
};

/* 
 * Simulates a competition between two agents in 2D on a torus with mines.
 * 
 * The output file will hold the total areas covered at the specified
 * u values by each agent.
 */
class simulation_2D_1v1_mines {
    public:
        agent_2D * agent1 = NULL;
        agent_2D * agent2 = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        double mine_chance = 0.01; 
        std::ofstream output_file; 
        std::ofstream output_file_sums; 
        unsigned long long team1_area_total[U_LIST_LEN];
        unsigned long long team2_area_total[U_LIST_LEN];

    simulation_2D_1v1_mines(agent_2D * agent1, agent_2D * agent2, double mine_chance, int sample_size, const char * file, const char * file_sum) {
        this->agent1 = agent1;
        this->agent2 = agent2;
        this->mine_chance = mine_chance;
        this->sample_size = sample_size;
        double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
        for (int i = 0; i < U_LIST_LEN; i++) {
            scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * log(TORUS_SIZE));
            team1_area_total[i] = 0;
            team2_area_total[i] = 0;
        }
        srand(time(NULL));
        output_file.open(file);
        output_file_sums.open(file_sum);
    }

    void simulate_sample_size() {
        output_file << "[";
        for (int i = 0; i < sample_size; i++) {
            simulate();
            if (i != sample_size - 1) output_file << ", ";
        }
        output_file << "]";
        output_file.close();

        output_file_sums << "[";
        for (int i = 0; i < U_LIST_LEN; i++) {
            output_file_sums << "[" << team1_area_total[i] << ", " << team2_area_total[i] << "]";
            if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
        }
        output_file_sums << "]";
        output_file_sums.close();
    }

    void simulate() {
        agent1->t->reset_torus_with_mines(this->mine_chance);
        this->agent1->reset_agent();
        this->agent2->reset_agent();
        int current_u_list_position = 0;
        output_file << "[";
        for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
            if (rand() % 2 == 0) {
                this->agent1->move();
                this->agent2->move();
            } else {
                this->agent2->move();
                this->agent1->move();
            }
            if (i + 1 == scaled_u_list[current_u_list_position]) {
                unsigned long long team1_area_covered = this->agent1->area_covered;
                unsigned long long team2_area_covered = this->agent2->area_covered;
                team1_area_total[current_u_list_position] += team1_area_covered;
                team2_area_total[current_u_list_position] += team2_area_covered; 
                output_file << "[" << this->agent1->area_covered << ", "; 
                output_file << this->agent2->area_covered << "]";
                if (i + 1 != scaled_u_list[U_LIST_LEN - 1]) output_file << ", ";
                current_u_list_position++;
            }
        }
        output_file << "]";
    }
};

/*
 * Simulates a competition between two agents in 3D.
 * 
 * The output file will hold the total areas covered at the specified
 * u values by each agent.  
 */
class simulation_3D_1v1 {
    public:
        agent_3D * agent1 = NULL;
        agent_3D * agent2 = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        std::ofstream output_file; 
        std::ofstream output_file_sums; 
        unsigned long long team1_area_total[U_LIST_LEN];
        unsigned long long team2_area_total[U_LIST_LEN];

        simulation_3D_1v1(agent_3D * agent1, agent_3D * agent2, int sample_size, const char * file, const char * file_sum) {
            this->agent1 = agent1;
            this->agent2 = agent2;
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            for (int i = 0; i < U_LIST_LEN; i++) {
                scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * TORUS_SIZE);
                team1_area_total[i] = 0;
                team2_area_total[i] = 0;
            }
            srand(time(NULL));
            output_file.open(file);
            output_file_sums.open(file_sum);
        }

        void simulate_sample_size() {
            output_file << "[";
            for (int i = 0; i < sample_size; i++) {
                simulate();
                if (i != sample_size - 1) output_file << ", ";
            }
            output_file << "]";
            output_file.close();

            output_file_sums << "[";
            for (int i = 0; i < U_LIST_LEN; i++) {
                output_file_sums << "[" << team1_area_total[i] << ", " << team2_area_total[i] << "]";
                if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
            }
            output_file_sums << "]";
            output_file_sums.close();
        }

        void simulate() {
            agent1->t->reset_torus();
            this->agent1->reset_agent();
            this->agent2->reset_agent();
            int current_u_list_position = 0;
            output_file << "[";
            for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
                if (rand() % 2 == 0) {
                    this->agent1->move(i);
                    this->agent2->move();
                } else {
                    this->agent2->move();
                    this->agent1->move(i);
                }
                if (i + 1 == scaled_u_list[current_u_list_position]) {
                    unsigned long long team1_area_covered = this->agent1->area_covered;
                    unsigned long long team2_area_covered = this->agent2->area_covered;
                    team1_area_total[current_u_list_position] += team1_area_covered;
                    team2_area_total[current_u_list_position] += team2_area_covered; 
                    output_file << "[" << this->agent1->area_covered << ", "; 
                    output_file << this->agent2->area_covered << "]";
                    if (i != (long long) (U_LIST_MAX) * TORUS_SIZE * TORUS_SIZE * TORUS_SIZE - 1) output_file << ", ";
                    current_u_list_position++;
                }
            }
            output_file << "]";
        }
};

/*
 * Simulates two teams of three agents competing with each other in 3D.
 * 
 * agent1, agent2, and agent3 are Team 1
 * agent4, agent5, and agent6 are Team 2
 * 
 * The output file will hold the total areas covered at the specified
 * u values by Team 1 and Team 2
 */
class simulation_3D_3v3 {
    public:
        agent_3D * agent1 = NULL;
        agent_3D * agent2 = NULL;
        agent_3D * agent3 = NULL;
        agent_3D * agent4 = NULL;
        agent_3D * agent5 = NULL;
        agent_3D * agent6 = NULL;
        long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        std::ofstream output_file;
        std::ofstream output_file_sums; 
        unsigned long long team1_area_total[U_LIST_LEN];
        unsigned long long team2_area_total[U_LIST_LEN];

        simulation_3D_3v3(agent_3D * agent1, agent_3D * agent2, agent_3D * agent3, 
                          agent_3D * agent4, agent_3D * agent5, agent_3D * agent6, 
                          int sample_size, const char * file, const char * file_sum) {
            this->agent1 = agent1;
            this->agent2 = agent2;
            this->agent3 = agent3;
            this->agent4 = agent4;
            this->agent5 = agent5;
            this->agent6 = agent6;
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            for (int i = 0; i < U_LIST_LEN; i++) {
                scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * TORUS_SIZE);
                team1_area_total[i] = 0;
                team2_area_total[i] = 0;
            }
            srand(time(NULL));
            output_file.open(file);
            output_file_sums.open(file_sum);
        }

        void simulate_sample_size() {
            output_file << "[";
            for (int i = 0; i < sample_size; i++) {
                simulate();
                if (i != sample_size - 1) output_file << ", ";
            }
            output_file << "]";
            output_file.close();

            output_file_sums << "[";
            for (int i = 0; i < U_LIST_LEN; i++) {
                output_file_sums << "[" << team1_area_total[i] << ", " << team2_area_total[i] << "]";
                if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
            }
            output_file_sums << "]";
            output_file_sums.close();
        }

        void simulate() {
            this->agent1->t->reset_torus();
            this->agent1->reset_agent();
            this->agent2->reset_agent();
            this->agent3->reset_agent();
            this->agent4->reset_agent();
            this->agent5->reset_agent();
            this->agent6->reset_agent();
            int current_u_list_position = 0;
            output_file << "[";
            for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
                unsigned long long total_area_covered = this->agent1->area_covered + this->agent2->area_covered + this->agent3->area_covered +
                                                        this->agent4->area_covered + this->agent5->area_covered + this->agent6->area_covered;
                if (total_area_covered < TORUS_SIZE * TORUS_SIZE * TORUS_SIZE) {
                    if (rand() % 2 == 0) {
                        this->agent1->move();
                        this->agent2->move();
                        this->agent3->move();
                        this->agent4->move();
                        this->agent5->move();
                        this->agent6->move();
                    } else {
                        this->agent4->move();
                        this->agent5->move();
                        this->agent6->move();
                        this->agent1->move();
                        this->agent2->move();
                        this->agent3->move();
                    }
                }
                if (i + 1 == scaled_u_list[current_u_list_position]) {
                    unsigned long long team1_area_covered = this->agent1->area_covered + this->agent2->area_covered + this->agent3->area_covered;
                    unsigned long long team2_area_covered = this->agent4->area_covered + this->agent5->area_covered + this->agent6->area_covered;
                    team1_area_total[current_u_list_position] += team1_area_covered;
                    team2_area_total[current_u_list_position] += team2_area_covered; 
                    output_file << "[" << team1_area_covered << ", " << team2_area_covered << "]";
                    if (i != (long long) (U_LIST_MAX) * TORUS_SIZE * TORUS_SIZE * TORUS_SIZE - 1) output_file << ", ";
                    current_u_list_position++;
                }
            }
            output_file << "]";
        }
};

/*
 * Simulates a team of one agent competing against a team of three agents in 3D.
 *
 * solo_agent is Team 1
 * agent1, agent2, and agent3 are Team 2
 * 
 * Every time step, the solo agent moves three times and each member of Team 2 moves once.
 * 
 * The output file will hold the total areas covered at the specified
 * u values by Team 1 and Team 2
 */
class simulation_3D_1v3 {
    public:
        agent_3D * solo_agent = NULL;
        agent_3D * agent1 = NULL;
        agent_3D * agent2 = NULL;
        agent_3D * agent3 = NULL;
        unsigned long long scaled_u_list[U_LIST_LEN];
        int sample_size = 1;
        std::ofstream output_file;
        std::ofstream output_file_sums; 
        unsigned long long team1_area_total[U_LIST_LEN];
        unsigned long long team2_area_total[U_LIST_LEN];

        simulation_3D_1v3(agent_3D * solo_agent, agent_3D * agent1, agent_3D * agent2, 
                          agent_3D * agent3, int sample_size, const char * file, const char * file_sum) {
            this->solo_agent = solo_agent;
            this->agent1 = agent1;
            this->agent2 = agent2;
            this->agent3 = agent3;
            this->sample_size = sample_size;
            double u_step = ((double) U_LIST_MAX) / ((double) U_LIST_LEN); 
            for (int i = 0; i < U_LIST_LEN; i++) {
                scaled_u_list[i] = (int) round((i + 1) * u_step * TORUS_SIZE * TORUS_SIZE * TORUS_SIZE);
                team1_area_total[i] = 0;
                team2_area_total[i] = 0;
            }
            srand(time(NULL));
            output_file.open(file);
            output_file_sums.open(file_sum);
        }

        void simulate_sample_size() {
            output_file << "[";
            for (int i = 0; i < sample_size; i++) {
                simulate();
                if (i != sample_size - 1) output_file << ", ";
            }
            output_file << "]";
            output_file.close();

            output_file_sums << "[";
            for (int i = 0; i < U_LIST_LEN; i++) {
                output_file_sums << "[" << team1_area_total[i] << ", " << team2_area_total[i] << "]";
                if (i != U_LIST_LEN - 1) output_file_sums << ", "; 
            }
            output_file_sums << "]";
            output_file_sums.close();
        }

        void simulate() {
            this->agent1->t->reset_torus();
            this->solo_agent->reset_agent();
            this->agent1->reset_agent();
            this->agent2->reset_agent();
            this->agent3->reset_agent();
            int current_u_list_position = 0;
            output_file << "[";
            for (long long i = 0; i < scaled_u_list[U_LIST_LEN - 1]; i++) {
                unsigned long long total_area_covered = this->solo_agent->area_covered + this->agent1->area_covered + 
                                                        this->agent2->area_covered + this->agent3->area_covered;
                if (total_area_covered < TORUS_SIZE * TORUS_SIZE * TORUS_SIZE) {
                    if (rand() % 2 == 0) {
                        this->solo_agent->move();
                        this->solo_agent->move();
                        this->solo_agent->move();
                        this->agent1->move();
                        this->agent2->move();
                        this->agent3->move();
                    } else {
                        this->agent1->move();
                        this->agent2->move();
                        this->agent3->move();
                        this->solo_agent->move();
                        this->solo_agent->move();
                        this->solo_agent->move();
                    }
                }
                if (i + 1 == scaled_u_list[current_u_list_position]) {
                    unsigned long long team1_area_covered = this->solo_agent->area_covered;
                    unsigned long long team2_area_covered = this->agent1->area_covered + this->agent2->area_covered + this->agent3->area_covered;
                    team1_area_total[current_u_list_position] += team1_area_covered;
                    team2_area_total[current_u_list_position] += team2_area_covered; 
                    output_file << "[" << team1_area_covered << ", " << team2_area_covered << "]";
                    if (i != (long long) (U_LIST_MAX) * TORUS_SIZE * TORUS_SIZE * TORUS_SIZE - 1) output_file << ", ";
                    current_u_list_position++;
                }
            }
            output_file << "]";
        }
};

