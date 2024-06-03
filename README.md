# podofo.js

podofo.js is a JavaScript interface for the [PoDoFo](https://github.com/podofo/podofo) library.

Library provides functions to parse, create and modify PDF.

## NPM package
podofo.js npm pacakge is available - [npm/podofo.js](https://www.npmjs.com/package/podofo.js)

```bash
npm i podofo.js
```

## Build with docker

podofo.js is a compiled PoDoFo library to WebAssembly using Emscripten. 
Docker [image](https://github.com/Antonov548/podofo.js-docker) can be used to build library from scratch.
This docker image contains minimal required tools and dependencies to build a JavaScript module.

Check [CI pipeline](https://github.com/Antonov548/podofo.js/blob/main/.github/workflows/ci.yaml) for more details how to build library.

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
