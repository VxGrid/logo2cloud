#include "exporter.h"
#include <iomanip>


exporter::~exporter()
{
}


void exporter::setPath(std::string path)
{
  path_ = path;
}


void exporter::setData(const std::vector<Point> &cloud)
{
  cloudData_ = cloud;
}

//////// Start exporterXYZ ////////
exporterXYZ::exporterXYZ(std::string path)
{
  out_.open(path, std::ofstream::out);
}


void exporterXYZ::run()
{
  for (const auto &point : cloudData_)
  {
    out_ << std::setprecision(4) << point.x_ << '\t' << point.y_ << '\t' << point.z_ << '\t';

    out_ << std::setprecision(0) <<
        //int(point.i_)<< '\t' << // TODO: we need to check if we have set refl/intensity
        int(point.r_) << '\t' << int(point.g_) << '\t' << int(point.b_) << '\n';
  }
}

//////// Start exporterLAZ ////////
void exporterLAZ::run()
{
}
