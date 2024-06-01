const { readFile } = require('./utils')

describe('podofo.js', () => {
  it('jpeg image', async () => {
    const Podofo = global.Podofo;

    const document = new Podofo.Document();
    const size = Podofo.getPageSize(Podofo.PageSize.A4, false);
    const page = document.getPages()
                            .createPage(size);

    const buffer = readFile('image.jpg'); 

    const image = new Podofo.Image(document);
    image.loadFromBuffer(buffer);
    
    const painter = new Podofo.Painter();
    painter.setCanvas(page);
    painter.drawImage(image, 0, 0, 1, 1);
    painter.finishDrawing();

    const pdf = document.save();

    const parsedDocument = new Podofo.Document();
    parsedDocument.loadFromBuffer(pdf);

    const parsedPage = parsedDocument.getPages().getPage(0);
    const resources = parsedPage.getResources();

    const objects = resources.getArray(Podofo.ResourceType.XObject);
    expect(Object.keys(objects).length).toEqual(1);

    for (const key in objects) {
      const parsedImage = new Podofo.Image(key, objects[key]);
      expect(parsedImage.width).toEqual(image.width);
      expect(parsedImage.height).toEqual(image.height);

      parsedImage.delete();
    }

    painter.delete();
    image.delete();
    document.delete();
  });
});
