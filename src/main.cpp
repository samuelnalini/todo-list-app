#include "headers/repl.hpp"


int main(int argc, char* argv[]) {
    const char* configPath{ "config.cfg" };

    if (argc > 1) {
        configPath = argv[2];
    }

    REPL repl;
    repl.Run(configPath);
}
