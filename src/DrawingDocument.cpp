#include "DrawingDocument.hpp"
#include "StreamUtils.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(DrawingDocument, wxDocument);

std::ostream& DrawingDocument::SaveObject(std::ostream& stream)
{
  wxXmlDocument doc = serializer.SerializePaths(squiggles);

  OStreamWrapper wrapper(stream);
  serializer.CompressXml(doc, wrapper);

  return stream;
}

std::istream& DrawingDocument::LoadObject(std::istream& stream)
{
  IStreamWrapper wrapper(stream);
  wxXmlDocument doc = serializer.DecompressXml(wrapper);

  squiggles = serializer.DeserializePaths(doc);

  stream.clear();
  return stream;
}