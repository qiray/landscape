import java.awt.*;//gcj --main=JavaGUI JavaGUI.java -o JavaGUI
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.awt.image.*;

@SuppressWarnings("serial")
public class cppJavaGUI extends JPanel {

	Image img;
	
	static {
		try {
			System.loadLibrary("loadMap");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("Native code library failed to load.\n" + e);
		}
	}

	private native int[] loadMap(String fileName);

	public Color getColorFromInt(int c) {
		if (c < -40)
			return new Color(0, 0, 32);
		else if (c < -10)
			return new Color(0, 0, 64 + c/2);
		else if (c < 1)
			return new Color(0, 0, 128);
		else if (c < 10)
			return new Color(255, 255, 180 - c*5); 
		else if (c < 25)
			return new Color(0, 128 - c*4, 0);
		else if (c < 60)
			return new Color(139 - c*2, 69 - c, 20 - c/3);
		else if (c < 100)
			return new Color(155 + c, 155 + c, 155 + c);
		return new Color(255, 255, 255);
	}
	
	public cppJavaGUI() {
		String fileName = "1.txt";
		System.out.println("init");
		int [] mem = loadMap(fileName);
		System.out.println(mem.length);
		int len = (int)Math.sqrt(mem.length);
		for (int i = 0; i < mem.length; i++)
			mem[i] = getColorFromInt(mem[i]).getRGB();
		img = createImage(new MemoryImageSource(len, len, mem, 0, len));
	}	
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(this.img, 0, 0, null);
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame();
		frame.setTitle("Java landsacpe GUI");
		frame.setSize(512, 512);
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		});
		Container contentPane = frame.getContentPane();
		contentPane.add(new cppJavaGUI());
		frame.setVisible(true);
	}
}
