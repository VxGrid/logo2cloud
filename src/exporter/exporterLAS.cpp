#include "exporterLAS.h"

#include <iostream>
#include <string.h>


ExporterLAZ::ExporterLAZ(std::string path)
{
#ifdef WIN32
  if (laszip_load_dll())
  {
    std::cout << "DLL ERROR: loading LASzip DLL" << std::endl;
    throw std::runtime_error("DLL ERROR: loading LASzip DLL");
  }
#endif

  // get version of LASzip DLL
  laszip_U8 version_major;
  laszip_U8 version_minor;
  laszip_U16 version_revision;
  laszip_U32 version_build;

  if (laszip_get_version(&version_major, &version_minor, &version_revision, &version_build))
  {
    std::cout << "DLL ERROR: getting LASzip DLL version number" << std::endl;
    throw std::runtime_error("DLL ERROR: getting LASzip DLL version number");
  }

  std::cout << "LASzip DLL version (maj.min.rev.build): " << int(version_major) << "." << int(version_minor) << "." << int(version_revision) << "." << int(version_build) << std::endl;

  if (laszip_create(&laszip_writer_))
  {
    std::cout << "DLL ERROR: creating laszip writer" << std::endl;
    throw std::runtime_error("DLL ERROR: creating laszip writer");
  }


  laszip_header *header_; // get a pointer to the header of the writer so we can populate it

  if (laszip_get_header_pointer(laszip_writer_, &header_))
  {
    std::cout << "DLL ERROR: getting header pointer from laszip writer" << std::endl;
    throw std::runtime_error("DLL ERROR: getting header pointer from laszip writer");
  }

  // populate the header
  header_->global_encoding = (1 << 0); // see LAS specification for details
  header_->version_major = 1;
  header_->version_minor = 2;
  strncpy(header_->system_identifier, "logo2Cloud", 32);
  header_->file_creation_day = 1;
  header_->file_creation_year = 2021;
  header_->point_data_format = 2;
  header_->point_data_record_length = 28;
  header_->number_of_point_records = 500; // this number should get updated anyway
  header_->number_of_points_by_return[0] = 3;
  header_->number_of_points_by_return[1] = 2;
  header_->max_x = 1000.0; // these numbers should get updated anyway
  header_->min_x = -1000.0;
  header_->max_y = 1000.0;
  header_->min_y = -1000.0;
  header_->max_z = 1000.0;
  header_->min_z = -1000.0;

  // optional: use the bounding box and the scale factor to create a "good" offset
  if (laszip_auto_offset(laszip_writer_))
  {
    std::cout << "DLL ERROR: during automatic offset creation" << std::endl;
    throw std::runtime_error("DLL ERROR: during automatic offset creation");
  }

  // open the writer
  const char *file_name_out = path.c_str();
  laszip_BOOL compress = (strstr(file_name_out, ".laz") != 0);

  if (laszip_open_writer(laszip_writer_, file_name_out, compress))
  {
    std::cout << "DLL ERROR: opening laszip writer for " << path << std::endl;
    throw std::runtime_error("DLL ERROR: opening laszip writer");
  }
}


ExporterLAZ::~ExporterLAZ()
{
  // close the writer
  if (laszip_close_writer(laszip_writer_)) // I guess throwing in a destructor is a nono? See E.16 ore guidelines
    std::cout << "DLL ERROR: closing laszip writer" << std::endl;

  // destroy the writer
  if (laszip_destroy(laszip_writer_))
    std::cout << "DLL ERROR: destroying laszip writer" << std::endl;

  #ifdef WIN32
  if (laszip_unload_dll())
    std::cout << "DLL ERROR: unloading DLL failed" << std::endl;
#endif
}


void ExporterLAZ::run()
{

  for (const auto &pointXYZ : cloudData_)
  {
    laszip_point *point;

    if (laszip_get_point_pointer(laszip_writer_, &point))
    {
      std::cout << "DLL ERROR: getting point pointer from laszip writer" << std::endl;
      throw std::runtime_error("DLL ERROR: getting point pointer from laszip writer");
    }

    laszip_F64 coordinates[3];
    coordinates[0] = pointXYZ.x_;
    coordinates[1] = pointXYZ.y_;
    coordinates[2] = pointXYZ.z_;

    if (laszip_set_coordinates(laszip_writer_, coordinates))
    {
      std::cout << "DLL ERROR: setting coordinates into point" << std::endl;
      throw std::runtime_error("DLL ERROR: setting coordinates into point");
    }

    point->rgb[0] = uint16_t(pointXYZ.r_) * 255;
    point->rgb[1] = uint16_t(pointXYZ.g_) * 255;
    point->rgb[2] = uint16_t(pointXYZ.b_) * 255;
    point->intensity = (int(pointXYZ.r_) + int(pointXYZ.g_) + int(pointXYZ.b_)) / 3;

    if (laszip_write_point(laszip_writer_))
    {
      std::cout << "DLL ERROR: writing point" << std::endl;
      throw std::runtime_error("DLL ERROR: writing point");
    }

    if (laszip_update_inventory(laszip_writer_))
    {
      std::cout << "DLL ERROR: updating inventory for point" << std::endl;
      throw std::runtime_error("DLL ERROR: updating inventory for point");
    }
  }
}
