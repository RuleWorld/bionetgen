#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>

int main() {
    std::unordered_map<std::string, int> cache;
    cache["test"] = 1;
    return 0;
}
