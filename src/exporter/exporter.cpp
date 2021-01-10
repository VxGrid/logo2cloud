#include "exporter.h"
#include <fstream>
#include <iomanip>
#include <iostream>


exporter::~exporter()
{

}


void exporter::setPath(std::string path)
{
    path_=path;
}


void exporter::setData(const std::vector<Point> &cloud)
{
    cloudData_ = cloud;
}

//////// Start exporterXYZ ////////
void exporterXYZ::run()
{
    std::cout<<"Run: "<<__func__<<"function: "<<__FUNCTION__<<'\n';
    std::cout<<"We have values: "<<cloudData_.size()<<'\n';
    std::ofstream out;
    out.open(path_, std::ofstream::out);
    for (const auto &point : cloudData_)
    {
        out<<std::setprecision(4)<<
             point.x_ << '\t' <<
             point.y_ << '\t' <<
             point.z_ << '\t';

        out<<std::setprecision(0)<<
             //int(point.i_)<< '\t' << // TODO: we need to check if we have set refl/intensity
             int(point.r_) << '\t' <<
             int(point.g_) << '\t' <<
             int(point.b_) << '\n';
    }
    out.close();
}

//////// Start exporterLAZ ////////
void exporterLAZ::run()
{

}
