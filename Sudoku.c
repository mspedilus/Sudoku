#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

int repeat(int x, int y, int size, int sudoku[size][size], int old_value, int new_value, int random_val, int fill, char check_location[][3], char* function);
void valid_location(int* x, int* y, int size, int* new_value, int* old_value, int sudoku[size][size], char check_location[][3], int fill, char* function);
void display_board(int size, int sudoku[size][size], int fill, char check_location[][3]);
void print_dashes(int size, int m);
void valid_value(int* x, int* y, int size, int* new_value, int sudoku[size][size], int* old_value, char check_location[][3], int fill, char* function);
int auto_fill(int size, int sudoku[size][size]);
void abort_game(char* response, char* function, int size, int fill, int sudoku[size][size], char check_location[][3]);
int print_dot(int m, int n, int counter, char check_location[][3]);


//Fix autofill bug for sizes 6 & 9
//Fix repeat
//DIAGONAL FROM LEFT TO RIGHT ISNT WORKING PROPERLY. SHOULD ONLY BE MIDDLE ROW BUT ITS COUNTING OTHER DIAGONAl ROWS TOO (SOMETIMES??)
void main(void){

  //Initializes rand() function & sudoku board
  srand(time(0));
  //printf("Treat the 0's as empty spaces that need to be filled/n");

  int x = 0, // row location
      y = 0, // column location
      size = 9, // Size of sudoku board
      valid = 0,
      new_value = 0,
      has_zero = 1,
      answer,
      old_value,
      random_val,
      sudoku[size][size],
      fill = auto_fill(size, sudoku);
  char check_location[fill][3],
       function; //L is in valid_location, V is in valid_value


      //Generates random numbers (from 1-4) in random locations (from 0-3) in the array
      for(int i = 0; i < fill; i++){
        int temp_x = rand()%size,
            temp_y = rand()%size,
            result;
        random_val = rand()%size + 1;

        //Makes sure that there will be 3 filled for the start of sudoku
        while (sudoku[temp_x][temp_y] != 0){
          temp_x = rand()%size;
          temp_y = rand()%size;
        }

        //Autofill random values in random locations
        old_value = sudoku[temp_x][temp_y];
        sudoku[temp_x][temp_y] = random_val;
        result = repeat(temp_x, temp_y, size, sudoku, old_value, new_value, random_val, fill, check_location, &function);

        //Makes sure that the random val can be placed in the random location
        while(result == 0){
          sudoku[temp_x][temp_y] = rand()%size + 1;
          result = repeat(temp_x, temp_y, size, sudoku, old_value, new_value, random_val, fill, check_location, &function);
        }

        //Stores locations of the values that you cannot change
        sprintf(check_location[i], "%d%d", temp_x, temp_y);
      }
      for(int p = 0; p < fill; p++){
        printf("%d) %s\n", p, check_location[p]);
}
    random_val = 0;


    while(has_zero){
      display_board(size, sudoku, fill, check_location); //prints the array
      printf("\nEnter Q/q anytime to quit\nEnter P/p to print the board again\n");
      valid_location(&x, &y, size, &new_value, &old_value, sudoku, check_location, fill, &function); //Makes sure location is valid
      valid_value(&x, &y, size, &new_value, sudoku, &old_value, check_location, fill, &function); //Makes sure value is valid
      answer = repeat(x, y, size, sudoku, old_value, new_value, random_val, fill, check_location, &function);
      //Checks to see if there is any repeating digit in the corresponding row and column
      while(answer == 0){
        answer = repeat(x, y, size, sudoku, old_value, new_value, random_val, fill, check_location, &function);
        printf("HI");
      }


      //Checks if array puzzle is complete
      for(int r = 0; r < size; r++){
        for(int t = 0; t < size; t++){
          if(sudoku[r][t] == 0){
            has_zero == 1;
            break;
          }
          else{
            has_zero == 0;
            break;
          }
       }
      }
  }

  printf("\nFinished sudoku puzzle");
  display_board(size, sudoku, fill, check_location);

}

//Checks if there is any repeating digit
int repeat(int x, int y, int size, int sudoku[size][size], int old_value, int new_value, int random_val, int fill, char check_location[][3], char* function){
  int counter = size,
      size2 = size;
  char response;

  for(int i = 0; i < counter; i++){
    size2--;
    if((sudoku[x][y] == sudoku[x][i]) && (i != y)){ // Checks corresponding row for any repeating digit
      sudoku[x][y] = old_value;
      if (random_val == 0 ){
        printf("Not possible\n");
        valid_value(&x, &y, size, &new_value, sudoku, &old_value, check_location, fill, &*function);
      }
      return 0;
    }
    else if(sudoku[x][y] == sudoku[i][y] && (i != x)){ // Checks corresponding column for any repeating digit
      sudoku[x][y] = old_value;
      if (random_val == 0 ){
        printf("Not possible\n");
        valid_value(&x, &y, size, &new_value, sudoku, &old_value, check_location, fill, &*function);
      }
      return 0;
    }
    else if(x == y){ //Checks corresponding diagonal (left to right) column for any repeating digit
      if((sudoku[x][y] == sudoku[i][i]) && (x != i)){
        sudoku[x][y] = old_value;
        if (random_val == 0 ){
          printf("Not possible\n");
          valid_value(&x, &y, size, &new_value, sudoku, &old_value, check_location, fill, &*function);
        }
        return 0;
      }
      //If its an odd sudoku checks center element with both diagonals column for any repeating digit
      if( (counter % 2 != 0) && ((((sudoku[x][y] == sudoku[size2][i]) && (x != size2 && y != i))
                  || ((sudoku[x][y] == sudoku[i][size2]) && (x != i && y != size2)))
                  && (i < (counter/2)))){
        sudoku[x][y] = old_value;
        if (random_val == 0 ){
          printf("Not possible\n");
          valid_value(&x, &y, size, &new_value, sudoku, &old_value, check_location, fill, &*function);
        }
        return 0;
      }
    }
    //Checks corresponding diagonal (right to left) column for any repeating digit
    else if( (((sudoku[x][y] == sudoku[size2][i]) && (x != size2 && y != i))
                || ((sudoku[x][y] == sudoku[i][size2]) && (x != i && y != size2)))
                && (i < (counter/2)) ){
        sudoku[x][y] = old_value;
        if (random_val == 0 ){
          printf("Not possible\n");
          valid_value(&x, &y, size, &new_value, sudoku, &old_value, check_location, fill, &*function);
        }
        return 0;
      }
    //Special case when checking for any repeating digit for diagonal column with center element
    else if((counter % 2 != 0) && (sudoku[x][y] == sudoku[counter/2][counter/2])){
      sudoku[x][y] = old_value;
      if (random_val == 0 ){
        printf("Not possible\n");
        valid_value(&x, &y, size, &new_value, sudoku, &old_value, check_location, fill, &*function);
      }
      return 0;
    }
  }
  if (random_val == 0 ){
    printf("It's possible\n");
  }
  return 1;
}

//Checks that location input is valid
void valid_location(int* x, int* y, int size, int* new_value, int* old_value, int sudoku[size][size], char check_location[][3], int fill, char* function){
  int valid = 0,
      counter = 0;
  char char_x, char_y;
  *function = 'L';

  printf("\nEnter location: ");
  scanf(" %c", &char_x);
  abort_game(&char_x, &*function, size, fill, sudoku, check_location); // Checks to see if user wants to end the game

  scanf(" %c", &char_y);
  abort_game(&char_y, &*function, size, fill, sudoku, check_location); // Checks to see if user wants to end the game

  *x = char_x - '0'; //Changes char to int
  *y = char_y - '0'; //Changes char to int

  //Makes sure autofilled location valuse cannot be changed
  while(counter < fill){
    if((atoi(check_location[counter])/10 == *x) && (atoi(check_location[counter])%10 == *y)){
      printf("Invalid location\nThe value from this location cannot be changed\n");
      printf("Re-enter the location: ");
      scanf(" %c", &char_x);
      abort_game(&char_x, &*function, size, fill, sudoku, check_location); // Checks to see if user wants to end the game
      scanf(" %c", &char_y);
      abort_game(&char_y, &*function, size, fill, sudoku, check_location); // Checks to see if user wants to end the game

      *x = char_x - '0'; //Changes char to int
      *y = char_y - '0'; //Changes char to int
      counter = 0;
    }
    else{
      counter++;}
  }

  //Checks if location input is valid
  while(!valid){
    if(((*x >= 0) && (*x < size)) && ((*y >= 0) && (*y < size))){
      *old_value = sudoku[*x][*y];
      valid = 1;
    }
    else{
      printf("Invalid input\nPlease enter valid digits from 0 - %d: ", size-1);
      scanf(" %c", &char_x);
      abort_game(&char_x, &*function, size, fill, sudoku, check_location); // Checks to see if user wants to end the game
      scanf(" %c", &char_y);
      abort_game(&char_y, &*function, size, fill, sudoku, check_location); // Checks to see if user wants to end the game

      *x = char_x - '0'; //Changes char to int
      *y = char_y - '0'; //Changes char to int
    }
  }
  char_x == '\0';
  char_y == '\0';
}

//Checks that value input is valid
void valid_value(int* x, int* y, int size, int* new_value, int sudoku[size][size], int* old_value, char check_location[][3], int fill, char* function){
  *function = 'V';
  char char_value;
  int valid = 0;
  printf("Enter the value or enter L/l to change location: ");
  //abort_game(&char_value, &*function, size, sudoku);
  //Checks if new_value input is valid
  while(!valid){
    scanf(" %c", &char_value);
    abort_game(&char_value, &*function, size, fill, sudoku, check_location); // Checks to see if user wants to end the game
    //Checks to see if user wants to enter a new location
    while(toupper(char_value) == 'L'){
      valid_location(&*x, &*y, size, &*new_value, &*old_value, sudoku, check_location, fill, &*function);
      printf("Enter value: ");
      scanf(" %c", &char_value);
      abort_game(&char_value, &*function, size, fill, sudoku, check_location); // Checks to see if user wants to end the game
    }

    if(isdigit(char_value)){
      *new_value = char_value - '0'; //Turns char to integar
    }

    //Checks if new_value input is valid
    if((*new_value >= 0) && (*new_value <= size)){
      sudoku[*x][*y] = *new_value;
      valid = 1;
    }
    else{
      abort_game(&char_value, &*function, size, fill, sudoku, check_location);
      printf("Invalid input\nPlease enter L/l or a valid digit from 0 - %d: ", size);
    }
  }
  char_value == '\0';
}


//Displays the sudoku board
void display_board(int size, int sudoku[size][size], int fill, char check_location[][3]){
  print_dashes(size, 5);

  for(int m = 0; m < size; m++){
      printf("|| ");
    for(int n = 0; n < size; n++){
      int num = print_dot(m, n, fill, check_location);
      if(size == 4 && n%2 != 0){
        if(num == 1){
          printf("%3d. || ", sudoku[m][n]);
        }
        else{
          printf("%3d  || ", sudoku[m][n]);
        }
      }
      else if(size == 6 && (n == 2 || n == 5)){
        if(num == 1){
          printf("%3d. || ", sudoku[m][n]);
        }
        else{
        printf("%3d  || ", sudoku[m][n]);
        }
      }
      else if(size == 9 && (n == 2 || n == 5 || n ==8) ){
        if(num == 1){
          printf("%3d. || ", sudoku[m][n]);
        }
        else{
        printf("%3d  || ", sudoku[m][n]);
        }
      }
      else
        if(num == 1){
          printf("%3d. | ", sudoku[m][n]);
        }
        else{
        printf("%3d  | ", sudoku[m][n]);
        }
      }
  print_dashes(size, m);
  }

}

//Aesthetics for the sudoku board
void print_dashes(int size, int m){
  switch(size){
    case 4:
      if(m%(int)sqrt(size) != 0 ){
        printf("\n================================\n");
      }
      else{
        printf("\n----- ------ ------- ------ ----\n");
      }

      break;
    case 6:
      if(m%2 != 0 || m == 5){
        printf("\n==============================================\n");
      }
      else{
        printf("\n----- ------ ------ ------- ------ ------ ----\n");
      }
      break;
    case 9:
    if((m == 2 || m == 5 || m == 8)){
      printf("\n====================================================================\n");
    }
    else{
      printf("\n----- ------ ------ ------- ------ ------- ------ ------ ------ ----\n");
    }
      break;
  }
}

//Identify's which loction are autofilled and where values cannot be changed
int print_dot(int m, int n, int fill, char check_location[][3]){
  for(int x = 0; x < fill; x++){
    if(((atoi(check_location[x])/10 == m)) && ((atoi(check_location[x])%10 == n))){
      return 1;
    }
  }
  return 0;
}

//Determines how much numbers to autfo fill the sudoku board depending on the size
int auto_fill(int size, int sudoku[size][size]){

  //Fills array with 0's
  for(int x = 0; x < size; x++){
    for(int y = 0; y < size; y++){
      sudoku[x][y] = 0;
    }
  }

  //Asigns number of random number auto filled in game based on size of board
  if(size == 4){
        return 5;
  }
  else if(size == 6){
        return 15;
  }
  else if(size == 9){
        return 30;
  }
}

void abort_game(char* response, char* function, int size, int fill, int sudoku[size][size], char check_location[][3]){
  if(toupper(*response) == 'Q'){
    printf("\nThank you for playing! :D");
    exit(1);
  }

  while(toupper(*response) == 'P'){
    if(*function == 'L'){
    display_board(size, sudoku, fill, check_location);
    printf("\nEnter location: ");
    scanf(" %c", &*response);
    abort_game(&*response, &*function, size, fill, sudoku, check_location);
    }
    else{
      display_board(size, sudoku, fill, check_location);
      printf("\nEnter the value or enter L/l to change location: ");
      scanf(" %c", &*response);
      abort_game(&*response, &*function, size, fill, sudoku, check_location);
    }
  }
}


/* Sorts an array alphabetically & prints results

char temp[10];
char ar[][10] = {"face","cream", "nice", "art"};

for(int x = 0; x < 4; x++){
  for(int y = x+1; y < 4; y++){
    if(strcmp(ar[x], ar[y]) > 0){
      strcpy(temp, ar[x]);
      strcpy(ar[x], ar[y]);
      strcpy(ar[y], temp);
    }
 }
printf("%s\n",ar[x]);
}

*/
