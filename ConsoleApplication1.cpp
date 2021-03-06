#include "stdafx.h"
#include "AudioFile.h"
#include <ctime>
#include <limits>
#include "filt.h"
#define _USE_MATH_DEFINES
#include <cmath>
#define M_PI       3.14159265358979323846  //pi


AudioFile<double> audioFile;

using namespace std;

double *sr;
double *taps;

void entertocontinue()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.get();
}

double do_sample(double data_sample, int num_taps)
{
	double result;

	for (int i = num_taps - 1; i >= 1; i--) {
		sr[i] = sr[i - 1];
	}
	sr[0] = data_sample;

	result = 0;
	for (int i = 0; i < num_taps; i++) result += sr[i] * taps[i];

	return result;
}
void prep(int num_taps,double lambda)
{

	double mm;
	for (int n = 0; n < num_taps; n++) {
		sr[n] = 0;
		mm = n - (num_taps - 1.0) / 2.0;
		if (mm == 0.0) taps[n] = lambda / M_PI;
		else taps[n] = sin(mm * lambda) / (mm * M_PI);
	}
}
void lowpass(double fx, int sampling_freq, int numSamples,int num_taps)
{
	double lambda = M_PI * fx / (sampling_freq / 2);
	sr = (double*)malloc(num_taps * sizeof(double));
	taps = (double*)malloc(num_taps * sizeof(double));
	prep(num_taps, lambda);
	//Filter *my_filter;
	//my_filter = new Filter(LPF, num_taps, sampling_freq, fx);
	for (int i = 0; i < audioFile.getNumChannels(); i++)
	{
		for (int j = 0; j < (numSamples-1); j++)
		{
			double next_sample = audioFile.samples[i][j + 1];
			audioFile.samples[i][j] = do_sample(next_sample, num_taps);
		}
	}
}


int main()
{
	string f_in, f_out;
	cout << "source directory?" << endl;
	cin >> f_in;
	cout << "output directory?" << endl;
	cin >> f_out;
	audioFile.load(f_in);
	int numSamples = audioFile.getNumSamplesPerChannel();
	audioFile.printSummary();
	cout << "which fx?" << '\n' << "1 for low pass" << '\n' << "2 for exit" << endl;
	char answer = 'a';
	cin >> answer;
	if (answer == '1') {
		cout << "cutoff frequency?" << endl;
		double cutoff = 0;
		cin >> cutoff;
		lowpass(cutoff, audioFile.getSampleRate(),numSamples,100);
		audioFile.save(f_out);
		cout << "Done. Press ENTER to quit" << endl;
		entertocontinue();
		return 0;
	}
	cout << "press ENTER to quit" << endl;
	entertocontinue();
	return 0;

}




























































/*#include <iostream>
#include <fstream>
#include <string>
#include "AudioFile.h"

using namespace std;

int main()
{
	ifstream f_in;
	ofstream out;

	out.open("L:/programming/try1/ConsoleApplication1/output.txt");
	f_in.open("L:/programming/try1/ConsoleApplication1/s.txt", std::ios::in | std::ios::binary);

	string line;
	while (!f_in.eof())
	{
		getline(f_in, line, '\t');
		float sample = atof(line.c_str());
		out << sample / 2 << '\t';
		getline(f_in, line, '\n');
		sample = atof(line.substr(0, line.length() - 1).c_str());
		out << sample / 2 << '\n';
	}

	out.close();
	f_in.close();
	cout << "done" << endl;
	cin.get();
	return 0;
}*/
