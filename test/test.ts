import { BitmapFont, Canvas } from "../lib";
import { createWriteStream } from "fs";

const font = new BitmapFont();
const canvas = new Canvas(512, 512);
font.family = "Nunito";
font.fill = "white";
font.size = 40;
const glyphInfo = font.glyph("a");
font.draw(canvas, "a", 0, glyphInfo.height);
createWriteStream("a.png").end(canvas.blob(0));
