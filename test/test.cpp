#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <tuple>
#include <stdlib.h>


void display_frame(const std::vector<std::vector<char>>& hourglass);
bool calculate_frame(std::vector<std::vector<char>>& hourglass, bool flag_dam);
std::vector<std::vector<char>> read_hourglass();
void remove_dam(std::vector<std::vector<char>>& hourglass);


int main(int argc, char *argv[]){
    std::vector<std::vector<char>> project = read_hourglass();

    size_t sand_input = 5;
    size_t counter = 0;
    bool flag_add_dam = false;
    do {
        if (counter == 0){
            // remove the dam.
            remove_dam(project);
        }
        else if (counter == sand_input){
            // add the dam back in.
            flag_add_dam = true;
        }
        counter += 1;
        system("cls");
        display_frame(project);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } while (calculate_frame(project, flag_add_dam));
    // display_frame one more time probably depending on how you close out of calculate frame.
    // display_frame(project);
    // write_hourglass()
    std::cin.get();

}

// calculate_frame needs to receive a flag regarding the dam status.
// 


void display_frame(const std::vector<std::vector<char>>& hourglass){
    // string buffer to avoid cout flush cost?
    std::string buffer;
    for (const auto& row : hourglass){
        buffer.append(row.begin(), row.end());
        buffer.push_back('\n');
    }
    std::cout << buffer;
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
                // no longer need this since we are not toggling, only removing.
                // else if (hourglass[i][j-1] == ' '){
                //     dam_row = i;
                //     dam_column = j-1;
                //     hourglass[dam_row][dam_column] = 'V';
                //     break;
                // }
            }
            j++;
        }
    }
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



std::vector<std::vector<char>> read_hourglass(){
    std::vector<std::vector<char>> project;
    std::ifstream infile;
    infile.open("../templates/template_100.txt");
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