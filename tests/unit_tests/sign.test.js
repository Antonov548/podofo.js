const { readFile, writeFile } = require("./utils");

describe('podofo.js', () => {
  it('sign pdf document', async () => {
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

    const text = 'Signed document';
    painter.drawText(text, 56.69, page.getRect()[3] - 56.69);
    painter.finishDrawing();

    const buffer = readFile('image.png');

    const image = new Podofo.Image(document);
    image.loadFromBuffer(buffer);
    
    const certificate = readFile('certificate.der');
    const key = readFile('private_key.der');

    page.sign(image, [20, 20, 100, 100], certificate, key);
  });
});
