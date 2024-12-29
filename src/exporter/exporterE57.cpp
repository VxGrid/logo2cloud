#include "exporterE57.h"

#include <limits>

#include <E57Format/E57Exception.h>
#include <E57Format/E57Export.h>
#include <E57Format/E57SimpleData.h>
#include <E57Format/E57SimpleWriter.h>
#include <E57Format/E57Version.h>

ExporterE57::ExporterE57(std::string path)
{
    std::cerr << "E57 version:\n";
    std::cerr << e57::Version::astm() << ' ' << e57::Version::library() << '\n';
    e57::WriterOptions options;
    options.guid = "logo2Cloud GUID";
    e57Writer_ = std::make_unique<e57::Writer>(path, options);
}


void ExporterE57::run()
{
    ++headerCounter_;
    e57::Data3D header;
    header.guid = std::string("logo2Cloud ") + std::to_string(headerCounter_);
    header.description = "logo2Cloud - libE57Format";
    header.pointCount = cloudData_.size();
    /// set header stuff
    header.pointFields.cartesianXField = true;
    header.pointFields.cartesianYField = true;
    header.pointFields.cartesianZField = true;
    header.pointFields.colorRedField = true;
    header.pointFields.colorGreenField = true;
    header.pointFields.colorBlueField = true;
    header.colorLimits.colorRedMaximum = 255;
    header.colorLimits.colorGreenMaximum = 255;
    header.colorLimits.colorBlueMaximum = 255;
    /// set data stuff
    e57::Data3DPointsDouble pointsData(header);
    int64_t i{0};

    for (const auto &pointXYZ : cloudData_)
    {
        pointsData.cartesianX[i] = pointXYZ.x_;
        pointsData.cartesianY[i] = pointXYZ.y_;
        pointsData.cartesianZ[i] = pointXYZ.z_;
        pointsData.colorRed[i] = pointXYZ.r_;
        pointsData.colorGreen[i] = pointXYZ.g_;
        pointsData.colorBlue[i] = pointXYZ.b_;
        ++i;
    }

    try {
        e57Writer_->WriteData3DData(header, pointsData);
    }
    catch (const e57::E57Exception &e)
    {
        std::cerr << "E57 Exception error: " << e.what() << ' ' << e.errorStr() << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << "STD E57 error: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "... E57 error\n";
    }
}
