#include <iostream>

#include <span>
#include <vector>
#include <ranges>

int main()
{
    std::array<int8_t, 64*64> array{};
    for(int i = 0; i < 64 *64; ++i)
    {
        array[i] = i;
    }

    return 0;
}
