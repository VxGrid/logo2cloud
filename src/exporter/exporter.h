#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

struct Point
{
    /// Position cartesian coordinates
    double x_{}, y_{}, z_{};

    /// Color, red, green, blue [0; 255]
    uint8_t r_{}, g_{}, b_{};

    /// Intensity/Reflectivity [0.0; 1.0]
    float i_{};
};


class Exporter
{
public:
    /// Destructor
    virtual ~Exporter() = default;

    /// Set the data
    virtual void setData(const std::vector<Point> &cloud);

    /// Run method to export point cloud
    virtual void run() = 0;

    /// returns the date as in Year, month, day of month [1;31], day of year[1;366]
    std::tuple<uint16_t, uint8_t, uint8_t, uint16_t> getDate();

protected:
    /// The cloud data to export
    std::vector<Point> cloudData_{};
};
