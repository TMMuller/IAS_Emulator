

#include "Memory.h"


int main()
{

	Memory File;
	File.Read_file();


	for (File.PC = File.membegin; File.PC <= File.memhalt; File.PC++)
	{
		File.MAR = File.PC;

		regex val("\\d+");
		regex_match(File._memory[File.MAR], File.dumbresult, regex("(\\d+)"));
		istringstream(File.dumbresult[1]) >> File.MBR; //Stores the value retrieved as a integer into the MBR
		File.IR = File._memory[File.MAR];
													   
		//File.MBR = File._memory[File.MAR];

		File.decode(File._memory[File.MAR]);
		//cout << File.PC<<endl;

		cout << "PC:   " << File.PC << endl;
		cout << "IR:   " << File.IR << endl;

	}

	for (File.PC = 0; File.PC <= File.memhalt; File.PC++)
	{
		cout << File.PC << "     "<< File._memory[File.PC]<<endl;
	}
	

	return 0;
}



