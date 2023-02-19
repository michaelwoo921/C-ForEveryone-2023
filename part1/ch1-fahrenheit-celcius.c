/* Convert Fahrenheit to Cecius
 * 
 */

#include <stdio.h>


int convert_to_celcius(int fahrenheit){

	return (fahrenheit - 32) /180.0 * 100;
}

int main(void){
	int fahrenheit, celcius, i;

	printf("Fahrenheit and Celcius table\n");
	printf("Fahrenheit --- Celcius\n");
	for(i=0; i<120; i +=10){
		fahrenheit = i;
		celcius = convert_to_celcius(fahrenheit);
		printf("%d --- %d \n", fahrenheit, celcius);
	}
		

	return 0;
}


