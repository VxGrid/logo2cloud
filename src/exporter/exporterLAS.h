#pragma once

#include "exporter.h"

#include "laszip/laszip_api.h"


class ExporterLAZ final : public Exporter
{
public:
  /// ctor
  ExporterLAZ(std::string path);
  virtual ~ExporterLAZ();
  void run() override;

private:
  laszip_POINTER laszip_writer_{};
  laszip_header *header_{};
};
