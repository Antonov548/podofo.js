#include <emscripten/bind.h>

#include <podofo/podofo.h>

namespace em = emscripten;

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

    em::val save(PoDoFo::PdfMemDocument& document)
    {
        std::vector<char> buffer;
        PoDoFo::VectorStreamDevice device{buffer};
        
        document.Save(device);
        const auto Uint8ClampedArray{em::val::global("Uint8ClampedArray")};

        return Uint8ClampedArray.new_(em::typed_memory_view(buffer.size(), reinterpret_cast<const uint8_t*>(buffer.data())));
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

    // PdfPage
    em::val getRect(const PoDoFo::PdfPage& page)
    {
        std::array<double, 4> array_rect{};

        const auto rect{page.GetRect()};
        array_rect[0] = rect.X;
        array_rect[1] = rect.Y;
        array_rect[2] = rect.Width;
        array_rect[3] = rect.Height;

        return em::val::array(std::begin(array_rect), std::end(array_rect));
    }

    std::vector<char> makeBuffer(em::val jbuffer)
    {
        return vecFromJSArray<char>(jbuffer);
    }
}

EMSCRIPTEN_BINDINGS(PODOFO)
{
    em::class_<PoDoFo::PdfPageCollection>("PdfPageCollection")
        .function("getCount", &PoDoFo::PdfPageCollection::GetCount)
        .function("getPage", &getPage, em::allow_raw_pointers())
        .function("createPage", createPage, em::allow_raw_pointers())
    ;

    em::class_<PoDoFo::PdfPage>("PdfPage")
        .function("getRect", &getRect)    
    ;

    em::class_<PoDoFo::PdfMemDocument>("PdfMemDocument")
        .constructor()
        .function("loadFromBuffer", &loadFromBuffer)
        .function("getPages", &getPages, em::allow_raw_pointers())
        .function("save", &save);
    ;

    em::function("makeBuffer", &makeBuffer);

    em::register_vector<char>("Buffer");
}