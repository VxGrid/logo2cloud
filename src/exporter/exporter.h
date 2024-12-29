#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

struct Point
{
    /// Position cartesian coordinates
    double x_{}, y_{}, z_{};

    /// Color, red, green, blue
    unsigned char r_{}, g_{}, b_{};

    /// Intensity/Reflectivity
    unsigned char i_{};
};


class Exporter
{
public:
    /// Destructor
    virtual ~Exporter();

    /// Sets the output file path
    virtual void setPath(std::string path);

    /// Set the data
    virtual void setData(const std::vector<Point> &cloud);

    /// Run method to export point cloud
    virtual void run() = 0;

    /// returns the date as in Year, month, day of month [1;31], day of year[1;366]
    std::tuple<uint16_t, uint8_t, uint8_t, uint16_t> getDate();

protected:
    /// The cloud data to export
    std::vector<Point> cloudData_{};

    /// Path where to export the point cloud to
    std::string path_{};
};
