#include <iostream>
#include "TimeSeriesTransformations.h"

int main() {
    // Example usage of the TimeSeriesTransformations class
    TimeSeriesTransformations ts("Problem3_DATA.csv");

    // Print the number of observations
    std::cout << "Number of observations: " << ts.count() << std::endl;

    // Calculate and print the mean
    double meanValue;
    if (ts.mean(&meanValue)) {
        std::cout << "Mean price: " << meanValue << std::endl;
    } else {
        std::cerr << "Failed to calculate mean." << std::endl;
    }

    return 0;
}