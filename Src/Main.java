//MAIN CLASS
package main;

import java.util.Scanner;
import java.io.*;
import com.opencsv.CSVReader;

class Main{
    
    private static String[] codeTableInput;
    private static String[] codeTableOutput;

    //private String splitBy=",";
    //Function <String>.split(splitBy);

    public static void main(String args[]) throws IOException{

	Scanner scan=new Scanner(System.in);

	getCodeTable rcd=new getCodeTable(args[0],args[2]);
	codeTableInput=rcd.getCodeTableInput();
	codeTableOutput=rcd.getCodeTableOutput();

	File inputFile=new File("InputFile/"+args[1]);
	FileReader inputReader=new FileReader(inputFile);
	
	File outputFile=new File("OutputFile/"+args[3]);
	FileWriter outputWriter=new FileWriter(outputFile);

	String character=new String("");
	String tmp=new String();
	
        TransferCode tsfc=new TransferCode(codeTableInput, codeTableOutput);

	int tmpInt;
	while((tmpInt=inputReader.read())!=-1){//read each character (interger)
	    tmp=String.valueOf((char)tmpInt);//transfer interger to character using ascii code
	    
	    if(codeTableInput[2].equals("")){//if in code table input there are nothing to split character...
		character=tmp;
                tsfc.putChar(character);//transfercode
		outputWriter.write(tsfc.getChar()+codeTableOutput[2]);
	    }else{//if in code table input there is something split character...
		//if the character tmp is a special character: character_split/word_split/new_line=> that's the end of the character of character variable
		if(tmp.equals(codeTableInput[2])||tmp.equals(codeTableInput[3])||tmp.equals(codeTableInput[4])){
		    if(!character.equals("")){//if character variable is holding data=> we have a character to transfer
			tsfc.putChar(character);
		        outputWriter.write(tsfc.getChar()+codeTableOutput[2]);
		    }
		    character=tmp;//we transfer tmp
		    tsfc.putChar(character);
		    outputWriter.write(tsfc.getChar()+codeTableOutput[2]);
		    
		    character=new String("");//start to storing new character
		}else character=character+tmp;//if not, it's still one part of the character we are taking
	    }
	}

	outputWriter.close();
	inputReader.close();
    }
}

class TransferCode{

    private String[] codeTableInput;
    private String[] codeTableOutput;
    private int numOfCodeChar;

    private String inputChar;
    private String outputChar;
    
    public TransferCode(String[] codeTableInput, String[] codeTableOutput){
	this.codeTableInput=codeTableInput;
	this.codeTableOutput=codeTableOutput;
        numOfCodeChar=codeTableInput.length;
    }

    public void putChar(String inputChar){
	this.inputChar=inputChar;
	
	int i=1;
	while(i<numOfCodeChar&&!inputChar.equals(codeTableInput[i]))
	    i=i+1;

	if(i==numOfCodeChar)
	    outputChar=codeTableOutput[1];//ký tự lỗi trong dãy outputCode
	else outputChar=codeTableOutput[i];
    }

    public String getChar(){
	return outputChar;
    }
}

class getCodeTable{
    
    private String[] codeTableInput;
    private String[] codeTableOutput;
    private String error;
    
    public getCodeTable(String codeInput, String codeOutput) throws IOException{
	File fileCodeTable=new File("CodeTable/CodeTable.csv");
	int errorCode=0;//NO ERRORS
	try{
	    FileReader fileReader=new FileReader(fileCodeTable);
	    CSVReader reader= new CSVReader(fileReader);

	    //read and pass the first row
	    reader.readNext();

	    String[] tmp;
	
	    int flag=0;
	    while(flag!=2&&(tmp=reader.readNext())!=null){
		if(tmp[0].equals(codeOutput)){
		    this.codeTableOutput=tmp;
		    flag=flag+1;
		}

		if(tmp[0].equals(codeInput)){
		    this.codeTableInput=tmp;
		    flag=flag+1;
		}
	    }
	
	    //If the program could not find code table?
	
	    fileReader.close();
	    reader.close();
	}catch(IOException e){
	    e.printStackTrace();
	    errorCode=1;//ERROR
	}
    }

    public String[] getCodeTableInput(){
	return codeTableInput;
    }

    public String[] getCodeTableOutput(){
	return codeTableOutput;
    }

    public String getError(){
	return error;
    }
}
