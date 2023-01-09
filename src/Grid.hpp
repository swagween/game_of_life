//
//  Grid.hpp
//  Game of Life
//
//  Created by Alex Frasca on 01/05/2023.
//
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

namespace conway {

//several values so we can draw nice colors as the cells die instead of just on/off
enum class CellState {
    cell_off = 0,
    cell_low = 1,
    cell_medium = 2,
    cell_high = 3,
    cell_on = 4
};

//set colors for style
inline sf::Color GoL_White          = sf::Color(235, 232, 249);
inline sf::Color GoL_Red            = sf::Color(236, 63,  95 );
inline sf::Color GoL_Fucshia        = sf::Color(215, 53,  180);
inline sf::Color GoL_Blue           = sf::Color(110, 98,  173);
inline sf::Color GoL_NavyBlue       = sf::Color(25,  35,  65 );
inline sf::Color GoL_DarkFucshia    = sf::Color(148, 40,  84 );
inline sf::Color GoL_Goldenrod      = sf::Color(247, 199, 74 );
inline sf::Color GoL_Orange         = sf::Color(226, 93,  11 );
inline sf::Color GoL_Black          = sf::Color(55,  49,  64 );
inline sf::Color GoL_Periwinkle     = sf::Color(159, 138, 247);
inline sf::Color GoL_Green          = sf::Color(81,  186, 155);

constexpr float default_spacing = 16.0;

struct Cell {
    
    Cell(sf::Vector2<int> idx, sf::Vector2<float> pos, float spc, CellState st);
    
    void tick();
    void set_state();
    // for a dead cell, use a frame-wise decrement for animation/visualization purposes
    void decrement();
    // only for when the size or spacing of the grid is changed, not for each tick
    void update_position();
    
    // Cell member vars
    sf::Vector2<int> index{};
    sf::Vector2<float> position{};
    sf::ConvexShape drawable{}; // for drawing the cell
    
    float spacing{};
    
    // bools for determining the next state of the grid. we need this otherwise grid cells
    // will update as we tick them resulting in undesired neighbor calculations for future cells
    bool kill_next_cycle{};
    bool activate_next_cycle{};
    
    CellState state{};

};

class Grid {
public:
    
    Grid() = default;
    Grid(uint32_t width, uint32_t height);
    
    uint32_t grid_width{ 0 };
    uint32_t grid_height{ 0 };
    
    float spacing = default_spacing; //how far apart are the grid elements?
    
    //main grid functions
    void initialize();
    void random_init();
    void update();
    void tick_cells();
    void push_cells(int i); //helper for setting vertices and offsets
    
    //for Game of Life rules and helper functions
    int get_neighbors(const Cell& current_cell) const;

    //getters
    sf::ConvexShape& get_drawable_at(int i);
    int get_size() const;
    
    //setters
    void set_spacing(float spc);
    
private:
    //the cells of the grid
    std::vector<Cell> cells;
    
};

} // end conway

/* Grid_hpp */
