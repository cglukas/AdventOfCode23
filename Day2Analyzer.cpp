
#include "DDImage/Iop.h"
#include "DDImage/Knobs.h"
#include "DDImage/Row.h"
#include "DDImage/Thread.h"
#include "DDImage/Tile.h"
#include <string.h>
#include <stdio.h>


using namespace DD::Image;


class Day2Analyzer: public Iop {
public:
    Day2Analyzer(Node *node) : Iop(node) {
        puzzleResult = 0;
    };

    void engine(int y, int x, int r, ChannelMask, Row &) override;
    static const Iop::Description d;
    const char *Class() const override {return d.name;} 
    const char *node_help() const override {return "Second puzzle for advend of code."; }
    void knobs(Knob_Callback) override;
    void _open() override;
    void _validate(bool for_real) override;

private:
    float max_values[3];
    float puzzleResult;
    bool _firstTime;
    std::vector<int> too_high_rows;
    Lock _lock;
};

void Day2Analyzer::engine(int y, int x, int r, ChannelMask channels, Row &row)
{   
    {
    Guard guard(_lock);
        if(_firstTime){
            // do anaylsis.
            Format format = input0().format();

            // these useful format variables are used later
            const int fx = format.x();
            const int fy = format.y();
            const int fr = format.r();
            const int ft = format.t();

            ChannelSet readChannels = input0().info().channels();

            Interest interest( input0(), fx, fy, fr, ft, readChannels, true );
            interest.unlock();

            puzzleResult = 0;
            
            for ( int ry = fy; ry < ft; ry++) {
                progressFraction( ry, ft - fy );
                Row row( fx, fr );
                row.get( input0(), ry, fx, fr, readChannels );
                if ( aborted() )
                    return;
                bool is_too_high = false;
                foreach( z, readChannels ) {                    
                    const float max_allowed_value = max_values[colourIndex(z)];
                    const float *CUR = row[z] + fx;
                    const float *END = row[z] + fr;
                    while ( CUR < END ) {
                        if (*CUR > max_allowed_value){
                            is_too_high = true;
                        }
                        CUR++;
                    }
                    std::cout << "ColorIndex" << colourIndex(z) << ":"<<z<<"\n";
                    std::cout << max_allowed_value << "->" << z << "\n";
                }
                std::cout << "ry: " << ry << "-> to high: " << is_too_high << "\n";

                if(!is_too_high){
                    puzzleResult+= ry;
                }else{
                    too_high_rows.push_back(ry);
                }
               
            }
            _firstTime = false;
            std::cout << "The result is: " << puzzleResult << "\n";
        }
    } // end lock

    Row in( x,r);
    in.get( input0(), y, x, r, channels );
    if ( aborted() )
        return;
    
    float color = std::find(too_high_rows.begin(), too_high_rows.end(), y) != too_high_rows.end() ? 1.0f : 0.0f;
    foreach( z, channels ) {

        float *CUR = row.writable(z) + x;
        const float* inptr = in[z] + x;
        const float *END = row[z] + r;
        while ( CUR < END ) {
            inptr++;
            *CUR++ = color;
        }
    }
}

void Day2Analyzer::knobs(Knob_Callback f)
{
    Color_knob(f, max_values, "max_values");
}

void Day2Analyzer::_open()
{
    _firstTime = true;
}

void Day2Analyzer::_validate(bool for_real)
{
    copy_info();
}

static Iop* build(Node* node) { return new Day2Analyzer(node); }
const Iop::Description Day2Analyzer::d("Day2Analyzer", "AdventOfCode/Day2Analyzer", build);

