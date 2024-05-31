describe('podofo.js', () => {
    it('draw text with embeded font', async () => {
        const Podofo = global.Podofo;

        const document = new Podofo.Document();
        const pages = document.getPages();

        const size = Podofo.getPageSize(Podofo.PageSize.A4, false);
        const page = pages.createPage(size);
        
        const fonts = document.getFonts();
        const font = fonts.getDefaultFont();

        const painter = new Podofo.Painter();
        painter.setCanvas(page);
        painter.setFont(font, 10);

        const text = 'ABCDEFGHIKLMNOPQRSTVXYZ';
        painter.drawText(text, 56.69, page.getRect()[3] - 56.69);
        
        const text2 = 'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЬЫЭЮЯ';
        painter.drawText(text2, 56.69, 56.69);

        painter.finishDrawing();

        const pdf = document.save();
        
        painter.delete();
        document.delete();

        const buffer = new Podofo.Buffer(pdf);
        
        const parsedDocument = new Podofo.Document();
        parsedDocument.loadFromBuffer(buffer);

        const parsedPages = parsedDocument.getPages();
        expect(parsedPages.getCount()).toEqual(1);

        const parsedPage = parsedPages.getPage(0);
        expect(parsedPage.getRect()).toEqual(size);

        const entries = parsedPage.extractText();

        const entry = entries.find((e) => { return e.text == text });
        expect(entry).not.toBeUndefined();
        expect(entry.text).toEqual(text);

        const entry2 = entries.find((e) => { return e.text == text2 });
        expect(entry2).not.toBeUndefined();
        expect(entry2.text).toEqual(text2);

        parsedDocument.delete();
        buffer.delete();
    });
});
