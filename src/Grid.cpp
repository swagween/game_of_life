//
//  Grid.cpp
//  Game of Life
//
//  Created by Alex Frasca on 01/05/2023.
//

#include "Grid.hpp"

namespace conway {

Cell::Cell(sf::Vector2<int> idx, sf::Vector2<float> pos, float spc, CELL_STATE st) {
    index = idx;
    position = pos;
    spacing = spc;
    state = st;
    kill_next_cycle = false;
    activate_next_cycle = false;
    
    //init cell drawable
    drawable = sf::ConvexShape();
    drawable.setPointCount(4);
    drawable.setPoint(0, sf::Vector2f( position.x          , position.y           ));
    drawable.setPoint(1, sf::Vector2f( position.x + spacing, position.y           ));
    drawable.setPoint(2, sf::Vector2f( position.x + spacing, position.y + spacing ));
    drawable.setPoint(3, sf::Vector2f( position.x          , position.y + spacing ));
}

void Cell::tick() {
    switch (state) {
        case CELL_STATE::CELL_OFF:
            drawable.setOutlineThickness(0);
            drawable.setFillColor(GoL_Black);
            break;
        case CELL_STATE::CELL_LOW:
            drawable.setOutlineColor(GoL_NavyBlue);
            drawable.setOutlineThickness(-2);
            drawable.setFillColor(GoL_DarkFucshia);
            break;
        case CELL_STATE::CELL_MEDIUM:
            drawable.setOutlineColor(GoL_DarkFucshia);
            drawable.setOutlineThickness(-2);
            drawable.setFillColor(GoL_Fucshia);
            break;
        case CELL_STATE::CELL_HIGH:
            drawable.setOutlineColor(GoL_Fucshia);
            drawable.setOutlineThickness(2);
            drawable.setFillColor(GoL_Goldenrod);
            break;
        case CELL_STATE::CELL_ON:
            drawable.setOutlineColor(GoL_Goldenrod);
            drawable.setOutlineThickness(2);
            drawable.setFillColor(GoL_White);
            break;
        default:
            break;
    }
    set_state();
}

void Cell::set_state() {
    if(kill_next_cycle && !activate_next_cycle) {
        decrement();
        kill_next_cycle = false;
    }
    if(activate_next_cycle) {
        state = CELL_STATE::CELL_ON;
        activate_next_cycle = false;
    }
}

void Cell::decrement() {
    if(state != CELL_STATE::CELL_OFF) {
        state = (CELL_STATE)((int)state - 1);
    }
}

void Cell::update_position() {
    position.x = index.x * spacing;
    position.y = index.y * spacing;
    drawable.setPoint(0, sf::Vector2f( position.x          , position.y           ));
    drawable.setPoint(1, sf::Vector2f( position.x + spacing, position.y           ));
    drawable.setPoint(2, sf::Vector2f( position.x + spacing, position.y + spacing ));
    drawable.setPoint(3, sf::Vector2f( position.x          , position.y + spacing ));
}

Grid::Grid() {
    
    grid_width = 0;
    grid_height = 0;
    spacing = DEFAULT_SPACING;
    
}

Grid::Grid(uint32_t width, uint32_t height) {
    
    grid_width = width;
    grid_height = height;
    spacing = DEFAULT_SPACING;
    
    initialize();
    
}

void Grid::random_init() {
    if(!cells.empty()) {
        for(auto& cell : cells) {
            int r = floor(rand()*100);
            if(r > 40) {
                cell.state = CELL_STATE::CELL_ON;
            }
        }
    }
}

void Grid::initialize() {
    
    cells.clear();
    
    for(int i = 0; i < grid_width * grid_height; i++) {
        
        // calculate index values
        int xidx = std::floor(i%grid_width);
        int yidx = std::floor(i/grid_width);
        
        //calculate positions with offsets
        float xpos, ypos;
        if(yidx % 2 == 0) {
            xpos = xidx*spacing;
        } else {
            xpos = xidx*spacing;
        }
        
        if(xidx % 2 == 0) {
            ypos = yidx*spacing;
        } else {
            ypos = yidx*spacing;
        }
        cells.push_back(Cell({xidx, yidx}, {xpos, ypos}, spacing, CELL_STATE::CELL_OFF));
        
    }
    if(cells.empty()) {
        printf("Cells were not initialized properly.\n");
        return;
    }
    
}

void Grid::update() {
    
    for(int i = 0; i < grid_width * grid_height; i++) {
        push_cells(i);
        if(i < cells.size()) {
            float xpos = cells.at(i).position.x;
            float ypos = cells.at(i).position.y;
            float dim = spacing;
            cells.at(i).drawable.setPoint(0, sf::Vector2f( xpos      , ypos       ));
            cells.at(i).drawable.setPoint(1, sf::Vector2f( xpos + dim, ypos       ));
            cells.at(i).drawable.setPoint(2, sf::Vector2f( xpos + dim, ypos + dim ));
            cells.at(i).drawable.setPoint(3, sf::Vector2f( xpos      , ypos + dim ));
        }
    }
    
}

void Grid::set_spacing(float spc) {
    
    spacing = spc;
    for(int i = 0; i < grid_width * grid_height; i++) {
        float xpos = cells.at(i).index.x * spacing;
        float ypos = cells.at(i).index.y * spacing;
        cells.at(i).position = {xpos, ypos};
        cells.at(i).update_position();
    }
    
}

sf::ConvexShape& Grid::get_drawable_at(int i) {
    assert(i >= 0 && i < cells.size());
    return cells.at(i).drawable;
}

int Grid::get_size() {
    return cells.size();
}

void Grid::push_cells(int i) {
    // calculate index values
    int xidx = std::floor(i%grid_width);
    int yidx = std::floor(i/grid_width);
    
    //calculate positions with offsets
    float xpos, ypos;
    if(yidx % 2 == 0) {
        xpos = xidx*spacing;
    } else {
        xpos = xidx*spacing;
    }
    
    if(xidx % 2 == 0) {
        ypos = yidx*spacing;
    } else {
        ypos = yidx*spacing;
    }
    
    //populate the grid
    cells.at(i).index = {xidx, yidx};
    cells.at(i).update_position();
}

void Grid::tick_cells() {
    
    std::vector<std::reference_wrapper<Cell> > neighbor_list{};
    int living_neighbor_counter = 0;
    
    //game logic
    for(auto& cell : cells) {
        neighbor_list = get_neighbors(cell);
        for(auto& neighbor : neighbor_list) {
            if(neighbor.std::reference_wrapper<Cell>::get().state == CELL_STATE::CELL_ON) {
                ++living_neighbor_counter;
            }
        }
        if(cell.state == CELL_STATE::CELL_ON && (living_neighbor_counter == 3 || living_neighbor_counter == 2)) {
            cell.state = CELL_STATE::CELL_ON;
        } else if (living_neighbor_counter > 3) {
            cell.kill_next_cycle = true;
        } else if (living_neighbor_counter < 3) {
            cell.kill_next_cycle = true;
        } else if (cell.state != CELL_STATE::CELL_ON && living_neighbor_counter == 3) {
            cell.activate_next_cycle = true;
        }
        living_neighbor_counter = 0;
        neighbor_list.clear();
    }
    
    //now change the cell states
    for(auto& cell : cells) {
        cell.tick();
    }
    
}

std::vector<std::reference_wrapper<Cell> > Grid::get_neighbors(Cell &current_cell) {
    
    int grid_size = grid_height * grid_width; // to use for edge cases
    std::vector<std::reference_wrapper<Cell> > neighbor_list;
    
    //calculate current cell's index
    int this_index = current_cell.index.x + current_cell.index.y * grid_width;
    
    //indeces of eight neighbors, which are 1D and refer to the place in the cells vector
    int up_left_index;
    int up_index;
    int up_right_index;
    int left_index;
    
    int right_index;
    int down_left_index;
    int down_index;
    int down_right_index;
    
    //calculate indeces
    up_left_index = this_index - grid_width - 1;
    up_index = this_index - grid_width;
    up_right_index = this_index - grid_width + 1;
    left_index = this_index - 1;
    
    right_index = this_index + 1;
    down_left_index = this_index + grid_width - 1;
    down_index = this_index + grid_width;
    down_right_index = this_index + grid_width + 1;
    
    //edge cases. here we implement a simple wrap-around
    if(up_index < 0) { up_index += grid_size; up_left_index += grid_size; up_right_index += grid_size; } //wrap top edge
    if(down_index > grid_size - 1) { down_index %= grid_size; down_left_index %= grid_size; down_right_index %= grid_size; } //wrap bottom edge
    
    if(right_index % grid_width == 0) { right_index -= grid_width; up_right_index -= grid_width; down_right_index -= grid_width; } //wrap right side
    if(left_index % grid_width == grid_width - 1) { left_index += grid_width; up_left_index += grid_width; down_left_index += grid_width;} //wrap left side
    
    //ensure neighbor is within vector bounds
    //calculate indeces
    up_left_index %= grid_size;
    up_index %= grid_size;
    up_right_index %= grid_size;
    left_index %= grid_size;
    
    right_index %= grid_size;
    down_left_index %= grid_size;
    down_index %= grid_size;
    down_right_index %= grid_size;
    
    //assert indeces in bounds before trying to access the cells vector
    assert(up_left_index >= 0 && up_left_index < grid_size);
    assert(up_index >= 0 && up_index < grid_size);
    assert(up_right_index >= 0 && up_right_index < grid_size);
    assert(left_index >= 0 && left_index < grid_size);
    assert(right_index >= 0 && right_index < grid_size);
    assert(down_left_index >= 0 && down_left_index < grid_size);
    assert(down_index >= 0 && down_index < grid_size);
    assert(down_right_index >= 0 && down_right_index < grid_size);
    
    
    //push references to the neighbors into the vetor to be returned
    neighbor_list.push_back(cells.at(up_left_index));
    neighbor_list.push_back(cells.at(up_index));
    neighbor_list.push_back(cells.at(up_right_index));
    neighbor_list.push_back(cells.at(left_index));
    neighbor_list.push_back(cells.at(right_index));
    neighbor_list.push_back(cells.at(down_left_index));
    neighbor_list.push_back(cells.at(down_index));
    neighbor_list.push_back(cells.at(down_right_index));
    
    return neighbor_list;
}

} // end conway