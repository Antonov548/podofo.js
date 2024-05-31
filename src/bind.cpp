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

namespace
{
    // PdfMemDocument
    void loadFromBuffer(PoDoFo::PdfMemDocument& document, const std::vector<char>& buffer)
    {
        document.LoadFromBuffer({buffer.data(), buffer.size()});
    }

    PoDoFo::PdfPageCollection* getPages(PoDoFo::PdfMemDocument& document)
    {
        return &document.GetPages();
    }

    PoDoFo::PdfMetadata* getMetadata(PoDoFo::PdfMemDocument& document)
    {
        return &document.GetMetadata();
    }

    PoDoFo::PdfFontManager* getFonts(PoDoFo::PdfMemDocument& document)
    {
        return &document.GetFonts();
    }

    em::val save(PoDoFo::PdfMemDocument& document)
    {
        std::vector<char> buffer;
        PoDoFo::VectorStreamDevice device{buffer};
        
        document.Save(device);
        const auto Uint8ClampedArray{em::val::global("Uint8ClampedArray")};

        return Uint8ClampedArray.new_(em::typed_memory_view(buffer.size(), reinterpret_cast<const uint8_t*>(buffer.data())));
    }

    // PdfFontManager
    PoDoFo::PdfFont* getDefaultFont(PoDoFo::PdfFontManager& manager)
    {
        const auto& embeded_font{embeded_fonts.entries[0]};
        return &manager.GetOrCreateFontFromBuffer({reinterpret_cast<const char*>(embeded_font.data), embeded_font.size});
    }

    // PdfMetadata
    void setTitle(PoDoFo::PdfMetadata& metadata, const std::string& title)
    {
        metadata.SetTitle(PoDoFo::PdfString(title));
    }

    void resetTitle(PoDoFo::PdfMetadata& metadata)
    {
        metadata.SetTitle(nullptr);
    }

    em::val getTitle(PoDoFo::PdfMetadata& metadata)
    {
        const auto title{metadata.GetTitle()};
        return title.has_value() ? em::val(title->GetString()) : em::val::null();
    }

    // PdfPageCollection
    PoDoFo::PdfPage* createPage(PoDoFo::PdfPageCollection& pages, em::val jrect)
    {
        const auto& rect{vecFromJSArray<double>(jrect)};
        return &pages.CreatePage({rect[0], rect[1], rect[2], rect[3]});
    }

    PoDoFo::PdfPage* getPage(PoDoFo::PdfPageCollection& pages, int index)
    {
        return &pages.GetPageAt(index);
    }

    // PdfPainter
    void setFont(PoDoFo::PdfPainter& painter, const PoDoFo::PdfFont& font, double size)
    {
        painter.TextState.SetFont(font, size);
    }

    void drawText(PoDoFo::PdfPainter& painter, const std::string& text, double x, double y)
    {
        painter.DrawText(text, x, y);
    }

    // PdfPage
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

    em::val getPageSize(const PoDoFo::PdfPageSize& page_size, bool landscape = false)
    {
        const auto size{PoDoFo::PdfPage::CreateStandardPageSize(page_size, landscape)};
        return rectToArray(size);
    }

    std::vector<char> makeBuffer(em::val jbuffer)
    {
        return vecFromJSArray<char>(jbuffer);
    }
}

EMSCRIPTEN_BINDINGS(PODOFO)
{
    em::class_<PoDoFo::PdfPageCollection>("PageCollection")
        .function("getCount", &PoDoFo::PdfPageCollection::GetCount)
        .function("getPage", &getPage, em::allow_raw_pointers())
        .function("createPage", createPage, em::allow_raw_pointers())
    ;

    em::class_<PoDoFo::PdfFontManager>("FontManager")
        .function("getDefaultFont", &getDefaultFont, em::allow_raw_pointers())
    ;

    em::class_<PoDoFo::PdfFont>("Font")
    ;

    em::class_<PoDoFo::PdfPage, em::base<PoDoFo::PdfCanvas>>("Page")
        .function("getRect", &getRect)
        .function("extractText", &extractText)
    ;

    em::class_<PoDoFo::PdfCanvas>("Canvas")
    ;

    em::class_<PoDoFo::PdfMemDocument>("Document")
        .constructor()
        .function("loadFromBuffer", &loadFromBuffer)
        .function("getPages", &getPages, em::allow_raw_pointers())
        .function("getFonts", &getFonts, em::allow_raw_pointers())
        .function("getMetadata", &getMetadata, em::allow_raw_pointers())
        .function("save", &save);
    ;

    em::class_<PoDoFo::PdfMetadata>("Metadata")
        .function("setTitle", &setTitle)
        .function("getTitle", &getTitle)
        .function("resetTitle", &resetTitle)
    ;

    em::class_<PoDoFo::PdfPainter>("Painter")
        .constructor()
        .function("setCanvas", &PoDoFo::PdfPainter::SetCanvas)
        .function("setFont", &setFont, em::allow_raw_pointers())
        .function("drawText", &drawText)
        .function("drawCircle", &PoDoFo::PdfPainter::DrawCircle)
        .function("finishDrawing", &PoDoFo::PdfPainter::FinishDrawing)
    ;

    em::enum_<PoDoFo::PdfPathDrawMode>("PathDrawMode")
        .value("Stroke", PoDoFo::PdfPathDrawMode::Stroke)
        .value("Fill", PoDoFo::PdfPathDrawMode::Fill)
        .value("StrokeFill", PoDoFo::PdfPathDrawMode::StrokeFill)
        .value("FillEvenOdd", PoDoFo::PdfPathDrawMode::FillEvenOdd)
        .value("StrokeFillEvenOdd", PoDoFo::PdfPathDrawMode::StrokeFillEvenOdd)   
    ;

    em::function("getPageSize", &getPageSize);

    em::class_<std::vector<char>>("Buffer")
        .constructor(&makeBuffer)
    ;

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
}
