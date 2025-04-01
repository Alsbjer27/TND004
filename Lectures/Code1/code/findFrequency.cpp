#include <iostream>
#include <vector>

std::vector<int> findFrequency(const std::vector<int>& V, int m);

int main() {
    // The sorted sequence, each value in the range [0, 10]
    std::vector<int> S = {1, 1, 1, 2, 3, 3, 5, 5, 8, 8, 8, 9, 9, 10};

    std::vector<int> freq = findFrequency(S, 10);

    int i = 0;
    // Display the frequencies
    for (int k : freq) {
        if (k != 0) {
            std::cout << "Item " << i << " occurs " << k << " times\n";
        }
        ++i;
    }
}


// A divide-and-conquer based function to count number of occurrences
// for each element in V
void findFrequency(const std::vector<int>& V, int low, int high, std::vector<int>& freq) {
    // Base case: all items from low to high are the same
    if (V[low] == V[high]) {
        freq[V[low]] += high - low + 1;
    } else {
        // Same as (low + high)/2, but avoids overflow for
        // large low and high
        auto mid = low + (high - low) / 2;

        findFrequency(V, low, mid, freq);       // computer frequencies for the left-half
        findFrequency(V, mid + 1, high, freq);  // computer frequencies for the right-half
    }
}

// Call recursive function
std::vector<int> findFrequency(const std::vector<int>& V, int m) {
    std::vector<int> freq(m + 1, 0);  // m+1 counters initialized to zero
    findFrequency(V, 0, std::ssize(V) - 1, freq);
    return freq;
}

/*
// Iterative
std::vector<int> findFrequency(const std::vector<int>& V, int m) {
    std::vector<int> freq(m + 1, 0);  // m+1 counters initialized to zero

    for (int i = 0; i < std::ssize(V); ++i) {
        ++freq[V[i]];
    }
    return freq;
}
*/
