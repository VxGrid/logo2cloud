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
    virtual ~ExporterE57() = default;
    void run() override;

private:
    /// E57 writer
    std::unique_ptr<e57::Writer> e57Writer_{nullptr};

    /// E57 header - since we want to write only "one" scan
    std::unique_ptr<e57::Data3D> e57Header_;

    /// Amount of headers written
    uint16_t headerCounter_{};
};

