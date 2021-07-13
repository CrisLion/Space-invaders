#include<iostream>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>
using namespace std;

#define DISPARAR 72

//Prototipo de funcion
void gotoxy(int, int);
void OcultarCursor();
void pintar_limites();
void menu();

//Definicion de funcion
void gotoxy(int x, int y){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;	
	
	SetConsoleCursorPosition(hCon, dwPos);
} 

//Definicion de funcion
void OcultarCursor(){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible =false;
	cci.dwSize= 1;
		
	SetConsoleCursorInfo(hCon,&cci);
}

//Definicion de funcion
void pintar_limites(){
	for(int i= 2; i<78;i++){
		gotoxy(i,3); printf("%c",205);
		gotoxy(i,33);printf("%c",205);
	}
	
	for(int i=4;i<33;i++){
		gotoxy(2,i);printf("%c",186);
		gotoxy(77,i); printf("%c",186);
	}
	
	gotoxy(2,3); printf("%c",201);
	gotoxy(2,33); printf("%c",200);
	gotoxy(77,3); printf("%c",187);
	gotoxy(77,33); printf("%c",188);
}

//CLASE NAVE
class NAVE{
	private: //atributos
		int x,y, corazones, vidas;
	public: //metodos
		NAVE(int,int,int, int);
		int X(){return x; }
		int Y(){return y; }
		int VID(){return vidas; }
		void COR(){corazones--; }
		void pintar();
		void borrar();
		void mover();	
		void pintar_corazones();
		void morir();
		void aumentar_vida();	
};

NAVE::NAVE(int _x,int _y, int _corazones,int _vidas){
	x = _x;
	y = _y;
	corazones = _corazones;
	vidas = _vidas;
}

void NAVE::pintar(){
	gotoxy(x,y)  ;printf("  %c",142);
	gotoxy(x,y+1);printf(" %c%c%c",40,254,41);
	gotoxy(x,y+2);printf("%c%c %c%c",192,190,190,217);
}

void NAVE::borrar(){
	gotoxy(x,y)  ;printf("     ");
	gotoxy(x,y+1)  ;printf("     ");
	gotoxy(x,y+2)  ;printf("     ");	
}

void NAVE::mover(){
	if(kbhit()){
		char tecla = getch();
		borrar();
		if(tecla == 'a' && x>3) x--;
		if(tecla == 'd' && x+5<77) x++;
		if(tecla == 'w' && y>4) y--;
		if(tecla == 's'&& y+3<33) y++;
		pintar_corazones();
		pintar();
	}
}

void NAVE::pintar_corazones(){
	gotoxy(50,2);printf("VIDAS: %d",vidas);
	gotoxy(64,2);printf("Salud:");
	gotoxy(70,2);printf("        ");
	for(int i=0; i<corazones; i++){
		
		gotoxy(70+i,2);printf("%c",3);
	}	
}

void NAVE::morir(){
	if(corazones == 0){
		borrar();
		gotoxy(x,y);  printf("  ** ");
		gotoxy(x,y+1);printf(" ****");
		gotoxy(x,y+2);printf("  ** ");
		Sleep(200);
		
		borrar();
		gotoxy(x,y);  printf("  **");
		gotoxy(x,y+1);printf(" *  *");
		gotoxy(x,y+2);printf("  **");
		Sleep(200);
		borrar();
		
		vidas--;
		corazones = 3;
		pintar_corazones();
		pintar();
	}
}

void NAVE::aumentar_vida(){
	vidas++;
	Beep(600,200);
}

//CLASE ASTEROIDE
class AST{
	private:
		int x,y;
	public:
		AST(int, int);
		int X(){return x; }
		int Y(){return y; }
		void pintar();
		void mover();
		void choque(class NAVE &N);	
};

AST::AST(int _x, int _y){
	x = _x;
	y = _y;	
}

void AST::pintar(){
	gotoxy(x,y); printf("%c",184);	
}

void AST::mover(){
	gotoxy(x,y); printf(" ");
	y++;
	if(y>32){
		x = rand()%71 +4;
		y= 4;
	}
	pintar();
}

void AST::choque(class NAVE &N){
	if(x >= N.X() && x < N.X()+5 && y >=N.Y() && y<= N.Y()+2 ){
		N.COR();
		N.borrar();
		N.pintar();
		N.pintar_corazones();
		x = rand()%71 +4;
		y = 4;			
	}
}

//CLASE BALA
class BALA{
	private:
		int x,y;
	public:
		BALA(int, int);
		int X() {return x; }
		int Y() {return y; }
		void mover();
		bool fuera();
};

BALA::BALA(int _x, int _y){
	x = _x;
	y = _y;
}

void BALA::mover(){
	gotoxy(x,y);printf(" ");
	y--;
	gotoxy(x,y);printf("I");
	 
}

bool BALA::fuera(){
	if(y == 4) return true;
	return false;
	
}

int main(){
	int opc_menu =0;
	
	do{
		NAVE n(37,30,3,3);
		//Creando lista de asteroides con su interador
		list<AST*> A;
		list<AST*>::iterator itA;
					
		//Llenando lista con objetos de clase asteroide y spawneandolos en un rango aleatorio dentro del margen solo al iniciar el juego
		for(int i = 0; i<5; i++){
			A.push_back(new AST(rand()%75 + 3, rand()%5 +4));
		}
		
		//Creando lista de balas con su iterador
		list<BALA*> B;
		list<BALA*>::iterator it;
					
		bool game_over = false;
		int puntos = 0;
		int contador_vida_extra = 0;
		int contador_bala_2 =0;
		int contador_bala_3 =0;
		int contador_dificultad= 0;
		
		pintar_limites();
		gotoxy(12,10);printf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c %c%c%c %c%c%c%c %c%c%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c",220,223,223,219,223,220,220,223,220,220,223,223,219,223,223,46,223,219,223,219,220,176,219,219,176,176,176,219,220,223,220,219,223,220,219,223,223,219,223,220,220,223,223);
		gotoxy(12,11);printf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c %c%c%c %c%c%c%c %c%c%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c",176, 223, 220, 219, 223, 176, 219, 220, 219, 219, 176, 176, 219, 223, 223, 46, 176, 219, 176, 219, 223, 219, 219, 176, 219, 176, 219, 176, 219, 220, 219, 219, 176, 219, 219, 223, 223, 219, 223, 220, 176, 223, 220);
		gotoxy(12,12);printf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c %c%c%c %c%c%c%c %c%c%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c",223, 223, 176, 223, 176, 176, 223, 176, 223, 176, 223, 223, 223, 223, 223, 46, 223, 223, 223, 223, 176, 176, 223, 176, 176, 223, 176, 176, 223, 176, 223, 223, 223, 176, 223, 223, 223, 223, 176, 223, 223, 223, 176);
		
		gotoxy(32,15);cout<<"1. Jugar";
		gotoxy(32,16);cout<<"2. Controles";
		gotoxy(32,17);cout<<"3. Reglas";
		gotoxy(32,18);printf("4. Cr%cditos",130);
		gotoxy(32,19);cout<<"5. Salir";
		gotoxy(29,21);printf("->Elija una opci%cn: ",162);cin>>opc_menu;Beep(350,260);
		
		switch (opc_menu){
			case 1:
				system("cls");
				pintar_limites();
				OcultarCursor();
				n.pintar();	
				n.pintar_corazones();
				
				//Bucle del juego
				while(!game_over){
					gotoxy(4,2);printf("Puntos: %d",puntos);
					n.mover();
					if(kbhit()){
						char tecla = getch();
						if(tecla == 72){
							B.push_back(new BALA(n.X()+2,n.Y()-1));	//Creando bala
							
							if(contador_bala_2>=20){ //Creando bala 2 si alcanza el puntaje 100
								B.push_back(new BALA(n.X()+4,n.Y()+1));
							}
							
							if(contador_bala_3>=60){//Creando bala 3 si alacanza el puntaje 300;
								B.push_back(new BALA(n.X(),n.Y()+1));
							}	
						}
					}
					
					//Aumento cantidad de asteroides cuando consigues el segundo cañon
					if(contador_dificultad == 20){
						for(int i = 0; i<6; i++){
							A.push_back(new AST(rand()%75 + 3, rand()%5 +4));
						}
						contador_dificultad =21;
					}
					
					//Aumenta la cantidad de asteroides cuando consigues el tercer cañon
					if (contador_dificultad == 60){
						for(int i = 0; i<10; i++){
							A.push_back(new AST(rand()%75 + 3, rand()%5 +4));
						}
						contador_dificultad =61;
					}
					
					//Eliminando la bala cuando este fuera del rango
					for(it = B.begin(); it != B.end(); it++){
						(*it)->mover();	
						if((*it)->fuera()){
							gotoxy((*it)->X(),(*it)->Y()); printf(" ");
							delete (*it);
							it = B.erase(it);
						}
					}
					
					//llamando a los metodos de los 5 asteroides
					for(itA = A.begin(); itA != A.end(); itA++){
						(*itA)->mover();
						(*itA)->choque(n);
					}
					
					//Revisa si los asteroides han chocado con alguna bala
					for(itA = A.begin(); itA != A.end(); itA++){
						for(it = B.begin(); it != B.end(); it++){
							if((*itA)->X() == (*it)->X() && ( (*itA)->Y()+1 == (*it)->Y() || (*itA)->Y() == (*it)->Y())){
								//borrando la bala
								gotoxy((*it)->X(),(*it)->Y()); printf(" ");
								delete(*it);
								it = B.erase(it);
								
								//creando un nuevo asteroide y borrando el asteroide colicionado
								A.push_back(new AST(rand()%74 +3,4));
								gotoxy((*itA)->X(),(*itA)->Y()); printf(" ");
								delete(*itA);
								itA = A.erase(itA);
								
								puntos+=5;
								contador_vida_extra++;
								contador_bala_2++;
								contador_bala_3++;
								contador_dificultad++;
							}
						}	
					}
						
					if(n.VID() == 0){
						game_over = true;
					}
					
					if (contador_vida_extra == 100 && contador_vida_extra!=0){
						n.aumentar_vida();
						contador_vida_extra =0;
					}	
				
					n.morir();	
					Sleep(30);
				}
				system("cls");
					
					cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&&&&&&%#%%&###%&&&&%##%%#%%#######%####%&#######&#(#@@&@@@&&@@@@@@@@@@@@@@@@@@@@@"<<endl;
					cout<<"@@@@@@@@@@@@@@@@&&&&&&&&&&&&&&&&&&&&&&&&&%##(#&&&&&*&&&&&%%&&%###(#(((##(#######(#@&&&&&&&&&&&&&&&&&&@@@@@@@@@@@@@@"<<endl;
					cout<<"@@@@@@@@@@@@@@&&&&&&&&&&#@&&&&&&&&&&&&&&&%%&&&&&&&&&&&&&&&%&&&@@@@@@@@&&@@@@@@@@@@&&&&&&&&&&&&&&&&&&&&&&@@@@@@@@@@&"<<endl;
					cout<<"@@@@@@@&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&/#&&&%/%&@@%%%%&&&&&&&&&&%%%%%%%%#%%&@@&&&&&&&&&&&&&&&&&&&&&&&&@&@@"<<endl;
					cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%&&@%%&@%(*,,,,*,*,,*/(%%&@@&&&&&&&&%&@&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
					cout<<"&@&&&&&&&&&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%&@&%%&&/,/((((((((//**,,,,,. ,**,,,*(&@&&&@&%&&&&&&&&&&&&&&&&&&&&&"<<endl;
					cout<<"&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@&#%%@(,/####################(#(///*,,,,,  ./%@&&%%%%&&&&&&&&&&&&&&&"<<endl;
					cout<<"&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%&@@@@@@@@@@@%%%%&(,/##################################((*,,. ./@%%%%%%&&%%&&&&&&&"<<endl;
					cout<<"&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%@&%%%%%&&&&@&%%%%&#,*##########################################(**/@%%%%%%%%%%%%&&&&"<<endl;
					cout<<"&&%%%%%%%%%%%%%%%%%%%%%%%%#%@&%%%%%&&&&@@%%%%%&&*,*#############################################(,%%%%%%%%%%%%%%%%%"<<endl;
					cout<<"%%%%%%%%%%%%%%%%%%%%####@@%%%%%&&&&&@&%%%%%%&@*  ,/(((((########################################*(&%%%%%%%%%%%%%%%%"<<endl;
					cout<<"%%%%%%%%%%%%%%%%###%##%@&%%%&&&&&@@@&%%%%%%&&&@**.,*,***/(((((((((((((###########################//@#####%%%%%%%%%%"<<endl;
					cout<<"%%%%%%%%%%%%########&@%#%%&&&&&@@@@#%%%%%&&&&&&&&/,,**   .,,,**/((((((((((((((((((###############//@@##########%%%%"<<endl;
					cout<<"%%%%##############&@%%%%&&&&&@@@@%%#%%%%&&&&&&&&@&@@&#*,,*,,.    .,,***/(((((((((((((((((########//&%@###########%%"<<endl;
					cout<<"################@@%%%%%&&&&@@@@&#%%%%%%&&&&&&&&&@@@@@@@@@@@@&(/*,,*,,*,,  .,****,,,**///(((((((##,/@#&&(###########"<<endl;
					cout<<"#############(%@%%%%&&&&@@@@@@%%%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@&&@@&%#/,,,,,,,,,..        *   ,#&%%&&((#########"<<endl;
					cout<<"#########(((#@%%%%%&&&&@@@@@&%%%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&@@@&@@@&&&&&%%%%@#((((((####"<<endl;
					cout<<"((#((((((((&&%%%%%&&@@@@@@@%%%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&&&%%%@(((((((((("<<endl;
					cout<<"(((((((((#@%%%%%&&&&@@@@@@%%%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&&%%%%@((((((((("<<endl;
					cout<<"(((((((/%&%%%%%&&&@@@@@@&%%%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&@&&&&%%%&%//(((((("<<endl;
					cout<<"((((((/&&%%%%%&&&@@@@@@&%%%%%%%&&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&&%%%%@(///(((("<<endl;
					cout<<"(((///&&%%%%%&&&@@@@@@&%%%%%%&&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&&%%%%&&//////("<<endl;
					cout<<"((///%&%%%%%%&&&@@@@@&%#%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&%#%%%@#//////"<<endl;
					cout<<"////(@%%%%%%&&&&@@@@&%%%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&&%%%%&@*/////"<<endl;
					cout<<"(#/*@&%%%%%%&&&&@@@&%%%%%%%&&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&%%%%@%*////"<<endl;
					cout<<",,*%@%%%%%%%%&&&&@&%%%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&%%%%%@***//"<<endl;
					cout<<",,,#@%%%%%%%%&&&@&%%%%%%%&&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&%%%%%@(****"<<endl;
					cout<<",,,#@%%%%%%%%%%&@%%%%%%%%&&&&&&&&@@@@@@@@@@@@@@@@@@@@&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@@@@@@@@@@@@@@&&&&&&%%%%%&&****"<<endl;
					cout<<",,,/@#%%%%%%%%%@%%%%%%%%&&&&&&@&&@@@@@@@@@@@@@@&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@@@@@@@@@@&&&&&&&%%%%%@/***"<<endl;
					cout<<",,,,(@%%%%%%%#&%%%%%%%%&&&&&&&&@@@@@@@@@@@@&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@@@@&&&&&&&%%%%%&%***"<<endl;
					cout<<",,,,,*@&%%%%%&&%%%%%%%&&&&&&&&&@@@@@@@@&&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%&&&&&&&&&&&&&&&&&&&&@@&&&&&&%%%%%%@***"<<endl;
					cout<<",,,,,,,,#@&&&@%%%%%%%&&&&&&&&&&@@@@@&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&&&&&&&&&&&&&&&&&&&&&%%%%%%@/**"<<endl;
					cout<<",,,,,,,,,.,,@%%%%%%%%&&&&&&&&&&@@&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&&&&&&&&&&&&&&&%%%%%%&#**"<<endl;
					cout<<",,,,,,,,,,,@&%%%%%%%&&&&&&&&&&@@&&&&&&&&&&&%%%%%%%%%%%%%%%%%#######%%%%%%%%%%%%%%%%%%%%%%%%%&&&&&&&&&&&&%%%%%%%%&**"<<endl;
					cout<<",,,,,,,...%&#%%%%%%&&&&&&&&&&&&&&&&&&&&&%%%%%%%%%%###############################%###%%%%%%%%%%%&&&&&&&&%%%%%%%%@,,"<<endl;
					cout<<",,,,,,,,./@%%%%%%%%&&&&&&&&&&&&&&&&&&%%%%%%%%%%%########################################%%%%%%%%%%%&&%&&%%%#%#%%@*,"<<endl;
					cout<<",,..,,..,@%%%%%%%%&&&&&&&&&&&&&&&&&%%%%%%%%%%#####((((((((((((((((((((((((((((((((((#######%%%%%%%%%&%%%%%%###%%&#,"<<endl;
					cout<<",,..,,..%%%%%%%%%&&&&&&&&&&&&&&&&%%%%%%%%%%###(##(((((////////////////////////(((((((((((####%%%%%%%%%%%%%%####%%&,"<<endl;
					cout<<"......./&%%%%%%%%&&&&&%&&&&&&&&%%%%%%%%%%####(((((////////////////////////////////////((((###%%%%%%%%%%%%%%####%#@,"<<endl;
					cout<<"......,@%%%%%%%%&&&&&%&&&&&&&&&%%%%%%%%####(((((////////((((/,.....      ..,/((((//////((((#####%%%%%%%%%%%######@,"<<endl;
					cout<<"....,,(&%%%%%%%%&&&&%&&&&&&&&%%%%%%%%%####(((((////////(,                        /(//////((((####%%%%%%%%%%%#####@/"<<endl;
					cout<<"......@%%%%%%%%%&&%%%&&&&&&&%%%%%%%%%###(((((/////////(                           .#//////((((#####%%%%%%%%%#####%#"<<endl;
					cout<<"\nOoops. Parece que se te acabaron las vidas D:"<<endl;
					cout<<"Tu puntuacion ha sido de "<<puntos<<". Felicidades!!"<<endl<<endl;
					system("pause");
					Beep(350,260);
					system("cls");
					break;
			case 2:
				system("cls");
				pintar_limites();
				gotoxy(35,11);cout<<"Controles";
				gotoxy(29,13);cout<<"-'w' mover arriba.";
				gotoxy(29,14);cout<<"-'s' mover abajo.";
				gotoxy(29,15);cout<<"-'a' mover a la derecha.";
				gotoxy(29,16);cout<<"-'d' mover a la izquierda.";
				gotoxy(29,17);cout<<"-'tecla direccional arriba' disparar.";
				
				gotoxy(24,20);system("pause");
				Beep(350,260);
				system("cls");
				break;
			case 3:
				system("cls");
				pintar_limites();
				gotoxy(35,11);cout<<"Reglas";
				gotoxy(14,13);cout<<"Vuela, esquiva y dispara a los asteroides. Mientras";
				gotoxy(14,14);cout<<"mas asteroides derribes, mas puntos tendras y mejor";
				gotoxy(14,15);cout<<"aun... ¡MAS ARMAS!";
				gotoxy(14,16);cout<<"->RECUERDA<-";
				gotoxy(14,17);printf("A los 100 puntos --> ¡Un segundo ca%con!",164);;
				gotoxy(14,18);printf("A los 300 puntos --> ¡Un tercer ca%con!",164);
				gotoxy(14,19);cout<<"Por cada 500 puntos --> ¡Una vida extra!";
				gotoxy(14,20);cout<<"->CUIDADO<-";
				gotoxy(14,21);cout<<"Cada vez que consigas un arma, la dificultad aumenta";
													
				gotoxy(22,25);system("pause");
				Beep(350,260);
				system("cls");
				break;
			case 4:
				system("cls");
				pintar_limites();
				gotoxy(35,11);printf("Cr%cditos",130);
				gotoxy(22,13);cout<<"CrisLion10 (Discord) --> TODO XD";
				gotoxy(22,14);printf("Dedede988  (Discord) --> El t%ctulo BRUH",161);
				
				gotoxy(22,20);system("pause");
				Beep(350,260);
				system("cls");
				break;				
		}
		
	}while(opc_menu !=5);
	
	system("pause");	
	return 0;
}


