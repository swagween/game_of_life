//
//  main.cpp
//  Game of Life
//

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Grid.hpp"


namespace {

const int SCREEN_WIDTH = 1420;
const int SCREEN_HEIGHT = 1000;

void run(char** argv) {
    
    sf::Clock clock;
    sf::Time elapsed;
    
    int time_step = 64;
    bool paused = false;
    
    //some SFML variables for drawing a basic window + background
    auto window = sf::RenderWindow{sf::VideoMode{SCREEN_WIDTH, SCREEN_HEIGHT}, "Game of Life v1.0"};
    sf::RectangleShape background = sf::RectangleShape();
    background.setSize({SCREEN_WIDTH, SCREEN_HEIGHT});
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(20, 20, 30));
    
    //declare main grid
    conway::Grid main_grid = conway::Grid(128, 96);
    main_grid.set_spacing(12.0f);
    main_grid.update();
    
    //start with a basic random init with 40% cell genesis
    main_grid.random_init();
    
    //game loop
    while (window.isOpen()) {
        //SFML event variable
        auto event = sf::Event{};
        //check window events
        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    return;
                    break;
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
        if(elapsed.asMilliseconds() >= time_step && !paused) {
            
            main_grid.tick_cells();
            
            window.clear();
            window.draw(background);
            
            //draw the cells
            for(int i = 0; i < main_grid.get_size(); ++i) {
                window.draw(main_grid.get_drawable_at(i));
            }
            
            window.display();
            
            elapsed = sf::Time::Zero;
        }
        
        //update clock
        elapsed += clock.getElapsedTime();
        clock.restart();
        
    }
}
} // namespace

int main(int argc, char** argv) {
    assert(argc > 0);
    run(argv);
    return 0;
}
