#include "exporter.h"


exporter::~exporter()
{
}


void exporter::setPath(std::string path)
{
    path_ = path;
}


void exporter::setData(const std::vector<Point>& cloud)
{
    cloudData_ = cloud;
}
