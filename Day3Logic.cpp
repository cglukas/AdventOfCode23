#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>


struct Number{
    std::string str;
    int row;
    int start;
    int end;
};


std::ostream& operator<<(std::ostream& os, const Number& num){
    os << "Number(" << num.str << ", Row: " << num.row << ", " << num.start << "-" << num.end <<")";
    return os;
}

struct Star{
    bool is_gear;
    int row;
    int column;
};


std::ostream& operator<<(std::ostream& os, const Star& star){
    std::string type = star.is_gear ? "GearStar"  : "Start";
    os << type << "(" << star.row << "," << star.column << ")";
    return os;
}


/// @brief Check if a letter is a digit.
/// @param letter the letter to check. 
/// @return true if the letter is a digit.
bool isNumber(const char letter);

/// @brief Fetch all numbers and special characters (aka stars) from the input text.
/// @param puzzle_input text with numbers and stars.
/// @param stars empty vector that will be filled with found stars.
/// @param all_numbers 2d vector of numbers that will be filled with found numbers. First dimension will be the row followed by the entries of the line.
void fetchNumbersAndStars(const std::string &puzzle_input, std::vector<Star> &stars, std::vector<std::vector<Number>> &all_numbers);

/// @brief Fetch all numbers and stars of the text line.
/// @param line text line with numbers and stars.
/// @param row current row number for defining the row attribute of found stars.
/// @param stars vector of stars where new found stars are added.
/// @return vector of all numbers of the current line. Ordered by appearance.
std::vector<Number> processLine(const std::string &line, const int row, std::vector<Star> &stars);

/// @brief Compute the result of the puzzle.
/// 
/// This will sum up all numbers that are adjacent to a star by one step in the grid.
/// Also diagonal steps are included so that the result of this input would add up to 10:
/// ..1
/// .#1
/// 8..
///
/// @param stars vector of stars that will be used for the computation.
/// @param all_numbers 2d vector of numbers. First dimension needs to be the row followed by the entries of the line.
/// @return sum of the numbers that are adjacent to any star.
int computeResult(const std::vector<Star> &stars, const std::vector<std::vector<Number>> &all_numbers);

/// @brief Print counts of found Stars and GearStars.
/// @param stars vector of all stars.
void printStarInfo(const std::vector<Star> &stars);

/// @brief Process the input and print out the result.
/// @param puzzle_input input text.
void solve_day3(const std::string puzzle_input)
{
    std::vector<std::vector<Number>> all_numbers;
    std::vector<Star> stars;

    fetchNumbersAndStars(puzzle_input, stars, all_numbers);    
    printStarInfo(stars);
    int result = computeResult(stars, all_numbers);
    std::cout << "The result is: "<< result << "\n";
}


int main(int argc, char **argv){
    std::cout << "Start" << "\n";
    std::string inputFile("../../Day3TestInput.txt");
    if(argc == 2){
        inputFile = argv[1];
    }
    solve_day3(inputFile);
    std::cout << "Finished\n";
    return 0;
}


int computeResult(const std::vector<Star> &stars, const std::vector<std::vector<Number>> &all_numbers)
{
    int all_found_numbers = 0;
    for(int p=0; p<all_numbers.size(); p++){
        all_found_numbers += all_numbers[p].size();
    }
    std::cout << "Found Numbers: " << all_found_numbers << "\n";

    int result = 0;
    int star_result;
    int min_request = 0;
    int max_request = 0;
    for (int i = 0; i < stars.size(); i++)
    {
        Star curr_star = stars[i];
        // std::cout << curr_star << "\n";

        min_request = std::max(0, curr_star.row - 1);
        max_request = std::min(curr_star.row + 1, (int)all_numbers.size()-1);
        star_result = 0;
        // std::cout << min_request << "-" << max_request << "\n";
        for (int row = min_request; row <= max_request; row++)
        {
            // std::cout << "Row: " << row << "\n";
            auto numbers = all_numbers[row];
            for (int k = 0; k < numbers.size(); k++)
            {
                Number num = numbers[k];
                if (
                    (curr_star.column == num.start || curr_star.column == num.start+1 || curr_star.column == num.start -1) ||
                    (curr_star.column == num.end || curr_star.column == num.end+1 || curr_star.column == num.end -1)
                    )
                {
                    // std::cout << num << "\n";
                    star_result += std::stoi(num.str);
                }
            }
        }
        // std::cout << "Star Result: " << star_result << "\n";
        result += star_result;
    }
    return result;
}

void printStarInfo(const std::vector<Star> &stars)
{
    int raw_stars = 0;
    int gear_stars = 0;
    for (int i = 0; i < stars.size(); i++)
    {
        Star s = stars[i];
        if (s.is_gear)
        {
            gear_stars++;
        }
        else
        {
            raw_stars++;
        }
    }
    std::cout << "Stars: " << raw_stars << "\n";
    std::cout << "GearStars: " << gear_stars << "\n";
}

bool isNumber(const char letter){
    char *numbers = "0123456789";
    while (*numbers != '\0') {
        if (letter == *numbers ){
            return true;
        }
        numbers++;
    }
    return false;
}

void fetchNumbersAndStars(const std::string &puzzle_input, std::vector<Star> &stars, std::vector<std::vector<Number>> &all_numbers)
{

    int row = 0;

    std::ifstream raw_data(puzzle_input);

    if(!raw_data.is_open()){
        std::cout << "Could not open file.\n";
        return;
    }

    for (std::string line; std::getline(raw_data, line);)
    {
        std::vector<Number> row_numbers = processLine(line, row, stars);
        all_numbers.push_back(row_numbers);
        row++;
    }
}

std::vector<Number> processLine(const std::string &line, const int row, std::vector<Star> &stars)
{
    std::vector<Number> row_numbers = {};

    Number *n;
    bool writingNumber = false;
    char curr_char;

    for (int i = 0; i < line.size(); i++)
    {
        curr_char = line[i];

        if (isNumber(curr_char))
        {
            if (!writingNumber)
            {
                n = new Number();
                n->start = i;
                n->row = row;
                writingNumber = true;
            }
            n->str += curr_char;
            continue;
        }

        if (writingNumber)
        {
            n->end = i - 1;
            row_numbers.push_back(*n);
            writingNumber = false;
        }

        if (curr_char == '.')
        {
            continue;
        }
        bool is_gear = curr_char == '*';
        Star s{is_gear, row, i};
        stars.push_back(s);
    }

    // If the line ends and we are still writing a number, we need to finish it and add it too.
    if (writingNumber)
    {
        n->end = (int)line.size() - 1;
        row_numbers.push_back(*n);
        writingNumber = false;
    }

    return row_numbers;
}
