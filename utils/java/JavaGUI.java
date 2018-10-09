import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.awt.image.*;

@SuppressWarnings("serial")
public class JavaGUI extends JPanel {

	int xpos, ypos;
	String LandscapeGeneratorName = "./landscape";
	Image img;
	JTextField mapNameTextField, seedTextField, startHeightTextField, roughnessTextField, 
		islandsTextField, amplitudeTextField, persistenceTextField, frequencyTextField,
		riversNumberTextField, riverLengthTextField, riverWidthTextField;
	JComboBox<String> algoritmsList;
	JPanel diamondSquareElements, perlinNoiseElements;

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
	
	private String algorithmNameByIndex(int n) {
		switch (n) {
			case 1:
				return "hill_algorithm ";
			case 2:
				return "perlin_noise ";
			default:
				return "diamond_square ";
		}	
	}

	private String makeArguments() {
		String result = " --size 512 --noise --output " + mapNameTextField.getText() + " --algorithm ";
		result += algorithmNameByIndex(algoritmsList.getSelectedIndex());
		String temp = seedTextField.getText();
		result += "--seed ";
		if (temp.equals(""))
			result += (int)(Math.random()*2147483647) + " ";
		else
			result += Integer.parseInt(temp) + " ";
		result += "--height " + Integer.parseInt(startHeightTextField.getText()) + " ";
		result += "--roughness " + Float.parseFloat(roughnessTextField.getText()) + " ";
		result += "--islands " + Integer.parseInt(islandsTextField.getText()) + " ";
		result += "--amplitude " + Float.parseFloat(amplitudeTextField.getText()) + " ";
		result += "--persistence " + Float.parseFloat(persistenceTextField.getText()) + " ";
		result += "--frequency " + Float.parseFloat(frequencyTextField.getText()) + " ";
		result += "--rivers_number " + Integer.parseInt(riversNumberTextField.getText()) + " ";
		result += "--river_length " + Integer.parseInt(riverLengthTextField.getText()) + " ";
		result += "--river_width " + Integer.parseInt(riverWidthTextField.getText()) + " ";
		return result;
	}
	
	private void runLandscapeGenerator(String arguments) {
		try {
			//String LandscapeGeneratorName = "./landscape";
			System.out.println("Running " + LandscapeGeneratorName + arguments);
			Runtime r = Runtime.getRuntime();
			Process p = r.exec(LandscapeGeneratorName + arguments, null, new File("."));
			p.waitFor();
			BufferedReader b = new BufferedReader(new InputStreamReader(p.getInputStream()));
			String line = "";
			while ((line = b.readLine()) != null) {
				System.out.println(line);
			}
			b.close();
		} catch (IOException | InterruptedException e) {
			e.printStackTrace(); 
		}
	}
	
	private JTextField makeLabelText(JPanel panel, String labelText, int x, int y, int w, int h, String fieldText, int fieldX, int fieldY, int fieldWidth) {
		JLabel label = new JLabel(labelText);
		label.setBounds(x, y, w, h);
		panel.add(label);
		JTextField textField = new JTextField(20);
		textField.setText(fieldText);
		textField.setBounds(fieldX, fieldY, fieldWidth, h);
		panel.add(textField);
		return textField;
	}
	
	private void loadConfig(String fileName) {
		try {
			File file = new File(fileName);
			FileReader fr = new FileReader(file); 
			BufferedReader br = new BufferedReader(fr);
			String line = br.readLine();
			while ((line = br.readLine()) != null) {
				line = line.replaceAll("\\s","");
				int length = line.length();
				if (length == 0 || line.charAt(0) == '#')
					continue;
				else {
					String parts[] = line.split("=");
					if (parts[0].equals("positionx"))
						xpos = Integer.parseInt(parts[1]);
					else if (parts[0].equals("positiony"))
						ypos = Integer.parseInt(parts[1]);
					else if (parts[0].equals("mapfile"))
						mapNameTextField.setText(parts[1]);
					else if (parts[0].equals("algorithm"))
						algoritmsList.setSelectedIndex(Integer.parseInt(parts[1]));
					else if (parts[0].equals("seed"))
						seedTextField.setText(parts.length == 1 ? "" : parts[1]);
					else if (parts[0].equals("height"))
						startHeightTextField.setText(parts[1]);
					else if (parts[0].equals("roughness"))
						roughnessTextField.setText(parts[1]);
					else if (parts[0].equals("islands"))
						islandsTextField.setText(parts[1]);
					else if (parts[0].equals("riversnumber"))
						riversNumberTextField.setText(parts[1]);
					else if (parts[0].equals("riverswidth"))
						riverWidthTextField.setText(parts[1]);
					else if (parts[0].equals("riverslength"))
						riverLengthTextField.setText(parts[1]);
					else if (parts[0].equals("amplitude"))
						amplitudeTextField.setText(parts[1]);
					else if (parts[0].equals("persistence"))
						persistenceTextField.setText(parts[1]);
					else if (parts[0].equals("frequency"))
						frequencyTextField.setText(parts[1]);
					else if (parts[0].equals("bin"))
						LandscapeGeneratorName = parts[1];
				}
			}
			fr.close();
			br.close();
		} catch (Exception e) {
			System.err.println("Failed to load config " + fileName);
			e.printStackTrace(); 
		}
	}
	
	private void saveConfig(String fileName, int x, int y) {
		try {
			PrintWriter writer = new PrintWriter(fileName, "UTF-8");
			writer.println("#Landscape GUI config file");
			writer.println("positionx=" + x);
			writer.println("positiony=" + y);
			writer.println("");
			writer.println("mapfile=" + mapNameTextField.getText());
			writer.println("bin=" + LandscapeGeneratorName);
			writer.println("");
			writer.println("algorithm=" + algoritmsList.getSelectedIndex());
			writer.println("seed=" + seedTextField.getText());
			writer.println("height=" + startHeightTextField.getText());
			writer.println("roughness=" + roughnessTextField.getText());
			writer.println("islands=" + islandsTextField.getText());
			writer.println("riversnumber=" + riversNumberTextField.getText());
			writer.println("riverslength=" + riverLengthTextField.getText());
			writer.println("riverswidth=" + riverWidthTextField.getText());
			writer.println("amplitude=" + amplitudeTextField.getText());
			writer.println("persistence=" + persistenceTextField.getText());
			writer.println("frequency=" + frequencyTextField.getText());
			writer.close();
		} catch (IOException e) {
			System.err.println("Failed to save config " + fileName);
			e.printStackTrace(); 
		}
	}	
	
	public JavaGUI() {
		this.setLayout(null);
		Action action = new AbstractAction() {
			@Override
			public void actionPerformed(ActionEvent e) {
				loadMap(mapNameTextField.getText());
				repaint();
			}
		};
		
		JButton button = new JButton("Load file");
		button.addActionListener(action);
		button.setBounds(20, 0, 120, 25);
		this.add(button);
		
		mapNameTextField = new JTextField(20);
		mapNameTextField.addActionListener(action);
		mapNameTextField.setText("1.map");
		mapNameTextField.setBounds(372, 0, 120, 25);
		this.add(mapNameTextField);
		
		JLabel algorithmLabel = new JLabel("Algorithm:");
		algorithmLabel.setBounds(520, 0, 80, 25);
		this.add(algorithmLabel);
		String[] algoritmsTexts = {
			"Diamond square",
			"Hill algorithm",
			"Perlin noise",
		};
		algoritmsList = new JComboBox<>(algoritmsTexts);
		algoritmsList.setBounds(600, 0, 150, 25);
		algoritmsList.addActionListener (new ActionListener () { //for changing algorithms
			public void actionPerformed(ActionEvent e) {
				switch(algoritmsList.getSelectedIndex()) {
					case 0: //diamond_square
						diamondSquareElements.setVisible(true);
						perlinNoiseElements.setVisible(false);
						break;
					case 1: //hill_algorithm
						diamondSquareElements.setVisible(true);
						perlinNoiseElements.setVisible(false);
						break;
					case 2: //perlin_noise
						perlinNoiseElements.setVisible(true);
						diamondSquareElements.setVisible(false);
						break;
				}
			}
		});
		this.add(algoritmsList);
		
		diamondSquareElements = new JPanel();
		diamondSquareElements.setLayout(null);
		diamondSquareElements.setBounds(520, 90, 250, 90);
		this.add(diamondSquareElements);
		diamondSquareElements.setVisible(false);
		perlinNoiseElements = new JPanel();
		perlinNoiseElements.setLayout(null);
		perlinNoiseElements.setBounds(520, 90, 250, 90);
		this.add(perlinNoiseElements);
		perlinNoiseElements.setVisible(false);
		
		seedTextField = makeLabelText(this, "Seed (empty for random):", 520, 30, 220, 25, "", 520, 60, 230);
		
		roughnessTextField = makeLabelText(diamondSquareElements, "Roughness:", 0, 0, 100, 25, "0.2", 140, 0, 90);
		startHeightTextField = makeLabelText(diamondSquareElements, "Init height:", 0, 30, 100, 25, "5", 140, 30, 90);
		islandsTextField = makeLabelText(diamondSquareElements, "Islands:", 0, 60, 230, 25, "4", 140, 60, 90);
		
		riversNumberTextField = makeLabelText(this, "Number of rivers:", 520, 180, 230, 25, "10", 660, 180, 90);
		riverLengthTextField = makeLabelText(this, "River's min length:", 520, 210, 230, 25, "20", 660, 210, 90);
		riverWidthTextField = makeLabelText(this, "River's max width:", 520, 240, 230, 25, "3", 660, 240, 90);
		
		amplitudeTextField = makeLabelText(perlinNoiseElements, "Amplitude:", 0, 0, 100, 25, "0.25", 140, 0, 90);
		persistenceTextField = makeLabelText(perlinNoiseElements, "Persistence:", 0, 30, 100, 25, "0.7", 140, 30, 90);
		frequencyTextField = makeLabelText(perlinNoiseElements, "Frequency:", 0, 60, 100, 25, "0.01", 140, 60, 90);
		
		JButton generateLandscapeButton = new JButton("Generate landscape");
		generateLandscapeButton.addActionListener(new AbstractAction() {
			@Override
			public void actionPerformed(ActionEvent e) {
				runLandscapeGenerator(makeArguments());
				loadMap(mapNameTextField.getText());
				repaint();
			}
		});
		generateLandscapeButton.setBounds(520, 270, 230, 25);
		this.add(generateLandscapeButton);
		this.loadConfig("settings.cfg");
	}
	
	public int getXPosition() {
		return xpos;
	}

	public int getYPosition() {
		return ypos;
	}
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(this.img, 0, 30, null);
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame();
		frame.setTitle("Landsacpe GUI");
		frame.setSize(770, 582);
		frame.setMinimumSize(new Dimension(770, 582));
		Container contentPane = frame.getContentPane();
		JavaGUI jgui = new JavaGUI();
		contentPane.add(jgui);
		frame.setVisible(true);
		frame.setLocation(jgui.getXPosition(), jgui.getYPosition());
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.out.println("Saving config");
				jgui.saveConfig("settings.cfg", frame.getX(), frame.getY());
				System.exit(0);
			}
		});
	}
}
