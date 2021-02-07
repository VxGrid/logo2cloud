#include "pointcloudgenerator.h"

#ifdef BUILD_LAS_WRITER
#include "exporterLAS.h"
#endif

#ifdef BUILD_XYZ_WRITER
#include "exporterXYZ.h"
#endif

//#include "laszip/laszip_api.h"


#include <algorithm>
#include <cmath>
#include <future>
#include <memory>
#include <thread>

#include <iostream>


namespace liblas
{

class Header;
class Writer;
} // namespace liblas

pointcloudgenerator::pointcloudgenerator()
{
  processorCount_ = std::max(std::thread::hardware_concurrency(), 8u); // we use at least 8 threads
}


void pointcloudgenerator::setData(const unsigned char *imgDataPtr,
                                  unsigned int rows, unsigned int cols)
{
  imgDataPtr_ = imgDataPtr;
  rows_ = rows;
  cols_ = cols;
}


void pointcloudgenerator::setProperties(double width, double height, double depth, double pointSpacing)
{
  cWidth_ = width;
  cHeight_ = height;
  cDepth_ = depth;
  cPointSpacing_ = pointSpacing;
}


void pointcloudgenerator::setRandomizeValue(double randomizeValue)
{
  cRandomRange_ = randomizeValue;
}

/// free functions
void randomizeCloudFunc(std::vector<Point> &cloudInOut, double randomRange, unsigned int threads2Spawn)
{
  const int randNum = randomRange * 10000;

  // lambda where the cloud is randomized
  auto cloudPartRandomizer = [&cloudInOut, randNum](unsigned long startIter, unsigned long endIter) {
    std::for_each(std::begin(cloudInOut) + startIter, std::begin(cloudInOut) + endIter, [randNum](Point &point) {
      point.x_ += double((rand() % (2 * randNum)) - randNum) / 10000.0;
      point.y_ += double((rand() % (2 * randNum)) - randNum) / 10000.0;
      point.z_ += double((rand() % (2 * randNum)) - randNum) / 10000.0;
    });
  };

  // calculate proper thread division
  // use a thread for every 2^16 points
  std::vector<int> beginAccess, endAccess;
  double stepSize = std::pow(2, 16);
  unsigned int neededCycles = cloudInOut.size() / stepSize;
  unsigned long startIterLoc{0}, endIterLoc;
  endIterLoc = stepSize - 1;

  while (neededCycles > 0)
  {
    int howManyThreadsDoIWantToCreate = std::min(neededCycles, threads2Spawn);

    std::vector<std::future<void>> futures;
    for (int i = 0; i < howManyThreadsDoIWantToCreate; ++i) // start all threads with the lambda
    {
      futures.push_back(std::async(std::launch::async, cloudPartRandomizer, startIterLoc, endIterLoc));

      startIterLoc = endIterLoc + 1;
      endIterLoc = startIterLoc + stepSize - 1;
      --neededCycles;

      if (endIterLoc >= cloudInOut.size())
        break; // to be sure not to overshoot, should not happen
    }

    for (auto &future : futures) // wait till all threads have finished
      future.get();
  }

  // calculate remaining points
  cloudPartRandomizer(startIterLoc, cloudInOut.size() - 1);
}


void addDepthLayer2CloudFunc(const std::vector<Point> &cloud, std::vector<Point> &depthLayer, const double YDepthCoordinate)
{
  depthLayer.reserve(cloud.size());

  std::transform(std::begin(cloud), std::end(cloud), std::back_inserter(depthLayer), [YDepthCoordinate](const auto p1) {
    Point p2 = p1;
    p2.y_ = YDepthCoordinate;
    return p2;
  });
}


int pointcloudgenerator::exportCloud(std::string path, pointcloudgenerator::EXPORTER cloudFormat, bool structured)
{
  image2XZCloud(cloud_); // Calculate image into point cloud
  try
  {
    // instantiate export format, will open for write
    switch (cloudFormat)
    {
#ifdef BUILD_XYZ_WRITER
    case EXPORTER::XYZ:
      path.append(".xyz");
      expClass_ = std::make_unique<exporterXYZ>(path);
      break;
#endif

#ifdef BUILD_LAS_WRITER
    case EXPORTER::LAS:
      path.append(".las");
      expClass_ = std::make_unique<exporterLAZ>(path);
      break;
#endif
#ifdef BUILD_LAS_WRITER
    case EXPORTER::LAZ:
      path.append(".laz");
      expClass_ = std::make_unique<exporterLAZ>(path);
      break;
#endif
    default:
      // no format defined
      return EXIT_FAILURE;
    }
  }
  catch (...)
  {
    return EXIT_FAILURE;
  }

  expClass_->setPath(path);
  return EXIT_SUCCESS;
}


void pointcloudgenerator::run()
{
  if (!expClass_)
  {
    std::cout << "No Export pointer, exit\n";
    return;
  }

  layerNum2Calculate_ = 100.0 / (std::max(1.0, cDepth_ / cPointSpacing_) + 2.0); // somewhat correct, only estimate
  std::vector<Point> depthLayer, depthLayer2Randomize, depthLayerRandomized;
  const double depthStep = cPointSpacing_; // based on spacing between height and width
  // streamline here

  std::vector<std::future<void>> futures;
  double depth2Calc = depthStep;

  depthLayer2Randomize = cloud_;

  // streamline output, calculate first depthlayer while randomizing cloud_
  // second iteration: calculate second depth layer, randomize first depthlayer, output randomized cloud_
  // third till nth iteration: calculate third depth layer, randomize second depthlayer, output randomized first depth layer
  // end iterations: randomize&output -> only output
  while (!depthLayerRandomized.empty() || !depthLayer2Randomize.empty() || !depthLayer.empty())
  {
    currentLayerNumCalculating_ += 1.0;

    if (depth2Calc < cDepth_)
      futures.emplace_back(std::async(std::launch::async, addDepthLayer2CloudFunc, cloud_, std::ref(depthLayer), depth2Calc));


    if (cRandomRange_ && !depthLayer2Randomize.empty())
      futures.emplace_back(std::async(std::launch::async, randomizeCloudFunc, std::ref(depthLayer2Randomize), cRandomRange_, processorCount_ - 1));


    if (!depthLayerRandomized.empty())
    {
      expClass_->setData(depthLayerRandomized);
      futures.emplace_back(std::async(std::launch::async, &exporter::run, expClass_.get()));
    }

    for (auto &future : futures)
      future.get();

    depthLayerRandomized = std::move(depthLayer2Randomize);
    depthLayer2Randomize = std::move(depthLayer);
    depthLayer.clear();
    futures.clear();
    depth2Calc += depthStep;
  }

  // Finished calculating, now reset
  expClass_.reset();
  currentLayerNumCalculating_ = 0.0;
  cloud_.clear();
}


double pointcloudgenerator::getProgressPercent() const noexcept
{
  return (currentLayerNumCalculating_ * layerNum2Calculate_);
}

/*
// To remember:
int red=imgDataPtr_[iter]; // blue
int green=imgDataPtr_[iter+1]; // green
int blue=imgDataPtr_[iter+2]; // red
int alpha=imgDataPtr_[iter+3]; // alpha

// calculate 3d position of the pixel
// cHeight_ == rows_ ---> Z
// cWidth_ == cols_  ---> X
// cDepth_ == depending on the above ---> Y
*/

void pointcloudgenerator::image2XZCloud(std::vector<Point> &cloud)
{
  cloud.reserve(cWidth_ * cHeight_ / (cPointSpacing_ * cPointSpacing_) + 1);
  double X{0.0}, Z{0.0}, Y{0.0};
  unsigned int lookUpCol{}, lookUpRow{}, iter{};
  const double pixelSpacing = double(cols_) / cWidth_;

  for (int row = 0; row < int(cHeight_ / cPointSpacing_); ++row)
  {
    for (int col = 0; col < int(cWidth_ / cPointSpacing_); ++col)
    {
      X = double(col) * cPointSpacing_;
      Z = double(row) * cPointSpacing_;

      lookUpCol = int(X * pixelSpacing);
      if (lookUpCol >= cols_)
        continue;

      lookUpRow = int(Z * pixelSpacing);
      if (lookUpRow >= rows_)
        continue;

      iter = (lookUpRow * cols_ + lookUpCol) * 4; // position in image buffer

      if (!imgDataPtr_[iter + 3])
        continue; // alpha empty, continue

      // Z axis and rows coordinate system are inverted to each other
      cloud.emplace_back(Point{X, Y, cHeight_ - Z, imgDataPtr_[iter + 2], imgDataPtr_[iter + 1], imgDataPtr_[iter], 0});
    }
  }
}

/*
void pointcloudgenerator::addDepth2Cloud(std::vector<Point> &cloud)
{
  const double pointspacing = cPointSpacing_; // based on spacing between height and width
  double currentDepthLoc = pointspacing;

  cloud.reserve(cloud.size() * (cDepth_ / pointspacing + 1)); // neccessary for transform algorithm, crash if data copy happens
  using std::cbegin, std::cend;
  const auto currBegIter = begin(cloud);
  const auto currEndIter = end(cloud);

  while (currentDepthLoc < cDepth_)
  {
    std::transform(currBegIter, currEndIter, std::back_inserter(cloud), [currentDepthLoc](const auto p1) {
      Point p2 = p1;
      p2.y_ = currentDepthLoc;
      return p2;
    });

    currentDepthLoc += pointspacing;
  }
}


void pointcloudgenerator::addDepth2Cloud(const std::vector<Point> &cloudIn, std::vector<Point> &cloudOut, const double YDepthCoordinate)
{
  cloudOut.reserve(cloudIn.size());
  std::transform(std::begin(cloudIn), std::end(cloudIn), std::back_inserter(cloudOut), [YDepthCoordinate](const auto p1) {
    Point p2 = p1;
    p2.y_ = YDepthCoordinate;
    return p2;
  });
}
*/
/*
void pointcloudgenerator::randomizeCloud(std::vector<Point> &cloud)
{
  const int randNum = cRandomRange_ * 1000;

  std::vector<int> test(5);
  auto lambda = [&test](int threadID, int threadCount) {
    test[threadID] = threadID;
  };

  std::vector<std::future<void>> futures;
  int howManyThreadsDoIWantToCreate = processorCount_ - 1;
  for (int i = 0; i < howManyThreadsDoIWantToCreate; ++i) // start all threads with the lambda
    futures.push_back(std::async(std::launch::async, lambda, i, howManyThreadsDoIWantToCreate));

  for (auto &future : futures) // wait till all threads have finished
    future.get();

  for (auto &point : cloud)
  {
    point.x_ += double((rand() % (2 * randNum)) - randNum) / 1000.0;
    point.y_ += double((rand() % (2 * randNum)) - randNum) / 1000.0;
    point.z_ += double((rand() % (2 * randNum)) - randNum) / 1000.0;
  }
}


void pointcloudgenerator::randomizeCloud(std::vector<Point> &cloudInOut, double randomRange, unsigned int threads2Spawn)
{
  const int randNum = randomRange * 1000;

  // lambda where the cloud is randomized
  auto cloudPartRandomizer = [&cloudInOut, randNum](unsigned long startIter, unsigned long endIter) {
    std::for_each(std::begin(cloudInOut) + startIter, std::begin(cloudInOut) + endIter, [randNum](Point &point) {
      point.x_ += double((rand() % (2 * randNum)) - randNum) / 1000.0;
      point.y_ += double((rand() % (2 * randNum)) - randNum) / 1000.0;
      point.z_ += double((rand() % (2 * randNum)) - randNum) / 1000.0;
    });
  };

  // calculate proper thread division
  // use a thread for every 2^16 points
  std::vector<int> beginAccess, endAccess;
  double stepSize = std::pow(2, 16);
  unsigned int neededCycles = cloudInOut.size() / stepSize;
  unsigned long startIterLoc{0}, endIterLoc;
  endIterLoc = stepSize - 1;

  while (neededCycles > 0)
  {
    int howManyThreadsDoIWantToCreate = std::min(neededCycles, threads2Spawn);

    std::vector<std::future<void>> futures;
    for (int i = 0; i < howManyThreadsDoIWantToCreate; ++i) // start all threads with the lambda
    {
      futures.push_back(std::async(std::launch::async, cloudPartRandomizer, startIterLoc, endIterLoc));

      startIterLoc = endIterLoc + 1;
      endIterLoc = startIterLoc + stepSize - 1;
      --neededCycles;

      if (endIterLoc >= cloudInOut.size())
        break; // to be sure not to overshoot, should not happen
    }

    for (auto &future : futures) // wait till all threads have finished
      future.get();
  }

  // calculate remaining points
  cloudPartRandomizer(startIterLoc, cloudInOut.size() - 1);
}
*/
