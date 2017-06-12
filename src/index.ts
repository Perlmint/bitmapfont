const native: {
    BitmapFont: BitmapConstructor;
    Canvas: CanvasConstructor;
} = require("../build/Release/bitmapfont.node");

export interface GlyphInfo {
    x1: number;
    x2: number;
    y1: number;
    y2: number;
    originX: number;
    originY: number;
    width: number;
    height: number;
    ascender: number;
    descender: number;
    advance: number;
}

export type BitmapConstructor = new () => IBitmapFont;
export interface IBitmapFont {
    family: string;
    size: number;
    fill: string;
    weight: number;
    strokeThickness: number;
    strokeColor: string;
    style: Style;

    shadowEnabled: boolean;
    shadowDistance: number;
    shadowAngle: number;
    shadowColor: string;

    draw(canvas: ICanvas, character: string, x: number, y: number): void;
    glyph(character: string): GlyphInfo;
}

export enum ImageFormat {
    PNG = 0,
    JPEG = 1,
    GIF = 2
}

export enum Style {
    unset = 0,
    normal = 1,
    italic = 2,
    oblique = 3
}

export type CanvasConstructor = new(width: number, height: number) => ICanvas;
export interface ICanvas {
    blob(format: ImageFormat): Buffer;
}

export const Canvas = native.Canvas;
export const BitmapFont = native.BitmapFont;
