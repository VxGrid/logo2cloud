#include "pointcloudgenerator.h"

#include <algorithm>
#include <memory>

pointcloudgenerator::pointcloudgenerator()
{
}


void pointcloudgenerator::setData(const unsigned char *imgDataPtr, unsigned int rows, unsigned int cols)
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


void pointcloudgenerator::exportCloud(std::string path, pointcloudgenerator::EXPORTER cloudFormat, bool structured)
{
    // Calculate image into point cloud
    std::vector<Point> cloud;
    image2Cloud(cloud);

    // instantiate export format
    std::unique_ptr<exporter> exp;
    switch (cloudFormat) {
    case EXPORTER::XYZ:
        path.append(".xyz");
        exp=std::make_unique<exporterXYZ>();
        break;
    default:
        // no format defined
        break;
    }

    exp->setPath(path);
    exp->setData(cloud);
    exp->run();
}


void pointcloudgenerator::image2Cloud(std::vector<Point> &cloud)
{
    pixel2Cloud(cloud);

    addDepth2Cloud(cloud);

    // TODO: add option to randomize the point position by certain value (so we reduce aliasing effects)

    randomizeCloud(cloud);
}


void pointcloudgenerator::pixel2Cloud(std::vector<Point> &cloud)
{
    // Calculate size we will need for our vector:
    cloud.reserve(rows_ * cols_ * (cDepth_ * rows_ / cHeight_)); // FIXME: currently we have one pixel converts to one point.
    unsigned int iter{};

    for (auto r = 0; r < rows_; ++r)
    {
        for (auto c = 0; c < cols_; ++c)
        {
            iter = (r * cols_ + c) * 4; // position in image buffer
            if(!imgDataPtr_[iter+3]) // alpha empty, continue
                continue;

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

            double X = double(c) / double(cols_) * cWidth_;
            double Z = double(r) / double(rows_) * cHeight_;
            double Y = 0.0;
            cloud.emplace_back(Point{X,
                               Y,
                               Z,
                               imgDataPtr_[iter+2],
                               imgDataPtr_[iter+1],
                               imgDataPtr_[iter],
                               0});
        }
    }
}


void pointcloudgenerator::addDepth2Cloud(std::vector<Point> &cloud)
{
    const double pointspacing = cPointSpacing_; // based on spacing between height and width
    double currentDepthLoc = pointspacing;

    cloud.reserve(cloud.size() * (cDepth_ / pointspacing +1)); // neccessary for transform algorithm, crash if data copy happens
    using std::cbegin, std::cend;
    const auto currBegIter = begin(cloud);
    const auto currEndIter = end(cloud);

    while (currentDepthLoc < cDepth_)
    {
        std::transform(currBegIter, currEndIter, std::back_inserter(cloud), [currentDepthLoc](const auto p1)
        {
            Point p2 = p1;
            p2.y_ = currentDepthLoc;
            return p2;
        });

        currentDepthLoc += pointspacing;
    }
}


void pointcloudgenerator::randomizeCloud(std::vector<Point> &cloud)
{
    const int randNum = cRandomRange_ * 1000;


    for (auto &point : cloud)
    {
        point.x_+=double((rand() % (2*randNum)) - randNum) / 1000.0;
        point.y_+=double((rand() % (2*randNum)) - randNum) / 1000.0;
        point.z_+=double((rand() % (2*randNum)) - randNum) / 1000.0;
    }

}
