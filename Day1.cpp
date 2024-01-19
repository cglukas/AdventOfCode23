// Nuke engine solution for the day one puzzle of the advent of code.
// https://adventofcode.com/2023/day/1

#include "DDImage/Iop.h"
#include "DDImage/Knobs.h"
#include "DDImage/MetaData.h"
#include <string>
#include <sstream>


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
    std::stringstream text_stream(text);

    int sum = 0;
    for(std::string line; std::getline(text_stream, line); ){

        std::size_t left = line.find_first_of(digits);
        std::size_t right = line.find_last_of(digits);
        std::string a = line.substr(left, 1);
        std::string b = line.substr(right, 1);

        sum += std::stoi(a+b);

    }

    return sum;
}   

static Iop* build(Node* node) { return new Day1(node); }
const Iop::Description Day1::d("Day1", "AdventOfCode/Day1", build);
