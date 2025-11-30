#include <iostream>
#include <fstream>
#include <limits>
#include <string>

enum class MenuState {
    MAIN,
    CREATE_NAME,
    CREATE_SIZE,
    DELETE_NAME,
    DELETE_CONFIRM,
    EXIT
};

void display_menu();
void display_path(MenuState state);
MenuState create_project(std::string name, std::string size);
MenuState delete_project(std::string name, std::string confirmation);

int main(int argc, char *argv[]){
    MenuState state = MenuState::MAIN;
    std::string selection;
    std::string input;
    std::string name;
    std::string size;
    std::string confirmation;
    

    // 




    while (state != MenuState::EXIT) {
        system("cls");
        display_menu();
        display_path(state);


        std::getline(std::cin, input);
        std::cout << "]" << std::endl;

        // Creating Global Commands
        if (input == "exit"){
            state = MenuState::EXIT;
            continue;
        }
        if (input == "main"){
            state = MenuState::MAIN;
            continue;
        }
        if (input == "create"){
            state = MenuState::CREATE_NAME;
            continue;
        }
        if (input == "delete"){
            state = MenuState::DELETE_NAME;
            continue;
        }
        // Perform Action based upon State
        switch (state) {
            case MenuState::MAIN:
                break;
            case MenuState::CREATE_NAME:
                name = input;
                state = MenuState::CREATE_SIZE;
                break;
            case MenuState::CREATE_SIZE:
                size = input;
                create_project(name, size);
                break;
            case MenuState::DELETE_NAME:
                name = input;
                state = MenuState::DELETE_CONFIRM;
                break;
            case MenuState::DELETE_CONFIRM:
                confirmation = input;
                state = delete_project(name, confirmation);
                break;
            default:
                break;
        }
    }
    std::cout << "Closing Application";
    return 0;
}

MenuState create_project(std::string name, std::string size){
    // Ensure name does not already exist
    // Create the file
    // Organize the data by Goal: and Current:
    std::cout << "Name: " << name << " Size: " << size << std::endl;
    return MenuState::MAIN;
}

MenuState delete_project(std::string name, std::string confirmation){
    // Delete project should have a return
    // The return value should be the state the menu should be in depending on what the funciton does
        // If the the value is y or n
            // return the main menu state
        // if the value is giberish
            // return the confirmation state to allow it to be asked again.
    if (confirmation == "y"){
        std::cout << "This counted as YES" << std::endl;
        return MenuState::MAIN;
    }
    else if (confirmation == "n"){
        return MenuState::MAIN;
    }
    else {
        return MenuState::DELETE_CONFIRM;
    }
}

void display_menu(){
    std::cout << "#####~ SandLog ~#####" << std::endl;
    std::cout << "#####################" << std::endl;
    std::cout << "[create]" << std::endl;
    std::cout << "[delete]" << std::endl;
    std::cout << "[exit]" << std::endl;
    std::cout << "#####################" << std::endl;
    std::cout << "[1] projects go here" << std::endl << std::endl;
}

void display_path(MenuState state){
    std::string example_path;
    std::string path;

    switch (state) {
        case MenuState::MAIN:
            example_path =  "[::::][main][choice]";
            path =          "[::::][main][";
            break;
        case MenuState::CREATE_NAME:
            example_path =  "[::::][main][create][name]";
            path =          "[::::][main][create][";
            break;
        case MenuState::CREATE_SIZE:
            example_path =  "[::::][main][create][name][size]";
            path =          "[::::][main][create][name][";
            break;
        case MenuState::DELETE_NAME:
            example_path =  "[::::][main][delete][name]";
            path =          "[::::][main][delete][";
            break;
        case MenuState::DELETE_CONFIRM:
            example_path =  "[::::][main][delete][name][confirm y/n]";
            path =          "[::::][main][delete][name][";
            break;
        default:
            break;

    }
    std::cout << example_path << std::endl;
    std::cout << path;
}





// The infile needs to be a variable




// User Input
// Add user Input
// Write to file
// 


// Main
    // User Input
    // Passes user Input Value To the Simulator
