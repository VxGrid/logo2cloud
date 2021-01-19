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

  void setRandomizeValue(double randomizeValue);

  /// Exports the provided cloud in the format
  void exportCloud(std::string path, EXPORTER cloudFormat, bool structured = false);

  void run();

  [[nodiscard]] double getProgressPercent() const noexcept;

private:
  /// calculates out of the image a XZ point cloud (no depth)
  void image2XZCloud(std::vector<Point> &cloud);

  /// Adds the Y coordinate 3D points to the cloud
  // void addDepth2Cloud(std::vector<Point> &cloud);
  // static void addDepth2Cloud(const std::vector<Point> &cloudIn, std::vector<Point> &cloudOut, const double YDepthCoordinate);

  /// adds random value on all points on all coordinates
  // void randomizeCloud(std::vector<Point> &cloud);
  // static void randomizeCloud(std::vector<Point> &cloudInOut, double randomRange, unsigned int threads2Spawn);

  std::vector<Point> cloud_;

  std::unique_ptr<exporter> expClass_;

  /// The image data row major
  unsigned char const *imgDataPtr_{};

  /// image rows and cols
  unsigned int rows_{};
  unsigned int cols_{};

  /// Values for our resulting point cloud dimensions
  double cWidth_{10.0};
  double cHeight_{3.0};
  double cDepth_{1.0};
  double cPointSpacing_{.0};
  double cRandomRange_{0.001};
  unsigned int processorCount_{};

  /// the number of depth layers which is currently calculated
  double currentLayerNumCalculating_{};
  double layerNum2Calculate_{};
};

#endif // POINTCLOUDGENERATOR_H
