import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.awt.image.*;

@SuppressWarnings("serial")
public class JavaGUI extends JPanel {

	Image img;
	JTextField textField;

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
	
	private void loadMap(String fileName) {
		System.out.println("Loading map " + fileName);
		try {
			File file = new File(fileName);
			FileReader fr = new FileReader(file); 
			BufferedReader br = new BufferedReader(fr);
			String line = br.readLine();
			int len = Integer.parseInt(line), j = 0;
			int [] mem = new int[len * len];
			while ((line = br.readLine()) != null) {
				int length = line.length();
				for (int i = 0, index = 0; i < length; i++, index++) {
					String s = "";
					char c = line.charAt(i);
					do {
						s += c;
						i++;
						c = line.charAt(i);
					} while (c != ' ');
					mem[j*len + index] = getColorFromInt(Integer.parseInt(s)).getRGB();
				}
				j++;
			}
			fr.close();
			br.close();
			img = createImage(new MemoryImageSource(len, len, mem, 0, len));
			System.out.println("Done");
		} catch (IOException e) {
			e.printStackTrace(); 
		}
	}
	
	public JavaGUI() {
		JButton button = new JButton("Load file");
		button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				loadMap(textField.getText());
				repaint();
			}
		});
		this.add(button);
		textField = new JTextField(20);
		textField.setText("1.txt");
		this.add(textField);
	}	
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(this.img, 0, 30, null);
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame();
		frame.setTitle("Java landsacpe GUI");
		frame.setSize(512, 582);
		frame.setMinimumSize(new Dimension(512, 582));
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		});
		Container contentPane = frame.getContentPane();
		contentPane.add(new JavaGUI());
		frame.setVisible(true);
	}
}
