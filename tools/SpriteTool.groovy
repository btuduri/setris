import javax.imageio.*;

public class SpriteTool{

	public static void main(def args){
		// expect a file name as argument
		def argList = args.toList();
		def fileName = argList[0];
		println fileName;
		def file = new File(fileName);
		println file.getName();
		def fileNameBase = file.getName()[0..file.getName().lastIndexOf(".")-1];
		println fileNameBase;
		
		// ok, als Bild laden
		def image = ImageIO.read(file);
		def width = image.getWidth();
		def height = image.getHeight();
		
		def colors = new HashSet();
		
		// Einzelfarben im Hash spechern und ausgeben.
		def x = 0;
		def y = 0;
		while (y < height) {
			while (x < width) {
				def pixel = image.getRGB(x, y);
				colors.add(argb8torgb5(pixel));
				++x;
			}
			x = 0;
			++y;
		}
		
		// Alle Daten eingesammelt, jetzt strukturiert ausgeben.
		def paletteString =
			"const unsigned short ${fileNameBase}_Pal[${colors.size()}] __attribute__ ((aligned (4))) = {\n\t";
		def count = 0;
		def colorList = new ArrayList(colors);
		colorList.each { color ->
			paletteString += color + ", ";
			if (++count == 8){
				paletteString += "\n\t";
				count = 0;
			}
		};
		paletteString = paletteString[0..-3];
		paletteString += " };\n\n"
		print paletteString;
		
		def spriteSb = new StringBuilder(width*height*3);
		spriteSb.append(
				"const unsigned char ${fileNameBase}_Sprites[${width*height}] __attribute__ ((aligned (4))) = {");
		def xtile = 0;
		def ytile = 0;
		def xstride = 32;
		def ystride = 32;
		x = 0;
		y = 0;
		def xtiles = width / xstride;
		def ytiles = height / ystride;
		while (ytile < ytiles) {
			while (xtile < xtiles) {
				while (y < ystride) {
					while (x < xstride) {
						def yy = 0;
						while (yy < 8)
						{
							def xx = 0;
							while (xx < 8)
							{
								def pixel = image.getRGB(xtile*xstride+x+xx, ytile*ystride+y+yy);
								def index = colorList.indexOf(argb8torgb5(pixel));
								spriteSb.append(index + ", ");
								++xx;								
							}
							spriteSb.append("\n\t");
							++yy;
						}
						x+=8;
					}
					x = 0;
					y+= 8;
				}
				y = 0;
				++xtile;
			}
			xtile = 0;
			++ytile;
		}
		spriteSb.delete(spriteSb.length()-4, spriteSb.length()-1);
		spriteSb.append(" };\n\n");
		print spriteSb.toString();
		file = new File("stones_data.cpp");
		file.write(paletteString + spriteSb.toString());
	}
	
	public static int argb8torgb5(int argb)
	{
		def r, g, b;
		// Jeweils 3 mehr shiften als von der mask her nötig, da wir von 8 auf 5 bit reduzieren.
		r = (argb & 0x00ff0000) >>> 19;
		g = (argb & 0x0000ff00) >>> 11;
		b = (argb & 0x000000ff) >>> 3;
		return ((1 << 15) + r + (g << 5) + (b << 10));
	}	
}
