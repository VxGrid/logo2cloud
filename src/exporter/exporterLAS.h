#pragma once

#include "exporter.h"

#include "laszip/laszip_api.h"

#include <memory>


class exporterLAZ final : public exporter
{
public:
  /// ctor
  exporterLAZ(std::string path);
  virtual ~exporterLAZ();
  void run() override;

private:
  /// our output file stream
  std::ofstream out_;

  laszip_POINTER laszPtr_{};

  laszip_POINTER laszip_writer_{};

  laszip_header *header_{};
  /// header of las file
  //std::unique_ptr<liblas::Header> lasHeader_;

  /// the actual las writer
  //std::unique_ptr<liblas::Writer> lasWriter_;
};
