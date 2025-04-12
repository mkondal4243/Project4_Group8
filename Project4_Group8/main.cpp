#include <iostream>
#include "server_db.h"

int main() {
    std::cout << "This is group 8" << std::endl;

    if (initializeDatabase()) {
        std::cout << "✅ Database initialized successfully.\n";
    } else {
        std::cerr << "❌ Database initialization failed.\n";
    }

    std::cout << "Hello World" << std::endl;
    return 0;
}
