#include <emscripten/bind.h>
#include <podofo/podofo.h>

namespace em = emscripten;

struct embeded_resource
{
  const uint8_t* data;
  size_t size;
};

struct embeded_header
{
  const embeded_resource* entries;
  int count;
};

extern "C" const embeded_header embeded_fonts;

namespace Document
{
struct DocumentWrapper
{
  PoDoFo::PdfMemDocument document;
  std::vector<uint8_t> buffer;
};

void loadFromBuffer(DocumentWrapper& wrapper, em::val jbuffer)
{
  wrapper.buffer = vecFromJSArray<uint8_t>(jbuffer);
  wrapper.document.LoadFromBuffer(
      {reinterpret_cast<const char*>(wrapper.buffer.data()), wrapper.buffer.size()});
}

PoDoFo::PdfPageCollection* getPages(DocumentWrapper& wrapper)
{
  return &wrapper.document.GetPages();
}

PoDoFo::PdfMetadata* getMetadata(DocumentWrapper& wrapper)
{
  return &wrapper.document.GetMetadata();
}

PoDoFo::PdfFontManager* getFonts(DocumentWrapper& wrapper)
{
  return &wrapper.document.GetFonts();
}

em::val save(DocumentWrapper& wrapper)
{
  std::vector<char> buffer;
  PoDoFo::VectorStreamDevice device{buffer};

  wrapper.document.Save(device);
  const auto Uint8ClampedArray{em::val::global("Uint8ClampedArray")};

  return Uint8ClampedArray.new_(em::typed_memory_view(
      buffer.size(), reinterpret_cast<const uint8_t*>(buffer.data())));
}
}  // namespace Document

namespace FontManager
{
PoDoFo::PdfFont* getDefaultFont(PoDoFo::PdfFontManager& manager)
{
  const auto& embeded_font{embeded_fonts.entries[0]};
  return &manager.GetOrCreateFontFromBuffer(
      {reinterpret_cast<const char*>(embeded_font.data), embeded_font.size});
}

PoDoFo::PdfFont* getFontFromBuffer(PoDoFo::PdfFontManager& manager, em::val jbuffer)
{
  const auto buffer{vecFromJSArray<uint8_t>(jbuffer)};
  return &manager.GetOrCreateFontFromBuffer({reinterpret_cast<const char*>(buffer.data()), buffer.size()});
}
}  // namespace FontManager

namespace Metadata
{
void setAuthor(PoDoFo::PdfMetadata& metadata, const std::string& author)
{
  metadata.SetAuthor(PoDoFo::PdfString(author));
}

em::val getAuthor(PoDoFo::PdfMetadata& metadata)
{
  const auto author{metadata.GetAuthor()};
  return author.has_value() ? em::val(author->GetString()) : em::val::null();
}

void setSubject(PoDoFo::PdfMetadata& metadata, const std::string& subject)
{
  metadata.SetSubject(PoDoFo::PdfString(subject));
}

em::val getSubject(PoDoFo::PdfMetadata& metadata)
{
  const auto subject{metadata.GetSubject()};
  return subject.has_value() ? em::val(subject->GetString()) : em::val::null();
}

void setCreator(PoDoFo::PdfMetadata& metadata, const std::string& creator)
{
  metadata.SetCreator(PoDoFo::PdfString(creator));
}

em::val getCreator(PoDoFo::PdfMetadata& metadata)
{
  const auto creator{metadata.GetCreator()};
  return creator.has_value() ? em::val(creator->GetString()) : em::val::null();
}

void setTitle(PoDoFo::PdfMetadata& metadata, const std::string& title)
{
  metadata.SetTitle(PoDoFo::PdfString(title));
}

em::val getTitle(PoDoFo::PdfMetadata& metadata)
{
  const auto title{metadata.GetTitle()};
  return title.has_value() ? em::val(title->GetString()) : em::val::null();
}

void resetTitle(PoDoFo::PdfMetadata& metadata) { metadata.SetTitle(nullptr); }

}  // namespace Metadata

namespace PageCollection
{
PoDoFo::PdfPage* createPage(PoDoFo::PdfPageCollection& pages, em::val jrect)
{
  const auto& rect{vecFromJSArray<double>(jrect)};
  return &pages.CreatePage({rect[0], rect[1], rect[2], rect[3]});
}

PoDoFo::PdfPage* getPage(PoDoFo::PdfPageCollection& pages, int index)
{
  return &pages.GetPageAt(index);
}
}  // namespace PageCollection

namespace Painter
{
void setFont(PoDoFo::PdfPainter& painter, const PoDoFo::PdfFont& font,
             double size)
{
  painter.TextState.SetFont(font, size);
}

void drawText(PoDoFo::PdfPainter& painter, const std::string& text, double x,
              double y)
{
  painter.DrawText(text, x, y);
}
}  // namespace Painter

namespace Image
{

std::unique_ptr<PoDoFo::PdfImage> makeFromObject([[maybe_unused]] const std::string& key, PoDoFo::PdfObject& object)
{
  std::unique_ptr<PoDoFo::PdfImage> image;
  PoDoFo::PdfXObject::TryCreateFromObject<PoDoFo::PdfImage>(object, image);

  return image;
}

std::unique_ptr<PoDoFo::PdfImage> makeImage(Document::DocumentWrapper& wrapper)
{
  return wrapper.document.CreateImage();
}

void loadFromBuffer(PoDoFo::PdfImage& image, em::val jbuffer)
{
  const auto buffer{vecFromJSArray<uint8_t>(jbuffer)};
  image.LoadFromBuffer({reinterpret_cast<const char*>(buffer.data()), buffer.size()});
}
}  // namespace Image

namespace Page
{
PoDoFo::PdfResources* getResources(PoDoFo::PdfPage& page)
{
  return page.GetResources();
}

em::val extractText(const PoDoFo::PdfPage& page)
{
  std::vector<PoDoFo::PdfTextEntry> es;
  page.ExtractTextTo(es);

  em::val entries{em::val::array()};
  for (const auto& e : es)
  {
    em::val entry{em::val::object()};
    entry.set("text", e.Text);

    entries.call<void>("push", entry);
  }

  return entries;
}

em::val rectToArray(const PoDoFo::Rect& rect)
{
  std::array<double, 4> array_rect{};
  array_rect[0] = rect.X;
  array_rect[1] = rect.Y;
  array_rect[2] = rect.Width;
  array_rect[3] = rect.Height;

  return em::val::array(std::begin(array_rect), std::end(array_rect));
}

em::val getRect(const PoDoFo::PdfPage& page)
{
  const auto rect{page.GetRect()};
  return rectToArray(rect);
}

em::val getPageSize(const PoDoFo::PdfPageSize& page_size,
                    bool landscape = false)
{
  const auto size{
      PoDoFo::PdfPage::CreateStandardPageSize(page_size, landscape)};
  return rectToArray(size);
}
}  // namespace Page

namespace Resources
{
em::val getArray(PoDoFo::PdfResources& resources, PoDoFo::PdfResourceType type)
{
  em::val objects{em::val::object()};
  for (auto& res : resources.GetResourceIterator(type))
    objects.set(res.first.GetString(), res.second);

  return objects;
}
}

namespace
{
std::vector<char> makeBuffer(em::val jbuffer)
{
  return vecFromJSArray<char>(jbuffer);
}

std::string error(std::uintptr_t p) 
{
  return reinterpret_cast<std::exception*>(p)->what();
}

}  // namespace

// clang-format off

EMSCRIPTEN_BINDINGS(PODOFO)
{
  em::class_<PoDoFo::PdfObject>("PdfObject")
  ;

  em::class_<PoDoFo::PdfPageCollection>("PageCollection")
    .function("getCount", &PoDoFo::PdfPageCollection::GetCount)
    .function("getPage", &PageCollection::getPage, em::allow_raw_pointers())
    .function("createPage", &PageCollection::createPage, em::allow_raw_pointers())
  ;

  em::class_<PoDoFo::PdfFontManager>("FontManager")
    .function("getDefaultFont", &FontManager::getDefaultFont, em::allow_raw_pointers())
    .function("loadFromBuffer", &FontManager::getFontFromBuffer, em::allow_raw_pointers())
  ;

  em::class_<PoDoFo::PdfFont>("Font")
  ;

  em::class_<PoDoFo::PdfPage, em::base<PoDoFo::PdfCanvas>>("Page")
    .function("getRect", &Page::getRect)
    .function("extractText", &Page::extractText)
    .function("getResources", &Page::getResources, em::allow_raw_pointers())
  ;

  em::class_<PoDoFo::PdfCanvas>("Canvas")
  ;

  em::class_<PoDoFo::PdfResources>("Resources")
    .function("getArray", &Resources::getArray)
  ;

  em::enum_<PoDoFo::PdfResourceType>("ResourceType")
    .value("Unknown", PoDoFo::PdfResourceType::Unknown)
    .value("ExtGState", PoDoFo::PdfResourceType::ExtGState)
    .value("ColorSpace", PoDoFo::PdfResourceType::ColorSpace)
    .value("Pattern", PoDoFo::PdfResourceType::Pattern)
    .value("Shading", PoDoFo::PdfResourceType::Shading)
    .value("XObject", PoDoFo::PdfResourceType::XObject)
    .value("Font", PoDoFo::PdfResourceType::Font)
    .value("Properties", PoDoFo::PdfResourceType::Properties)
  ;

  em::class_<Document::DocumentWrapper>("Document")
    .constructor()
    .function("loadFromBuffer", &Document::loadFromBuffer)
    .function("getPages", &Document::getPages, em::allow_raw_pointers())
    .function("getFonts", &Document::getFonts, em::allow_raw_pointers())
    .function("getMetadata", &Document::getMetadata, em::allow_raw_pointers())
    .function("save", &Document::save);
  ;

  em::class_<PoDoFo::PdfMetadata>("Metadata")
    .function("setTitle", &Metadata::setTitle)
    .function("getTitle", &Metadata::getTitle)
    .function("setSubject", &Metadata::setSubject)
    .function("getSubject", &Metadata::getSubject)
    .function("setAuthor", &Metadata::setAuthor)
    .function("getAuthor", &Metadata::getAuthor)
    .function("setCreator", &Metadata::setCreator)
    .function("getCreator", &Metadata::getCreator)
    .function("resetTitle", &Metadata::resetTitle)
  ;

  em::class_<PoDoFo::PdfPainter>("Painter")
    .constructor()
    .function("setCanvas", &PoDoFo::PdfPainter::SetCanvas)
    .function("setFont", &Painter::setFont, em::allow_raw_pointers())
    .function("drawText", &Painter::drawText)
    .function("drawImage", &PoDoFo::PdfPainter::DrawImage)
    .function("drawCircle", &PoDoFo::PdfPainter::DrawCircle)
    .function("drawLine", &PoDoFo::PdfPainter::DrawLine)
    .function("drawCubicBezier", &PoDoFo::PdfPainter::DrawCubicBezier)
    .function("drawArc", &PoDoFo::PdfPainter::DrawArc)
    .function("drawEllipse", &PoDoFo::PdfPainter::DrawEllipse)
    .function("finishDrawing", &PoDoFo::PdfPainter::FinishDrawing)
  ;

  em::class_<PoDoFo::PdfImage>("Image")
    .constructor(&Image::makeImage)
    .constructor(&Image::makeFromObject)
    .function("loadFromBuffer", &Image::loadFromBuffer)
    .property("width", &PoDoFo::PdfImage::GetWidth)
    .property("height", &PoDoFo::PdfImage::GetHeight)
  ;

  em::enum_<PoDoFo::PdfPathDrawMode>("PathDrawMode")
    .value("Stroke", PoDoFo::PdfPathDrawMode::Stroke)
    .value("Fill", PoDoFo::PdfPathDrawMode::Fill)
    .value("StrokeFill", PoDoFo::PdfPathDrawMode::StrokeFill)
    .value("FillEvenOdd", PoDoFo::PdfPathDrawMode::FillEvenOdd)
    .value("StrokeFillEvenOdd", PoDoFo::PdfPathDrawMode::StrokeFillEvenOdd)   
  ;

  em::function("getPageSize", &Page::getPageSize);

  em::enum_<PoDoFo::PdfPageSize>("PageSize")
    .value("A0", PoDoFo::PdfPageSize::A0)
    .value("A1", PoDoFo::PdfPageSize::A1)
    .value("A2", PoDoFo::PdfPageSize::A2)
    .value("A3", PoDoFo::PdfPageSize::A3)
    .value("A4", PoDoFo::PdfPageSize::A4)
    .value("A5", PoDoFo::PdfPageSize::A5)
    .value("A6", PoDoFo::PdfPageSize::A6)
    .value("Letter", PoDoFo::PdfPageSize::Letter)
    .value("Legal", PoDoFo::PdfPageSize::Legal)
    .value("Tabloid", PoDoFo::PdfPageSize::Tabloid)
  ;

  em::function("error", &error);
}
