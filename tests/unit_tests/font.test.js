const { readFile } = require('./utils')

describe('podofo.js', () => {
    it('draw text with external font', async () => {
      const Podofo = global.Podofo;
  
      const document = new Podofo.Document();
      const pages = document.getPages();
  
      const size = Podofo.getPageSize(Podofo.PageSize.A4, false);
      const page = pages.createPage(size);
      
      const fonts = document.getFonts();

      const buffer = readFile('font.ttf'); 
      const font = fonts.loadFromBuffer(buffer);
  
      const painter = new Podofo.Painter();
      painter.setCanvas(page);
      painter.setFont(font, 10);
  
      const text = 'ABCDEFGHIKLMNOPQRSTVXYZ';
      painter.drawText(text, 56.69, page.getRect()[3] - 56.69);
  
      painter.finishDrawing();
  
      const pdf = document.save();
      
      painter.delete();
      document.delete();
      
      const parsedDocument = new Podofo.Document();
      parsedDocument.loadFromBuffer(pdf);
  
      const parsedPages = parsedDocument.getPages();
      expect(parsedPages.getCount()).toEqual(1);
  
      const parsedPage = parsedPages.getPage(0);
      expect(parsedPage.getRect()).toEqual(size);
  
      const entries = parsedPage.extractText();
  
      const entry = entries.find((e) => { return e.text == text });
      expect(entry).not.toBeUndefined();
      expect(entry.text).toEqual(text);
  
      parsedDocument.delete();
    });
  });
  