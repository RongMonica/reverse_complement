#include "reverse_complement/application.hpp"

#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        const Application app;
        return app.run(argc, argv);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
}
