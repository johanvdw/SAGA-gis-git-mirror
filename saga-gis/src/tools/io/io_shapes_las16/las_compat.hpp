#ifndef LAS_COMPAT_H
#define LAS_COMPAT_H

#include <liblas/version.hpp>
#if defined(LIBLAS_VERSION) && LIBLAS_VERSION >= 106000
#include <liblas/point.hpp>
#include <liblas/writer.hpp>
#include <liblas/reader.hpp>

#define LAS_COLOR liblas::Color
#define LAS_POINT liblas::Point
#define LAS_HEADER liblas::Header
#define LAS_READER liblas::Reader
#define LAS_WRITER liblas::Writer
#define LAS_UINT8 boost::uint8_t
#define LAS_UINT16 boost::uint16_t
#define LAS_UINT32 boost::uint32_t
#define LAS_INT8 boost::int8_t
#define LAS_EPOINTFORMAT0 liblas::ePointFormat0
#define LAS_EPOINTFORMAT1 liblas::ePointFormat1
#define LAS_EPOINTFORMAT2 liblas::ePointFormat2
#define LAS_EPOINTFORMAT3 liblas::ePointFormat3
#else
#include <liblas/laspoint.hpp>
#include <liblas/laswriter.hpp>
#include <liblas/lasreader.hpp>

#define LAS_COLOR liblas::LASColor
#define LAS_POINT liblas::LASPoint
#define LAS_HEADER liblas::LASHeader
#define LAS_READER liblas::LASReader
#define LAS_WRITER liblas::LASWriter
#define LAS_UINT16 liblas::uint16_t
#define LAS_UINT32 liblas::uint32_t
#define LAS_INT8 liblas::int8_t
#define LAS_EPOINTFORMAT0 liblas::LASHeader::ePointFormat0
#define LAS_EPOINTFORMAT1 liblas::LASHeader::ePointFormat1
#define LAS_EPOINTFORMAT2 liblas::LASHeader::ePointFormat2
#define LAS_EPOINTFORMAT3 liblas::LASHeader::ePointFormat3
#endif

#endif // LAS_COMPAT_H
