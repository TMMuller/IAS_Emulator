#include "Memory.h"

///////// Notes///////////////
/*


*/


void Memory::Read_file()
{
	string line;
	string file_address;
	int i(0);
	cout << "Input the file you would like to be read: ";
	cin >> file_address;
	ifstream myfile(file_address);
	if (myfile.is_open())
	{
		while (!myfile.eof()) //Reads lines until end of file
		{
			getline(myfile, line); //This line reads each line of code and puts it into line
			//cout << line<< endl;

			match_results<std::string::const_iterator> result;

			
			if (regex_match(line, result, regex("(\\d+) (.*)"))) {
				
				istringstream(result[1]) >> i; //Stores the address of current string line into "i" for memory transfer
				_memory[i] = result[2];
				/*cout << _memory[i]<<endl;
				i++;*/

			}
			//This if statement finds the location of the begin in memory
			if (regex_search(line, dumbresult, regex("begin")))
			{
				membegin = i;
				PC = membegin;
			}
			//This if statement find the location of the halt in memory
			if (regex_search(line, dumbresult, regex("halt")))
			{
				memhalt = i;
			}
		} 
		
		memory_size = i; //Stores the "size" of memory
		
		//end of while loop
	}
	else cout << "Unable to open file"<<endl;

	if (membegin == -1 || memhalt == -1)
	{
		cout << "Error: Missing halt or begin statement..." << endl;
	}

}


//This method will find where a number in the string and then will replace any values (if there are any there) 
//and then will set a value or replace the old one.
void Memory::setmemory(int memaddress, string value) 
{
	regex val("\\d+");
	regex_replace(_memory[memaddress], val, value);
	
}

//This method transfers memory into the MBR
void Memory::getmemory(int memaddress)  
{
	regex val("\\d+");
	regex_match(_memory[memaddress], dumbresult, regex("(\\d+)"));
	istringstream(result[1]) >> MBR; //Stores the value retrieved as a integer into the MBR
}

void Memory::decode(string line)
{
	int command;
	int address;

	regex LoadMQ("load\\s*MQ");  
	regex LoadMQMX("load\\s*\\MQ,\\s*M\\((\\d+)\\)");
	regex StorMX("stor\\s*M\\((\\d+)\\)");
	regex LoadMX("load\\s*M\\((\\d+)\\)");
	regex LoadnegMX("load\\s*-M\\((\\d+)\\)");
	regex LoadabsMX("load\\s*\\|M\\((\\d+)\\)\\|");
	regex LoadabsnegMX("load\\s*-\\|M\\((\\d+)\\)\\|");
	regex Comment("\\.");

	if (regex_match(line, dumbresult, LoadMQ)) { command = 1; }
	else if (regex_match(line, dumbresult, LoadMQMX)) { command = 2; }
	else if (regex_match(line, dumbresult, StorMX)) { command = 3; }
	else if (regex_match(line, dumbresult, LoadMX)) { command = 4; }
	else if (regex_match(line, dumbresult, LoadnegMX)) { command = 5; }
	else if (regex_match(line, dumbresult, LoadabsMX)) { command = 6; }
	else if (regex_match(line, dumbresult, LoadabsnegMX)) { command = 7; }
	else if (regex_match(line, dumbresult, Comment)) { command = 8; }
	else { command = 9; }

	cout << command;
		execute(command, MAR);

}

void Memory::execute(int command, int address)
{
	string ACstring;
	switch (command)
	{
		//Transfer contents of MQ int AC
	case 1: AC = MQ;
		MQ = 0;
		break;
		//Transfer contents of Memory into MQ
	case 2: getmemory(address);
		MQ = MBR;
		MQ = 0;
		break;
		//Store contents of AC into memory location X
	case 3: ACstring = AC;
		setmemory(address, ACstring);
		break;
		//Transfer M(X) to AC
	case 4: getmemory(address);
		AC = MBR;
		MBR = 0;
		//Transfer -M(X) to AC
	case 5: getmemory(address);
		AC = -MBR;
		MBR = 0;
		//Transfer absolute value of M(X) to accumulator
	case 6: getmemory(address);
		AC = abs(MBR);
		MBR = 0;
		//Transfer the negative absolute value of M(X) to accumulator
	case 7: getmemory(address);
		AC = -abs(MBR);
		MBR = 0;
		//No case 8 was added yet
		//Report nop
	case 9: setmemory(address, "nop");
	}
	cout << "PC:   " << PC << endl;
	cout << "AC:   " << AC << endl;
	cout << "MQ:   " << MQ << endl;
}


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



