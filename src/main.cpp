//
//  main.cpp
//  Game of Life
//

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <chrono>
#include <thread>
#include "Grid.hpp"
#include "Stopwatch.hpp"


#include <imgui-SFML.h>
#include <imgui.h>

namespace {

const sf::Vector2<uint32_t> screen_dimensions { 764, 508 };
const int TIME_STEP_MILLI = 50;


void run(char** argv) {
    
    //set context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //some SFML variables for drawing a basic window + background
    auto window = sf::RenderWindow{sf::VideoMode{screen_dimensions.x, screen_dimensions.y}, "Game of Life v1.0"};
    ImGui::SFML::Init(window);
    ImGuiIO& io = ImGui::GetIO();
    
    io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Key::Delete;
    io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::Key::Backspace;
    io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Key::Space;
    io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Key::Enter;
    
    //set style
    ImGui::StyleColorsDark();
    
    //init clock
    using Clock = std::chrono::steady_clock;
    using Time = std::chrono::duration<float>;
    auto elapsed_time = Time{};
    auto time_step = Time{std::chrono::milliseconds(TIME_STEP_MILLI)}; //FPS
    
    bool paused = false;
    
   
    window.setVerticalSyncEnabled(true);
    sf::RectangleShape background{};
    background.setSize(static_cast<sf::Vector2<float> >(screen_dimensions));
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(20, 20, 30));
    
    //declare main grid
    conway::Grid main_grid = conway::Grid(48, 32);
    main_grid.set_spacing(12.0f);
    main_grid.update();
    
    //start with a basic random init with 20% cell genesis
    main_grid.random_init();
    
    sf::Clock deltaClock{};
    
    //game loop
    auto start = Clock::now();
    while (window.isOpen()) {
        
        auto now = Clock::now();
        auto dt = Time{now - start};
        start = now;
        elapsed_time += dt;
        
        //SFML event variable
        auto event = sf::Event{};
        //check window events
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
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
        if(!paused && elapsed_time > time_step) {
            main_grid.tick_cells();
            elapsed_time = Time::zero();
        }
        
        ImGui::SFML::Update(window, deltaClock.restart());
        
        //ImGui renders
        ImGui::Begin("Quick Actions");
        if(ImGui::Button("Spawn Cells")) {
            main_grid.random_init();
        }
        ImGui::End();
        
        //render
        window.clear();
        window.draw(background);
        //draw the cells
        for(int i = 0; i < main_grid.get_size(); ++i) {
            window.draw(main_grid.get_drawable_at(i));
        }
        ImGui::SFML::Render(window);
        
        window.display();
    }
    ImGui::SFML::Shutdown();
}
} // namespace

int main(int argc, char** argv) {
    assert(argc > 0);
    run(argv);
    return 0;
}
