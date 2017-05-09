#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <algorithm>

#include "../utility.h"
#include "../random.h"

template<typename IteratorT>
void print(const range<IteratorT>& r)
{
    for (auto x : r)
        std::cout << x << " ";
    std::cout << std::endl;
}

template<typename IteratorT>
size_t size(const range<IteratorT>& r)
{
    return std::distance(r.begin(), r.end());
}

int main()
{
    std::list<int> v{ 3, 1, 4, 22, 31 };

    auto range = make_range(v.begin(), v.end());

    print(range);
    std::cout << "Size : " << size(range) << std::endl;

    std::vector<int> w;
    std::copy(v.begin(), v.end(), std::back_inserter(w));

    auto range2 = make_range(w.begin(), std::next(w.begin(), 4));

    print(range2);
    std::cout << "Size : " << size(range2) << std::endl;

    randomize();
    std::shuffle(range2.begin(), range2.end(), global_urng());

    std::copy(range2.begin(), range2.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::copy(range.begin(), range.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}
