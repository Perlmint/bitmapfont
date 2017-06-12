import { BitmapFont, Canvas } from "../lib";
import { createWriteStream } from "fs";
import { join } from "path";
import * as ShelfPack from "@mapbox/shelf-pack";

const font = new BitmapFont();
font.family = "Arial";
font.fill = "#FFF04D";
font.size = 40;
font.strokeThickness = 4;
font.strokeColor = "#E85600";
font.shadowEnabled = true;
font.shadowColor = "#E85600";
font.shadowAngle = 90;
font.shadowDistance = 4;
let x = 0;
const chars = "0123456789,".split("").map(ch => ({
    ch, ...font.glyph(ch)
}));
const pack = new ShelfPack(0, 0, { autoResize: true });
const requests = chars.map((info, i) => {
    return {
        id: i,
        w: Math.ceil(info.x2 - info.x1) + 2,
        h: Math.ceil(info.y2 - info.y1) + 2
    } as ShelfPack.RequestShort;
});
const bins = pack.pack(requests);
const canvas = new Canvas(pack.w + 2, pack.h + 4);
for (const bin of bins) {
    const ch = chars[bin.id as number];
    font.draw(canvas, ch.ch, Math.ceil(bin.x - ch.x1), Math.ceil(bin.y + ch.y2));
}
createWriteStream(join(__dirname, "a.png")).end(canvas.blob(0));
