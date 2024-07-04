# podofo.js

podofo.js is a JavaScript interface for the [PoDoFo](https://github.com/podofo/podofo) library.

Library provides functions to parse, create and modify PDF.

## Why podofo.js

Podofo.js is a JavaScript library that leverages the speed and efficiency of the PoDoFo C++ library for advanced PDF manipulation.

 - **High Performance**: Utilizes C++ under the hood for fast and efficient PDF operations.
 - **Comprehensive Functionality**: Offers extensive features for  creating, modifying, and extracting information from PDFs, including advanced operations like signing and font embedding.
 - **Ease of Use**: Provides a developer-friendly JavaScript API, making it easy to integrate powerful PDF capabilities without needing deep C++ knowledge. 
 - **Built-in Signing methods**: Supports digital signatures, encryption.

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
