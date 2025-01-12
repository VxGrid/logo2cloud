#pragma once

#include "exporter.h"

#include <E57Format/E57Export.h>
#include <E57Format/E57SimpleWriter.h>


class ExporterE57 final : public Exporter
{
public:
    /// ctor
    ExporterE57(std::string path);
    /// dtor
    virtual ~ExporterE57();
    void run() override;

private:
    /// E57 writer
    std::unique_ptr<e57::Writer> e57Writer_{nullptr};

    /// E57 header - since we want to write only "one" scan
    std::unique_ptr<e57::Data3D> e57Header_;

    /// The actual writer
    std::unique_ptr<e57::CompressedVectorWriter> e57comprVecWriter_;

    /// The buffers
    std::vector<double> x_, y_, z_, refl_;
    std::vector<uint16_t> r_, g_, b_;

    /// The data index where to open the compressed vector writer - used to index buffers just once
    int64_t dataIndex_{-1};
};

