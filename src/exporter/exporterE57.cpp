#include "exporterE57.h"

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


ExporterE57::~ExporterE57()
{
    e57comprVecWriter_->close();
    e57Writer_->Close();
}


void ExporterE57::run()
{
    if (dataIndex_ == -1)
    {
        e57::Data3D header;
        header.name = "Scan 0";
        header.guid = std::string("logo2Cloud Scan 0");
        header.description = "logo2Cloud - libE57Format";
        //header.pointCount = cloudData_.size();
        /// set header stuff
        // Pose
        header.pose.rotation.w = 1.0;
        header.pose.rotation.x = 0.0;
        header.pose.rotation.y = 0.0;
        header.pose.rotation.z = 0.0;
        header.pose.translation.x = .0;
        header.pose.translation.y = .0;
        header.pose.translation.z = .0;
        // Fields
        header.pointFields.cartesianXField = true;
        header.pointFields.cartesianYField = true;
        header.pointFields.cartesianZField = true;
        header.pointFields.intensityField = true;
        header.pointFields.colorRedField = true;
        header.pointFields.colorGreenField = true;
        header.pointFields.colorBlueField = true;
        header.intensityLimits.intensityMinimum = .0;
        header.intensityLimits.intensityMaximum = 1.;
        header.colorLimits.colorRedMinimum = 0;
        header.colorLimits.colorGreenMinimum = 0;
        header.colorLimits.colorBlueMinimum = 0;
        header.colorLimits.colorRedMaximum = 255;
        header.colorLimits.colorGreenMaximum = 255;
        header.colorLimits.colorBlueMaximum = 255;
        dataIndex_ = e57Writer_->NewData3D(header);
        /// set data stuff
        e57::Data3DPointsDouble pointsData;
        x_.resize(cloudData_.size());
        y_.resize(cloudData_.size());
        z_.resize(cloudData_.size());
        refl_.resize(cloudData_.size());
        r_.resize(cloudData_.size());
        g_.resize(cloudData_.size());
        b_.resize(cloudData_.size());
        pointsData.cartesianX = x_.data();
        pointsData.cartesianY = y_.data();
        pointsData.cartesianZ = z_.data();
        pointsData.intensity = refl_.data();
        pointsData.colorRed = r_.data();
        pointsData.colorGreen = g_.data();
        pointsData.colorBlue = b_.data();
        e57comprVecWriter_ = std::make_unique<e57::CompressedVectorWriter>
            (e57Writer_->SetUpData3DPointsData(dataIndex_, cloudData_.size(), pointsData));
    }

    if (cloudData_.size() > x_.size())
    {
        std::cerr << __func__ << " The data size we recieved was bigger than anticipated\n";
        throw std::runtime_error("This should not have happend...");
    }

    uint64_t i{0};

    for (const auto &pointXYZ : cloudData_)
    {
        x_[i] = pointXYZ.x_;
        y_[i] = pointXYZ.y_;
        z_[i] = pointXYZ.z_;
        r_[i] = pointXYZ.r_;
        g_[i] = pointXYZ.g_;
        b_[i] = pointXYZ.b_;
        refl_[i] = pointXYZ.i_;
        ++i;
    }

    try
    {
        e57comprVecWriter_->write(cloudData_.size());
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
