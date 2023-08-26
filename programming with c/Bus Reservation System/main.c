/*
 ============================================================================
 Name        : BusSystem.c
 Author      : amrFawzy
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int clean_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    return 1;
}

int readIntOnly(){
	char c ;
	int var ;
	while ((scanf("%d%c", &var, &c)!=2 || c!='\n') && clean_stdin()){};
	return var ;
}

int number_of_users = 0 ;
int number_of_trips = 0 ;
int ids = 0 ;

char f [20]={0};
char t [20] = {0};

typedef struct{
	int tripId ;
	char* from ;
	char* to ;
	int month ;
	int day ;
	int hour ;
	int remainnig_seats ;
}trip;

typedef struct{
	trip* trip_ptr ;
	int number_of_seats ;
}booked_trip;

typedef struct{
	int userId ;
	int password ;
	booked_trip* myTrips[5];
	int number_of_booked_trips ;
}user;

typedef struct{
	int userId ;
	int password ;
}admin;


booked_trip* booked(trip* t , int seats_number){
	booked_trip* t1 = (booked_trip*)malloc(sizeof(booked_trip));
	t1->trip_ptr = t ;
	t1->number_of_seats = seats_number ;
	return t1 ;
}

int chooseRole(){
	while(1){
		int role ;
		printf("Please choose your role (1 for user) and (0 for admin)\n") ;
		role = readIntOnly();
		if(0 == role || 1 == role)
			return role ;
		printf("invalid role\n") ;
	}
}

int display(trip** trips){
	while(1){
		printf("************************************\n");
		printf("			Avaliable trips			\n");
		for(int i=0 ; i<number_of_trips ; i++){
			printf("trip numebr %d\n" , (i+1));
			printf("from: %s    to: %s    date: %d/%d    time: %d    remaining seats: %d\n\n",trips[i]->from , trips[i]->to  , trips[i]->day , trips[i]->month , trips[i]->hour , trips[i]->remainnig_seats);
		}
		int trip_number ;
		printf("choose a trip or enter -1 to go back to home page\n");
		trip_number = readIntOnly();
		if(-1 == trip_number)
			return -1 ;
		if(trip_number > number_of_trips || trip_number < 1){
			printf("invaled trip number\n");
			continue ;
		}
		return trip_number ;
	}

}

int displayBooked(user** users , int userIndx){
	while(1){
		printf("************************************\n");
		printf("			Avaliable trips			\n");
		for(int i=0 ; i<users[userIndx]->number_of_booked_trips ; i++){
			trip* t = users[userIndx]->myTrips[i]->trip_ptr ;
			printf("trip numebr %d\n" , (i+1));
			printf("from: %s    to: %s    date: %d/%d    time: %d     seats: %d\n\n",t->from , t->to  , t->day , t->month , t->hour , users[userIndx]->myTrips[i]->number_of_seats);
		}
		int t_number = 0  ;
		printf("choose a trip or -1 to go back to  home page\n");
		t_number = readIntOnly();
		if(-1 == t_number)
			return -1 ;
		if(t_number < 1 || t_number>users[userIndx]->number_of_booked_trips){
			printf("invaled trip number\n");
			continue ;
		}
		return t_number ;
	}

}

void bookTicket(user** users , int userIndx ,trip** trips , int tripIndx){
	printf("**********************************\n");
	printf("please enter number of seats :");
	int number_of_seats = readIntOnly();
	if(number_of_seats <= trips[tripIndx]->remainnig_seats){
		int number_of_booked_trips = users[userIndx]->number_of_booked_trips ;
		trips[tripIndx]->remainnig_seats -= number_of_seats;
		users[userIndx]->myTrips[number_of_booked_trips] = booked(trips[tripIndx] , number_of_seats);
		++users[userIndx]->number_of_booked_trips;
		printf("confirmed\n");
	}
	else{
		printf("there is no enough seats in this trip");
	}
}

void cancel(user** users , int userIndx , int tripIndx){
	users[userIndx]->myTrips[tripIndx]->trip_ptr->remainnig_seats += users[userIndx]->myTrips[tripIndx]->number_of_seats ;
	for(int i=tripIndx ; i<users[userIndx]->number_of_booked_trips ; i++){
		users[userIndx]->myTrips[i] = users[userIndx]->myTrips[i+1] ;
	}
	--users[userIndx]->number_of_booked_trips;
	printf("user trip cancelled\n");
}

void homePage(user** users, int userIndx , trip** trips){
	int tripIndx ;
	while(1){
		printf("************************\n");
		printf("		Home Page		\n");
		printf("1- Book new ticket\n");
		printf("2- cancel an old ticket\n");
		printf("3- logout\n");

		int op = readIntOnly();

		switch(op){
			case 1:
				tripIndx = display(trips) - 1 ;
				if(-2 == tripIndx)
					continue;
				bookTicket(users ,userIndx , trips ,  tripIndx);
				break;
			case 2:
				tripIndx = displayBooked(users, userIndx) - 1 ;
				if(-2 == tripIndx)
					continue;
				cancel(users, userIndx , tripIndx);
				break;
			case 3:
				printf("back to login\n");
				return ;
			default :
				printf("Invalid operation\n");
		}
	}
}

int login(int role , user** users , admin* ad){
	int id , pass ;
	while(1){
		printf("please enter user id : ");
		id = readIntOnly();
		printf("please enter user password : ");
		pass = readIntOnly();

		if(1 == role){
			for(int i=0 ; i<number_of_users ; i++){
				if(id == users[i]->userId && pass == users[i]->password)
					return i ;
			}
			printf("Invalid user id or password\n");
		}
		else{
			if(id ==ad->userId && pass == ad->password)
				return -1 ;
			else
				printf("Invalid user id or password\n");
		}
	}
}

user* createUser(int id , int pass){
	user* u1 =(user*)malloc(sizeof(user));
	u1->userId = id ;
	u1->password = pass ;
	u1->number_of_booked_trips = 0;
	return u1 ;
}

trip* createTrip(char* f , char* t , int m , int d , int h , int s ){
	trip* t1 = (trip*)malloc(sizeof(trip));
	t1->from = f ;
	t1->to = t ;
	t1->month = m ;
	t1->day = d ;
	t1->hour = h ;
	t1->remainnig_seats = s ;
	t1->tripId = ++ids ;
	return t1 ;
}

void init(user** users , trip** trips){
	users[0] = createUser(1,11);
	users[1] = createUser(2,22);
	users[2] = createUser(3,33);
	number_of_users = 3 ;

	trips[0] = createTrip("cairo" , "dahab" , 8, 12, 10 , 20);
	trips[1] = createTrip("cairo" , "alex" , 8, 12, 10 , 20);
	trips[2] = createTrip("cairo" , "sharm" , 9, 12, 9 , 20);
	number_of_trips = 3 ;

}
void showTrips(trip** trips){
	printf("************************************\n");
	printf("			Avaliable trips			\n");
	for(int i=0 ; i<number_of_trips ; i++){
		printf("trip numebr %d\n" , (i+1));
		printf("from: %s    to: %s    date: %d/%d    time: %d    remaining seats: %d\n\n",trips[i]->from , trips[i]->to  , trips[i]->day , trips[i]->month , trips[i]->hour , trips[i]->remainnig_seats);
	}
}

void addTrip(trip** trips){
	++number_of_trips ;
	trips = (trip**)realloc(trips , number_of_trips*sizeof(trip*));
	int month , day , time , seats;

	printf("please pickup location: ");
	scanf("%s" , f);

	printf("please distanation location: ");
	scanf("%s",t);

	printf("please month of the trip: ");
	month = readIntOnly();

	printf("please day of the trip: ");
	day = readIntOnly();

	printf("please time of the trip: ");
	time = readIntOnly();

	printf("please enter number of seats in the trip: ");
	seats = readIntOnly();

	trips[number_of_trips-1] = createTrip(f,t,month,day,time,seats);

}

void cancel_trips(user** users, trip** trips ){
	printf("*********************************\n");
	printf("    cancellation page   \n");
	while(1){
		printf("please Enter number of trip to cancel or -1 to go back: ");
		int indx = readIntOnly();
		if(indx == -1)
			return ;
		if(indx > number_of_trips || indx < 1)
			printf("invalid trip number\n");
		--indx ;
		int id = trips[indx]->tripId ;
		for(int i=0 ; i<number_of_users ; i++){
			for(int j=0 ; j<users[i]->number_of_booked_trips ; j++){
				int user_trip_id = users[i]->myTrips[j]->trip_ptr->tripId ;
				if(id == user_trip_id)
					cancel(users , i ,j);
			}
		}
		for(int i=indx ; i<number_of_trips ; i++){
			trips[i] = trips[i+1] ;
		}
		--number_of_trips;
		break ;
	}
	printf("trip cancelled from system\n");
}

void adminPage(user** users ,trip** trips){
	while(1){
		printf("*********************************\n");
		printf("    Admin home page    \n");
		printf("1- add new trip\n");
		printf("2- cancel trip\n");
		printf("3- display current trips\n");
		printf("4- logout\n");

		int op = readIntOnly();
		switch(op){
		case 1:
			addTrip(trips);
			break ;
		case 2:
			cancel_trips(users,trips);
			break ;
		case 3:
			showTrips(trips);
			break ;
		case 4:
			return ;
		default :
			printf("invalid option\n");
		}
	}
}

int main(void) {
	setvbuf(stdout , NULL , _IONBF , 0) ;
	setvbuf(stderr , NULL , _IONBF , 0) ;

	user** users = (user**)malloc(3*sizeof(user*));
	trip** trips = (trip**)malloc(3*sizeof(trip*));
	admin* ad = (admin*)malloc(sizeof(admin));
	init(users , trips);

	ad->userId = 9 ;
	ad->password = 99 ;

	while(1){
		int role = chooseRole();
		int userIndx = login(role , users , ad);
		if(-1 == userIndx)
			adminPage(users ,trips);
		else
			homePage(users , userIndx, trips);
	}
	return EXIT_SUCCESS;
}
