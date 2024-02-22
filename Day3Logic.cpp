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
    int row;
    int column;
};


std::ostream& operator<<(std::ostream& os, const Star& star){
    os << "Star(" << star.row << "," << star.column << ")";
    return os;
}




/// @brief Check if a letter is a digit.
/// @param letter the letter to check. 
/// @return true if the letter is a digit.
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

void fetchNumbersAndStars(const std::string &puzzle_input, std::vector<Star> &stars, std::vector<std::vector<Number>> &all_numbers);

int computeResult(std::vector<Star> &stars, std::vector<std::vector<Number>> &all_numbers);

std::string solve_day3(const std::string puzzle_input)
{
    std::vector<std::vector<Number>> all_numbers;
    std::vector<Star> stars;

    fetchNumbersAndStars(puzzle_input, stars, all_numbers);    
    int result = computeResult(stars, all_numbers);
    std::cout << "The result is: "<< result << "\n";

    return "FINISHED";
}

int computeResult(std::vector<Star> &stars, std::vector<std::vector<Number>> &all_numbers)
{
    std::cout << "Stars: " << stars.size() << "\n";

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

void fetchNumbersAndStars(const std::string &puzzle_input, std::vector<Star> &stars, std::vector<std::vector<Number>> &all_numbers)
{
    Number *n;
    bool writingNumber = false;
    char curr_char;

    int row = 0;

    std::ifstream raw_data(puzzle_input);

    if(!raw_data.is_open()){
        std::cout << "Could not open file.\n";
        return;
    }

    for (std::string line; std::getline(raw_data, line);)
    {
        std::vector<Number> row_numbers = {};
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

            Star s{row, i};
            stars.push_back(s);
        }

        if (writingNumber)
        {
            n->end = (int) line.size() - 1;
            row_numbers.push_back(*n);
            writingNumber = false;
        }

        all_numbers.push_back(row_numbers);
        row++;
    }
}

int main(int argc, char **argv){
    std::cout << "Start" << "\n";
    std::string inputFile("../../Day3TestInput.txt");
    if(argc == 2){
        inputFile = argv[1];
    }
    std::cout << solve_day3(inputFile) << "\n";
    std::cout << inputFile << "\n";
    return 0;
}