export interface Font {}

export interface Metadata {
  setTitle: (title: string) => void;
  getTitile: () => string | null;
  setSubject: (subject: string) => void;
  getSubject: () => string | null;
  setAuthor: (author: string) => void;
  getAuthor: () => string | null;
  setCreator: (creator: string) => void;
  getCreator: () => string | null;
}

export interface FontManager {
  getDefaultFont: () => Font;
  loadFromBuffer: (buffer: Uint8Array) => Font;
}
    
export interface Canvas {}

export interface Page extends Canvas {
  getRect: () => Array<number>;
  extractText: () => Array<{text: string}>;
}

export interface PageCollection {
  getCount: () => number;
  getPage: () => Page;
  createPage: (size: Array<number>) => Page;
}

export interface Document {
  loadFromBuffer: (buffer: Uint8Array) => void;
  getPages: () => PageCollection;
  getFonts: () => FontManager;
  getMetadata: () => Metadata;
  save: () => Uint8ClampedArray;
}

export interface Image {
  loadFromBuffer: (buffer: Uint8Array) => void;

  height: number;
  width: number;
}

export interface Painter {
  setCanvas: (canvas: Canvas) => void;
  setFont: (font: Font, size: number) => void;
  drawText: (text: string, x: number, y: number) => void;
  drawImage: (image: Image, x: number, y: number, scaleX: number, scaleY: number) => void;
  drawCircle: (x: number, y: number, radius: number, mode: PathDrawMode) => void;
  drawLine: (x1: number, y1: number, x2: number, y2: number) => void;
  drawCubicBezier: (x1: number, y1: number, x2: number, y2: number, x3: number, y3: number, x4: number, y4: number) => void;
  drawArc: (x: number, y: number, radius: number, startAngle: number, endAngle: number, clockwise: boolean) => void;
  drawEllipse: (x: number, y: number, width: number, height: number, mode: PathDrawMode) => void;
  finishDrawing: () => void;
  save: () => void;
  restore: () => void;
}

export interface PageSize {
  A0: 1,
  A1: 2,
  A2: 3,
  A3: 4,
  A4: 5,
  A5: 6,
  A6: 7,
  Letter: 8,
  Legal: 9,
  Tabloid: 10,
}

export interface PathDrawMode {
  Stroke: 1,
  Fill: 2,
  StrokeFill: 3,
  FillEvenOdd: 4,
  StrokeFillEvenOdd: 5,
}

export interface PodofoModule {
  Document: new () => Document;
  Painter: new () => Painter;
  Image: new (document: Document) => Image;
 
  getPageSize: (size: PathDrawMode, landscape: boolean) => Array<number>;

  PathDrawMode: PathDrawMode;
  PageSize: PageSize;
}

export default function PodofoModule(options?: any): Promise<PodofoModule>;
