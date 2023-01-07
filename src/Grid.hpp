//
//  Grid.hpp
//  Game of Life
//
//  Created by Alex Frasca on 01/05/2023.
//
#pragma once
#ifndef Grid_hpp
#define Grid_hpp

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <functional>

namespace conway {

//several values so we can draw nice colors as the cells die instead of just on/off
enum CELL_STATE {
    CELL_OFF = 0,
    CELL_LOW = 1,
    CELL_MEDIUM = 2,
    CELL_HIGH = 3,
    CELL_ON = 4
};
const float DEFAULT_SPACING = 16.0;

struct Cell {
    
    Cell() {};
    Cell(sf::Vector2<int> idx, sf::Vector2<float> pos, float spc, CELL_STATE st);
    
    void tick();
    void setState();
    // for a dead cell, use a frame-wise decrement for animation/visualization purposes
    void decrement();
    // only for when the size or spacing of the grid is changed, not for each tick
    void updatePosition();
    
    // Cell member vars
    sf::Vector2<int> index;
    sf::Vector2<float> position;
    // for drawing the cell
    sf::ConvexShape drawable;
    
    float spacing;
    
    // bools for determining the next state of the grid. we need this otherwise grid cells
    // will update as we tick them resulting in undesired neighbor calculations for future cells
    bool kill_next_cycle;
    bool activate_next_cycle;
    
    CELL_STATE state;

};

class Grid {
public:
    
    Grid();
    Grid(uint32_t width, uint32_t height);
    
    uint32_t grid_width;
    uint32_t grid_height;
    
    // how far apart are the grid elements?
    float spacing;
    
    //the cells of the grid
    std::vector<Cell> cells;
    
    void random_init();
    void initialize();
    void update();
    
    //for Game of Life rules and helper functions
    void tick_cells();
    std::vector<std::reference_wrapper<Cell> > get_neighbors(Cell& current_cell);
    
    //grid init functions
    void setSpacing(float spc);
    
    //helper for setting vertices and offsets
    void pushCells(int i);
    
};

} // end conway

#endif /* Grid_hpp */
