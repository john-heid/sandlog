#include <iostream>
#include <fstream>
#include <filesystem>
#include <limits>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <tuple>
#include <stdlib.h>
#include <cctype> // isalphanumeric

// Folder
static const std::filesystem::path USER_DATA = "data";
static const std::filesystem::path PROJECTS_ACTIVE = USER_DATA / "projects_active";
static const std::filesystem::path PROJECTS_ARCHIVE = USER_DATA / "projects_archive";
static const std::filesystem::path TEMPLATES = "templates";

// Filenames
static const std::string FILENAME_PROJECT_LIST = "project_list.txt";
static const std::string FILENAME_TEMPLATE_100 = "template_100.txt";
static const std::string FILENAME_TEMPLATE_400 = "template_400.txt";
static const std::string FILENAME_TEMPLATE_MENU = "template_menu.txt";

enum class MenuState {
    MAIN,
    ARCHIVE_NAME,
    ARCHIVE_CONFIRM,
    ARCHIVE_DISPLAY,
    CREATE_NAME,
    CREATE_SIZE,
    CREATE_UNIT,
    CREATE_FROM_TEMPLATE,
    DELETE_NAME,
    DELETE_CONFIRM,
    PROJECT_ADD,
    PROJECT_DISPLAY,
    EXIT
};

std::vector<std::string> build_project_list(MenuState curr_state);
void ensure_directory();
void write_templates();
void display_menu(MenuState state, std::vector<std::string> project_list, const std::vector<std::vector<char>>& hourglass);
void display_path(MenuState state);
MenuState create_project(std::string name, std::string size, std::string unit);
MenuState delete_project(std::string name, std::string confirmation);
MenuState archive_project(std::string name, std::string confirmation);
void write_hourglass(MenuState state, std::string name, std::vector<std::vector<char>>& hourglass);
std::vector<std::vector<char>> read_hourglass(MenuState state, std::string name);
bool calculate_frame(std::vector<std::vector<char>>& hourglass, bool flag_dam);
void remove_dam(std::vector<std::vector<char>>& hourglass);
MenuState add_hours(std::string size);



int main(int argc, char *argv[]){
    MenuState state = MenuState::MAIN;

    std::string input;
    std::string name = "start_value";
    std::string size;
    std::string confirmation;
    std::string unit;

    ensure_directory();
    write_templates();

    while (state != MenuState::EXIT) {
        std::vector<std::vector<char>> hourglass = read_hourglass(state, name);
        if (hourglass.size() == 0){
            hourglass = read_hourglass(state, "start_value");
        }
        std::vector<std::string> project_list = build_project_list(state);
        if (state == MenuState::PROJECT_DISPLAY){
            size_t sand_input = std::stoi(size);
            size_t counter = 0;
            bool flag_add_dam = false;
            do {
                if (counter == 0){
                    // remove the dam.
                    remove_dam(hourglass);
                }
                else if (counter == sand_input){
                    // add the dam back in.
                    flag_add_dam = true;
                }
                counter += 1;
                system("cls");
                display_menu(state, project_list, hourglass);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            } while (calculate_frame(hourglass, flag_add_dam));
            write_hourglass(state, name, hourglass);
            state = MenuState::MAIN;
        }

        system("cls");
        display_menu(state, project_list, hourglass);
        display_path(state);

        std::getline(std::cin, input);

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
        if (input == "archive"){
            state = MenuState::ARCHIVE_NAME;
            continue;
        }
        if (state != MenuState::DELETE_NAME && state != MenuState::ARCHIVE_NAME && state != MenuState::ARCHIVE_CONFIRM){
            bool continue_flag = false;
            for (int i = 0; i < project_list.size(); i++){
                if (input + ".txt" == project_list[i]){
                    name = input;
                    state = MenuState::PROJECT_ADD;
                    continue_flag = true;
                }
            }
            if (continue_flag == true){
                continue;
            }
        }
        else if (state == MenuState::ARCHIVE_NAME || state == MenuState::ARCHIVE_DISPLAY){
            bool continue_flag = false;
            for (int i = 0; i < project_list.size(); i++){
                if (input + ".txt" == project_list[i]){
                    name = input;
                    state = MenuState::ARCHIVE_NAME;
                    continue_flag = true;
                }
            }
            if (continue_flag == true){
                continue;
            }

        }
        // States and some local commands
        switch (state) {
            case MenuState::MAIN:
                break;
            case MenuState::ARCHIVE_NAME:
                name = input;
                state = MenuState::ARCHIVE_CONFIRM;
                break;
            case MenuState::ARCHIVE_CONFIRM:
                confirmation = input;
                state = archive_project(name, confirmation);
                break;
            case MenuState::CREATE_NAME:
                name = input;
                state = MenuState::CREATE_SIZE;
                break;
            case MenuState::CREATE_SIZE:
                size = input;
                state = MenuState::CREATE_UNIT;
                break;
            case MenuState::CREATE_UNIT:
                unit = input;
                state = create_project(name, size, unit);
                break;
            case MenuState::DELETE_NAME:
                name = input;
                state = MenuState::DELETE_CONFIRM;
                break;
            case MenuState::DELETE_CONFIRM:
                confirmation = input;
                state = delete_project(name, confirmation);
                break;
            case MenuState::PROJECT_ADD:
                size = input;
                state = add_hours(size);
                break;
            default:
                break;
        }
    }
    std::cout << "Closing Application";
    return 0;
}

std::vector<std::string> build_project_list(MenuState curr_state){
    std::filesystem::path state_dir;
    if (curr_state == MenuState::ARCHIVE_NAME || curr_state == MenuState::ARCHIVE_CONFIRM) {
        state_dir = PROJECTS_ARCHIVE;
    }
    else {
        state_dir = PROJECTS_ACTIVE;
    }
    std::filesystem::path target_dir = state_dir;
    std::vector<std::string> project_list;
    for (const auto& entry : std::filesystem::directory_iterator{target_dir}) {
        project_list.push_back(entry.path().filename().string());
    }
    return project_list;
}

void ensure_directory(){
    if (!exists(USER_DATA)) {
        std::filesystem::create_directory(USER_DATA);
        std::filesystem::create_directory(PROJECTS_ACTIVE);
        std::filesystem::create_directory(PROJECTS_ARCHIVE);
    }
    if (!exists(TEMPLATES)) {
        std::filesystem::create_directory(TEMPLATES);
    }

}

void write_templates(){
    std::vector<std::string> hourglass_template = {
        "| |===================| |",
        " \\\\###################//",
        "  \\\\#################//",
        "   \\\\###############//",
        "    \\\\#############//",
        "     \\\\###########//",
        "      \\\\#########//",
        "       \\\\#######//",
        "        \\\\#####//",
        "         \\\\###//",
        "          \\\\#//",
        "           )V(~n",
        "          // \\\\~u",
        "         //   \\\\",
        "        //     \\\\",
        "       //       \\\\",
        "      //         \\\\",
        "     //           \\\\",
        "    //             \\\\",
        "   //               \\\\",
        "  //                 \\\\",
        " //                   \\\\",
        "| |```````````````````| |"         
    };
    std::ofstream outfile(TEMPLATES / FILENAME_TEMPLATE_100);
    if (outfile.is_open()) {
        for (const std::string& row : hourglass_template) {
            outfile << row << '\n';
        }
    }
    outfile.close();
    hourglass_template = {
        "| |========================================| |",
        " \\\\#######################################//",//39
        "  \\\\#####################################//",
        "   \\\\###################################//",
        "    \\\\#################################//",
        "     \\\\###############################//",
        "      \\\\#############################//",
        "       \\\\###########################//",
        "        \\\\#########################//",
        "         \\\\#######################//",
        "          \\\\#####################//",
        "           \\\\###################//",//19
        "            \\\\#################//",
        "             \\\\###############//",
        "              \\\\#############//",
        "               \\\\###########//",
        "                \\\\#########//",
        "                 \\\\#######//",
        "                  \\\\#####//",
        "                   \\\\###//",
        "                    \\\\#//",
        "                     )V(~n",
        "                    // \\\\~u",
        "                   //   \\\\",
        "                  //     \\\\",
        "                 //       \\\\",
        "                //         \\\\",
        "               //           \\\\",
        "              //             \\\\",
        "             //               \\\\",
        "            //                 \\\\",
        "           //                   \\\\",
        "          //                     \\\\",
        "         //                       \\\\",
        "        //                         \\\\",
        "       //                           \\\\",
        "      //                             \\\\",
        "     //                               \\\\",
        "    //                                 \\\\",
        "   //                                   \\\\",
        "  //                                     \\\\",
        " //                                       \\\\",
        "| |```````````````````````````````````````| |"         
    };
    outfile.open(TEMPLATES / FILENAME_TEMPLATE_400);
    if (outfile.is_open()) {
        for (const std::string& row : hourglass_template) {
            outfile << row << '\n';
        }
    }

    outfile.close();
    std::vector<std::string> menu_template = {
        "#####~ SandLog ~#####",
        "#####################",
        "[create]             ",
        "[archive]            ",
        "[delete]             ",
        "[exit]               ",
        "#####################"
    };

    outfile.open(TEMPLATES / FILENAME_TEMPLATE_MENU);
    if (outfile.is_open()) {
        for (const std::string& row : menu_template) {
            outfile << row << '\n';
        }
    }
    outfile.close();
}

MenuState create_project(std::string name, std::string size, std::string unit){
    // input cleaning here
    // NAME
    // STILL NEED TO MAKE SURE NAME IS UNIQUE
    std::vector<std::string> active_project_list = build_project_list(MenuState::MAIN);
    for (std::string existing_name : active_project_list){
        if (name + ".txt" == existing_name){
            return MenuState::CREATE_NAME;
            // This can never happen tho since if the name is typed it jumps to that project
        }
    }
    std::vector<std::string> archive_project_list = build_project_list(MenuState::ARCHIVE_NAME);
    for (std::string existing_name : archive_project_list){
        if (name + ".txt" == existing_name){
            return MenuState::CREATE_NAME;
        }
    }




    if (name.length() > 35){
        return MenuState::CREATE_NAME;
    }
    std::string invalid_inputs = "/\\.<>\":()[]{}%&!@#$^*,?';|+=`~ ";
    for (char c : name){
        if (invalid_inputs.find(c) != std::string::npos){
            return MenuState::CREATE_NAME;
        }
    }
    // SIZE
    if (size != "100" && size != "400"){
        return MenuState::CREATE_SIZE;
    }
    // UNIT
    if (unit.length() > 35){
        return MenuState::CREATE_UNIT;
    }


    std::string template_file;
    if (size == "100"){
        template_file = FILENAME_TEMPLATE_100;
    }
    else if (size == "400"){
        template_file = FILENAME_TEMPLATE_400;
    }

    std::vector<std::vector<char>> new_hourglass = read_hourglass(MenuState::CREATE_FROM_TEMPLATE, template_file);
    for (size_t i = 0; i < new_hourglass.size(); i++){
        for (size_t j = 0; j < new_hourglass[i].size(); j++){
            if (new_hourglass[i][j] == 'n'){
                // name location
                new_hourglass[i].pop_back();
                for (char c : name){
                    new_hourglass[i].push_back(c);
                }
                break;
            }
            if (new_hourglass[i][j] == 'u'){
                // unit location
                new_hourglass[i].pop_back();
                for (char c : unit){
                    new_hourglass[i].push_back(c);
                }
                break;
            }
        }
    }

    write_hourglass(MenuState::MAIN, name, new_hourglass);

    return MenuState::MAIN; // could be cool to return the menustate corresponding with the creating project.
    // Which now would be PROJECT_ADD I think.
}

MenuState delete_project(std::string name, std::string confirmation){
    std::string file = name + ".txt";
    if (confirmation == "y"){
        std::filesystem::remove(PROJECTS_ACTIVE / file);
        return MenuState::MAIN;
    }
    else if (confirmation == "n"){
        return MenuState::MAIN;
    }
    else {
        return MenuState::DELETE_CONFIRM;
    }
}

MenuState archive_project(std::string name, std::string confirmation){
    std::string file = name + ".txt";
    if (confirmation == "y"){
        // Copy the file into the archive directory
        try {
            std::filesystem::copy_file(PROJECTS_ACTIVE / file, PROJECTS_ARCHIVE / file, std::filesystem::copy_options::none);
            std::filesystem::remove(PROJECTS_ACTIVE / file);
            return MenuState::MAIN;
        }
        catch (...){
            return MenuState::ARCHIVE_NAME;
        }
    }
    else if (confirmation == "n"){
        return MenuState::MAIN;
    }
    else {
        return MenuState::ARCHIVE_CONFIRM;
    }
}

void write_hourglass(MenuState state, std::string name, std::vector<std::vector<char>>& hourglass) {
    std::ofstream outfile;
    name = name + ".txt";
    if (state == MenuState::ARCHIVE_NAME){
        outfile.open(PROJECTS_ARCHIVE / name);
    }
    else{
        outfile.open(PROJECTS_ACTIVE / name);
    }

    if (outfile.is_open()) {
        for (const auto& row : hourglass) {
            for (char c : row){
                outfile << c;
            }
            outfile << '\n';
        }
    }
    outfile.close();
}

void display_menu(MenuState state, std::vector<std::string> project_list, const std::vector<std::vector<char>>& hourglass){
    std::ifstream infile;
    // Build menu+projects list
    size_t biggest_length = 0;
    std::vector<std::string> menu;
    infile.open(TEMPLATES / FILENAME_TEMPLATE_MENU);
    if (infile.is_open()) {
        std::string row;
        while (std::getline(infile, row)) {
            menu.push_back(row);
            if (row.length() > biggest_length){
                biggest_length = row.length();
            }
        }
        for (std::string proj : project_list){
            menu.push_back(proj);
            if (proj.length() > biggest_length){
                biggest_length = proj.length();
            }
        }
    }
    
    // Output the display         // For each row it needs menu + offset + project
    size_t num_rows = std::max(menu.size(), hourglass.size());
    // Add the difference of rows as 'padding'  to the front of menu
    int vertical_padding = hourglass.size() - menu.size();
    if (vertical_padding < 0){
        vertical_padding = -1 * vertical_padding;
    }

    // adds empty vertical elements in preparation to receive padding.
    menu.insert(menu.begin(), vertical_padding, "");



    for (size_t i = 0; i < num_rows; i++){
        // Menu
        size_t padding;
        if (i < menu.size()){
            std::cout << menu[i];
            padding = biggest_length - menu[i].length();
        }
        else{
            padding = biggest_length;
        }

        // Padding + constant offset
        const size_t offset = 10;
        size_t total_padding_offset = padding + offset;
        std::cout << std::string(total_padding_offset, ' ');

        // Project
        if (i < hourglass.size()){
            std::string buffer;
            const auto& row = hourglass[i];
            buffer.append(row.begin(), row.end());
            std::cout << buffer;
        }
        std::cout << '\n';
    }
}

void display_path(MenuState state){
    std::string example_path;
    std::string path;

    switch (state) {
        case MenuState::MAIN:
            example_path =  "[::::][main][choice]";
            path =          "[::::][main][";
            break;
        case MenuState::ARCHIVE_NAME:
            example_path =  "[::::][main][archive][name]";
            path =          "[::::][main][archive][";
            break;
        case MenuState::ARCHIVE_CONFIRM:
            example_path =  "[::::][main][archive][name][confirm y/n]";
            path =          "[::::][main][archive][name][";
            break;
        case MenuState::ARCHIVE_DISPLAY:
            example_path =  "[::::][main][archive][name]";
            path =          "[::::][main][archive][";
        case MenuState::CREATE_NAME:
            example_path =  "[::::][main][create][name]";
            path =          "[::::][main][create][";
            break;
        case MenuState::CREATE_SIZE:
            example_path =  "[::::][main][create][name][size]";
            path =          "[::::][main][create][name][";
            break;
        case MenuState::CREATE_UNIT:
            example_path =  "[::::][main][create][name][size][hours/days?]";
            path =          "[::::][main][create][name][size][";
            break;
        case MenuState::DELETE_NAME:
            example_path =  "[::::][main][delete][name]";
            path =          "[::::][main][delete][";
            break;
        case MenuState::DELETE_CONFIRM:
            example_path =  "[::::][main][delete][name][confirm y/n]";
            path =          "[::::][main][delete][name][";
            break;
        case MenuState::PROJECT_ADD:
            example_path =  "[::::][main][project][hours]";
            path =          "[::::][main][project][";
        default:
            break;

    }
    std::cout << example_path << std::endl;
    std::cout << path;
}

std::vector<std::vector<char>> read_hourglass(MenuState state, std::string name){
    std::vector<std::vector<char>> project;
    std::ifstream infile;
    if (name == "start_value"){
        infile.open("templates/template_100.txt");
    }
    else{
        if (state == MenuState::ARCHIVE_NAME || state == MenuState::ARCHIVE_DISPLAY){
            name = name + ".txt";
            infile.open(PROJECTS_ARCHIVE / name);
        }
        else if (state == MenuState::CREATE_FROM_TEMPLATE){
            infile.open(TEMPLATES / name);
        }
        else if (state == MenuState::CREATE_NAME || state == MenuState::CREATE_SIZE || state == MenuState::CREATE_UNIT){
            infile.open("templates/template_100.txt");
        }
        else{
            name = name + ".txt";
            infile.open(PROJECTS_ACTIVE / name);
        }
    }    
    if (infile.is_open()) {
        std::string row;
        while (std::getline(infile, row)) {
            std::vector<char> temp;
            for (char val : row){
                temp.push_back(val);
            }
            project.push_back(temp);
        }
    }
    infile.close();
    return project;
}


bool calculate_frame(std::vector<std::vector<char>>& hourglass, bool flag_dam){
    bool something_moved = false;
    int numrows = static_cast<int>(hourglass.size()) -1;
    
    int dam_row;
    int dam_column;
    if (flag_dam == true){
        for (int i = numrows; i >= 0; i--){
            int j = 0;
            for (char c : hourglass[i]){
                if (c == '('){
                    dam_row = i;
                    dam_column = j-1;
                    break;
                }
                j++;
            }
        }
    }
    for (int i = numrows; i >= 0; i--){
        std::vector<std::tuple<std::string, int, int>> sand_indices;
        for (size_t j = 0; j < hourglass[i].size(); j++){
            if (hourglass[i][j] == '#'){
                if (i+1 <= numrows){
                    int downleft = j-1;
                    int downright = j+1;
                    // directly beneath empty priority
                    if (hourglass[i+1][j] == ' '){
                        std::tuple<std::string, int, int> myTuple("down", i, j);
                        sand_indices.push_back(myTuple);
                    }
                    // diagonal checks
                    else if (hourglass[i+1][downleft] == ' '){
                        std::tuple<std::string, int, int> myTuple("downleft", i, j);
                        sand_indices.push_back(myTuple);
                    }
                    else if (hourglass[i+1][downright] == ' '){
                        std::tuple<std::string, int , int> myTuple("downright", i, j);
                        sand_indices.push_back(myTuple);
                    }
                }
            }
            // If im on the final iteration of the row
            // Also this is where the changes actually get made.
            if (j+1 >= hourglass[i].size()){
                // Look through
                for (size_t k = 0; k < sand_indices.size(); k++){
                    if (std::get<0>(sand_indices[k]) == "down"){
                        if (flag_dam == true && std::get<1>(sand_indices[k]) == dam_row && std::get<2>(sand_indices[k]) == dam_column){
                            hourglass[std::get<1>(sand_indices[k])][std::get<2>(sand_indices[k])] = 'V';
                        }
                        else{
                            hourglass[std::get<1>(sand_indices[k])][std::get<2>(sand_indices[k])] = ' ';
                        }
                        hourglass[std::get<1>(sand_indices[k]) + 1][std::get<2>(sand_indices[k])] = '#';
                        something_moved = true;
                        break;
                    }
                    else if (k + 1 >= sand_indices.size() -1){
                        int rand_choice = std::rand() % sand_indices.size();
                        if (std::get<0>(sand_indices[rand_choice]) == "downleft"){
                            hourglass[std::get<1>(sand_indices[rand_choice])][std::get<2>(sand_indices[rand_choice])] = ' ';
                            hourglass[std::get<1>(sand_indices[rand_choice]) + 1][std::get<2>(sand_indices[rand_choice]) - 1] = '#';
                            something_moved = true;
                            break;
                        }
                        else if (std::get<0>(sand_indices[rand_choice]) == "downright"){
                            hourglass[std::get<1>(sand_indices[rand_choice])][std::get<2>(sand_indices[rand_choice])] = ' ';
                            hourglass[std::get<1>(sand_indices[rand_choice]) + 1][std::get<2>(sand_indices[rand_choice]) + 1] = '#';
                            something_moved = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (something_moved == true){
        return true;
    }
    else {
        return false;
    }
}

void remove_dam(std::vector<std::vector<char>>& hourglass){
    int numrows = static_cast<int>(hourglass.size()) -1;
    int dam_row;
    int dam_column;
    for (int i = numrows; i >= 0; i--){
        int j = 0;
        for (char c : hourglass[i]){
            if (c == '('){
                if (hourglass[i][j-1] == 'V'){
                    dam_row = i;
                    dam_column = j-1;
                    hourglass[dam_row][dam_column] = ' ';
                    break;
                }
                else if (hourglass[i][j-1] == ' '){
                    dam_row = i;
                    dam_column = j-1;
                    hourglass[dam_row][dam_column] = 'V';
                    break;
                }
                // else if # ? might need to return a flag or something
            }
            j++;
        }
    }
}

MenuState add_hours(std::string size){
    // look through the string 'size' for characters that are not 0-9. return
    if (size.empty()){
        return MenuState::PROJECT_ADD;
    }
    for (char c : size){
        if (!std::isdigit(static_cast<unsigned char>(c))){
            return MenuState::PROJECT_ADD;
        }
    }
    return MenuState::PROJECT_DISPLAY;
}