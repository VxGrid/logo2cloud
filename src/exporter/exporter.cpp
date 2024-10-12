#include "exporter.h"


Exporter::~Exporter()
{
}


void Exporter::setPath(std::string path)
{
    path_ = path;
}


void Exporter::setData(const std::vector<Point>& cloud)
{
    cloudData_ = cloud;
}
