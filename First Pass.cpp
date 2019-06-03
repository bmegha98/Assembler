/*
OBJECTIVE : TO IMPLEMENT FIRST PASS ASSEMBLER.
INPUT : ASM FILE
ASSUMPTIONS:
1. SPACE SHOULD BE PROVIDED BETWEEN LABEL NAME AND COLON.
2. ARRAY ELEMENTS SHOULD BE SEPARATED BY COMMAS ,NOT WHITESPACES.
3. EVERY NUMERIC VALUE SHOULD BE SUCCEEDED BY 'H' OR 'h'.
4. NO WHITESPACES BETWEEN COMMA AND NEXT WORD.
GIVEN:
1. DIRECTIVE TABLE 
2. MACHINE INSTRUCTION TABLE
OUPUT: SYMBOL TABLE THAT CONSISTS OF ALL VARIABLES AND LABELS DEFINED IN THE GIVEN FILE

FUNCTIONS USED:
push_back() : It adds new element to the vector from the end.
open() : Object of fstream to open a file.
close() : To close a file.

*/

#include<algorithm> 								//for transform function
#include<iostream>									//for input and output to console
#include <fstream>									//to handle files such as open,close etc.
#include <string>									//to perform string operations
#include<vector>									//to use vectors and related functions
#include <sstream>     								//for istringstream ,stringstream 
using namespace std;

string Machine[12]={"LEA","LDS","MOV","XCHG","ADD","LES","SUB","ADC","SBB","CMP","INC","DEC"};		//machine instruction table --> global

string Directive[9]={"DB","DW","DD","SEGMENT","ENDS","ASSUME","END","HLT","EQU"};		// Directive table--> global

int LocationCounter=0;																	//to determine offset of variables and labels-->global variable

struct Symbol																			//structure is used for each entry of symbol table
{																						// so that each entry has
	string symname;																		// symbol name ,
	string type;																		// type,
	string segment;																		// corresponding segment,
	int size;																			// own size and
	int offset;																			// offset
};

vector<Symbol> SymbolTable;																// vector of Structure in which each element has
																						// 5 attributes defined in Symbol structure.
bool Search_In_Machine(string word)
{
	for(int i=0;i<12;i++)
	{
		if(Machine[i]==word)														// function to check if current word is an instruction
		  return true;																// if it is,it should be in machine instruction table
		  																			// and this function returns true and false otherwise.
	}
	return false;
}


bool Search_In_Directive(string word)												//function to check if current word is a directive
{																					//if it is,it should be in directive table and this
	for(int i=0;i<9;i++)															//function returns true and false otherwise.
	{
		if(Directive[i]==word)
		  return true;
		  
	}
	return false;
}


bool Already_In_SymbolTable(string word)										//This function checks if a variable or label is already defined
{																				//in the symbol table and will be used to raise an error
	for(int i=0;i<SymbolTable.size();i++)										//in that case.
	{
		if(word==SymbolTable[i].symname)
		  return true;
	}
	return false;
}


bool Contains(string word,char search)										//This function determines if a given word contains a particular character
{																			//and returns true if character is present and returns false otherwise.
	for(int i=0;i<word.length();i++)
	{
		if(search==word[i])
		   return true;
	}
	return false;
}


int Occurrences(string word,char search)									//This function returns number of times a particular character is occurred
{																			//in that word.
	int count=0;
	for(int i=0;i<word.length();i++)
	{
		if(search==word[i])
		   count++;
	}
	return count;	
}


int Position(string word,char search)										//This function returns the position of a particular character in
{																			//a given word.
	for(int i=0;i<word.length();i++)
	{
		if(search==word[i])
		   return i;
	}
}


int Size_Of_MemoryWord(string word)											//This function returns the size of a particular member/element
{																			//of symbol table.
	for(int i=0;i<SymbolTable.size();i++)
	{
		if(word==SymbolTable[i].symname)
		  return SymbolTable[i].size;
	}
}

int Output()																//Function to display Symbol Table
{
	cout<<endl;
    cout<<"\t\t\t\tSYMBOL   TABLE"<<endl;
    cout<<endl;
	cout<<"Symbol"<<"\t\t"<<"Type"<<"\t\t"<<"Segment"<<"\t\t"<<"Size"<<"\t\t"<<"Offset"<<endl;
	cout<<endl;
	for(int i=0;i<SymbolTable.size();i++)
    cout<<SymbolTable[i].symname<<"\t\t"<<SymbolTable[i].type<<"\t\t"<<SymbolTable[i].segment<<"\t\t"<<SymbolTable[i].size<<"\t\t"<<SymbolTable[i].offset<<endl;
}


int main()
{
	cout<<"******************************************************FIRST PASS ASSEMBLER******************************************"<<endl;
	//tokenize assembly file	
	ifstream fin;																//object of ifstream to open or close file
	vector<string> tokens;														// vector to store individual words of file
	int k=0;

	string currword,filename,prevword="",segName="";
	cout<<"Enter file name : ";
	getline(cin,filename);
	 string line;
	 int segnum=0,flag1=0,flag2=0;												//segnum determines segment number and flags are used to detect
	fin.open(filename.c_str());																//errors.
	if(!fin.is_open())
	 {
	 	cout<<"File not found!!"<<endl;
	 	exit(0);
	 }
	 	 while(getline(fin,line))
	 	 {
	 	 	istringstream iss(line);						            						//To read file line by line
	 																							// and then tokenize each line separately.
		    k++; 																				//store tokens/words in tokens vector.
		    int count=0;
	 	 	Symbol s;																			//Object of symbol structure
	 	 
			while(iss>>currword)																//reading words of a line
	     	{	
			transform(currword.begin(), currword.end(), currword.begin(), ::toupper);    		//converting words to uppercase words
			
			
			 if(currword==";" || currword.at(0)==';')											//Comment starts and assembler ignores all
		        break;																			//comments and skip to next line
		        
		    if(prevword=="INC"||prevword=="DEC")
		    {
		    	if(Already_In_SymbolTable(currword))
		    	  LocationCounter+=4;
		    	else if(currword[currword.length()-1]=='X')
		    	 LocationCounter+=1;
		    	 
			}
		    
		    if(Contains(currword,',')==true)													// Could be array/machine instructions
		      {
		      	int pos=Position(currword,',');
		      	if(currword[currword.length()-1]=='H')											//array of elements or immediate instruction
		      	{ 
		      	//IMMEDIATE DATA
		      	 	if(Already_In_SymbolTable(currword.substr(0,pos)))
		      	 	{
		      	 	 	int size=Size_Of_MemoryWord(currword.substr(0,pos));
		      	 	 	if(prevword=="ADD"||prevword=="SUB"||prevword=="SBB"||prevword=="ADC"||prevword=="CMP")	// immediate data (MOV,ADD,ADC,SUB,SBB,CMP)
		      	 	 	 LocationCounter+=5;
		      	 	 	 else
		      	 	 	 {
		      	 	 	if(size==1)
		      	 	 	LocationCounter+=5;													//Immediate to memory 
		      	 	 	else if(size==2)
		      	 	 	LocationCounter+=6;
		      	 	     }
					}
					else if(currword[1]=='X' || currword[1]=='I') 				
					{
					  LocationCounter+=3;
					}
					else if(currword[1]=='L')										//Immediate to register 
					  LocationCounter+=2;
					   else
					  {
		      			LocationCounter=Occurrences(currword,',')+1;				// Else determine offset of array variable
		      			LocationCounter=LocationCounter*s.size;
		              }
				}
				else if(Already_In_SymbolTable(currword.substr(0,pos)) && (currword[currword.length()-1]=='X' || currword[currword.length()-1]=='L' ))
				{
					if(prevword=="XCHG")									 //XCHG instruction i.e. XHCG AX,BX
					LocationCounter+=4;													
					else
					  LocationCounter+=4;										//register to memory 			
				}
			
			//REGISTER TO REGISTER																
				else if(currword[1]==currword[4])																
				{
					if(prevword=="LEA"||prevword=="LDS"||prevword=="LES")		//FOR LEA/LDS/LES instruction
					 LocationCounter+=2;
					 else
					  LocationCounter+=2;										//FOR ADD,SUB,SBB,ADC,CMP,MOV																
				}
			
			// MEMORY TO REGISTER	     												
				else if(Already_In_SymbolTable(currword.substr(pos+1,currword.length()-pos)))		//(ADD,SUB,SBB,ADC,CMP)
				{
					if((prevword=="ADD"||prevword=="SUB"||prevword=="SBB"||prevword=="ADC"||prevword=="CMP" )&& currword[pos-1]=='X')
					  LocationCounter+=4;															
					else if((prevword=="ADD"||prevword=="SUB"||prevword=="SBB"||prevword=="ADC"||prevword=="CMP" ) && currword[pos-1]=='L')
					  LocationCounter+=3;
					else if(prevword=="XCHG")														//XCHG instruction
					  LocationCounter+=4;
					else if(prevword=="LDS" || prevword=="LES")		 //LDS ,LEA and LES instruction
					   LocationCounter+=4;
					else if(prevword=="LEA")
					  LocationCounter+=3;
					else
					{
					if(Size_Of_MemoryWord(currword.substr(pos+1,currword.length()-pos))==-5)       // memory to register(label)
					 LocationCounter+=3;
					else
					 LocationCounter+=4;															//other memory elements
			     	}
				}
			
			// REGISTER/MEMORY TO SEGMENT REGISTER	
				else if(currword[1]=='S')									                    	
				   LocationCounter+=2;
				else if(Contains(currword,'[')==true || prevword=="LDS" || prevword=="LEA"||prevword=="LES" )		//register indirect 
				{
					if(Contains(currword,'+'))
					{
						if(Occurrences(currword,'+')==2)							//relative base indexed 
						LocationCounter+=3;
						else if(Occurrences(currword,'+')==1)
						{
							if(prevword=="LDS" || prevword=="LEA"||prevword=="LES")					//LDS/LEA/LES instructions
							{
								int temp=Position(currword,'+');
								stringstream str(currword.substr(temp+1,currword.length()-temp-2));				
								int num=0;
								str>>num;
								if(num<=255)
								LocationCounter+=3;
								else
								LocationCounter+=4;
								
							}
							else
							{	
							if(currword[Position(currword,'+')+1]=='S' || currword[Position(currword,'+')+1]=='D')		// base indexed  instruction
							 LocationCounter+=2;
							 else 
							 LocationCounter+=3;
							 }																		//register relative  instruction
						}
						  
					}
				    else
				  LocationCounter+=2;
		     	}
		     		else
		     	{
		     	  flag1=1;
		          cout<<"ERROR !!!!  Not 8086 Instruction - Not supported yet . "<<endl;
		          break;
		        }
		 }
			  
			else if(currword[currword.length()-1]=='H')											// Single valued Variable
			{
					LocationCounter+=s.size;
			}
		    else if(currword==" ")																//Skip white spaces
		        continue;
		      
		        
			else if(Search_In_Machine(currword))
				{
			//If current word is a machine instruction		 
					 prevword=currword;
					 
				}
				else if(Search_In_Directive(currword) || currword==":")					 // Search if current word is an variable or a label
				{
				if(currword=="SEGMENT")
					{
						s.symname=prevword;
						s.type="SEGMENT"; 
						s.segment="ITSELF";
						segName=prevword;
						s.size=-5;
						LocationCounter=0;
								
					}
				else if(currword=="DB")
				{
					s.symname=prevword;
					s.type="VAR";
					s.size=1;
					s.segment=segName;
			
				
					
				}
				else if(currword=="DW")
				{
					s.symname=prevword;
					s.type="VAR";
					s.size=2;
					s.segment=segName;
				  
					
				}
				else if( currword=="DD")
				{
					s.symname=prevword;
					s.type="VAR";
					s.size=4;
					s.segment=segName;
				
					
				}
			    else if(currword=="ENDS")
			    {
			    	segName="";
			    	continue;
				}
				else if(currword=="END")
				   continue;
				   
				else if(currword==":")
				{
					s.symname=prevword;
					s.type="LABEL";
					s.segment=segName;
						s.size=-1;
					
					
				}
				
				else if(currword=="ASSUME")
				   break;
				else if(currword=="HLT")
				{
				  LocationCounter+=1;
				  break;
		     	}
				   
			if(currword=="SEGMENT")
			{
				s.offset=segnum;
				segnum++;
			}
			 else
			s.offset=LocationCounter;
		     	
			if(!Already_In_SymbolTable(prevword))												//Before adding any word to symbol table check if it 
			   SymbolTable.push_back(s);														//is already present in the symbol table
			 else
			 {
			 	flag2=1;
			 cout<<"Duplicate declaration of "<<prevword<<" at line "<<k<<"."<<endl;
			   break;
	       	}
	       	
	   }
	   		else
				{
				  prevword=currword;													//If a word is neither directive nor machine instruction and is 
				}																		//succeeded by either directive or colon(:) then add it to
	   																					//the symbol table
		}
		
}

// Display of Symbol Table
if(flag1==0 && flag2==0)
   {
   	Output();
   
  }
  fin.close();																			// To close the file.
	system("pause");
	return 0;
}
