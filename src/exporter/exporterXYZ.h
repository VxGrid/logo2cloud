#pragma once

#include "exporter.h"

class exporterXYZ final : public exporter
{
public:
  /// ctor
  exporterXYZ(std::string path);
  void run() override;

private:
  /// our output file stream
  std::ofstream out_;
};
