#pragma once

#include <wx/docview.h>
#include <wx/stdstream.h>
#include <iostream>
#include "XmlSerializer.hpp"
#include "Path.hpp"

class DrawingDocument: public wxDocument
{
public:
  std::ostream& SaveObject(std::ostream& stream) override;
  std::istream& LoadObject(std::istream& stream) override;

  std::vector<Path> squiggles;
  XmlSerializer serializer;

  wxDECLARE_DYNAMIC_CLASS(DrawingDocument);
};