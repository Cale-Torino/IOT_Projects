//https://www.youtube.com/user/greatscottlab
int ignite1=5;
int ignite2=6;
int ignite3=7;
int rx=0;
int timer1;
int timer2;
int timer3;
int turnoff=80;
int sensevoltage=A0;
int cutoffvoltage=800;
char inSerial[15];

void setup(){
  Serial.begin(9600);
  pinMode(ignite1, OUTPUT);
  pinMode(ignite2, OUTPUT);
  pinMode(ignite3, OUTPUT);
  pinMode(rx, INPUT);
  pinMode(sensevoltage, INPUT);
  timer1=0;
  timer2=0;
  timer3=0;
  allpinslow();}

void loop(){
    int i=0;
    int m=0;
    delay(100);                                         
    if (Serial.available() > 0) {             
       while (Serial.available() > 0) {
         inSerial[i]=Serial.read(); 
         i++;      
       }
       inSerial[i]='\0';
      Check_Protocol(inSerial);}
      if(digitalRead(ignite1)==HIGH){
        timer1++;}
      if(digitalRead(ignite2)==HIGH){
        timer2++;}
      if(digitalRead(ignite3)==HIGH){
        timer3++;}
      if(timer1>turnoff){
        digitalWrite(ignite1,LOW);
        timer1=0;}
      if(timer2>turnoff){
        digitalWrite(ignite2,LOW);
        timer2=0;}  
      if(timer3>turnoff){
        digitalWrite(ignite3,LOW);
        timer3=0;}}
     
void allpinslow()
{
digitalWrite(ignite1, LOW);
digitalWrite(ignite2, LOW);
digitalWrite(ignite3, LOW);
}     
  
void Check_Protocol(char inStr[]){   
  int i=0;
  int m=0;
  
  if(!strcmp(inStr,"Ignite1")){
    if(analogRead(sensevoltage)>cutoffvoltage){      
    digitalWrite(ignite1, HIGH);
    for(m=0;m<11;m++){
      inStr[m]=0;}
       i=0;}}
       
  if(!strcmp(inStr,"Ignite2")){
    if(analogRead(sensevoltage)>cutoffvoltage){ 
    digitalWrite(ignite2, HIGH);      
    for(m=0;m<11;m++){
      inStr[m]=0;}
       i=0;}}
   
   if(!strcmp(inStr,"Ignite3")){ 
    if(analogRead(sensevoltage)>cutoffvoltage){  
    digitalWrite(ignite3, HIGH);    
    for(m=0;m<11;m++){
      inStr[m]=0;}
       i=0;}}
       
    else{
    for(m=0;m<11;m++){
      inStr[m]=0;
    }
    i=0;

}}
  
