describe('podofo.js', () => {
    it('pdf meta data', async () => {
        const Podofo = global.Podofo;
        
        const document = new Podofo.Document();
        const metadata = document.getMetadata();

        const title = "TestTitle";
        metadata.setTitle(title);
        expect(metadata.getTitle()).toEqual(title);

        const title2 = "TestTitle2";
        metadata.setTitle(title2);
        expect(metadata.getTitle()).toEqual(title2);

        metadata.resetTitle();

        expect(metadata.getTitle()).toBeNull();
    });
});
