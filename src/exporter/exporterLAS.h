#pragma once

#include "exporter.h"

#include "laszip/laszip_api.h"


class exporterLAZ final : public exporter
{
public:
  /// ctor
  exporterLAZ(std::string path);
  virtual ~exporterLAZ();
  void run() override;

private:
  laszip_POINTER laszip_writer_{};
  laszip_header *header_{};
};
