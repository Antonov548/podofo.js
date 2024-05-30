describe('podofo.js', () => {
    it('simple paint', async () => {
        const Podofo = global.Podofo;

        const document = new Podofo.Document();
        const size = Podofo.getPageSize(Podofo.PageSize.A4, false);
        const page = document.getPages()
                             .createPage(size);
        
        
        const painter = new Podofo.Painter();
        painter.setCanvas(page);
        painter.drawCircle(100, 500, 20, Podofo.PathDrawMode.Fill);
        painter.finishDrawing();
    });
});
