#include "output.h"
#include "string/string.h"

vide principale(){
	imprimer("Ciao mondo");
	for (int i = 0; i < 10; i = i + 1){
		if (i == 9){
			imprimer("Le derniere");
		} else {
			imprimer("Pas la derniere");
		}
	}
	imprimer("Fine");
}
