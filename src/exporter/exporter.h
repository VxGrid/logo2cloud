#pragma once
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


class exporter
{
public:
  /// Destructor
  virtual ~exporter();

  /// Sets the output file path
  virtual void setPath(std::string path);

  /// Set the data
  virtual void setData(const std::vector<Point> &cloud);

  /// Run method to export point cloud
  virtual void run() = 0;

protected:
  /// The cloud data to export
  std::vector<Point> cloudData_{};

  /// Path where to export the point cloud to
  std::string path_{};
};
