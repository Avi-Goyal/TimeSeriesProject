#pragma once
#include <string>
#include <vector>
#include <utility>
#include <ctime>
#include <chrono>
#include <time.h>

class TimeSeriesTransformations {
public:
    // Constructors
    TimeSeriesTransformations();
    explicit TimeSeriesTransformations(const std::string& filenameandpath);
    TimeSeriesTransformations(const std::vector<int>& time, const std::vector<double>& price, std::string name = "");

    // Copy constructor
    TimeSeriesTransformations(const TimeSeriesTransformations& t);

    // Assignment operator
    TimeSeriesTransformations& operator=(const TimeSeriesTransformations& t);

    // Equality operator
    bool operator==(const TimeSeriesTransformations& t) const;

    // Statistical functions
    bool mean(double* meanValue) const;
    bool standardDeviation(double* standardDeviationValue) const;
    bool computeIncrementMean(double* meanValue) const;
    bool computeIncrementStandardDeviation(double* standardDeviationValue) const;

    // Data manipulation functions
    void addASharePrice(std::string datetime, double price);
    bool removeEntryAtTime(std::string time);
    bool removePricesGreaterThan(double price);
    bool removePricesLowerThan(double price);
    bool removePricesBefore(std::string date);
    bool removePricesAfter(std::string date);

    // Print functions
    std::string printSharePricesOnDate(std::string date) const;
    std::string printIncrementsOnDate(std::string date) const;

    // Utility functions
    bool findGreatestIncrements(std::string* date, double* price_increment) const;
    bool getPriceAtDate(const std::string date, double* value) const;
    void saveData(std::string filename) const;

    // Getters
    int count() const;
    std::string getName() const;
    char getSeparator() const;
    std::vector<double> getPrice() const;
    std::vector<int> getTime() const;

    // Static conversion functions
    static std::string unixToDateTime(const time_t& unix);
    static time_t dateTimeToUnix(const std::string& date);

    // Static helper functions
    static int truncData(std::string& date);
    static int truncUnix(const time_t& unix);
    
    // Public method to access increments
    std::vector<double> getIncrements() const; 
    
private:
    const int decimalPlaces = 5; // Number of decimal places for rounding
    std::vector<std::pair<int, double>> P3data{}; // Stores time and price data
    std::string _name{}; // Name of the time series
    size_t observations{}; // Number of observations

    // Private helper functions
    static double getMean(const std::vector<double>& vector);
    static double getSD(const std::vector<double>& vector);
    std::vector<double> computeIncrements() const;
};

        