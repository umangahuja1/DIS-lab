#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <math.h>

// function to get hours, minutes, seconds from character buffer
void buffer_to_int(int *hh, int *mm, int *ss, char clock[]){
	*hh = (clock[0]-'0')*10 + (clock[1]-'0');
	*mm = (clock[3]-'0')*10 + (clock[4]-'0');
	*ss = (clock[6]-'0')*10 + (clock[7]-'0');
}

// function to get no. of seconds
int clock_to_seconds(char clock[]){
	int hh = 0, mm = 0, ss = 0;
	buffer_to_int(&hh, &mm, &ss, clock);
	return hh*3600 + mm*60 + ss;
}

// function to print clock into buffer from no. of seconds
void seconds_to_clock(int sum, char * buffer){
	int hh = sum/3600, mm = (sum/60)%60, ss = sum%60;
	memset(buffer, 0, strlen(buffer));
	if(hh<10){
		char temp[2048] = {0};
		sprintf(temp, "%02d", hh);
		strcat(buffer, temp);
	}
	else{
		char temp[2048] = {0};
		sprintf(temp, "%d", hh);
		strcat(buffer, temp);
	}
	strcat(buffer, ":");
	if(mm<10){
		char temp[2048] = {0};
		sprintf(temp, "%02d", mm);
		strcat(buffer, temp);
	}
	else{
		char temp[2048] = {0};
		sprintf(temp, "%d", mm);
		strcat(buffer, temp);
	}
	strcat(buffer, ":");
	if(ss<10){
		char temp[2048] = {0};
		sprintf(temp, "%02d", ss);
		strcat(buffer, temp);
	}
	else{
		char temp[2048] = {0};
		sprintf(temp, "%d", ss);
		strcat(buffer, temp);
	}
	strcat(buffer, "\0");
}

// increment clock by one clock tick
void increment_clock(char * clock, int increment){
	printf("Clock tick\n");
	int seconds = clock_to_seconds(clock);
	seconds += increment;
	seconds_to_clock(seconds, clock);
}

#ifdef __WIN32__
   WORD versionWanted = MAKEWORD(1, 1);
   WSADATA wsaData;
   WSAStartup(versionWanted, &wsaData);
#endif

int main(int argc, char const *argv[]){

	// define variables to hold socket descriptor and other parameters
	int server_id, valread, PORT_SELF = atoi(argv[1]), n_processes = atoi(argv[2]), increment = atoi(argv[4+n_processes]);
	
	// array of processes
	int processes[n_processes];
	 int i; 
	// add 8000 to all to make them valid ports 
	PORT_SELF += 8000;
	for(i=0;i<n_processes;i++){
		processes[i] = atoi(argv[3+i]);
		processes[i] += 8000;
	}

	// store initial clock
	char *clock = (char*) malloc(strlen(argv[3+n_processes]));
	strcat(clock, argv[3+n_processes]);

	// structure to hold address parameters of self, next and previous process
	struct sockaddr_in address, *from;
	struct sockaddr_storage from_temp;
	socklen_t address_len = sizeof(address), from_temp_len = sizeof(from_temp);

	// create socket
	server_id = socket(PF_INET, SOCK_DGRAM, 0);

	// set parameters of address structures for self and next process
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT_SELF);

	// bind socket descriptor to address
	if (bind(server_id, (struct sockaddr*)&address, sizeof(address))<0){
		printf("%s\n", "Bind failed");
		exit(0);
	}

	// store the pre required buffers
	char buffer_clock[2048] = {0}, buffer_get_clock[2048] = {0}, buffer_receive[2048] = {0}, buffer_carry[2048] = {0};
	snprintf(buffer_get_clock, sizeof("Send clock") + 1, "%s", "Send clock");
	buffer_get_clock[sizeof("Send clock")] = '\0';
	snprintf(buffer_carry, sizeof("Carry on") + 1, "%s", "Carry on");
	buffer_carry[sizeof("Carry on")] = '\0';
	printf("\n");

	// repeat any no. of times
	while(1){
		printf("Synchronize clocks? ");
		char ch;
		scanf("%c", &ch);

		// if we ant to synchronize clocks
		if(ch=='y'){
			i = 0;
			// send message to all processes to retuen there instantaneous clocks
			for(i=0;i<n_processes;i++){

				// get next process' port
				int PORT_NEXT = processes[i];
				struct sockaddr_in next;
				next.sin_family = AF_INET;
				next.sin_addr.s_addr = INADDR_ANY;
				next.sin_port = htons(PORT_NEXT);
				sendto(server_id, buffer_get_clock, strlen(buffer_get_clock) + 1, 0, (struct sockaddr*) &next, sizeof(next));

				// now receive the clock
				valread = recvfrom(server_id, buffer_receive, 2048, 0, (struct sockaddr*) &from_temp, &from_temp_len);
			}

			// send master clock, or difference to all other processes
			printf("Sending master clock to all processes\n");
			i = 0;
			for(i=0;i<n_processes;i++){
				int PORT_NEXT = processes[i];
				struct sockaddr_in next;
				next.sin_family = AF_INET;
				next.sin_addr.s_addr = INADDR_ANY;
				next.sin_port = htons(PORT_NEXT);
				sendto(server_id, clock, strlen(clock) + 1, 0, (struct sockaddr*) &next, sizeof(next));
			}
		}

		// if we do not wish to synchronize
		else{
			i = 0;
			// send message to all other processes to carry on
			for(i=0;i<n_processes;i++){
				int PORT_NEXT = processes[i];
				struct sockaddr_in next;
				next.sin_family = AF_INET;
				next.sin_addr.s_addr = INADDR_ANY;
				next.sin_port = htons(PORT_NEXT);
				sendto(server_id, buffer_carry, strlen(buffer_carry) + 1, 0, (struct sockaddr*) &next, sizeof(next));
			}
		}

		// increment the clock by pre-defined clock tick
		printf("\n");
		increment_clock(clock, increment);
		printf("Clock now %s\n\n", clock);
		getchar();
	}
	
	return 0;
}