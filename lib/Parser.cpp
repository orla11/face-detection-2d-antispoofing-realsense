#include "../include/Parser.h"
#include <string>

using namespace std;

const string Parser::PROGRAM_DESCRIPTION = "./lidar_realsense";
const string Parser::MODULE_NAME = "RGOS Live Face Recognition Module";

Parser::Parser() : parser(Parser::MODULE_NAME,Parser::PROGRAM_DESCRIPTION) {};

Parser::Parser(const string moduleName_, const string programDescription_)
            : parser(moduleName_,programDescription_) {};

void Parser::initDefaultOptions() {
    this->parser.add_options()
        ("h,help", "Show this menu")
        ("c,confidence","Face confidence threshold <float>", cxxopts::value<float>()->default_value("0.65"))

        ("b,blink","Enable blink detection",cxxopts::value<bool>()->default_value("false"))
        ("n,blinks_number","number of blinks threshold",cxxopts::value<int>()->default_value("0"))

        ("r,resized","Enable Resized LiveView (size: 640x480)",cxxopts::value<bool>()->default_value("false"))

        ("d,distance",
         "Set the distance threshold: this option works with single face detection to clear out more distant people (in meters, eg: 0.800)",
         cxxopts::value<float>()->default_value("0.800"));
};

cxxopts::ParseResult Parser::parse(int& argc, char** argv) {

    cxxopts::ParseResult result = this->parser.parse(argc,argv);

    if(result.count("help")){
        std::cout<<this->parser.help()<<std::endl;
        exit(0);
    }

    return result;
}