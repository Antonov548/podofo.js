declare module 'podofo.js' {
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
    sign: (image: Image, rect: Array<number>, certificate: Uint8Array, privateKey: Uint8Array) => Uint8ClampedArray;
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
    delete: () => void;
  }
  
  export interface Image {
    loadFromBuffer: (buffer: Uint8Array) => void;
    delete: () => void;
  
    height: number;
    width: number;
  }
  
  export interface Painter {
    setCanvas: (canvas: Canvas) => void;
    setFont: (font: Font, size: number) => void;
    drawText: (text: string, x: number, y: number) => void;
    drawImage: (image: Image, x: number, y: number, scaleX: number, scaleY: number) => void;
    drawCircle: (x: number, y: number, radius: number, mode: PathDrawMode) => void;
    drawRectangle: (x: number, y: number, width: number, height: number, mode: PathDrawMode, roundX: number, roundY: number) => void;
    drawLine: (x1: number, y1: number, x2: number, y2: number) => void;
    drawCubicBezier: (x1: number, y1: number, x2: number, y2: number, x3: number, y3: number, x4: number, y4: number) => void;
    drawArc: (x: number, y: number, radius: number, startAngle: number, endAngle: number, clockwise: boolean) => void;
    drawEllipse: (x: number, y: number, width: number, height: number, mode: PathDrawMode) => void;
    finishDrawing: () => void;
    save: () => void;
    restore: () => void;
    setFillColor: (r: number, g: number, b: number) => void;
    setStrokeColor: (r: number, g: number, b: number) => void;
    setLineWidth: (width: number) => void;
    delete: () => void;
  }
  
  export interface PageSizeEnum {
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
  
  export interface PathDrawModeEnum {
    Stroke: 1,
    Fill: 2,
    StrokeFill: 3,
    FillEvenOdd: 4,
    StrokeFillEvenOdd: 5,
  }

  type PathDrawMode = number;

  type PageSize = number;
  
  export interface Podofo {
    Document: new () => Document;
    Painter: new () => Painter;
    Image: new (document: Document) => Image;
   
    getPageSize: (size: PageSize, landscape: boolean) => Array<number>;

    PathDrawMode: PathDrawModeEnum;
    PageSize: PageSizeEnum;
  }
  
  export default function PodofoModule(options?: any): Promise<Podofo>;
}
