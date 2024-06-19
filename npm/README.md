# podofo.js

podofo.js is a JavaScript interface for the [PoDoFo](https://github.com/podofo/podofo) library.

Library provides functions to parse, create and modify PDF.

## Why podofo.js

## Example
```js
import PodofoModule from 'podofo.js'

const Podofo = await PodofoModule();

const document = new Podofo.Document();
const pages = document.getPages();

const page = pages.createPage(
                   Podofo.getPageSize(Podofo.PageSize.A4, false));

const fonts = document.getFonts();
const font = fonts.getDefaultFont();

const painter = new Podofo.Painter();
painter.setCanvas(page);
painter.setFont(font, 10);
painter.drawText("Hello world!", 0, 0);
painter.finishDrawing();

const pdf = document.save();

painter.delete();
document.delete();

```


## API

podofo.js use WebAssembly and require loading of WebAssembly module.
```js
import PodofoModule from 'podofo.js'

const Podofo = await PodofoModule();
```
All `Podofo` objects created with `new` must be deleted for the wasm memory to be released. All available functions can be found in TypeScript module definition [file](https://github.com/Antonov548/podofo.js/blob/main/npm/podofo.d.ts).
