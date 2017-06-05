const native: {
    BitmapFont: BitmapConstructor;
    Canvas: CanvasConstructor;
} = require("../build/Release/bitmapfont.node");

export interface GlyphInfo {
    width: number;
    height: number;
    ascender: number;
    descender: number;
}

export type BitmapConstructor = new () => IBitmapFont;
export interface IBitmapFont {
    family: string;
    size: number;
    fill: string;

    draw(canvas: ICanvas, character: string, x: number, y: number): void;
    glyph(character: string): GlyphInfo;
}

export enum ImageFormat {
    PNG = 0,
    JPEG = 1,
    GIF = 2
}

export type CanvasConstructor = new(width: number, height: number) => ICanvas;
export interface ICanvas {
    blob(format: ImageFormat): Buffer;
}

export const Canvas = native.Canvas;
export const BitmapFont = native.BitmapFont;
