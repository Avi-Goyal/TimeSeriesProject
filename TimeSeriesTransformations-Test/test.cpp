#include "../TimeSeriesTransformations/TimeSeriesTransformations.h"
#include <iostream>
#include <cassert>
#include <fstream>

// Helper function to compare doubles with a tolerance
bool almostEqual(double a, double b, double tolerance = 1e-5) {
    return std::fabs(a - b) < tolerance;
}

// Test the constructor that loads data from a CSV file
void testConstructorFromCSV() {
    // Use an absolute path to the CSV file
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    assert(ts.count() > 0); // Ensure data is loaded
    std::cout << "testConstructorFromCSV passed!" << std::endl;
}

// Test the default constructor
void testDefaultConstructor() {
    TimeSeriesTransformations ts;
    assert(ts.count() == 0); // Default constructor should have no data
    std::cout << "testDefaultConstructor passed!" << std::endl;
}

// Test the constructor with time and price vectors
void testConstructorWithVectors() {
    std::vector<int> time = {1, 2, 3};
    std::vector<double> price = {10.5, 11.5, 12.5};
    TimeSeriesTransformations ts(time, price, "TestSeries");

    assert(ts.count() == 3);
    assert(ts.getName() == "TestSeries");
    assert(almostEqual(ts.getPrice()[0], 10.5));
    assert(almostEqual(ts.getPrice()[1], 11.5));
    assert(almostEqual(ts.getPrice()[2], 12.5));
    std::cout << "testConstructorWithVectors passed!" << std::endl;
}

// Test the copy constructor
void testCopyConstructor() {
    std::vector<int> time = {1, 2, 3};
    std::vector<double> price = {10.5, 11.5, 12.5};
    TimeSeriesTransformations ts1(time, price, "TestSeries");
    TimeSeriesTransformations ts2(ts1);

    assert(ts2.count() == 3);
    assert(ts2.getName() == "TestSeries");
    assert(almostEqual(ts2.getPrice()[0], 10.5));
    assert(almostEqual(ts2.getPrice()[1], 11.5));
    assert(almostEqual(ts2.getPrice()[2], 12.5));
    std::cout << "testCopyConstructor passed!" << std::endl;
}

// Test the assignment operator
void testAssignmentOperator() {
    std::vector<int> time = {1, 2, 3};
    std::vector<double> price = {10.5, 11.5, 12.5};
    TimeSeriesTransformations ts1(time, price, "TestSeries");
    TimeSeriesTransformations ts2;
    ts2 = ts1;

    assert(ts2.count() == 3);
    assert(ts2.getName() == "TestSeries");
    assert(almostEqual(ts2.getPrice()[0], 10.5));
    assert(almostEqual(ts2.getPrice()[1], 11.5));
    assert(almostEqual(ts2.getPrice()[2], 12.5));
    std::cout << "testAssignmentOperator passed!" << std::endl;
}

// Test the equality operator
void testEqualityOperator() {
    std::vector<int> time = {1, 2, 3};
    std::vector<double> price = {10.5, 11.5, 12.5};
    TimeSeriesTransformations ts1(time, price, "TestSeries");
    TimeSeriesTransformations ts2(time, price, "TestSeries");

    assert(ts1 == ts2);
    std::cout << "testEqualityOperator passed!" << std::endl;
}

// Test the getName() function
void testGetName() {
    TimeSeriesTransformations ts({1}, {10.5}, "TestSeries");
    assert(ts.getName() == "TestSeries");
    std::cout << "testGetName passed!" << std::endl;
}

// Test the count() function
void testCount() {
    std::vector<int> time = {1, 2, 3};
    std::vector<double> price = {10.5, 11.5, 12.5};
    TimeSeriesTransformations ts(time, price, "TestSeries");

    assert(ts.count() == 3);
    std::cout << "testCount passed!" << std::endl;
}

// Test the getTime() function
void testGetTime() {
    std::vector<int> time = {1, 2, 3};
    std::vector<double> price = {10.5, 11.5, 12.5};
    TimeSeriesTransformations ts(time, price, "TestSeries");

    assert(ts.getTime() == time);
    std::cout << "testGetTime passed!" << std::endl;
}

// Test the getPrice() function
void testGetPrice() {
    std::vector<int> time = {1, 2, 3};
    std::vector<double> price = {10.5, 11.5, 12.5};
    TimeSeriesTransformations ts(time, price, "TestSeries");

    assert(ts.getPrice() == price);
    std::cout << "testGetPrice passed!" << std::endl;
}

// Test the unixToDateTime() function
void testUnixToDateTime() {
    time_t unixTime = 1696156800; // October 1, 2023 12:00:00 UTC
    std::string dateTime = TimeSeriesTransformations::unixToDateTime(unixTime);

    // Debug print to see the actual output
    std::cout << "Actual output: " << dateTime << std::endl;

    // Verify the output is in UTC
    assert(dateTime == "2023-10-01 10:40:00" && "Incorrect date/time format or timezone");

    std::cout << "testUnixToDateTime passed!" << std::endl;
}
// Test the dateTimeToUnix() function
void testDateTimeToUnix() {
    std::string dateTime = "2025-02-28 10:40:00";
    time_t unixTime = TimeSeriesTransformations::dateTimeToUnix(dateTime);
    assert(unixTime == 1740739200);
    std::cout << "testDateTimeToUnix passed!" << std::endl;
}

// Test the truncData() function
void testTruncData() {
    std::string date = "2025-02-28 12:34:56";
    int truncatedUnix = TimeSeriesTransformations::truncData(date);
    assert(truncatedUnix == 1740700800); // October 1, 2023 00:00:00 UTC
    std::cout << "testTruncData passed!" << std::endl;
}

// Test the truncUnix() function
void testTruncUnix() {
    time_t unixTime = 1740739200; // October 1, 2023 10:40:00 UTC
    int truncatedUnix = TimeSeriesTransformations::truncUnix(unixTime);
    assert(truncatedUnix == 1740700800); // October 1, 2023 00:00:00 UTC
    std::cout << "testTruncUnix passed!" << std::endl;
}

// Test the mean() function
void testMean() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    double meanValue;
    bool success = ts.mean(&meanValue);
    assert(success);
    assert(almostEqual(meanValue, 51.5734)); // Approximate mean of the data
    std::cout << "testMean passed!" << std::endl;
}

// Test the standardDeviation() function
void testStandardDeviation() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    double sdValue;
    bool success = ts.standardDeviation(&sdValue);
    assert(success);
    assert(almostEqual(sdValue, 28.70325)); // Approximate standard deviation of the data
    std::cout << "testStandardDeviation passed!" << std::endl;
}

// Test the computeIncrements() function
void testComputeIncrements() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    std::vector<double> increments = ts.getIncrements();// Use the public method
    assert(increments.size() == 9999); // 500 data points => 499 increments
    std::cout << "testComputeIncrements passed!" << std::endl;
}

void testComputeIncrementMean() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    double meanValue;
    bool success = ts.computeIncrementMean(&meanValue);

    // Debug print to see the calculated mean of increments
    std::cout << "Mean of increments: " << meanValue << std::endl;

    assert(success);
    assert(almostEqual(meanValue, 0.00290)); // Mean of increments should be close to 0
    std::cout << "testComputeIncrementMean passed!" << std::endl;
}

// Test the computeIncrementStandardDeviation() function
void testComputeIncrementStandardDeviation() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    double sdValue;
    bool success = ts.computeIncrementStandardDeviation(&sdValue);

    // Debug print to see the calculated mean of increments
    std::cout << "SD of increments: " << sdValue << std::endl;

    assert(success);
    assert(almostEqual(sdValue, 40.359444049806953)); // Approximate standard deviation of increments
    std::cout << "testComputeIncrementStandardDeviation passed!" << std::endl;
}

// Test the addASharePrice() function
void testAddASharePrice() {
    TimeSeriesTransformations ts;
    ts.addASharePrice("2023-10-01 12:00:00", 100.0);
    ts.addASharePrice("2023-10-02 12:00:00", 200.0);

    assert(ts.count() == 2);
    assert(almostEqual(ts.getPrice()[0], 100.0));
    assert(almostEqual(ts.getPrice()[1], 200.0));
    std::cout << "testAddASharePrice passed!" << std::endl;
}

// Test the removeEntryAtTime() function
void testRemoveEntryAtTime() {
    TimeSeriesTransformations ts;
    ts.addASharePrice("2023-10-01 12:00:00", 100.0);
    ts.addASharePrice("2023-10-02 12:00:00", 200.0);

    bool removed = ts.removeEntryAtTime("2023-10-01 12:00:00");
    assert(removed);
    assert(ts.count() == 1);
    assert(almostEqual(ts.getPrice()[0], 200.0));
    std::cout << "testRemoveEntryAtTime passed!" << std::endl;
}

// Test the removePricesGreaterThan() function
void testRemovePricesGreaterThan() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    bool removed = ts.removePricesGreaterThan(100.0);
    assert(removed);
    assert(ts.count() < 10000); // Some prices should be removed
    std::cout << "testRemovePricesGreaterThan passed!" << std::endl;
}

// Test the removePricesLowerThan() function
void testRemovePricesLowerThan() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    bool removed = ts.removePricesLowerThan(10.0);
    assert(removed);
    assert(ts.count() < 10000); // Some prices should be removed
    std::cout << "testRemovePricesLowerThan passed!" << std::endl;
}

// Test the removePricesBefore() function
void testRemovePricesBefore() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    bool removed = ts.removePricesBefore("2021-04-23 00:00:00");
    assert(removed);
    assert(ts.count() < 10000); // Some prices should be removed
    std::cout << "testRemovePricesBefore passed!" << std::endl;
}

// Test the removePricesAfter() function
void testRemovePricesAfter() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    bool removed = ts.removePricesAfter("2021-04-23 00:00:00");
    assert(removed);
    assert(ts.count() < 10000); // Some prices should be removed
    std::cout << "testRemovePricesAfter passed!" << std::endl;
}

// Test the printSharePricesOnDate() function
void testPrintSharePricesOnDate() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    std::string prices = ts.printSharePricesOnDate("2021-04-23");
    assert(!prices.empty()); // Ensure output is not empty
    std::cout << "testPrintSharePricesOnDate passed!" << std::endl;
}

// Test the printIncrementsOnDate() function
void testPrintIncrementsOnDate() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    std::string increments = ts.printIncrementsOnDate("2021-04-23");
    assert(!increments.empty()); // Ensure output is not empty
    std::cout << "testPrintIncrementsOnDate passed!" << std::endl;
}

// Test the findGreatestIncrements() function
void testFindGreatestIncrements() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    std::string date;
    double increment;
    bool success = ts.findGreatestIncrements(&date, &increment);
    assert(success);
    assert(!date.empty());
    assert(increment > 0.0);
    std::cout << "testFindGreatestIncrements passed!" << std::endl;
}

// Test the getPriceAtDate() function
void testGetPriceAtDate() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    double price;
    bool success = ts.getPriceAtDate("2022-11-21 05:53:30", &price);

    // Debug print to see the success status
    //std::cout << "Success: " << success << std::endl;

    // Debug print to see the Unix timestamp
    time_t unix = TimeSeriesTransformations::dateTimeToUnix("2022-11-21 05:53:30");
   // std::cout << "Unix timestamp: " << unix << std::endl;

    // Debug print to see the retrieved price
    //std::cout << "Retrieved price: " << price << std::endl;

    // Debug print to see the contents of P3data
    std::cout << "P3data contents:" << std::endl;
    const std::vector<int>& times = ts.getTime();
    const std::vector<double>& prices = ts.getPrice();
    //for (size_t i = 0; i < times.size(); ++i) {
   //     std::cout << "Time: " << times[i] << ", Price: " << prices[i] << std::endl;
   // }

    assert(success);
    assert(price > 0.0);
   // std::cout << "testGetPriceAtDate passed!" << std::endl;
}

// Test the saveData() function
void testSaveData() {
    std::string absolutePath = "C:/Users/sanjk/Documents/TimeSeriesProject/Problem3_DATA.csv";
    TimeSeriesTransformations ts(absolutePath);
    ts.saveData("test_output");
    std::ifstream file("test_output.csv");
    assert(file.good());
    std::cout << "testSaveData passed!" << std::endl;
}

int main() {
    testConstructorFromCSV();
    testDefaultConstructor();
    testConstructorWithVectors();
    testCopyConstructor();
    testAssignmentOperator();
    testEqualityOperator();
    testGetName();
    testCount();
    testGetTime();
    testGetPrice();
    testUnixToDateTime();
    testDateTimeToUnix();
    testTruncData();
    testTruncUnix();
    testMean();
    testStandardDeviation();
    testComputeIncrements();
    testComputeIncrementMean();
    testComputeIncrementStandardDeviation();
    testAddASharePrice();
    testRemoveEntryAtTime();
    testRemovePricesGreaterThan();
    testRemovePricesLowerThan();
    testRemovePricesBefore();
    testRemovePricesAfter();
    testPrintSharePricesOnDate();
    testPrintIncrementsOnDate();
    testFindGreatestIncrements();
    testGetPriceAtDate();
    testSaveData();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}