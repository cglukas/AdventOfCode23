// Nuke engine solution for the day one puzzle of the advent of code.
// https://adventofcode.com/2023/day/1

#include "DDImage/Iop.h"
#include "DDImage/Knobs.h"
#include "DDImage/MetaData.h"
#include <string>
#include <sstream>
#include <regex>
#include <stdio.h>

using namespace DD::Image;

class Day1: public Iop {

public:
    Day1(Node *node) : Iop(node) {
        _meta = Iop::_fetchMetaData(nullptr);
    };
    static const Iop::Description d;
    const char *Class() const override {return d.name;} 
    const char *node_help() const override {return "First puzzle for advend of code."; }
    void engine(int y, int x, int r, ChannelMask, Row &) override {};
    void knobs(Knob_Callback) override;
    const MetaData::Bundle& _fetchMetaData(const char* keyname) override;
    
    void _validate(bool) override;
private:
    const char *text = nullptr;
    MetaData::Bundle _meta;
    int solve_the_puzzle();
};


void Day1::knobs(Knob_Callback f)
{
    Multiline_String_knob(f, &text, "puzzle_input");
}

const MetaData::Bundle &Day1::_fetchMetaData(const char *keyname)
{
    return _meta;
}

void Day1::_validate(bool)
{    
    _meta.setData("CalibrationValue", solve_the_puzzle());
}

int Day1::solve_the_puzzle()
{
    if(sizeof(text) == 0){
        return 0;
    }
    

    char const *digits = "0123456789";

    int sum = 0;

    std::string words = "one|two|three|four|five|six|seven|eight|nine";
    std::regex word_rgx(words);

    // Reversed regex will be used for right to left search to extract
    // substings of words: E.g. twone should match the "one" and not the "two"
    // if searched from the right.
    std::string words_reversed = words;
    std::reverse(words_reversed.begin(), words_reversed.end());
    std::regex word_reverse_rgx(words_reversed);

    // End state of any regex iterator used for ending the loop.
    std::regex_iterator<std::string::iterator> line_end;

    // Map for converting the regex matches back to number digits.
    std::map<std::string, std::string> word_to_digit{
        {"one", "1"}, 
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six","6"},
        {"seven","7"},
        {"eight","8"},
        {"nine","9"}
    };
    
    std::stringstream text_stream(text);
    for(std::string line; std::getline(text_stream, line); ){

        std::size_t left = line.find_first_of(digits);
        std::size_t right = line.find_last_of(digits);
        
        std::string a = "0";
        std::string b = "0";

        if(left != std::string::npos){
            a = line.substr(left, 1);
        }

        if(right != std::string::npos){
            b = line.substr(right, 1);
        }else{
            // Right needs to be set to zero so that the word count can be 
            // higher (aka more on the right side).
            right = 0;
        }

        std::regex_iterator<std::string::iterator> line_iter(line.begin(), line.end(), word_rgx);

        while(line_iter != line_end){
            if ((std::size_t) line_iter->position() < left){
                a=word_to_digit[line_iter->str()];
                break;
            }
            line_iter++;
        }

        std::string line_reversed = line;
        std::reverse(line_reversed.begin(), line_reversed.end());
        std::cout << line_reversed << "\n";

        std::regex_iterator<std::string::iterator> line_reverse_iter(line_reversed.begin(), line_reversed.end(), word_reverse_rgx);
        std::size_t reversed_right = line.length()-right;
        while(line_reverse_iter != line_end){
            if((std::size_t) line_reverse_iter->position() < reversed_right){
                std::string rev_word = line_reverse_iter->str();
                std::reverse(rev_word.begin(), rev_word.end());
                b=word_to_digit[rev_word];
                break;
            }
            line_reverse_iter++;
        }
        std::cout << a << "-" << b << ": "<< line << "\n";
        sum += std::stoi(a+b);

    }

    return sum;
}   

static Iop* build(Node* node) { return new Day1(node); }
const Iop::Description Day1::d("Day1", "AdventOfCode/Day1", build);
