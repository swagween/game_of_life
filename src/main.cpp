//
//  main.cpp
//  Game of Life
//

#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include "Grid.hpp"

namespace {

const sf::Vector2<uint32_t> screen_dimensions { 1420, 1000 };
const int SCREEN_WIDTH = 1420;
const int SCREEN_HEIGHT = 1000;

void run(char** argv) {
    
    //init clock
    std::chrono::time_point<std::chrono::system_clock> start{}, end{};
    std::chrono::duration<double, std::milli> work_time{}, sleep_time{};
    
    const double time_step = 1000.0/30.0; //30 FPS
    bool paused = false;
    
    //some SFML variables for drawing a basic window + background
    auto window = sf::RenderWindow{sf::VideoMode{screen_dimensions.x, screen_dimensions.y}, "Game of Life v1.0"};
    sf::RectangleShape background{};
    background.setSize(static_cast<sf::Vector2<float> >(screen_dimensions));
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(20, 20, 30));
    
    //declare main grid
    conway::Grid main_grid = conway::Grid(96, 64);
    main_grid.set_spacing(12.0f);
    main_grid.update();
    
    //start with a basic random init with 40% cell genesis
    main_grid.random_init();
    
    //game loop
    while (window.isOpen()) {
        
        start = std::chrono::system_clock::now(); //start clock
        std::chrono::duration<double, std::milli> work_time = start - end;
        
        if (work_time.count() < time_step)
        {
            std::chrono::duration<double, std::milli> delta_ms(time_step - work_time.count());
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }
        
        end = std::chrono::system_clock::now();
        sleep_time = end - start;
        
        //SFML event variable
        auto event = sf::Event{};
        //check window events
        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    return;
                case sf::Event::KeyPressed:
                    //player can refresh grid by pressing 'Z'
                    if(event.key.code == sf::Keyboard::Z) {
                        main_grid.random_init();
                    }
                    //player can pause grid by pressing 'X'
                    if(event.key.code == sf::Keyboard::X) {
                        paused = !paused;
                    }
                    break;
                default:
                    break;
            }
        }
        
        //game logic and rendering
        if(!paused) {
            
            main_grid.tick_cells();
            
            window.clear();
            window.draw(background);
            
            //draw the cells
            for(int i = 0; i < main_grid.get_size(); ++i) {
                window.draw(main_grid.get_drawable_at(i));
            }
            
            window.display();
            
        }
    }
}
} // namespace

int main(int argc, char** argv) {
    assert(argc > 0);
    run(argv);
    return 0;
}
