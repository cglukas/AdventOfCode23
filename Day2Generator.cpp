
#include "DDImage/PixelIop.h"
#include "DDImage/Knobs.h"
#include "DDImage/Row.h"
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <regex>


using namespace DD::Image;
struct Color{
    int rgb[3] = {0, 0, 0};
};
struct Game {
    int game_num = 0;
    std::vector<Color> colors = {};
};

class Day2Generator: public PixelIop {
public:
    Day2Generator(Node *node) : PixelIop(node) {};

    void pixel_engine(const Row &in, int y, int l, int r, ChannelMask, Row &) override;
    void _validate(bool for_real) override;
    static const Iop::Description d;
    const char *Class() const override {return d.name;} 
    const char *node_help() const override {return "Second puzzle for advend of code."; }
    void knobs(Knob_Callback) override;
private:
    const char *text = nullptr;
    Format my_format = Format(20, 1, 1);
    std::map<int, Game> games;
    void process_text();

    std::map<std::string, int> color_to_channel = {
        {"red", 0}, {"green", 1}, {"blue", 2}
    };
    std::regex game_rgx = std::regex("Game ([0-9]+):.*");
    std::regex color_rgx = std::regex("([0-9]+) (red|green|blue)");
};

void Day2Generator::pixel_engine(const Row &in, int y, int l, int r, ChannelMask channels, Row &out)
{   
    if(games.find(y) == games.end()){
        return;
    }

    Game g = games[y];

    foreach(z, channels){
        const int color_idx = colourIndex(z);
        const float *inptr= in[z];
        float *outptr = out.writable(z);
        std::size_t colors(g.colors.size());
        for (int x = l; x < r; x++){
            if(x < colors){
                outptr[x] = (float) g.colors[x].rgb[color_idx];
            }else{
                outptr[x] = 0.0f;
            }
        }
    }
    
}

void Day2Generator::_validate(bool for_real)
{
    process_text();
    info_.full_size_format(my_format);
    info_.format(my_format);
    info_.channels(Mask_RGB);
    info_.set(format());
}

void Day2Generator::knobs(Knob_Callback f)
{
    Multiline_String_knob(f, &text, "puzzle_input");
}

void Day2Generator::process_text()
{
    int h = (int) strlen(text);
    std::stringstream sstream(text);
    int game_count = 0; 
    std::string line;    
    std::smatch game_id;
    Color default_color;                
    std::sregex_iterator line_end;
    while(std::getline(sstream, line)){
        std::regex_match(line, game_id, game_rgx);
        if(game_id.empty()){
           continue;
        }
        Game g;
        g.game_num = std::stoi(game_id[1]);
        std::stringstream color_stream(line);
        std::string color_segment;
        while(std::getline(color_stream, color_segment, ';')){
            std::sregex_iterator color_iter(color_segment.begin(), color_segment.end(), color_rgx);
            Color color;
            
            while(color_iter != line_end){
                std::smatch match = *color_iter;
                int c_idx = color_to_channel[match[2]];
                color.rgb[c_idx] = std::stoi(match[1]);  
                color_iter++;      
            }
            if(color.rgb != default_color.rgb){
                g.colors.push_back(color);
            }
        }        
        game_count= g.game_num>game_count ? g.game_num : game_count;
        games[g.game_num] = g;
    }
    my_format= Format(20, game_count+1, 1);
}

static Iop* build(Node* node) { return new Day2Generator(node); }
const Iop::Description Day2Generator::d("Day2Generator", "AdventOfCode/Day2Generator", build);

