#pragma once

#include "exporter.h"

class ExporterXYZ final : public Exporter
{
public:
  /// ctor
  ExporterXYZ(std::string path);
  void run() override;

private:
  /// our output file stream
  std::ofstream out_;
};
