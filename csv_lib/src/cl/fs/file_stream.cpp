#include <cassert>

#include <pl/unreachable.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "fs/file_stream.hpp"

namespace cl::fs {
namespace {
const char* mapOpenMode(FileStream::OpenMode openMode)
{
  switch (openMode) {
  case FileStream::Read: return "rb";
  case FileStream::Write: return "wb";
  case FileStream::ReadWrite: return "wb+";
  }

  PL_UNREACHABLE();
}
} // namespace

Expected<FileStream> FileStream::create(const File& file, OpenMode openMode)
{
  std::FILE* fh{std::fopen(file.path().str().c_str(), mapOpenMode(openMode))};

  if (fh == nullptr) {
    return CL_UNEXPECTED(
      Error::Filesystem, fmt::format("Couldn't open \"{}\"", file.path()));
  }

  const std::int64_t size{file.size()};

  if (size == -1) {
    return CL_UNEXPECTED(
      Error::Filesystem,
      fmt::format("Couldn't determine size of file \"{}\"", file.path()));
  }

  return FileStream{fh, size};
}

FileStream::~FileStream()
{
  const int errC{std::fclose(m_file)};

  (void)errC;
  if (errC != 0) { assert(false && "Failure to close file in ~FileStream") }
}

bool FileStream::write(const void* data, std::size_t byteCount)
{
  const std::size_t res{std::fwrite(data, 1, byteCount, m_file)};

  return res == byteCount;
}

std::vector<pl::byte> FileStream::readAll() const
{
  using namespace pl::integer_literals;
  std::vector<pl::byte> result(m_size, 0x00_byte);

  const std::size_t v{std::fread(result.data(), 1, m_size, m_file)};
  (void)v;
  assert(v == m_size && "Read error in FileStream::readAll.");

  return result;
}
} // namespace cl::fs
