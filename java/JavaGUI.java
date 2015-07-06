import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.awt.image.*;

@SuppressWarnings("serial")
public class JavaGUI extends JPanel {

	Image img;
	JTextField mapNameTextField, seedTextField, startHeightTextField, roughnessTextField, 
		islandsTextField, amplitudeTextField, persistenceTextField, frequencyTextField,
		riversNumberTextField, riverLengthTextField;
	JComboBox<String> algoritmsList;

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

	private String makeArguments() {
		String result = " --size 512 --noise --output " + mapNameTextField.getText() + " --algorithm ";
		switch (algoritmsList.getSelectedIndex()) {
			case 1:
				result += "hill_algorithm ";
				break;
			case 2:
				result += "perlin_noise ";
				break;
			case 0:
			default:
				result += "diamond_square ";
		}
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
		result += "--rivers_number " + Float.parseFloat(riversNumberTextField.getText()) + " ";
		result += "--river_length " + Float.parseFloat(riverLengthTextField.getText()) + " ";
		return result;
	}
	
	private void runLandscapeGenerator(String arguments) {
		try {
			String LandscapeGeneratorName = "./landscape";
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
	
	private JTextField makeLabelText(String labelText, int x, int y, int w, int h, String fieldText, int fieldX, int fieldY, int fieldWidth) {
		JLabel label = new JLabel(labelText);
		label.setBounds(x, y, w, h);
		this.add(label);
		JTextField textField = new JTextField(20);
		textField.setText(fieldText);
		textField.setBounds(fieldX, fieldY, fieldWidth, h);
		this.add(textField);
		return textField;
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
		this.add(algoritmsList);
		
		seedTextField = makeLabelText("Seed (empty for random):", 520, 30, 220, 25, "", 520, 60, 230);
		roughnessTextField = makeLabelText("Roughness:", 520, 90, 100, 25, "0.2", 650, 90, 100);
		islandsTextField = makeLabelText("Islands (hill algorithm):", 520, 120, 230, 25, "4", 700, 120, 50);
		startHeightTextField = makeLabelText("Init height (diamond square):", 520, 150, 230, 25, "5", 520, 180, 230);
		riversNumberTextField = makeLabelText("Number of rivers:", 520, 210, 230, 25, "10", 660, 210, 90);
		riverLengthTextField = makeLabelText("River's min length:", 520, 240, 230, 25, "20", 660, 240, 90);
		
		JLabel perlinHillInfoLabel = new JLabel("Perlin noise algorithm only:");
		perlinHillInfoLabel.setBounds(520, 270, 230, 25);
		this.add(perlinHillInfoLabel);
		
		amplitudeTextField = makeLabelText("Amplitude:", 520, 300, 100, 25, "0.25", 630, 300, 120);
		persistenceTextField = makeLabelText("Persistence:", 520, 330, 100, 25, "0.7", 630, 330, 120);
		frequencyTextField = makeLabelText("Frequency:", 520, 360, 100, 25, "0.01", 630, 360, 120);
		
		JButton generateLandscapeButton = new JButton("Generate landscape");
		generateLandscapeButton.addActionListener(new AbstractAction() {
			@Override
			public void actionPerformed(ActionEvent e) {
				runLandscapeGenerator(makeArguments());
				loadMap(mapNameTextField.getText());
				repaint();
			}
		});
		generateLandscapeButton.setBounds(520, 390, 230, 25);
		this.add(generateLandscapeButton);
	}	
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(this.img, 0, 30, null);
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame();
		frame.setTitle("Landsacpe GUI");
		frame.setSize(770, 582);
		//frame.setLocation(200, 200);
		frame.setMinimumSize(new Dimension(770, 582));
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
