#include "exporterXYZ.h"
#include <iomanip>


ExporterXYZ::ExporterXYZ(std::string path)
{
    out_.open(path, std::ofstream::out);
}


void ExporterXYZ::run()
{
    for (const auto &point : cloudData_)
    {
        out_ << std::setprecision(4) << point.x_ << '\t' << point.y_ << '\t' << point.z_ << '\t';
        out_ << std::setprecision(0) <<
            //int(point.i_)<< '\t' << // TODO: we need to check if we have set refl/intensity
            int(point.r_) << '\t' << int(point.g_) << '\t' << int(point.b_) << '\n';
    }
}
