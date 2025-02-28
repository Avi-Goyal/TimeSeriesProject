#include "TimeSeriesTransformations.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <limits>

// Constructor to load data from a CSV file
TimeSeriesTransformations::TimeSeriesTransformations(const std::string& filenameandpath) {
    std::ifstream csv(filenameandpath);
    double five_dp = std::pow(10, decimalPlaces);

    if (!csv.is_open()) {
        throw std::runtime_error("Unable to open file " + filenameandpath);
    }

    if (csv.good()) {
        // Read the header line
        std::string header;
        std::getline(csv, header); // Assume the header contains "TIMESTAMP,ShareX"

        // Extract the name part (e.g., "ShareX")
        size_t commaPos = header.find(',');
        if (commaPos != std::string::npos) {
            _name = header.substr(commaPos + 1); // Extract everything after the comma
        } else {
            _name = header; // If no comma, use the entire header
        }

        std::string line;
        while (std::getline(csv, line)) {
            std::stringstream iss(line);
            std::string timeStr, priceStr;

            // Parse time and price from the CSV line
            if (std::getline(iss, timeStr, this->getSeparator()) && std::getline(iss, priceStr)) {
                try {
                    int time = std::stoi(timeStr);
                    double price = std::stod(priceStr);

                    // Round the price to 5 decimal places
                    price = std::round(price * five_dp) / five_dp;

                    // Add the data point
                    P3data.push_back({ time, price });
                } catch (const std::exception&) {
                    throw std::runtime_error("Invalid data format in file: " + filenameandpath);
                }
            }
        }

        // Sort the data by time
        std::sort(P3data.begin(), P3data.end());
        observations = P3data.size();
        csv.close();
    }
}

// Default constructor
TimeSeriesTransformations::TimeSeriesTransformations() {}

// Constructor to initialize with time and price vectors
TimeSeriesTransformations::TimeSeriesTransformations(const std::vector<int>& time,
    const std::vector<double>& price, std::string name) {
    if (time.size() != price.size()) {
        throw std::runtime_error("Error: Incomparable sizes of time and price vectors.");
    }

    _name = name;
    observations = time.size();

    for (int i = 0; i < observations; ++i) {
        P3data.push_back({ time[i], price[i] });
    }

    // Sort the data by time
    std::sort(P3data.begin(), P3data.end());
}

// Copy constructor
TimeSeriesTransformations::TimeSeriesTransformations(const TimeSeriesTransformations& t) {
    P3data = t.P3data;
    _name = t._name;
    observations = t.observations;
}

// Assignment operator
TimeSeriesTransformations& TimeSeriesTransformations::operator=(const TimeSeriesTransformations& t) {
    if (this != &t) {
        P3data = t.P3data;
        _name = t._name;
        observations = t.observations;
    }
    return *this;
}

// Equality operator
bool TimeSeriesTransformations::operator==(const TimeSeriesTransformations& t) const {
    return (P3data == t.P3data);
}

// Get the name of the time series
std::string TimeSeriesTransformations::getName() const {
    return _name;
}

// Get the number of observations
int TimeSeriesTransformations::count() const {
    return static_cast<int>(P3data.size());
}

// Get the time values
std::vector<int> TimeSeriesTransformations::getTime() const {
    std::vector<int> time;
    for (const auto& entry : P3data) {
        time.push_back(entry.first);
    }
    return time;
}

// Get the price values
std::vector<double> TimeSeriesTransformations::getPrice() const {
    std::vector<double> price;
    for (const auto& entry : P3data) {
        price.push_back(entry.second);
    }
    return price;
}

// Convert Unix time to a human-readable date and time string
std::string TimeSeriesTransformations::unixToDateTime(const time_t& unix) {
    std::tm timeStruct = {};
#ifdef _WIN32
    // Use gmtime_s on Windows to get UTC time
    gmtime_s(&timeStruct, &unix);
#else
    // Use gmtime on Unix-like systems to get UTC time
    timeStruct = *gmtime(&unix);
#endif

    // Format the date and time as "YYYY-MM-DD HH:MM:SS"
    char buffer[80];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             1900 + timeStruct.tm_year, 1 + timeStruct.tm_mon, timeStruct.tm_mday,
             timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec);

    return std::string(buffer);
}




// Convert a date and time string to Unix time
time_t TimeSeriesTransformations::dateTimeToUnix(const std::string& date) {
    // Debug output
    std::cout << "Parsing date: " << date << std::endl;

    // Validate the input string length
    if (date.length() != 19) { // "YYYY-MM-DD HH:MM:SS" is 19 characters
        throw std::invalid_argument("Invalid date format: " + date);
    }

    std::tm t = {};
    std::istringstream ss(date);

    // Parse the date string into the tm struct
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");

    // Check if parsing failed
    if (ss.fail()) {
        throw std::invalid_argument("Invalid date format: " + date);
    }

    // Validate the parsed date and time components
    if (t.tm_year < 0 || t.tm_mon < 0 || t.tm_mday < 0 ||
        t.tm_hour < 0 || t.tm_min < 0 || t.tm_sec < 0) {
        throw std::invalid_argument("Invalid date or time component in: " + date);
    }

#ifdef _WIN32
    // Use _mkgmtime for UTC time on Windows
    return _mkgmtime(&t);
#else
    // Use timegm for UTC time on Unix-like systems
    return timegm(&t);
#endif
}

// Truncate a date string to the start of the day
int TimeSeriesTransformations::truncData(std::string& date) {
    // Ensure the date string includes the time part
    if (date.length() < 19) {
        date += " 00:00:00"; // Append time part if missing
    }

    // Parse the date string to Unix time
    time_t unix = dateTimeToUnix(date);

    // Truncate the Unix time to the start of the day
    return truncUnix(unix);
}

// Truncate a Unix timestamp to the start of the day
int TimeSeriesTransformations::truncUnix(const time_t& unix) {
    std::tm timeStruct = {};
#ifdef _WIN32
    // Use gmtime_s on Windows
    gmtime_s(&timeStruct, &unix);
#else
    // Use gmtime on Unix-like systems
    timeStruct = *gmtime(&unix);
#endif

    // Set the time to 00:00:00
    timeStruct.tm_hour = 0;
    timeStruct.tm_min = 0;
    timeStruct.tm_sec = 0;

#ifdef _WIN32
    // Use _mkgmtime for UTC time on Windows
    return static_cast<int>(_mkgmtime(&timeStruct));
#else
    // Use timegm for UTC time on Unix-like systems
    return static_cast<int>(timegm(&timeStruct));
#endif
}


// Calculate the mean of a vector of doubles
double TimeSeriesTransformations::getMean(const std::vector<double>& vector) {
    if (vector.empty()) {
        throw std::invalid_argument("Cannot calculate mean of an empty vector.");
    }

    double sum = std::accumulate(vector.begin(), vector.end(), 0.0);
    return sum / vector.size();
}

// Calculate the standard deviation of a vector of doubles
double TimeSeriesTransformations::getSD(const std::vector<double>& vector) {
    if (vector.empty()) {
        throw std::invalid_argument("Cannot calculate standard deviation of an empty vector.");
    }

    double mean = getMean(vector);
    double sumSquares = 0.0;

    for (double value : vector) {
        sumSquares += (value - mean) * (value - mean);
    }

    return std::sqrt(sumSquares / (vector.size() - 1));
}

// Calculate the mean of the time series prices
bool TimeSeriesTransformations::mean(double* meanValue) const {
    try {
        if (getPrice().empty()) {
            throw std::runtime_error("Empty vector!!");
        }

        *meanValue = getMean(getPrice());
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        *meanValue = std::numeric_limits<double>::quiet_NaN();
        return false;
    }
}

// Calculate the standard deviation of the time series prices
bool TimeSeriesTransformations::standardDeviation(double* standardDeviationValue) const {
    try {
        if (getPrice().empty()) {
            throw std::runtime_error("Empty vector!!");
        }

        *standardDeviationValue = getSD(getPrice());
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        *standardDeviationValue = std::numeric_limits<double>::quiet_NaN();
        return false;
    }
}

// Calculate the increments (differences between consecutive prices)
std::vector<double> TimeSeriesTransformations::computeIncrements() const {
    std::vector<double> increments;
    if (P3data.size() < 2) {
        return increments; // Not enough data to compute increments
    }

    for (size_t i = 1; i < P3data.size(); ++i) {
        increments.push_back(P3data[i].second - P3data[i - 1].second);
    }

    return increments;
}

std::vector<double> TimeSeriesTransformations::getIncrements() const {
    return computeIncrements(); // Call the private method
}

// Calculate the mean of the increments
bool TimeSeriesTransformations::computeIncrementMean(double* meanValue) const {
    try {
        std::vector<double> increments = computeIncrements();
        if (increments.empty()) {
            throw std::runtime_error("Not enough data to compute increments.");
        }

        *meanValue = getMean(increments);
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        *meanValue = std::numeric_limits<double>::quiet_NaN();
        return false;
    }
}

// Calculate the standard deviation of the increments
bool TimeSeriesTransformations::computeIncrementStandardDeviation(double* standardDeviationValue) const {
    try {
        std::vector<double> increments = computeIncrements();
        if (increments.empty()) {
            throw std::runtime_error("Not enough data to compute increments.");
        }

        *standardDeviationValue = getSD(increments);
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        *standardDeviationValue = std::numeric_limits<double>::quiet_NaN();
        return false;
    }
}

// Add a share price at a specific date and time
void TimeSeriesTransformations::addASharePrice(std::string datetime, double price) {
    time_t unix = dateTimeToUnix(datetime);
    double five_dp = std::pow(10, decimalPlaces);
    double roundedPrice = std::round(price * five_dp) / five_dp;

    // Find the correct position to insert the new data point
    auto it = std::lower_bound(P3data.begin(), P3data.end(), std::make_pair(static_cast<int>(unix), 0.0),
        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.first < b.first;
        });

    P3data.insert(it, { static_cast<int>(unix), roundedPrice });
    observations = P3data.size();
}

// Remove an entry at a specific time
bool TimeSeriesTransformations::removeEntryAtTime(std::string time) {
    time_t unix = dateTimeToUnix(time);
    size_t initialSize = P3data.size();

    P3data.erase(std::remove_if(P3data.begin(), P3data.end(),
        [unix](const std::pair<int, double>& entry) {
            return entry.first == static_cast<int>(unix);
        }), P3data.end());

    observations = P3data.size();
    return (initialSize != observations);
}

// Remove prices greater than a specified value
bool TimeSeriesTransformations::removePricesGreaterThan(double price) {
    size_t initialSize = P3data.size();

    P3data.erase(std::remove_if(P3data.begin(), P3data.end(),
        [price](const std::pair<int, double>& entry) {
            return entry.second > price;
        }), P3data.end());

    observations = P3data.size();
    return (initialSize != observations);
}

// Remove prices lower than a specified value
bool TimeSeriesTransformations::removePricesLowerThan(double price) {
    size_t initialSize = P3data.size();

    P3data.erase(std::remove_if(P3data.begin(), P3data.end(),
        [price](const std::pair<int, double>& entry) {
            return entry.second < price;
        }), P3data.end());

    observations = P3data.size();
    return (initialSize != observations);
}

// Remove prices before a specified date
bool TimeSeriesTransformations::removePricesBefore(std::string date) {
    time_t unix = dateTimeToUnix(date);
    size_t initialSize = P3data.size();

    P3data.erase(std::remove_if(P3data.begin(), P3data.end(),
        [unix](const std::pair<int, double>& entry) {
            return entry.first < static_cast<int>(unix);
        }), P3data.end());

    observations = P3data.size();
    return (initialSize != observations);
}

// Remove prices after a specified date
bool TimeSeriesTransformations::removePricesAfter(std::string date) {
    time_t unix = dateTimeToUnix(date);
    size_t initialSize = P3data.size();

    P3data.erase(std::remove_if(P3data.begin(), P3data.end(),
        [unix](const std::pair<int, double>& entry) {
            return entry.first > static_cast<int>(unix);
        }), P3data.end());

    observations = P3data.size();
    return (initialSize != observations);
}

// Print share prices on a specific date
std::string TimeSeriesTransformations::printSharePricesOnDate(std::string date) const {
    // Truncate the input date to the start of the day
    time_t unix = truncData(date);
    std::string sharePrices;

    for (const auto& entry : P3data) {
        // Truncate the entry's timestamp to the start of the day
        time_t entryUnix = truncUnix(static_cast<time_t>(entry.first));

        // Compare the truncated timestamps
        if (entryUnix == unix) {
            sharePrices += std::to_string(entry.second) + '\n';
        }
    }

    std::cout << "SharePrices on the " + date + " are:" << std::endl << sharePrices << std::endl;
    return sharePrices;
}


// Print increments on a specific date
std::string TimeSeriesTransformations::printIncrementsOnDate(std::string date) const {
    time_t unix = truncData(date);
    std::string increments;

    for (size_t i = 0; i < P3data.size() - 1; ++i) {
        if (truncUnix(static_cast<time_t>(P3data[i].first)) == unix) {
            increments += std::to_string(P3data[i + 1].second - P3data[i].second) + '\n';
        }
    }

    std::cout << "Increments on the " + date + " are:" << std::endl << increments << std::endl;
    return increments;
}

// Find the greatest increment in the time series
bool TimeSeriesTransformations::findGreatestIncrements(std::string* date, double* price_increment) const {
    if (P3data.empty()) {
        *price_increment = std::numeric_limits<double>::quiet_NaN();
        return false;
    }

    std::vector<double> increments = computeIncrements();
    auto maxIncrement = std::max_element(increments.begin(), increments.end());

    if (maxIncrement != increments.end()) {
        size_t index = std::distance(increments.begin(), maxIncrement);
        *date = unixToDateTime(static_cast<time_t>(P3data[index].first));
        *price_increment = *maxIncrement;
        return true;
    }

    return false;
}

// Get the price at a specific date
bool TimeSeriesTransformations::getPriceAtDate(const std::string date, double* value) const {
    time_t unix = dateTimeToUnix(date);
    auto it = std::find_if(P3data.begin(), P3data.end(),
        [unix](const std::pair<int, double>& entry) {
            return entry.first == static_cast<int>(unix);
        });

    if (it != P3data.end()) {
        *value = it->second;
        return true;
    }

    *value = std::numeric_limits<double>::quiet_NaN();
    return false;
}

// Save the time series data to a CSV file
void TimeSeriesTransformations::saveData(std::string filename) const {
    std::ofstream newCsv(filename + ".csv");

    if (newCsv.is_open()) {
        newCsv << "Unix-TIME SERIES DATA: " << _name << std::endl;

        for (const auto& entry : P3data) {
            newCsv << entry.first << ',' << entry.second << std::endl;
        }

        newCsv.close();
    } else {
        throw std::runtime_error("Unable to save data to file: " + filename + ".csv");
    }
}

// Get the separator used in CSV files
char TimeSeriesTransformations::getSeparator() const {
    return ',';
}