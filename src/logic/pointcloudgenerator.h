#ifndef POINTCLOUDGENERATOR_H
#define POINTCLOUDGENERATOR_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "exporter.h"


class pointcloudgenerator
{
public:
  enum EXPORTER
  {
    XYZ = 0,
    //        LAS, // TODO: implement
    //        LAZ,
    //        E57,
    //        PLY,
  };

  const std::vector<std::string> fileFormats{"ASCII (*.xyz)"}; //, "Binary (*.las)", "Binary (*.laz)", "ASTM (*.e57)", "Stanford (*.ply)"};

  /// Constructor
  pointcloudgenerator();

  /// Set the neccessary data
  void setData(unsigned char const *imgDataPtr, unsigned int rows, unsigned int cols);

  /// Set the properties of the resulting point cloud
  void setProperties(double width, double height, double depth, double pointSpacing = .0);

  /// Exports the provided cloud in the format
  void exportCloud(std::string path, EXPORTER cloudFormat, bool structured = false);

  void run();

private:
  /// sets all pixel in XZ plane into one 3D point
  void pixel2Cloud(std::vector<Point> &cloud);

  /// Adds the Y coordinate 3D points to the cloud
  void addDepth2Cloud(std::vector<Point> &cloud);
  static void addDepth2Cloud(const std::vector<Point> &cloudIn, std::vector<Point> &cloudOut, const double YDepthCoordinate);

  /// adds random value on all points on all coordinates
  void randomizeCloud(std::vector<Point> &cloud);
  static void randomizeCloud(std::vector<Point> &cloudInOut, double randomRange, unsigned int threads2Spawn);

  std::vector<Point> cloud_;

  std::unique_ptr<exporter> expClass_;

  /// The image data row major
  unsigned char const *imgDataPtr_{};

  /// image rows and cols
  unsigned int rows_{};
  unsigned int cols_{};

  /// Values for our resulting point cloud dimensions // FIXME: Need to give the values via GUI
  double cWidth_{10.0}; // FIXME: set these values to GUI standard values?
  double cHeight_{3.0}; // FIXME: Add a value to set the point spacing and therefore the amount of generated points as well
  double cDepth_{1.0};
  double cPointSpacing_{.0};
  double cRandomRange_{0.001}; // FIXME: ...
  unsigned int processorCount_{};
};

#endif // POINTCLOUDGENERATOR_H
