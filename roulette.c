#include "roulette.h"


/**

to compile with cmake
	create a folder named cmake-build-debug (mkdir cmake-build-debug)
	open the directory by (cd cmake-build-debug)
	type (cmake ..)
	type (make)
	it will create an executable named roulette
	move the people.txt file from .. directory to here
	run he program by (./roulette)
	make sure cmake is installed in your system

compiler normal way

	type gcc -fopenmp roulette.c -o roulette.o
	./roulette.o


current iteration updates and what is there to fix
	currently before we get the result we see that 
	the print message of a thread losing or winning showing up before the result after spin
	that needs to be fixed 

	otherwise it seems to be solid

*/

int main () {
    // initilizing the roulette table 
    // the function call below will assign values of RESULT struct to the table
	RESULT* table = initTable();
    // each player will pay between 15-55 percent of what they have
    // we create a percent integer to get a random value
    // the random value will be between 15-55
	int percent;
    // number of players will be assigned to nPlayers variable	
	int nPlayers;
    // since we want to ensure the randomness of our simulation
    // we use the line of code down below
    // normally each rand() % (insert any integer here) would give us the same result
    // every time we run the program however, with the line below
    // it will not be the case
	srand(time(NULL));
    // we want out simulation to be as realistic as possible
    // let's say at nights the casino is crowded and daytime it's not
    // we get a random integer and assign it to nPlayers
    // so that the roulette table will not always full
	nPlayers = (rand() % (5 - 2 + 2)) + 2;
    // we are incrementing the number of players
    // because we want to create space for the dealer as well
    // because each player will be treated as a thread as well as the dealer
	nPlayers++;
    // this integer named spin will be used to get the index of the result after
    // each player make their bet
	int spin;
    // before players bet they ask for chips from the dealer
    // imagine the dealer as a monopoly bank
    // people ask for chips and if he can he will be able to do so
	double chips;
	// creating the player profile 
    // see the header file for more information
	PLAYER* playerNo = (PLAYER*)malloc(nPlayers * sizeof(PLAYER));
	// we want each player to have human like names
    // we have a .txt file named people.txt
    // inside that file we have 100 names
    // we will read open that file
    FILE *file = fopen("people.txt", "r");
    // looping for each thread
    // each thread will have their own name
    // we will pick a random name from the file and
    // assign it to the thread
    for (int i = 0; i < nPlayers; i++) {
        // count is the word index in the file
        // let's say count is equal to 5
        // it means that we are at the 5th word in the file
        int count = 0;
        // i will ask for a random name from 0-100
        // that random number is the index of the name i will choose
        // from the file
        int randomName = rand() % 100;
        // buffer for the word that is going to be read in the file
        char buffer[20];
        // reading the file word by word
        while( fscanf(file, "%s", buffer) != EOF ) {
            // if the random name index == count
            // then i found the name i randomly checked
            if (randomName == count) {
				playerNo[i].name = (char*)malloc(sizeof(buffer));
                // copying that name from buffer to the playerNames
                strcpy(playerNo[i].name, buffer);
                // this line will go back to the beginning of the file
                fseek(file, 0, SEEK_SET);
                break;
            }
            count++;
        }
    }
    // we are done with the file pointer
    fclose(file);

    // we want our casino to be healthy so it doesnt ggo bankrupt
    // we give the dealer a substential amount of money
    // players can ask up to 10m worth of chips
    // you can assume that this is not a ghetto casino
	playerNo[0].totalMoney = 10000000000000;
    // we ask a number of threads from the computer based on the number
    // of players we have
	omp_set_num_threads(nPlayers);
    // entering a parallel region
	#pragma omp parallel shared(spin)
	{
        // teach thread will have their own personal id
		int id = omp_get_thread_num();
        // we are assigning dealer to be thread id 0
        // since the dealer is not going to bet we want to 
        // exclude the dealer from making a bet
        // that is the reasoning behind the if id != 0 
		if (id != 0) {
            // we welcome the thread to the table
            printf("%s joined the table\n", playerNo[id].name);
            // this one is an easter egg
            // if the name of the player is karen then she automatically
            // will not like the dealer and
            // she will ask to speak to the manager of the casino
            if (strcmp(playerNo[id].name, "Karen") == 0) {
                printf("Karen does not like the dealer %s \nshe would like to talk to the manager", playerNo[0].name);
            }
            // this is a place for the goto statement to return
            tryagain:
            // each thread will ask for chips worth of a random amount
            // in this case it is going to be between 100k - 13m
            // we are aware that humans may make mistakes and as more than they are allowed to
            // since we are trying to make the casino as realistic as possible
            // there is going to be times where people will ask more than they are allowed to
            // people can ask maximum of 13m worth of chips
            // However, they are only allowed for 10m
            chips = (rand() % (13000000 - 100000 + 1)) + 100000;
            printf("%s is asking %lf worth of chips\n", playerNo[id].name, chips);
            if (chips > 10000000) {
                printf("%s is asking too many chips\n", playerNo[id].name);
                // if they ask more than 10m they go back to the tryagain line
                goto tryagain;
            } else {
                printf("GRANTED!\n");
                // casino is giving the chips
                playerNo[id].totalMoney = chips;
                playerNo[0].totalMoney -= playerNo[id].totalMoney;
            }
            // if you do not know what this line is then go back tp the percent decleration
             percent = (rand() % (55 - 15 + 1)) + 15;
             // line below will take the percentage of the total money
             playerNo[id].moneyBet = (playerNo[id].totalMoney * percent)/100;
             #pragma omp critical
             {
                playerNo[id].theType = bet(playerNo[id]);
             }
        }
        #pragma omp barrier
        // after each bet is made then it is time to spin
        // we spin and print out the result
        // might try something fancy like a loop saying spinning then slowing down and then
        // print out the result
        // and sleep(1) after each spin and slow down print stataements
        // so that people who are running the program can follow
            if (id == 0) {
                // our table is going to have 0---36 entries so we are going tet a
                // random value within that range
                spin = rand() % 36;
                printf("\nResult: %d of %s\n", table[spin].number, table[spin].color);
            }
        #pragma omp critical
        {
            // since the dealer did not bet 
            // we want to exlude him/her from the following lines
            // that determine the winners and losers
            if (id != 0) {
            // based on the bet types we check fi they win or lose
            switch (playerNo[id].theType) {
                case SPECIFIC:
                // for the highly unlikely scenerio if a player bets on a specific number and win
                    if (spin == playerNo[id].spe) {
                        // dealer lose money
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 36;
                        playerNo[id] = winner(id, 36, playerNo[id]);
                        playerNo[id].spe = -1;
                        break;
                    } else {
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money left: %lf\n", playerNo[id].totalMoney);
                        playerNo[id].moneyBet = 0;
                        printf("\n\n");
                        break;
                    }
                case ODD:
                    if (spin % 2 != 0) {
                        // the integer in the function parameter is the multiplier of the bet
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 2;
                        playerNo[id] = winner(id, 2, playerNo[id]);
                        break;
                    } else {
                        printf("%s lost\n", playerNo[id].name);
                        playerNo[id].moneyBet = 0;
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case EVEN:
                    if (spin % 2 == 0) {
                        
                        
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 2;
                        playerNo[id] = winner(id, 2, playerNo[id]);
                        break;
                    } else {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case RED:
                    if (strcmp(table[spin].color, "RED") == 0 ) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 2;
                        playerNo[id] = winner(id, 2, playerNo[id]);
                        break;
                    } else {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case BLACK:
                    if (strcmp(table[spin].color, "BLACK") == 0 ) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 2;
                        playerNo[id] = winner(id, 2, playerNo[id]);
                        break;
                    } else {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case ONE_TWELVE:
                    if (spin > 0 && spin <= 12) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 3;
                        playerNo[id] = winner(id, 3, playerNo[id]);
                        break;
                    } else {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case TWELVE_TWENTYFOUR:
                    if (spin > 12 && spin <= 24) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 3;
                        playerNo[id] = winner(id, 3, playerNo[id]);
                        break;
                    } else {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case TWENTYFOUR_THIRTYSIX:
                    if (spin > 24 && spin <= 36) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 3;
                        playerNo[id] = winner(id, 3, playerNo[id]);
                        break;
                    } else {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case DOZEN1:
                    if (spin % 3 == 1) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 3;
                        playerNo[id] = winner(id, 3, playerNo[id]);
                        break;
                    } else  {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case DOZEN2:
                    if (spin % 3 == 2) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 3;
                        playerNo[id] = winner(id, 3, playerNo[id]);
                        break;
                    } else  {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case DOZEN3:
                    if (spin % 3 == 0) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 3;
                        playerNo[id] = winner(id, 3, playerNo[id]);
                        break;
                    } else  {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case HALF1:
                    if (spin > 0 && spin <= 18) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 2;
                        playerNo[id] = winner(id, 2, playerNo[id]);
                        break;
                    } else {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                case HALF2:
                    if (spin > 18 && spin <= 36) {
                        playerNo[0].totalMoney -= playerNo[id].moneyBet * 2;
                        playerNo[id] = winner(id,2, playerNo[id]);
                        break;
                    } else {
                        playerNo[id].moneyBet = 0;
                        printf("%s lost\n", playerNo[id].name);
                        printf("total money: %lf\n", playerNo[id].totalMoney);
                        printf("\n\n");
                        break;
                    }
                }
            }
        }
	}
    // freeing space
    for (int i = 0 ; i < nPlayers; i++) {
        free(playerNo[i].name);
    }
    free(playerNo);
    free(table);


}



RESULT* initTable() {
    // allocating space for the table
	RESULT* table = (RESULT*)malloc(37 * sizeof(RESULT));
    // there is no pattern for 0 so we will hardcode it
	table[0].number = 0;
	strcpy(table[0].color, "GREEN");

    #pragma omp parallel sections
    {
        #pragma omp section
        evenBlack(1,11,table);
        // based on the patterns we foudn we fill the table
        // from 1 ... < 11 all even numbers are black and all odd numbers are red
        #pragma omp section
        evenRed(11,19,table);
        // from 11 ... < 19 all even numbers are red all odd numbers are black
        #pragma omp section
        evenBlack(19,29,table);
        // you get the logic
        #pragma omp section
        evenRed(29,37,table);
    }
	
	return table;
}



RESULT* evenBlack(int lower, int upper, RESULT* table) {
	// this one is just a debug statement
	// will get rid of the print statement later
	printf("\nyou are in evenblack function\n");
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = lower; i < upper; i++) {
        // if i % 2 = 0 which means it is an even number
        // so we set it as black
        // if not then we set it as red
        if (i % 2 == 0) {   
            strcpy(table[i].color, "BLACK");
        } else {
            strcpy(table[i].color, "RED");
        }
        table[i].number = i;
        }
    }



	return table;
}





RESULT* evenRed(int lower, int upper, RESULT* table) {
	// this one is just a debug statement
	// will get rid of the print statement later
	printf("\nyou are in evenblack function\n");
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = lower; i < upper; i++) {
        // if i % 2 = 0 which means it is an even number
        // so we set it as black
        // if not then we set it as red
        if (i % 2 == 0) {
            strcpy(table[i].color, "RED");
        } else {
            strcpy(table[i].color, "BLACK");
        }
        table[i].number = i;
        }
    }
	return table;
}


BETTYPE bet (PLAYER playerNo) {
    // as we said we want to make tis very realistic
    // we do not want the players to bet on specific  numbers
    // becasue in real world it is not very likely
    // for that realoons we are implementing a roulette wheel algorithm
    // for this we will get a random vlaue betweem 0 - 130
    // if the random value is < 5 which is going to be very unlikely
    // person will bet on a specific number
    // with this while it is not impossible players will more likely
    // to bet on other bet types to win
	int dummyType = rand() % 130;
	if (dummyType < 5) {
		playerNo.theType = SPECIFIC;
        playerNo.spe = rand() % 36;
        printf("%s betting %lf a specific %d\n",playerNo.name, playerNo.moneyBet, playerNo.spe);
	} else if (dummyType > 4 && dummyType < 16) {
        playerNo.theType = ODD;
        printf("%s is betting %lf on ODDS\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType >= 16 && dummyType <= 26) {
        playerNo.theType = EVEN;
        printf("%s is betting %lf EVENS\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType >= 27 && dummyType <= 37) {
        playerNo.theType = BLACK;
        printf("%s is betting %lf on BLACKS\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType >= 38 && dummyType <= 48) {
        playerNo.theType = RED;
        printf("%s is betting %lf on REDS\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType >= 49 && dummyType <= 59) {
        playerNo.theType = ONE_TWELVE;
        printf("%s is betting %lf on 1-12\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType >= 60 && dummyType <= 70) {
        playerNo.theType = TWELVE_TWENTYFOUR;
        printf("%s is betting %lf on 13-24\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType > 70 && dummyType <= 80) {
        playerNo.theType = TWENTYFOUR_THIRTYSIX;
        printf("%s is betting %lf on 25-36\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType > 80 && dummyType <= 90) {
        playerNo.theType = DOZEN1;
        printf("%s is betting %lf on DOZEN1\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType > 90 && dummyType <= 100) {
        playerNo.theType = DOZEN2;
        printf("%s is betting %lf on DOZEN2\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType > 100 && dummyType <= 110 ) {
        playerNo.theType = DOZEN3;
        printf("%s is betting %lf on DOZEN3\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType > 110 && dummyType <= 120) {
        playerNo.theType = HALF1;
        printf("%s is betting %lf on HALF1\n", playerNo.name,playerNo.moneyBet);
    } else if (dummyType > 120 && dummyType <= 130) {
        playerNo.theType = HALF2;
        printf("%s is betting %lf on HALF2\n", playerNo.name,playerNo.moneyBet);
    }

	return playerNo.theType;
}


// if the player wins function below is executed
// ID in the parameter is from former iterations of the code
// fore submission of the final project it will be removed
// mult is the multiplier of the bet amount
PLAYER winner (int ID, int mult, PLAYER playerNo) {
    printf("%s wins\nand won\n", playerNo.name);
    printf("%lf\n", playerNo.moneyBet * 2);
    playerNo.totalMoney = playerNo.totalMoney + (playerNo.moneyBet * mult);
    printf("total money: %lf\n", playerNo.totalMoney);
    playerNo.moneyBet = 0;
    printf("\n\n");
    return playerNo;
}



