#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "Nodo.h"

using namespace std;

const int IDENTIFICADOR_VACIO = 0;			// 0 -> espacio vacío ( )
const int IDENTIFICADOR_TERRICOLA = 1;		// 1 -> nave terrícola (^)
const int IDENTIFICADOR_ALIEN = 2;			// 2 -> nave alienígena (#)
const int IDENTIFICADOR_BOMBA = 3;			// 3 -> bomba (*)
const int IDENTIFICADOR_MISIL = 4;			// 4 -> misil (")

void comenzarJuego(Nodo **cabecera, int nf, int nc){
	
	int vidas = 3;
	bool primeraFila = true;
	int valorRandom = 0;
	bool primerRecorrido = true;
	bool movimientoValido = false;
	string dir = "";
	string acc = "";
	//Nodo *auxV = (*cabecera)->abajo;
	//Nodo *auxH = auxV->derecha;
	Nodo *auxV = (*cabecera)->abajo;
	Nodo *auxH = auxV->derecha;
	
	Nodo *nave = new Nodo();
	
	// ----------------- DESDE ESTE MOMENTO INICIA EL BUCLE -----------------
	do{
		//system("cls");
		//auxV = (*cabecera)->abajo;
		//auxH = auxV->derecha;
		
		//while(auxV != NULL){
			//while(auxH != NULL){
		srand(time(0));
		for(int i = 0; i < nf ; i++){
			for(int j = 0; j < nc ; j++){
				
				// Si el nodo ya realizó alguna acción, solo setea su valor a 0 y sigue con el siguiente nodo
				if(auxH->estado == 0){
					// Decide si el alien debe bajar o soltar una bomba
					if(auxH->valor == IDENTIFICADOR_ALIEN){
						
						valorRandom = rand() % 25;
						
						// En caso sea 0, el alien bajara al nodo inferior
						if(valorRandom < 5){
							// Verifica si el nodo inferior existe y esta vacio, solo si no está vacío, verifica si la nave terrícola
							// se encuentra en ese nodo y de ser así, se le resta una vida. Si está vacio, el alien desaparece.
							if(auxH->abajo != NULL){
								if(auxH->abajo->valor == IDENTIFICADOR_VACIO){
									auxH->abajo->valor = IDENTIFICADOR_ALIEN;
									auxH->valor = IDENTIFICADOR_VACIO;
									auxH->estado = 1;
								}else if(auxH->abajo->valor == IDENTIFICADOR_TERRICOLA){
									auxH->valor = IDENTIFICADOR_VACIO;
									//auxH->abajo->valor = IDENTIFICADOR_ALIEN;
									vidas --;
									// --------------------------- AQUI SE DEBE REINICIAR LA NAVE TERRICOLA ---------------------------
									//nave->valor = IDENTIFICADOR_TERRICOLA;
									cout<<"PIERDES UNA VIDA: x"<<vidas<<endl;
									Sleep(1000);
									
								}
							}else{
								auxH->valor = IDENTIFICADOR_VACIO;
							}
						}
						
						// En caso sea 1, el alien soltará una bomba
						if(valorRandom > 4 && valorRandom < 10){
							if(auxH->abajo != NULL){
								
								// Si abajo no existe un alien ni una bomba, entonces suelta la bomba.
								// Si abajo hay un misil, ambos proyectiles desaparecen.
								// Si abajo está el terrícola, pierde una vida.
								if(auxH->abajo->valor == IDENTIFICADOR_VACIO){
									auxH->abajo->valor = IDENTIFICADOR_BOMBA;
									auxH->abajo->estado = 1;
								}else if(auxH->abajo->valor == IDENTIFICADOR_MISIL){
									auxH->abajo->valor = IDENTIFICADOR_VACIO;
								}else if(auxH->abajo->valor == IDENTIFICADOR_TERRICOLA){
									//auxH->abajo->valor = IDENTIFICADOR_VACIO;
									vidas --;
									// --------------------------- AQUI SE DEBE REINICIAR LA NAVE TERRICOLA ---------------------------
									//nave->valor = IDENTIFICADOR_TERRICOLA;
									cout<<"PIERDES UNA VIDA: x"<<vidas<<endl;
									Sleep(1000);
								}
							}else{
								auxH->valor = IDENTIFICADOR_VACIO;
							}
						}	
					}
					
					// Si es una bomba, si la bomba no se acaba de mover, se moverá al nodo de abajo.
					if(auxH->valor == IDENTIFICADOR_BOMBA){
						if(auxH->abajo != NULL){
							// Si es un misil, ambos proyectiles desaparecen.
							if(auxH->abajo->valor == IDENTIFICADOR_MISIL){
								auxH->abajo->valor = IDENTIFICADOR_VACIO;
								auxH->valor = IDENTIFICADOR_VACIO;
							}
							
							// Si es la nave terrícola, la nave desaparece y pierde una vida.
							if(auxH->abajo->valor == IDENTIFICADOR_TERRICOLA){
								//auxH->abajo->valor = IDENTIFICADOR_VACIO;
								auxH->valor = IDENTIFICADOR_VACIO;
								vidas --;
								// --------------------------- AQUI SE DEBE REINICIAR LA NAVE TERRICOLA ---------------------------
								//nave->valor = IDENTIFICADOR_TERRICOLA;
								cout<<"PIERDES UNA VIDA: x"<<vidas<<endl;
								Sleep(1000);
							}
							
							// Si es un espacio vacío, la bomba baja y se cambia el estado a 1.
							if(auxH->abajo->valor == IDENTIFICADOR_VACIO){
								auxH->valor = IDENTIFICADOR_VACIO;
								auxH->abajo->valor = IDENTIFICADOR_BOMBA;
								auxH->abajo->estado = 1;
							}
						}else{
							auxH->valor = IDENTIFICADOR_VACIO;
						}
					}
					
					// Si es un misil, si el misil no se acaba de mover, se moverá hacia arriba.
					if(auxH->valor == IDENTIFICADOR_MISIL){
						if(auxH->arriba->valor != 100){
							// Si arriba hay un alien o una bomba, el misil y el objetivo desaparecen.
							if(auxH->arriba->valor == IDENTIFICADOR_BOMBA || auxH->arriba->valor == IDENTIFICADOR_ALIEN){
								auxH->arriba->valor = IDENTIFICADOR_VACIO;
								auxH->valor = IDENTIFICADOR_VACIO;
							}
							
							// Si arriba hay un espacio vacío, el misil sube
							if(auxH->arriba->valor == IDENTIFICADOR_VACIO){
								auxH->valor = IDENTIFICADOR_VACIO;
								auxH->arriba->valor = IDENTIFICADOR_MISIL;
							}
						}else{
							auxH->valor = IDENTIFICADOR_VACIO;
						}
					}
					
					// Si es la nave terrícola, pregunta si se mueve o si desea disparar
					if(auxH->valor == IDENTIFICADOR_TERRICOLA){
						do{
							do{
								//system("cls");
								cout<<"Desea moverse o disparar? [M/D] [ ]\b\b";
								cin>>acc;
								cin.ignore();
								if(acc != "m" && acc != "M" && acc != "d" && acc != "D"){
									cout<<endl<<"Ingrese una opcion correcta"<<endl;
								}else{
									movimientoValido = true;
								}
							}while(!movimientoValido);
							movimientoValido = false;
							
							if(acc == "m" || acc == "M"){
								//system("cls");
								cout<<"Hacia done desea moverse? [I/D] [ ]\b\b";
								cin>>dir;
								cin.ignore();
								// Si la nave se puede mover a la izquierda: si el nodo a la izquierda esta vacío, la nave se mueve
								// Si el nodo a la izquierda tiene un alien o bomba, desaparecen ambos elementos y pierde una vida.
								if(dir == "i" || dir == "I"){
									if(auxH->izquierda != NULL){
										if(auxH->izquierda->valor != 100){
											if(auxH->izquierda->valor == IDENTIFICADOR_VACIO){
												movimientoValido = true;
												auxH->izquierda->valor = IDENTIFICADOR_TERRICOLA;
												auxH->valor = IDENTIFICADOR_VACIO;
											}else if(auxH->izquierda->valor == IDENTIFICADOR_BOMBA || auxH->izquierda->valor == IDENTIFICADOR_ALIEN){
												movimientoValido = true;
												auxH->izquierda->valor = IDENTIFICADOR_VACIO;
												//auxH->valor = IDENTIFICADOR_VACIO;
												vidas --;
												// --------------------------- AQUI SE DEBE REINICIAR LA NAVE TERRICOLA ---------------------------
												//nave->valor = IDENTIFICADOR_TERRICOLA;
												cout<<"PIERDES UNA VIDA: x"<<vidas<<endl;
												Sleep(1000);
											}
										}
									}
								// Si la nave se puede mover a la derecha: si el nodo a la derecha esta vacío, la nave se mueve con estado 1
								// Si el nodo a la derecha tiene un alien o bomba, desaparecen ambos elementos y pierde una vida.
								}else if(dir == "d" || dir == "D"){
									if(auxH->derecha != NULL){
										if(auxH->derecha->valor == IDENTIFICADOR_VACIO){
											movimientoValido = true;
											auxH->derecha->valor = IDENTIFICADOR_TERRICOLA;
											auxH->valor = IDENTIFICADOR_VACIO;
											auxH->derecha->estado = 1;
										}else if(auxH->derecha->valor == IDENTIFICADOR_ALIEN || auxH->derecha->valor == IDENTIFICADOR_BOMBA){
											movimientoValido = true;
											auxH->derecha->valor = IDENTIFICADOR_VACIO;
											//auxH->valor = IDENTIFICADOR_VACIO;
											vidas --;
											// --------------------------- AQUI SE DEBE REINICIAR LA NAVE TERRICOLA ---------------------------
											//nave->valor = IDENTIFICADOR_TERRICOLA;
											cout<<"PIERDES UNA VIDA: x"<<vidas<<endl;
											Sleep(1000);
										}
									}
								}else{
									cout<<endl<<"Ingrese una opcion correcta"<<endl;
									//Sleep(3000);
								}
							// Si la accion es disparar...
							}else if(acc == "d" || acc == "D"){
								if(auxH->arriba->valor == IDENTIFICADOR_ALIEN || auxH->arriba->valor == IDENTIFICADOR_BOMBA){
									auxH->arriba->valor = IDENTIFICADOR_VACIO;
								}else if(auxH->arriba->valor == IDENTIFICADOR_VACIO){
									auxH->arriba->valor = IDENTIFICADOR_MISIL;
								}
								movimientoValido = true;
							}
						}while(!movimientoValido);
						movimientoValido = false;
					}
				}else{
					auxH->estado = 0;
				}
				
				// Solo se insertaran naves si es la primer fila
				if(primeraFila){
					valorRandom = rand() % 20;
					// Inserta un alien solo si la casilla está vacia
					if(auxH->valor == IDENTIFICADOR_VACIO){
						if(valorRandom > 5){
							auxH->valor = IDENTIFICADOR_VACIO;
						}else{
							auxH->valor = IDENTIFICADOR_ALIEN;
						}
					}
				}
				
				// Solo se insertara la nave terrícola si es el primer recorrido
				if(i == nf - 1 && j == nc/2 && primerRecorrido){
					auxH->valor = IDENTIFICADOR_TERRICOLA;
					nave = auxH;
					primerRecorrido = false;
				}
				
				// Imprime el valor del nodo
				/*switch(auxH->valor){
					case IDENTIFICADOR_VACIO:
						cout<<" _ ";
						break;
					case IDENTIFICADOR_TERRICOLA:
						cout<<" ^ ";
						break;
					case IDENTIFICADOR_ALIEN:
						cout<<" # ";
						break;
					case IDENTIFICADOR_BOMBA:
						cout<<" * ";
						break;
					case IDENTIFICADOR_MISIL:
						cout<<" \" ";
						break;
				}*/
				auxH = auxH->derecha;
				//Sleep(200);
			}
			primeraFila = false;
			auxV = auxV->abajo;
			if(auxV != NULL){
				auxH = auxV->derecha;
			}
			
				//cout<<endl<<endl;
		}
		/*do{
			cout<<"Hacia done desea moverse? [I/D] [ ]\b\b";
			cin>>dir;
			cin.ignore();
			if(dir == "i" || dir == "I"){
				if(nave->izquierda != NULL){
					if(nave->izquierda->valor != 100){
						movimientoValido = true;
						nave->izquierda->valor = nave->valor;
						nave->valor = IDENTIFICADOR_VACIO;
					}
				}
			}else if(dir == "d" || dir == "D"){
				if(nave->derecha != NULL){
					movimientoValido = true;
					nave->derecha->valor = nave->valor;
					nave->valor = IDENTIFICADOR_VACIO;
				}
			}
		}while(!movimientoValido);*/
		
		primeraFila = true;
		primerRecorrido = false;
		
		auxV = (*cabecera)->abajo;
		auxH = auxV->derecha;
		
		system("cls");
		while(auxV != NULL){
			while(auxH != NULL){
				switch(auxH->valor){
					case IDENTIFICADOR_VACIO:
						cout<<"   ";
						break;
					case IDENTIFICADOR_TERRICOLA:
						cout<<" ^ ";
						break;
					case IDENTIFICADOR_ALIEN:
						cout<<" # ";
						break;
					case IDENTIFICADOR_BOMBA:
						cout<<" * ";
						break;
					case IDENTIFICADOR_MISIL:
						cout<<" \" ";
						break;
				}
				auxH = auxH->derecha;
			}
			cout<<endl<<endl;
			auxV = auxV->abajo;
			if(auxV != NULL){
				auxH = auxV->derecha;	
			}
		}
		
		auxV = (*cabecera)->abajo;
		auxH = auxV->derecha;
		
	}while(vidas > 0);
	// ----------------- ACÁ TERMINA EL BLUCLE DEL JUEGO -----------------
	
	system("cls");
	cout<<"************************* GAME OVER *************************"<<endl;
	
	// Limpia la lista ortogonal para liberar el espacio en memoria
	
	Nodo *pDel = (*cabecera);
	Nodo *temp = NULL;
	
	do{
		while(pDel != NULL)
		{
		  temp = pDel->derecha;
		  delete[] pDel;
		  pDel = temp;
		}
		pDel = auxV;
		auxV = auxV->abajo;
	}while(auxV->abajo != NULL);
	
	Sleep(3000);
}

void crearLista(int nf, int nc){
	Nodo *cabecera = new Nodo();
	Nodo *ultimoH = NULL;
	Nodo *ultimoV = NULL;
	
	for(int i = 0; i < nc; i++){
		Nodo *p = new Nodo();
		p->valor = 100;
		
		if(cabecera->derecha == NULL){
			p->izquierda = cabecera;
			cabecera->derecha = p;
			ultimoH = cabecera->derecha;
		}else{
			p->izquierda = ultimoH;
			ultimoH->derecha = p;
			ultimoH = ultimoH->derecha;
		}
	}
	
	for(int i = 0; i < nf; i++){
		Nodo *p = new Nodo();
		p->valor = 100;
		
		if(cabecera->abajo == NULL){
			p->arriba = cabecera;
			cabecera->abajo = p;
			ultimoV = cabecera->abajo;
		}else{
			p->arriba = ultimoV;
			ultimoV->abajo = p;
			ultimoV = ultimoV->abajo;
		}
	}
	
	/*Nodo *aux = cabecera->derecha;
	while(aux != NULL){
		if(aux->derecha != NULL){
			cout<<" "<<aux->izquierda->valor<<" "<<aux->valor<<" "<<aux->derecha->valor<<" || ";
		}
		aux = aux->derecha;
	}
	cout<<endl<<endl;
	aux = cabecera->abajo;
	while(aux != NULL){
		if(aux->abajo != NULL){
			cout<<" "<<aux->arriba->valor<<" "<<aux->valor<<" "<<aux->abajo->valor<<" || ";	
		}
		aux = aux->abajo;
	}*/

	Nodo *auxH = cabecera->derecha;
	Nodo *auxV = cabecera->abajo;
	Nodo *auxUltimoH = cabecera->derecha;
	Nodo *auxUltimoV = cabecera->abajo;
	//int x = 1;
	// Inserta nuevos nodos en toda la matriz
	while(auxV != NULL){
		while(auxH != NULL){
			Nodo *q = new Nodo();
			//q->valor = x;
			//x++;
			
			q->izquierda = auxUltimoV;
			auxUltimoV->derecha = q;
			
			q->arriba = auxUltimoH;
			auxUltimoH->abajo = q;
			
			auxUltimoH = auxUltimoH->derecha;
			auxUltimoV = q;
			
			auxH = auxH->derecha;
			//cout<<q->arriba->valor<<"-"<<q->valor<<"-"<<"||";
			//cout<<q->izquierda->valor<<"-"<<q->valor<<"-"<<"||";
		}
		auxUltimoH = auxV->derecha;
		auxH = cabecera->derecha;
		auxV = auxV->abajo;
		auxUltimoV = auxV;
		//cout<<endl;
	}
	
	comenzarJuego(&cabecera, nf, nc);
	
	/*auxV = cabecera->abajo;
	auxH = auxV->derecha;
	
	while(auxV != NULL){
		while(auxH != NULL){
			/*if(auxH->arriba != NULL){
				cout<<auxH->arriba->valor<<"^";
			}
			if(auxH->abajo != NULL){
				cout<<auxH->abajo->valor<<"v";
			}
			cout<<" "<<auxH->valor<<"  ";
			/*if(auxH->izquierda != NULL){
				cout<<"<"<<auxH->izquierda->valor;
			}
			if(auxH->derecha != NULL){
				cout<<">"<<auxH->derecha->valor;
			}
			cout<<"\t";
			auxH = auxH->derecha;
		}
		cout<<endl<<endl;
		auxV = auxV->abajo;
		if(auxV != NULL){
			auxH = auxV->derecha;	
		}
		
	}
	
	system("pause");*/
}

int main(int argc, char** argv) {
	// Opción del menú
	int op = 0;
	// Dimension con que se creará la matriz
	int noCol = 0;
	int noFil = 0;
	
	do{
		system("cls");
		cout<<"*****************MENU*****************\n";
		cout<<"*         1 - NUEVO JUEGO            *\n";
		cout<<"*         2 - SALIR                  *\n";
		cout<<"**************************************\n\n";
		cout<<"Escriba la opcion que desee: [ ]\b\b";
		cin>>op;
		cin.ignore();
		switch(op){
			case 1:
				do{
					system("cls");
					cout<<"\nCuantas columnas desea? (minimo 5) [ ]\b\b";
					cin>>noCol;
					cin.ignore();
				}while(noCol < 5);
				
				do{
					system("cls");
					cout<<"\nCuantas filas desea? (minimo 5) [ ]\b\b";
					cin>>noFil;
					cin.ignore();
				}while(noFil < 5);
				system("cls");
				
				crearLista(noFil, noCol);
				
				break;
			case 2:
				cout<<"\n\nGracias por jugar!"<<endl;
				Sleep(3000);
				break;
			default:
				cout<<"\n\nIngrese una opcion valida."<<endl;
				Sleep(3000);
				break;
		}
	}while(op != 2);
}
