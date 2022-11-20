/**
 * @file model.cpp
 * @author Emmanuel Cortes
 * @brief
 * @date 2022-11-19
 *
 * @copyright Copyright (c) Emmanuel Cortes 2022
 */

#include <iostream>
#include <fstream>
#include <ctime>     // time
#include <algorithm> // swap
#include <vector>

#include <omp.h>

typedef std::vector<std::vector<int>> CELLULAR_STATE;

const std::string FILE_PATH = "data/output.csv";

// simulation parameters
const double TREE_PROB = .45;
const int WIDTH = 300;
const int HEIGHT = 300;
const int STEPS = 200;

// different cell states
const int EMPTY = 0;
const int TREE = 1;
const int FIRE = 2;
const int CHARRED = 3;

// forward declare functions
void init(CELLULAR_STATE &config, CELLULAR_STATE &next_config);
void step(CELLULAR_STATE &config, CELLULAR_STATE &next_config);
void getStateFromNeighbors(int i, int j, int &state, const CELLULAR_STATE &config);
void swapState(CELLULAR_STATE &source, CELLULAR_STATE &dest);
void outputState(const CELLULAR_STATE &config);

int main(int argc, char *argv[])
{
    srand(time(NULL)); // seed rand

    // clear file
    std::ofstream fout(FILE_PATH);
    fout.close();

    // declare CA variables
    int time;
    CELLULAR_STATE config;
    CELLULAR_STATE next_config;

    // initialize CA state and output it
    init(config, next_config);
    outputState(config);

    // main simulation loop
    for (time = 0; time < STEPS; time++)
    {
        step(config, next_config);
        outputState(config);
    }

    return 0;
}

/**
 * @brief Initializes the 2d vectors used for storing the cellular automata state
 *
 * @param config      : used for storing the current state
 * @param next_config : used for storing the update state
 */
void init(CELLULAR_STATE &config, CELLULAR_STATE &next_config)
{
    int state; // cell state

    for (int i = 0; i < HEIGHT; i++)
    {
        // initialize WIDTH x HEIGHT vector with zeros
        config.push_back(std::vector<int>(WIDTH, 0));
        next_config.push_back(std::vector<int>(WIDTH, 0));
#pragma omp parallel for private(state)
        for (int j = 0; j < WIDTH; j++)
        {
            if (((double)rand() / (RAND_MAX)) < TREE_PROB)
                state = TREE;
            else
                state = EMPTY;
            config.at(i).at(j) = state;
        }
    }
    // set middle cell on fire
    config.at(HEIGHT / 2).at(WIDTH / 2) = FIRE;
}

/**
 * @brief Simulate a time step. If a cell is already on FIRE then set
 * it to CHARRED. If any neighbor in the current cell's Moore neighborhood is on FIRE
 * then set current cell on FIRE. Otherwise don't modify the cell's state.
 *
 * @param config      : the current CA config state
 * @param next_config : the next config state derived from `config`
 */
void step(CELLULAR_STATE &config, CELLULAR_STATE &next_config)
{
    int i, j;  // iterators
    int state; // cell state
#pragma omp parallel for private(state) collapse(2)
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            state = config.at(i).at(j);
            if (state == FIRE)
            {
                state = CHARRED;
            }
            else if (state == TREE)
            {
                getStateFromNeighbors(i, j, state, config);
            }
            next_config.at(i).at(j) = state;
        }
    }
    // save next_config to config for next step
    swapState(config, next_config);
}

/**
 * @brief Get the State From neighbors subarray
 *
 * @param i      : current cell's height
 * @param j      : current cell's width
 * @param state  : stores the cell's new state
 * @param config : the current CA config state
 */
void getStateFromNeighbors(int i, int j, int &state, const CELLULAR_STATE &config)
{
#pragma omp parallel for collapse(2)
    for (int di = -1; di < 2; di++)
    {
        for (int dj = -1; dj < 2; dj++)
        {
            if (di == 0 && dj == 0)
                continue;
            // periodic boundaries
            if (config.at((i + di + HEIGHT) % HEIGHT).at((j + dj + WIDTH) % WIDTH) == FIRE)
            {
                state = FIRE; // update state; race-conditions don't matter in this case
            }
        }
    }
}

/**
 * @brief Swap the source and dest CELLULAR_STATEs
 *
 * @param source
 * @param dest
 */
void swapState(CELLULAR_STATE &source, CELLULAR_STATE &dest)
{
#pragma omp parallel for collapse(2)
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            std::swap(source.at(i).at(j), dest.at(i).at(j));
        }
    }
}

/**
 * @brief Append the current CA config to an output file
 *
 * @param config : the CA current config state
 */
void outputState(const CELLULAR_STATE &config)
{
    int i, j; // iterators
    std::ofstream fout(FILE_PATH, std::ios_base::app);
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            fout << config.at(i).at(j) << ',';
        }
        fout << '\n';
    }
    fout.close();
}
