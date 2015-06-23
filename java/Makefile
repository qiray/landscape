
all: JavaGUI.class cppJavaGUI.class libloadMap.so
	
JavaGUI.class: JavaGUI.java
	javac JavaGUI.java
	
cppJavaGUI.class: cppJavaGUI.java loadMap.cpp
	javac cppJavaGUI.java
	javah cppJavaGUI
	
libloadMap.so: loadMap.cpp cppJavaGUI.java
	g++ -o libloadMap.so -shared -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux loadMap.cpp -lc -fPIC
	
clean:
	rm -f *.o *.class *.so *.h
#-Djava.library.path="${workspace_loc:project}\lib;${env_var:PATH}"