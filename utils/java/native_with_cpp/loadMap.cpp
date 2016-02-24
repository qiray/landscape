#include <iostream>
#include <fstream>
#include <string>

#include <jni.h>
#include "cppJavaGUI.h"

JNIEXPORT jintArray JNICALL
Java_cppJavaGUI_loadMap(JNIEnv *env, jobject obj, jstring fileName) {
	std::string str = env->GetStringUTFChars(fileName, 0);
	std::ifstream is (str.c_str(), std::ifstream::in);
	int len = 0;
	if (is) {
		is >> len;
		int bigLength = len*len;
		jintArray result = env->NewIntArray(bigLength);
		int *data = new int[bigLength];
		std::string line;
		for (int i = 0; i < bigLength; i++)
			is >> data[i];
		env->SetIntArrayRegion(result, 0, bigLength, data);
		is.close();
		delete [] data;
		return result;
	}
	return env->NewIntArray(1);
}
