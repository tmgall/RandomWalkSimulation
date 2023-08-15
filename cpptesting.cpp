#include <iostream>
#include <chrono>
#include <ctime>
#include "simulate.cpp"
#include <thread>

using namespace std;

void run_simulation_2d(int strat1, int strat2, const char * file, const char * file_sums, int sample_size, int id) {
    torus_2D * tor = new torus_2D();
    agent_2D * agent1 = new agent_2D(tor, strat1, 1);
    agent_2D * agent2 = new agent_2D(tor, strat2, 2);
    simulation_2D_1v1 sim(agent1, agent2, sample_size, file, file_sums);
    std::cout << "Simulation " << id << " starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time: " << elapsed.count() << "s\n" << std::endl;
    free(tor);
    free(agent1);
    free(agent2);
}

void run_simulation_3d(int strat1, int strat2, const char * file, const char * file_sums, int sample_size, int id) {
    torus_3D * tor = new torus_3D();
    agent_3D * agent1 = new agent_3D(tor, strat1, 1);
    agent_3D * agent2 = new agent_3D(tor, strat2, 2);
    simulation_3D_1v1 sim(agent1, agent2, sample_size, file, file_sums);
    std::cout << "Simulation " << id << " starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time: " << elapsed.count() << "s\n" << std::endl;
    free(tor);
    free(agent1);
    free(agent2);
}

void run_simulation_2d_mines(int strat1, int strat2, const char * file, const char * file_sums, int sample_size, double m, int id) {
    torus_2D * tor = new torus_2D();
    agent_2D * agent1 = new agent_2D(tor, strat1, 1);
    agent_2D * agent2 = new agent_2D(tor, strat2, 2);
    simulation_2D_1v1_mines sim1(agent1, agent2, m, sample_size, file, file_sums);
    std::cout << "Simulation " << id << " starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim1.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time for simulation " << id << ": " << elapsed.count() << "s" << std::endl;
    free(tor);
    free(agent1);
    free(agent2);
}

void run_simulation_2d_mines_solo(int strat1, const char * file, const char * file_sums, int sample_size, double m, int id) {
    torus_2D * tor = new torus_2D();
    agent_2D * agent = new agent_2D(tor, strat1, 1);
    simulation_2D_solo_mines sim1(agent, m, sample_size, file, file_sums);
    std::cout << "Simulation " << id << " starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim1.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time for simulation " << id << ": " << elapsed.count() << "s" << std::endl;
    free(tor);
    free(agent);
}

void run_simulation_3_collab(int strat1, const char * file, const char * file_sums, int sample_size, int id) {
    torus_2D * tor = new torus_2D();
    agent_2D * agent1 = new agent_2D(tor, strat1, 1);
    agent_2D * agent2 = new agent_2D(tor, strat1, 2);
    agent_2D * agent3 = new agent_2D(tor, strat1, 3);
    simulation_3_collab_2D sim1(agent1, agent2, agent3, sample_size, file, file_sums);
    std::cout << "Simulation " << id << " starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim1.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time for simulation " << id << ": " << elapsed.count() << "s" << std::endl;
    free(tor);
    free(agent1);
    free(agent2);
    free(agent3);
}

void run_simulation_2d_mines_competition(int strat1, int strat2, const char * file, const char * file_sums, int sample_size, double m, int id) {
    torus_2D * tor = new torus_2D();
    agent_2D * agent1 = new agent_2D(tor, strat1, 1);
    agent_2D * agent2 = new agent_2D(tor, strat2, 2);
    simulation_2D_1v1_mines sim(agent1, agent2, m, sample_size, file, file_sums);
    std::cout << "Simulation " << id << " starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time for simulation " << id << ": " << elapsed.count() << "s" << std::endl;
    free(tor);
    free(agent1);
    free(agent2);
}

int main() {
    int sample_size = 5000;

    auto start = std::chrono::system_clock::now();

    // need to run gb_vs_gu for mine levels 5, 10, 15, 20, 25, 30 with 5000 samples each 
    // pick the right length of simulation to get to just over 98% combined area covered

    // run_simulation_2d_mines_competition(GREEDY_BIASED, GREEDY_UNBIASED, "gb_vs_gu_30.txt", "gb_vs_gu_30_sums.txt", sample_size, 0.30, 1);
    // run_simulation_2d_mines_competition(GREEDY_BIASED, GREEDY_UNBIASED, "gb_vs_gu_25.txt", "gb_vs_gu_25_sums.txt", sample_size, 0.25, 2);
    // run_simulation_2d_mines_competition(GREEDY_BIASED, GREEDY_UNBIASED, "gb_vs_gu_20.txt", "gb_vs_gu_20_sums.txt", sample_size, 0.20, 3);
    // run_simulation_2d_mines_competition(GREEDY_BIASED, GREEDY_UNBIASED, "gb_vs_gu_15.txt", "gb_vs_gu_15_sums.txt", sample_size, 0.15, 4);
    run_simulation_2d_mines_competition(GREEDY_BIASED, GREEDY_UNBIASED, "gb_vs_gu_10.txt", "gb_vs_gu_10_sums.txt", sample_size, 0.10, 5);
    run_simulation_2d_mines_competition(GREEDY_BIASED, GREEDY_UNBIASED, "gb_vs_gu_5.txt", "gb_vs_gu_5_sums.txt", sample_size, 0.05, 6);



    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Total elapsed time: " << elapsed.count() << "s\n" << std::endl; 
    std::cout << "Success!" << std::endl;
    return 0;
}

