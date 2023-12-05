#include <iostream>
#include <chrono>
#include <ctime>
#include "simulate.cpp"
#include <thread>

using namespace std;

void run_simulation_2d_solo(int strat, const char * file, const char * file_sums, int sample_size, int id) {
    torus_2D * tor = new torus_2D();
    agent_2D * agent = new agent_2D(tor, strat, 1);
    simulation_2D sim(agent, sample_size, file, file_sums);
    std::cout << "Simulation " << id << " starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time: " << elapsed.count() << "s\n" << std::endl;
    free(tor);
    free(agent);
}

void run_simulation_2d_solo_mines(int strat, double m, const char * file, const char * file_sums, int sample_size, int id) {
    torus_2D * tor = new torus_2D();
    agent_2D * agent = new agent_2D(tor, strat, 1);
    simulation_2D_solo_mines sim(agent, m, sample_size, file, file_sums);
    std::cout << "Simulation " << id << " starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time: " << elapsed.count() << "s\n" << std::endl;
    free(tor);
    free(agent);
}

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

void run_simulation_2d_interface(int strat1, int strat2, const char * file, const char * file_sums, int sample_size, int id, int distance) {
    torus_2D * tor = new torus_2D();
    agent_2D * agent1 = new agent_2D(tor, strat1, 1);
    agent_2D * agent2 = new agent_2D(tor, strat2, 2);
    simulation_2D_1v1_interface sim(agent1, agent2, sample_size, file, file_sums, distance);
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

void run_simulation_1d(int second_starting_position, int sample_size, const char * torus_file_name, const char * interface_file_name) {
    torus_1D * tor = new torus_1D();
    agent_1D * agent1 = new agent_1D(tor, 1, 0);
    agent_1D * agent2 = new agent_1D(tor, 2, second_starting_position);
    simulation_1D_1v1 sim(agent1, agent2, sample_size, torus_file_name, interface_file_name);
    std::cout << "Simulation starting..." << std::endl;
    auto start = std::chrono::system_clock::now();
    sim.simulate_sample_size();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Elapsed time for simulation: " << elapsed.count() << "s" << std::endl;
    free(tor);
    free(agent1);
    free(agent2);}

int main() {
    int sample_size = 10000;

    auto start = std::chrono::system_clock::now();

    // torus_<torus_size>_<a>
    run_simulation_1d(1, sample_size, "torus_10001_1.txt", "interface_10001_1.txt");

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start; 
    std::cout << "Total elapsed time: " << elapsed.count() << "s\n" << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}

