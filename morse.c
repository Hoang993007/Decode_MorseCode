#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define morseCodeLength 10
#define characterLength 6//max length is 5 and one slot for storing '\0' -  the end of the string

//NOTE: ở file đầu vào, kết thúc mỗi từ bắt buộc phải có ' ', ko đc bỏ bớt, bỏ bớt sễ ko nhận điện đc. Đặc biệt là cuối dòng thường quên viết ' ' mà thêm vào luôn là '\n'
//NOTE: file đầu ra mình cũng làm y như nguyên tắc trên, luôn có ' '
//NOTE: sau dấu '/' luôn là 1 dấu ' '

enum inputFileType{MorseCode, Text};
  
int main(int argc, char *argv[]){

  
  //OPEN INPUT FILE
  FILE *inputFile;
  if((inputFile=fopen(argv[1],"r"))==NULL){
    printf("Error: %s could not be opened.\n", argv[1]);
    return 0;
  }

  
  //DISCOVER TYPE OF INPUT FILE: MorseCode OR Text
  int checkType=MorseCode;
  char c;
  while((c=fgetc(inputFile))!=EOF)
    if(c!=' '&&c!='.'&&c!='/'&&c!='-'&&c!='\n'){
      checkType=Text;
      break;
    }
  
  if(checkType==MorseCode)
    printf("File input consist of morse code.\n");
  else printf("File input consist of text.\n");

  
  //CHECK OUTPUT FILE EXISTS OR NOT
  FILE *outputFile;
  if((outputFile=fopen(argv[2],"r"))!=NULL){
    char checkYN;
    printf("\nWarning: %s already exists. Do you wish to overwrite (y/n)?: ", argv[2]);
    checkYN=getchar();
    printf("\n");
    if(checkYN=='n') return 0;
    }
  fclose(outputFile);
  outputFile=fopen(argv[2],"w");//REOPEN TO WRITE

  
  //READ FILE morseCode.txt CONSIST OF MORSE CODE
  int numMorseCode;
  FILE *file;
  if((file=fopen("morseCode.txt","r"))==NULL){
    printf("Error: morseCode.txt could not be opened.\n");
    return 0;
  }
  
  fscanf(file,"%d", &numMorseCode);
  fgetc(file);

  char morseCodeArray[numMorseCode][morseCodeLength];
  char characterArray[numMorseCode][characterLength];
  
  for(int i=0;i<numMorseCode;i++){
    fscanf(file,"%s",characterArray[i]);
    fgetc(file);
    fscanf(file,"%s",morseCodeArray[i]);
  }
  fclose(file);
  
  //START TO CONVERT FILE
  int i=0;
  char character[characterLength];
  char morseCode[morseCodeLength];
  int countLine;
  
  fseek(inputFile, 0, SEEK_SET);//SET THE FILE POSITION TO THE BEGINNING OF THE FILE OF THE GIVEN STREAM
  
  switch(checkType){
    
  case MorseCode:
    //INPUT FILE CONSIST OF MORSE CODE
    countLine=1;
    int morseCodeCount=0;
    
    //'\n', ' ', '/'
    while((c=fgetc(inputFile))!=EOF){
      if(c=='/')
	fprintf(outputFile," ");
       else if(c!=' '&&c!='\n'){
	morseCode[morseCodeCount]=c;
	morseCodeCount=morseCodeCount+1;
      }else{
	 
	 if(morseCodeCount!=0){//IF MEET '\n' OR ' ' AND THE morseCode NOT NULL
	  morseCode[morseCodeCount]='\0';//ADD END OF THE STRING
	  
	  //TRANSFER CHARACTER TO MORSE CODE
	  i=0;
	  //COMPARE TO FIND CORRESPONDING MORSE CODE
	  while(i<numMorseCode && strcmp(morseCode,morseCodeArray[i])!=0)
	    i=i+1;
	  
	  if(i<numMorseCode) {//FIND OUT
	    strcpy(character,characterArray[i]);
	    fprintf(outputFile, "%s", character);
	  }else{//NOT EXITST
	    fprintf(outputFile, "*");
	    printf("Error: Invalid Morse code %s on line %d.\n",morseCode, countLine);
	  }
	  
	  morseCodeCount=0;//RESET morseCodeCount to 0 - START TO READ A NEW MORSE CODE
	}
	
	//END OF LINE
	if(c=='\n'){
	  fprintf(outputFile,"\n");
	  countLine=countLine+1;
	}
	
      }
    }
    break;
    
  case Text:
    //INPUT FILE CONSIST OF TEXT
    countLine=1;
    //' ','\n'
    while((c=fgetc(inputFile))!=EOF){
      if(c==' '){
	fprintf(outputFile,"/");
	fprintf(outputFile," ");
      }else if(c=='\n'){
	fprintf(outputFile,"\n");
	countLine=countLine+1;
      }else{

	
	//TRANSFER CHARACTER TO MORSE CODE
	i=0;
	if(c>='A'&&c<='Z')
	  c=c+32;//change c to lowercase
	
	character[0]=c;
	character[1]='\0';

	//COMPARE
	while(i<numMorseCode && strcmp(character,characterArray[i])!=0)
	  i=i+1;
	
	if(i<numMorseCode){
	  strcpy(morseCode, morseCodeArray[i]);
	  fprintf(outputFile, "%s", morseCode);
	  fprintf(outputFile, " ");
	}
	else
	  printf("Error: Unrecognised character %s on line %d.\n",character,countLine);
	
      }
    }
    break;
    
  default://DO NOTHING
    break;
  }
  
  fclose(outputFile);
  fclose(inputFile);
  return 0;
}
