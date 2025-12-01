#include <iostream>
#include <fstream>
#include <filesystem>
#include <limits>
#include <string>
#include <vector>


enum class MenuState {
    MAIN,
    CREATE_NAME,
    CREATE_SIZE,
    DELETE_NAME,
    DELETE_CONFIRM,
    PROJECT,
    EXIT
};

std::vector<std::string> build_project_list();
void ensure_directory(std::filesystem::path folder_path);
void display_menu();
void display_path(MenuState state);
MenuState create_project(std::string name, std::string size);
MenuState delete_project(std::string name, std::string confirmation);

int main(int argc, char *argv[]){
    static const std::filesystem::path user_projects_path = "user_projects";
    MenuState state = MenuState::MAIN;

    std::string selection;
    std::string input;
    std::string name;
    std::string size;
    std::string confirmation;

    std::vector<std::string> project_list = build_project_list();
    

    // Should be able to do setup tasks here.
    // Create project directory on every boot if it doesn't exists
    ensure_directory(user_projects_path);

    // NEXT TIME: I'm working on getting the user projects list working
        // Need to actually define the build_project_list()
            // It might need to take in a filepath as a parameter
            // Inside the filepath/dir look for the text file and open it in read mode.
                // If it doesn't exist create it? and/or return an empty list.
        // Also thinking about having a setup style function that only runs the first time the program opens.
            // You would set a flag from 0 to 1 in a build data text file so that it doesn't run again.
            // It would setup all this directory stuff.
            // But then if a text file is deleted you would still want to have checks and balances that recreate them. So, maybe unnecessary.
    // WHAT I REALIZED: Maybe use project class? Create objects? Haven't researched it yet tho.




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
        // if input is a project name OR number
        for (int i = 0; i < project_list.size(); i++){
            if (input == project_list[i]){
                state = MenuState::PROJECT;
                // Determine if projects have states
            }
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
                state = create_project(name, size);
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

void ensure_directory(std::filesystem::path folder_path){
    if (!exists(folder_path)) {
        std::filesystem::create_directory(folder_path);
    }
}

MenuState create_project(std::string name, std::string size){
    // Ensure name uniqueness?
        // To know if this is important I need to decide whether or not the user can just type their "project name" as a reserved keyword and navigate to it.
            // To decide, I need to know if I can create these reserved keywords at runtime.

    


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
        case MenuState::PROJECT:
            example_path =  "[::::][main][project][hours]";
            path =          "[::::][main][project][";
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
