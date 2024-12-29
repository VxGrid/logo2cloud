#include "exporter.h"

#include <chrono>

Exporter::~Exporter()
{
}


void Exporter::setPath(std::string path)
{
    path_ = path;
}


void Exporter::setData(const std::vector<Point>& cloud)
{
    cloudData_ = cloud;
}


std::tuple<uint16_t, uint8_t, uint8_t, uint16_t> Exporter::getDate()
{
    auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    std::tm* lt = localtime(&t_c);
    return {lt->tm_year, lt->tm_mon + 1, lt->tm_mday, lt->tm_yday};
}
