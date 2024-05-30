describe('podofo.js', () => {
    it('create single page', async () => {
        const Podofo = global.Podofo;

        const document = new Podofo.Document();
        const pages = document.getPages();

        const size = Podofo.getPageSize(Podofo.PageSize.A4, false);
        
        const page = pages.createPage(size);
        expect(page.getRect()).toEqual(size);

        expect(pages.getCount()).toEqual(1);

        const pdf = document.save();
        document.delete();

        const buffer = Podofo.makeBuffer(pdf);
        
        const parsedDocument = new Podofo.Document();
        parsedDocument.loadFromBuffer(buffer);

        const parsedPages = parsedDocument.getPages();
        expect(parsedPages.getCount()).toEqual(1);

        const parsedPage = parsedPages.getPage(0);
        expect(parsedPage.getRect()).toEqual(size);

        parsedDocument.delete();
        buffer.delete();
    });
});
